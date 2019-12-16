//
// Created by Niklaus on 11.12.19.
//
#pragma once

#include <nori/object.h>
#include <nori/bbox.h>
#include <nori/PhaseFunction.h>
#include <nori/sampler.h>

NORI_NAMESPACE_BEGIN

struct MediumInteractionQuery {
        Point3f p; //Interaction point

        //max free path
        float tMax;

        //used to pass back to the integrator if sampling was successful or not.
        bool isValid;

        MediumInteractionQuery() : tMax(0), isValid(false) {}

};

//homogeneous medium

class Medium : public NoriObject {
public:
    Medium(const PropertyList &props);
    Color3f Tr(const Point3f &a, const Point3f &b) const;
    Color3f Tr(const Ray3f &ray, Sampler *sampler, MediumInteractionQuery &mi) const;
    Color3f sample(const Ray3f &ray, Sampler *sampler, MediumInteractionQuery &mi) const;
    Color3f get_sigmaA() const { return m_sigmaA; }
    Color3f get_sigmaS() const { return m_sigmaS; }
    Color3f get_sigmaT() const { return m_sigmaT; }
    Color3f get_albedo() const { return m_albedo; }

    PhaseFunction* get_phaseFunction() const { return m_phaseFunction; }

    void addChild(NoriObject *child) override;
    float getDensity(const Point3f &p) const;

    std::string toString() const override;

    EClassType  getClassType() const override { return EMedium; }

private:
    Color3f m_sigmaA;
    Color3f m_sigmaS;
    Color3f m_sigmaT;
    Color3f m_albedo;

    float m_invDensityMax;
    PhaseFunction * m_phaseFunction = nullptr;

    float m_maxDensity;
    int m_density_function;

    BoundingBox3f bounds;
};


NORI_NAMESPACE_END