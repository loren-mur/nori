//
// Created by Niklaus on 11.12.19.
//

#include <nori/PhaseFunction.h>

NORI_NAMESPACE_BEGIN

PhaseFunction::PhaseFunction() = default;
PhaseFunction::PhaseFunction(const PropertyList &props) {}

float PhaseFunction::sample_p(const Vector3f &wo, Vector3f &wi, const Point2f &sample) const {
    wi = Warp::squareToUniformSphere(sample);
    return INV_FOURPI;
}

std::string PhaseFunction::toString() const {
    return tfm::format("[ Isotropic ]");
}


// reference from here https://www.csie.ntu.edu.tw/~cyy/courses/rendering/09fall/lectures/handouts/chap17_volume_4up.pdf
HenyeyGreenstein::HenyeyGreenstein(const PropertyList &props) : m_g(props.getFloat("g", 0.0f)) {}

float HenyeyGreenstein::sample_p(const Vector3f &wo, Vector3f &wi, const Point2f &sample) const {
    float cosTheta;
    float m_g2 = m_g * m_g;
    if (abs(m_g) < 1e-3) {
        cosTheta = 1.f - 2.f * sample.x();
    } else {
        float t = (1.f - m_g2) / (1.f - m_g + 2.f * m_g * sample.x());
        cosTheta = -(1.f + m_g2 - t * t) / abs(2.f * m_g);
    }

    float sinTheta = sqrt(std::max(0.f, 1.f - cosTheta * cosTheta));
    float phi = 2.f * M_PI * sample.y();

    Vector3f v1;
    if (abs(wo.x()) > abs(wo.y()))
        v1 = Vector3f(-wo.z(), 0, wo.x() / sqrt(wo.x() * wo.x() + wo.z() * wo.z()));
    else
        v1 = Vector3f(0, wo.z(), -wo.y() / sqrt(wo.y() * wo.y() + wo.z() * wo.z()));

    Vector3f v2(wo.y() * v1.z() - wo.z() * v1.y(),
                wo.z() * v1.x() - wo.x() * v1.z(),
                wo.x() * v1.y() - wo.y() * v1.x());

    wi = sinTheta * cos(phi) * v1 + sinTheta * sin(phi) * v2 + cosTheta * -wo;

    return INV_FOURPI * (1.f - m_g2) / pow(1.f + m_g2 - 2.f * m_g * cosTheta, 1.5);
}

std::string HenyeyGreenstein::toString() const {
    return tfm::format("[ HenyeyGreenstein g: %f ]", m_g);
}

NORI_REGISTER_CLASS(PhaseFunction, "iso");
NORI_REGISTER_CLASS(HenyeyGreenstein, "henyey");
NORI_NAMESPACE_END


