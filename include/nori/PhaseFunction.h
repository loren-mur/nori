//
// Created by Niklaus on 11.12.19.
//
#pragma once

#include <nori/object.h>

NORI_NAMESPACE_BEGIN

class PhaseFunction : public NoriObject {
    public:
        virtual float p(const Vector3f &wo, const Vector3f &wi) const = 0;
        virtual float sample_p(const Vector3f &wo, Vector3f &wi, const Point2f &sample) const = 0;

        std::string toString() const override = 0;

        EClassType  getClassType() const override { return EPhaseFunction; }

};

class HenyeyGreenstein : public PhaseFunction {
    public:
        HenyeyGreenstein(const PropertyList &props);

        float getG() { return m_g; }

        virtual float p(const Vector3f &wo, const Vector3f &wi) const;
        virtual float sample_p(const Vector3f &wo, Vector3f &wi, const Point2f &sample) const;

        std::string toString() const override;

        EClassType  getClassType() const override { return EPhaseFunction; }
private:
    float m_g;
};

NORI_NAMESPACE_END