//
// Created by Alessia Paccagnella on 28/11/2019.
//

#include <nori/bsdf.h>
#include <nori/frame.h>

NORI_NAMESPACE_BEGIN

class Conductor : public BSDF {
public:
    Conductor(const PropertyList &propList) {
        m_eta = propList.getFloat("eta", 0.23086);
        m_k = propList.getFloat("k", 2.9102);
        m_reflectance = propList.getFloat("specularReflectance", 0.90751);
        m_rgb = propList.getColor("rgb", Color3f(255, 255, 255)); // albedo of the base material
        m_rgb /= 255.f; // normalize
    }

    virtual Color3f eval(const BSDFQueryRecord &) const override {
        return Color3f(0.0f);
    }

    virtual float pdf(const BSDFQueryRecord &) const override {
        return 0.0f;
    }

    virtual Color3f sample(BSDFQueryRecord &bRec, const Point2f &sample) const override {
        if (Frame::cosTheta(bRec.wi) <= 0)
            return Color3f(0.0f);

        bRec.measure = EDiscrete;

        //no refraction
        bRec.wo = Vector3f( //perfect reflectance
                -bRec.wi.x(),
                -bRec.wi.y(),
                bRec.wi.z()
        );
        bRec.measure = EDiscrete;

        //index of ref
        bRec.eta = 1.0f;
        return m_reflectance * m_rgb * fresnelConductor(Frame::cosTheta(bRec.wi), m_eta, m_k);
    }

    Color3f fresnelConductor(float cosTheta_i, Color3f eta, Color3f k) const { 
        float cosTheta2 = cosTheta_i * cosTheta_i;
        Color3f temp = eta*eta + k*k;

        Color3f t1 = temp * cosTheta2;
        Color3f t2 = 2 * eta * cosTheta_i;

        Color3f c1 = (t1 - t2 + 1) / (t1 + t2 + 1);
        Color3f c2 = (temp - t2 + cosTheta2) / (temp + t2 + cosTheta2);

        return (c1 + c2) / 2;
    }

    virtual std::string toString() const override {
        return tfm::format(
                "Conductor[\n"
                "  eta = \"%f\"\n"
                "  k = \"%f\"\n"
                "  reflectance = \"%f\"\n"
                "]",
                m_eta, m_k, m_reflectance);
    }

private:
    float m_eta;
    float m_k;
    float m_reflectance;
    Color3f m_rgb;
};

NORI_REGISTER_CLASS(Conductor, "conductor");
NORI_NAMESPACE_END
