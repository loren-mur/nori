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
                        "EnvironmentMap[\n"
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

            Vector3f pixelToDirection(const Point2f &pixel) const{
                float theta = pixel[0] * M_PI /(mrows - 1);
                float phi = pixel[1] * 2 * M_PI / (mcols - 1);
                //spherical coordinates
                return Vector3f(sin(theta) * cos(phi), sin(theta)*sin(phi), cos(theta)).normalized();
            }

            Point2f directionToPixel(const Vector3f &vec) const {
                Point2f coordinates = sphericalCoordinates(vec);
                float theta = coordinates.x();
                float phi = coordinates.y();

                float u = theta * (mrows -1) / M_PI ;
                float v = phi  * 0.5 * (mcols - 1) / M_PI ;

                if(std::isnan(u) || std::isnan(v)) {
                    return Point2f(0,0);
                }

                return Point2f(u,v);
            }


            //formular for bilinear interpolation http://www.cs.umd.edu/~djacobs/CMSC427/Interpolation.pdf
            Color3f bilinearInterpolation(float Dx, float Dy, Color3f Q11, Color3f Q21, Color3f Q12, Color3f Q22, float dx1, float dy1, float dx2, float dy2) const {
                //float du, float dv, Color3f Q00, Color3f Q01, Color3f Q10, Color3f Q11
                //Linearly interpolates between f(0) and f(1)
                //Color3f color1 = (1 - du) * Q00 + du * Q01;
                //Color3f color2 = (1 - du) * Q10 + du * Q11;
                //Linearly interpolates between color1 and color2
                //Color3f result = (1 - dv) * color1 + dv * color2;
                //return result;

                if (Dx == 0.f || Dy == 0.f) return 0.f;
                return 1.0 / (Dx * Dy) * (Q11 * dx2 * dy2 +
                                          Q21 * dx1 * dy2 +
                                          Q12 * dx2 * dy1 +
                                          Q22 * dx1 * dy1);
            }



            Color3f eval(const EmitterQueryRecord & lRec) const override {
                //Point2f pixel = directionToPixel(dir);
                //int u = floor(pixel.x());
                //int v = floor(pixel.y());

                //if (u >= rows() - 1 || v >= cols() - 1) {
                //    return (*this)(u,v);

                //then we have to do the linear interpolation
                //Color3f Q00 = (*this)(u, v);
                //Color3f Q01 = (*this)(u, v+1);
                //Color3f Q10 = (*this)(u+1, v);
                //Color3f Q11 = (*this)(u+1, v+1);

                //float du = pixel.x() - u;
                //float dv = pixel.y() - v;

                //return bilinearinterpolation(du, dv, Q00, Q01, Q10, Q11);

                Point2f uv = directionToPixel(lRec.wi.normalized());
                float x = uv[0];
                float y = uv[1];
                int x1 = floor(x);
                int y1 = floor(y);
                int x2 = x1 + 1;
                int y2 = y1 + 1;
                Color3f Q11 = 0.f;
                if (x1 >= 0 && x1 < mrows && y1 >= 0 && y1 < mcols)
                    Q11 = imageMap(x1, y1);
                Color3f Q12 = 0.f;
                if (x1 >= 0 && x1 < mrows && y2 >= 0 && y2 < mcols)
                    Q12 = imageMap(x1, y2);
                Color3f Q21 = 0.f;
                if (x2 >= 0 && x2 < mrows && y1 >= 0 && y1 < mcols)
                    Q21 = imageMap(x2, y1);
                Color3f Q22 = 0.f;
                if (x2 >= 0 && x2 < mrows && y2 >= 0 && y2 < mcols)
                    Q22 = imageMap(x2, y2);
                int Dx = x2 - x1;
                int Dy = y2 - y1;
                float dx1 = x - x1;
                float dy1 = y - y1;
                float dx2 = x2 - x;
                float dy2 = y2 - y;

                return bilinearInterpolation(Dx, Dy, Q11, Q12, Q21, Q22, dx1, dy1, dx2, dy2);
            }




            virtual Color3f sample(EmitterQueryRecord & lRec, const Point2f & sample) const override {
                // From the paper we know that the jacobian is
                float jacobian = (mcols - 1) * (mrows - 1) / (2 * std::pow(M_PI, 2) * Frame::sinTheta(lRec.wi));
                float u, v;
                float pdfu, pdfv;
                sample1D(0, pmarginal, cmarginal, sample.x(), u, pdfu);
                sample1D(u, mpdf, mcdf, sample.y(), v, pdfv);
                Point2f pixel = Point2f(u, v); //lui qua usava solo u e v.
                Vector3f w = pixelToDirection(pixel);
                lRec.wi = w;
                lRec.shadowRay = Ray3f(lRec.ref, lRec.wi, Epsilon, 100000);
                pdfv = pdf(lRec) * jacobian;
                return eval(lRec) / pdf(lRec) / jacobian;

                //Point2f pixel = samplePixel(sample);
                //dir = pixelToDirection(pixel);
                //int i = floor(pixel.x());
                //int j = floor(pixel.y());
                //pdf_pixel = m_pmarginal(0, i) * m_pdf(i, j);
                //float pdf = pdf_pixel * jacobian;

                //again bilinear interpolation from u and v to get an interpolated value from the two colors
                //int u = floor(pixel.x());
                //int v = floor(pixel.y());
                //if ((u >= rows() - 1 || v >= cols() - 1)) {
                //    return (*this)(u,v);
                //}
                //float du = pixel.x() - u;
                //float dv = pixel.y() - v;
                //Color3f Q00 = (*this)(u, v);
                //Color3f Q01 = (*this)(u, v+1);
                //Color3f Q10 = (*this)(u+1, v);
                //Color3f Q11 = (*this)(u+1, v+1);
                //return bilinearInterpolation(du, dv, Q00, Q01, Q10, Q11) / pdf;

            }





            virtual float pdf(const EmitterQueryRecord &lRec) const override {
                Point2f pixel = directionToPixel(lRec.wi.normalized());
                int i = floor(pixel.x());
                int j = floor(pixel.y());
                if (i >= mrows) i = mrows - 1;
                if (j >= mcols) j = mcols - 1;
                if (i < 0) i = 0;
                if (j < 0) j = 0;
                return (pmarginal(0, i) * mpdf(i,j));
            }


protected:
    Bitmap imageMap;
    int mcols;
    int mrows;
    matrix luminance;
    matrix mpdf;
    matrix mcdf;
    matrix pmarginal;
    matrix cmarginal;
};

NORI_REGISTER_CLASS(EnvMap, "envmap")
NORI_NAMESPACE_END



















