//
// Created by Niklaus on 07.12.19.
//

#include <nori/bsdf.h>
#include <nori/texture.h>
#include <nori/warp.h>
#include <nori/frame.h>

NORI_NAMESPACE_BEGIN

Color3f lerp(float t, const Color3f& c1, const Color3f& c2) {
    return (1.f - t) * c1 + t * c2;
}

float SchlickWeight(float x) {
    x = clamp(1.f - x, 0.f, 1.f);
    return pow(x, 5);
}

float SmithGGX(float cosTheta, float a){
    float a2 = a * a;
    float c2 = cosTheta * cosTheta;
    return 1.f / (cosTheta + sqrt(c2 + a2 - a2 * c2));
}

class DisneyBRDF : public BSDF {
public:

    DisneyBRDF(const PropertyList &propList) {
        m_baseColor = propList.getColor("baseColor", Color3f(0.f, 0.f, 0.f));

        // (0: dialectric, 1: metallic)
        m_metallic = propList.getFloat("metallic", 0.0f);

        m_specular = propList.getFloat("specular", 0.2f);
        m_specularTint = propList.getFloat("specularTint", 0.0f);
        m_roughness = propList.getFloat("roughness", 0.5f);
        m_clearcoat = propList.getFloat("clearcoat", 0.5f);
        m_clearcoatGloss = propList.getFloat("clearcoatGloss", 0.0f);
    }

    /// Evaluate the BRDF for the given pair of directions
    virtual Color3f eval(const BSDFQueryRecord &bRec) const override {
        float dotNL = Frame::cosTheta(bRec.wo);
        float dotNV = Frame::cosTheta(bRec.wi);
        if (dotNV < 0.f || dotNL < 0.f)
            return 0;

        Vector3f v = bRec.wi;
        Vector3f l = bRec.wo;
        Vector3f wh = (v + l).normalized();

        //Color
        float luminance = m_baseColor.getLuminance();
        Color3f Ctint = (luminance > 0.f) ? Color3f(m_baseColor.r() / luminance, m_baseColor.g() / luminance, m_baseColor.b() / luminance) : Color3f(1);
        Color3f CtintMix = 0.08 * m_specular * lerp(m_specularTint, Color3f(1), Ctint);
        Color3f Cspec0 = lerp(m_metallic, CtintMix, m_baseColor);

        /*
         * Diffuse
        */
        float fl = SchlickWeight(dotNL);
        float fv = SchlickWeight(dotNV);
        float diffuse_fresnel = (1.0f - 0.5f * fl) * (1.0f - 0.5f * fv);

        //Retro reflection
        float cosThetaD = v.dot(wh);
        float RR = 2.f * m_roughness * cosThetaD * cosThetaD;
        float retro = RR * (fl + fv + fl * fv * (RR - 1.f));

        //hanrahanKrueger subsurface
        float fss90 = pow(v.dot(wh), 2) * m_roughness;
        float fss = lerp(fl, 1.f, fss90) * lerp(fv, 1.f, fss90);
        float hanrahanKrueger = 1.25f * (fss * (1.f / (dotNL + dotNV) - 0.5f) + 0.5f);


        Color3f diffuseTerm = m_baseColor * INV_PI * (retro + diffuse_fresnel);
        /*
         * Specular
         */
        float alpha = std::max(0.01f, m_roughness * m_roughness);
        float Ds = Warp::squareToGTR2Pdf(wh, alpha);
        float fh = SchlickWeight(l.dot(wh));
        Color3f Fs = lerp(fh, Cspec0, Color3f(1.f, 1.f, 1.f));
        float Gs = SmithGGX(dotNL, alpha) * SmithGGX(dotNV, alpha);

        Color3f specularityTerm = Gs * Fs * Ds;

        /*
         * Clearcoat
         */
        float Dr = Warp::squareToGTR1Pdf(wh, lerp(m_clearcoatGloss, .1f, .001f));
        float Fr = lerp(fh, 0.04f, 1.f); //Fresnel schlick with ior = 1.5 -> F0 = 0.04
        float Gr = SmithGGX(dotNL, 0.25f) * SmithGGX(dotNV, 0.25f);

        Color3f clearcoatTerm = Gr * Fr * Dr;

        return (1.f - m_metallic) * diffuseTerm + specularityTerm + 0.25f * m_clearcoat * clearcoatTerm;

    }

    virtual float pdf(const BSDFQueryRecord &bRec) const override {
        float cosTheta = Frame::cosTheta(bRec.wo);
        if (cosTheta <= 0)
            return 0.f;

        Vector3f wh = (bRec.wi + bRec.wo).normalized();
        float diffuse = (1.f - m_metallic) * 0.5f;
        float GTR2 = 1.f / (1.f + m_clearcoat);
        float Jh = 1.f / (4.f * abs(wh.dot(bRec.wo)));
        float cosTheta_wh = Frame::cosTheta(wh);

        float alpha = std::max(0.01f, m_roughness * m_roughness);
        return diffuse * cosTheta * INV_PI + (1.f - diffuse) *
                (GTR2 * Warp::squareToGTR2Pdf(wh, alpha) * Jh * cosTheta_wh +
                (1.f - GTR2) * Warp::squareToGTR1Pdf(wh, lerp(m_clearcoatGloss, .1f, .001f)) * Jh * cosTheta_wh);
    }

    /// Sample the BRDF
    virtual Color3f sample(BSDFQueryRecord &bRec, const Point2f &_sample) const override {
        if (Frame::cosTheta(bRec.wi) <= 0.f)
            return 0;

        float diffuse = (1.f - m_metallic) * 0.5f;
        Point2f sample;
        if (_sample.x() < diffuse) {
            sample = Point2f(_sample.x() / diffuse, _sample.y());
            bRec.wo = Warp::squareToCosineHemisphere(sample);
        } else {
            sample = Point2f((_sample.x() - diffuse) /  (1.f - diffuse), _sample.y());
            float GTR2 = 1.f / (1.f + m_clearcoat);

            Point2f sample1;
            Vector3f wh;
            if (sample.x() < GTR2) {
                sample1 = Point2f(sample.x() / GTR2, sample.y());
                float alpha = std::max(0.01f, m_roughness * m_roughness);
                wh = Warp::squareToGTR2(sample1, alpha);
            } else {
                sample1 = Point2f((sample.x() - GTR2) / (1.f - GTR2), sample.y());
                wh = Warp::squareToGTR1(sample1, lerp(m_clearcoatGloss, .1f, .001f));
            }
            bRec.wo = ((2.f * bRec.wi.dot(wh) * wh) - bRec.wi).normalized();
        }
        float cosTheta = Frame::cosTheta(bRec.wo);
        if (cosTheta <= 0.f)
            return 0;

        return eval(bRec) * cosTheta / pdf(bRec);
    }

    virtual std::string toString() const override {
        return tfm::format(
                "DisneyBRDF[\n"
                "  baseColor = %f %f %f,\n"
                "  metallic = %f,\n"
                "  specular = %f,\n"
                "  specularTint = %f,\n"
                "  roughness = %f\n"
                "  clearcoat = %f\n"
                "  clearcoatGloss = %f\n"
                "]",
                m_baseColor.x(), m_baseColor.y(), m_baseColor.z(),m_metallic, m_specular, m_specularTint, m_roughness, m_clearcoat, m_clearcoatGloss
        );
    }

private:
    Color3f m_baseColor;
    float m_metallic, m_specular, m_specularTint, m_roughness, m_clearcoat, m_clearcoatGloss;

};

NORI_REGISTER_CLASS(DisneyBRDF, "disney");
NORI_NAMESPACE_END