//
// Created by Alessia Paccagnella on 28/11/2019.
//

#include <nori/bitmap.h>
#include <nori/emitter.h>
#include <nori/frame.h>


NORI_NAMESPACE_BEGIN

typedef Eigen::Array<float, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> matrix;

class EnvMap: public Emitter {
public:

        EnvMap(const PropertyList &propList) {

            if (propList.has("strength")) {
                m_strength = propList.getFloat("strength");
            }
            // loads the exr file, then create the imageMap with the Bitmap function
            std::string filename = propList.getString("filename");
            imageMap = Bitmap(filename);
            mcols = imageMap.cols();
            mrows = imageMap.rows();

            luminance = matrix(mrows, mcols);
            mpdf = matrix(mrows, mcols);
            mcdf = matrix(mrows, mcols + 1);
            pmarginal = matrix(1, mrows);
            cmarginal = matrix(1, mrows + 1);

            for (int i = 0; i < mrows; i++) {
                for (int j = 0; j < mcols; j++) {
                    luminance(i,j) = sqrt(0.3* imageMap(i,j).r() + 0.6*imageMap(i,j).g() + 0.1*imageMap(i,j).b()) + Epsilon / 10000000;
                }
            }

            //precomputing the pdf and cdf values
            matrix sum(1, mrows);
            for (int i = 0; i < mpdf.rows(); ++i) {
                sum(0, i) = precompute1D(i, luminance, mpdf, mcdf);
            }
            precompute1D(0, sum, pmarginal, cmarginal);  //precompute the marginal pdf and cdf
        }

        virtual std::string toString() const override {
            return tfm::format(
                    "EnvironmentMap"
            );
        }


        //From the paper
        void sample1D(int rowNumber, const matrix &pf, const matrix &Pf , const float &sample , float &x, float &prob) const {
            //Binary search
            int i;
            for (i = 0; i < Pf.cols(); i++) {
                if ((sample >= Pf(rowNumber, i)) && (sample < Pf(rowNumber, i+1))) // Pf[i]<=unif<Pf[i+1]
                    break;
            }
            float t = (Pf(rowNumber, i+1) - sample) / (Pf(rowNumber, i+1) - Pf(rowNumber, i));
            x = (1-t) * i + t * (i+1);
            prob = pf(rowNumber, i);
        }

        //From the paper
        float precompute1D(int row, const matrix &f, matrix &pf, matrix &Pf) const {
            float I = 0;
            int i;
            for(i= 0; i < f.cols(); i++)
                I = i+f(row, i);
            if (I == 0)
                return I;
            for(int j = 0; j < f.cols(); j++)
                pf(row, j) = f(row, j)/I;
            Pf(row, 0) = 0;
            for(i = 1; i < f.cols(); i++)
                Pf(row,i) = Pf(row, i-1) + pf(i-1);
            Pf(row,i) = 1;
            return I;
        }

        //returns the spherical coordinates from uv coordinates of a 2d pixel
        Vector3f pixelToDirection(const Point2f &pixel) const{
            //from u and v, calculate spherical coordinates
            float theta = pixel[0] * M_PI /(mrows - 1);
            float phi = pixel[1] * 2 * M_PI / (mcols - 1);
            //spherical coordinates
            return Vector3f(sin(theta) * cos(phi), sin(theta)*sin(phi), cos(theta)).normalized();
        }


        //returns the 2d coordinates of the pixel from 3d spherical coordinates
        Point2f directionToPixel(const Vector3f &vec) const {
            //take the spherical coordinates phi and theta
            Point2f coordinates = sphericalCoordinates(vec);
            float theta = coordinates.x();
            float phi = coordinates.y();

            //calculate u and v from spherical coordinates
            float u = theta * (mrows -1) / M_PI ;
            float v = phi  * 0.5 * (mcols - 1) / M_PI ;

            if(std::isnan(u) || std::isnan(v)) {
                return Point2f(0,0);
            }

            //return the indexes
            return Point2f(u,v);

        }


