/*
    This file is part of Nori, a simple educational ray tracer

    Copyright (c) 2015 by Wenzel Jakob

    Nori is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License Version 3
    as published by the Free Software Foundation.

    Nori is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include <nori/warp.h>
#include <nori/vector.h>
#include <nori/frame.h>

NORI_NAMESPACE_BEGIN

Vector3f Warp::sampleUniformHemisphere(Sampler *sampler, const Normal3f &pole) {
    // Naive implementation using rejection sampling
    Vector3f v;
    do {
        v.x() = 1.f - 2.f * sampler->next1D();
        v.y() = 1.f - 2.f * sampler->next1D();
        v.z() = 1.f - 2.f * sampler->next1D();
    } while (v.squaredNorm() > 1.f);

    if (v.dot(pole) < 0.f)
        v = -v;
    v /= v.norm();

    return v;
}

Point2f Warp::squareToUniformSquare(const Point2f &sample) {
    return sample;
}

float Warp::squareToUniformSquarePdf(const Point2f &sample) {
    return ((sample.array() >= 0.f).all() && (sample.array() <= 1.f).all()) ? 1.f : 0.f;
}

Point2f Warp::squareToUniformDisk(const Point2f &sample) {
    float r = sqrt(sample.x());
    float phi = 2.f * M_PI * sample.y();
    return {r * cos(phi), r * sin(phi)};
}

float Warp::squareToUniformDiskPdf(const Point2f &p) {
    return (p.norm() < 1.f) * INV_PI;
}

Vector3f Warp::squareToUniformCylinder(const Point2f &sample) {
    float phi = 2.f * M_PI * sample.y();
    return {cos(phi), sin(phi), 2.f * sample.x() - 1.f};
}

Vector3f Warp::squareToUniformSphereCap(const Point2f &sample, float cosThetaMax) {
    Vector3f cylinder = squareToUniformCylinder(sample);
    float h = (1 - cosThetaMax);
    float z = abs(cylinder.z() * h) + cosThetaMax;
    float r = sqrt(1.f - pow(z,2));
    return {r * cylinder.x(), r * cylinder.y(), z};
}

float Warp::squareToUniformSphereCapPdf(const Vector3f &v, float cosThetaMax) {
    if (abs(v.norm() - 1.f) < Epsilon && v.z() >= cosThetaMax)
        return 0.5f * INV_PI / (1 - cosThetaMax);
    return 0.f;
}

Vector3f Warp::squareToUniformSphere(const Point2f &sample) {
    Vector3f cylinder = squareToUniformCylinder(sample);
    float r = sqrt(1.f - pow(cylinder.z(),2));
    return {r * cylinder.x(), r * cylinder.y(), cylinder.z()};
}

float Warp::squareToUniformSpherePdf(const Vector3f &v) {
    return (abs(v.norm() - 1.f) < Epsilon) * INV_PI * 0.25f;
}

Vector3f Warp::squareToUniformHemisphere(const Point2f &sample) {
    Vector3f sphere = squareToUniformSphere(sample);
    return {sphere.x(), sphere.y(), abs(sphere.z())};
}

float Warp::squareToUniformHemispherePdf(const Vector3f &v) {
    return (abs(v.norm() - 1.f) < Epsilon && v.z() >= 0) * INV_PI * 0.5f;
}

Vector3f Warp::squareToCosineHemisphere(const Point2f &sample) {
    float theta = acos(sqrt(1 - sample.x()));
    float phi = 2.f * M_PI * sample.y();
    return {sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta)};
}

float Warp::squareToCosineHemispherePdf(const Vector3f &v) {
    return (abs(v.norm() - 1) < Epsilon && v.z() >= 0) * INV_PI * v.z();
}

Vector3f Warp::squareToBeckmann(const Point2f &sample, float alpha) {
    float theta = atan(sqrt(-pow(alpha, 2) * log(1-sample.x())));
    float phi = 2 * M_PI * sample.y();
    return {sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta)};
}

float Warp::squareToBeckmannPdf(const Vector3f &m, float alpha) {
    float theta = acos(m.z());
    return (abs(m.norm() - 1) < Epsilon && m.z() >= 0) * exp(-pow(tan(theta), 2)/ pow(alpha, 2)) / (M_PI * pow(alpha, 2) * pow(cos(theta), 3));
}

Vector3f Warp::squareToUniformTriangle(const Point2f &sample) {
    float su1 = sqrtf(sample.x());
    float u = 1.f - su1, v = sample.y() * su1;
    return Vector3f(u,v,1.f-u-v);
}

NORI_NAMESPACE_END
