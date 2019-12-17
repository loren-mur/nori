//
// Created by Niklaus on 12.12.19.
//

#include <nori/medium.h>



NORI_NAMESPACE_BEGIN

Medium::Medium(const PropertyList &props) {
    m_sigmaA = (props.getColor("sigma_a", Color3f(0.2f)));
    m_sigmaS = (props.getColor("sigma_s", Color3f(0.2f)));
    m_sigmaT = m_sigmaA + m_sigmaS;
    m_albedo = m_sigmaS / m_sigmaT;
    m_density_function = props.getInteger("density_function", 1);
    Vector3f dims = props.getVector3("dimensions", Vector3f(0.4)).cwiseAbs();
    Vector3f origin = props.getVector3("origin", Vector3f(0.f));
    m_maxDensity = std::max(0.0f, props.getFloat("max_density", 1.f));
    m_invDensityMax = m_maxDensity > Epsilon  ? 1.f / m_maxDensity : 0.f;

    bounds = BoundingBox3f(origin - dims, origin + dims);
}

//Homogeneous transmission
Color3f Medium::Tr(const Point3f &a, const Point3f &b) const {
    float norm = (a - b).norm();
    return {exp(-m_sigmaT.x() * norm), exp(-m_sigmaT.y() * norm), exp(-m_sigmaT.z() * norm)};
}

//Heterogeneous transmission
Color3f Medium::Tr(const Ray3f &ray, Sampler *sampler, MediumInteractionQuery &mi) const {
    //reference from here https://www.csie.ntu.edu.tw/~cyy/courses/rendering/09fall/lectures/handouts/chap17_volume_4up.pdf
    float td = Epsilon;
    float transmission = 1.0f;

    while (true) {
        td -= log(1.f - sampler->next1D()) * m_invDensityMax / m_sigmaT.maxCoeff();
        if (td >= mi.tMax || !bounds.rayIntersect(Ray3f(ray(td), ray.d)))
            break;

        float density = getDensity(ray(td));
        transmission *= 1.f - std::max(0.0f, density * m_invDensityMax);
    }
    return {transmission};
}

//sample a heterogeneous medium interaction
Color3f Medium::sample(const Ray3f &ray, Sampler *sampler, MediumInteractionQuery &mi) const {
    float td = Epsilon;
    //delta tracking iterations to sample a medium interaction
    //reference from here https://www.csie.ntu.edu.tw/~cyy/courses/rendering/09fall/lectures/handouts/chap17_volume_4up.pdf
    while (true) {

        td -= log(1.f - sampler->next1D()) * m_invDensityMax / m_sigmaT.maxCoeff();
        if (td >= mi.tMax || !bounds.rayIntersect(Ray3f(ray(td), ray.d))) {
            mi.isValid = false;
            break;
        }

        float density = getDensity(ray(td));
        if (density * m_invDensityMax > sampler->next1D()) {
            mi.isValid = true;
            mi.p = ray(td);
            return m_albedo * density;
        }

    }
    return {1.f};
}

void Medium::addChild(NoriObject *child) {
    switch (child->getClassType()) {
        case EPhaseFunction:
            if (m_phaseFunction) {
                throw NoriException("Medium: tried register multiple phaseFunctions");
            }
            m_phaseFunction = static_cast<PhaseFunction*>(child);
            break;
        default:
            throw NoriException("Medium: addChild is not supported other than phaseFunction");
    }
}


float Medium::getDensity(const Point3f &p) const {
    if (bounds.contains(p)) {
        switch(m_density_function) {
            case 1:
                return m_maxDensity;
            case 2:
                //std::cout << p.z() << " " << bounds.min.z() << " " << bounds.getExtents().z() << " " << 10 * std::max(0.0f, (p.z()-bounds.min.z()) / bounds.getExtents().z()) << std::endl;
                return m_maxDensity * exp( - 2 * std::max(0.0f, (p.z()-bounds.min.z()) / bounds.getExtents().z()));
            default:
                return m_maxDensity;
        }
    }
    else {
        return 0.0f;
    }
}

std::string Medium::toString() const {
    return "medium";
}

NORI_REGISTER_CLASS(Medium, "medium")
NORI_NAMESPACE_END