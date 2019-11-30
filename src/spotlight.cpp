//
// Created by Alessia Paccagnella on 30/11/2019.
//

#include <nori/integrator.h>
#include <nori/scene.h>
#include <nori/warp.h>
#include <nori/emitter.h>

NORI_NAMESPACE_BEGIN

class SpotLight : public Emitter {
public:
        SpotLight(const PropertyList &propertyList){
            positionLight = propertyList.getPoint3("position"); //vector 3 dimension
            color = propertyList.getColor("color"); //vector
            directionSpot = propertyList.getVector3("direction");

            cosFalloffStart = cos(M_PI / 180 * propertyList.getFloat("cosFalloffStart"));
            cosTotalWidth = cos(M_PI / 180 * propertyList.getFloat("cosTotalWidth"));

        }

        std::string toString() const {
            return tfm::format(
                    "Spotlight[\n"
                    "  position = \"%s\"\n"
                    "  color = \"%s\"\n"
                    "  direction = \"%s\"\n"
                    "  cosTotalWidth = %f\n"
                    "  cosFalloffStart = %f\n"
                    "]",
                    positionLight.toString(), color.toString(), directionSpot.toString(), cosTotalWidth, cosFalloffStart
            );
        }

        virtual Color3f sample(EmitterQueryRecord &lRec, const Point2f &sample)const override {
            lRec.wi = (positionLight - lRec.ref).normalized();
            lRec.p = positionLight;
            lRec.pdf = 1.f;
            lRec.n = directionSpot.normalized();
            lRec.shadowRay = Ray3f(lRec.ref, lRec.wi, Epsilon, (positionLight - lRec.ref).norm() - Epsilon);

            Color3f intensity = color / (4.f*M_PI);
            float cosTheta = directionSpot.normalized().dot((lRec.ref - positionLight).normalized()); //dot product between the direction of the light and the ray from the point to the light normalized
            return intensity * cutOff(cosTheta, cosFalloffStart, cosTotalWidth) / (lRec.ref - lRec.p).squaredNorm(); //cut off tells how much the light is reduced
        }

        virtual float cutOff(float cosTheta, float cosFalloffStart, float cosTotalWidth) const {
            //from pbr book and mistuba ref
            if (cosTheta < cosTotalWidth)     return 0;
            if (cosTheta > cosFalloffStart)   return 1;
            float transitionWidth = (std::acos(cosTotalWidth) - std::acos(cosFalloffStart));
            return ((std::acos(cosTotalWidth) - std::acos(cosTheta)) / transitionWidth);
        }

        virtual float pdf(const EmitterQueryRecord &lRec)const override {
            return lRec.pdf; //or lRec.pdf???
        }


        //from pbr book
        virtual Color3f eval(const EmitterQueryRecord &lRec) const override {
            Color3f I = color / (4.f*M_PI);
            return  I * 2.f * M_PI * (1.f - 0.5f * (cosFalloffStart + cosTotalWidth));
        }

protected:
        Point3f positionLight; //position of the spotlight
        Color3f color;  //value of the spotlight
        Vector3f directionSpot; //direction of the ray of spotlight
        float cosFalloffStart; //cosine of cutoff angle
        float cosTotalWidth; //total cosine

};

NORI_REGISTER_CLASS(SpotLight, "spotlight");
NORI_NAMESPACE_END