        //formular for bilinear interpolation
        Color3f bilinearInterpolation(float dx21, float dy21, Color3f Q11, Color3f Q21, Color3f Q12, Color3f Q22, float dx01, float dy01, float dx20, float dy20) const {
            // check if the denominator is zero or not
            if (dx21 == 0.f || dy21 == 0.f) return 0.f;
            //wikipedia formula extended
            return ((1.0 / (dx21 * dy21)) * (Q11 * dx20 * dy20 + Q21 * dx01 * dy20 + Q12 * dx20 * dy01 + Q22 * dx01 * dy01));
        }



Color3f eval(const EmitterQueryRecord & lRec) const override {
            Point2f uv = directionToPixel(lRec.wi.normalized());

            //prepare for bilinear interpolation
            float x = uv[0]; //u
            float y = uv[1]; //v
            int x1 = floor(x); //floor of u
            int y1 = floor(y); //floor of y
            int x2 = x1 + 1; //superior value
            int y2 = y1 + 1; //superior value
            Color3f Q11 = 0.f;
            if (x1 >= 0 && x1 < mrows && y1 >= 0 && y1 < mcols)
                Q11 = imageMap(x1, y1); //value left down
            Color3f Q12 = 0.f;
            if (x1 >= 0 && x1 < mrows && y2 >= 0 && y2 < mcols)
                Q12 = imageMap(x1, y2); //value left up
            Color3f Q21 = 0.f;
            if (x2 >= 0 && x2 < mrows && y1 >= 0 && y1 < mcols)
                Q21 = imageMap(x2, y1); //value right down
            Color3f Q22 = 0.f;
            if (x2 >= 0 && x2 < mrows && y2 >= 0 && y2 < mcols)
                Q22 = imageMap(x2, y2); //value right up
            int dx21 = x2 - x1; //difference between superior and inferior value (single step)
            int dy21 = y2 - y1; //difference between superior and inferior value (single step)
            float dx20 = x2 - x;
            float dx01 = x - x1;
            float dy01 = y - y1;
            float dy20 = y2 - y;

            //bilinear interpolation
            return m_strength * bilinearInterpolation(dx21, dy21, Q11, Q12, Q21, Q22, dy01, dx01, dy20, dx20);
        }


        virtual Color3f sample(EmitterQueryRecord & lRec, const Point2f & sample) const override {
            //from the paper we know that the jacobian is
            float jacobian = (mcols - 1) * (mrows - 1) / (2 * std::pow(M_PI, 2) * Frame::sinTheta(lRec.wi));
            float u, v;
            float pdfu, pdfv;
            //sample the pixel, for u = 0 and v with u , marginal and conditional
            sample1D(0, pmarginal, cmarginal, sample.x(), u, pdfu);
            sample1D(u, mpdf, mcdf, sample.y(), v, pdfv);
            Point2f pixel = Point2f(u, v);
            Vector3f w = pixelToDirection(pixel);
            //set the lRec parameters
            lRec.wi = w;
            lRec.shadowRay = Ray3f(lRec.ref, lRec.wi, Epsilon, 100000);
            pdfv = pdf(lRec) * jacobian;
            //return Color
            return eval(lRec) / pdfv;
        }


        virtual float pdf(const EmitterQueryRecord &lRec) const override {
            Point2f pixel = directionToPixel(lRec.wi.normalized());
            int i = floor(pixel.x());
            int j = floor(pixel.y());
            if (i >= mrows) i = mrows - 1;
            if (j >= mcols) j = mcols - 1;
            if (i < 0) i = 0;
            if (j < 0) j = 0;
            //pdf = pdfmarginal * pdf
            return (pmarginal(0, i) * mpdf(i,j));
        }


protected:
    Bitmap imageMap;
    //number of cols of the map
    int mcols;
    //number of rows of the map
    int mrows;
    //luminance matrix
    matrix luminance;
    //pdf matrix
    matrix mpdf;
    //cdf matrix
    matrix mcdf;
    //pdf marginal matrix
    matrix pmarginal;
    //cdf marginal matrix
    matrix cmarginal;

    float m_strength = 1;
};

NORI_REGISTER_CLASS(EnvMap, "envmap")
NORI_NAMESPACE_END



















