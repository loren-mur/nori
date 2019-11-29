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

#if !defined(__NORI_EMITTER_H)
#define __NORI_EMITTER_H

#include <nori/object.h>

NORI_NAMESPACE_BEGIN

struct Intersection;
/**
 * \brief Data record for conveniently querying and sampling the
 * direct illumination technique implemented by a emitter
 */
struct EmitterQueryRecord {
    /// Origin point from which we sample the emitter
    Point3f ref;
    /// Sampled point on the emitter
    Point3f p;
    /// Normal at the emitter point
    Normal3f n;
    /// Direction between the hit point and the emitter point
    Vector3f wi;
    /// Probability
    float pdf;
    /// Shadow ray
    Ray3f shadowRay;

    /// Create an unitialized query record
    EmitterQueryRecord() { }

    /// Create a new query record that can be used to sample a emitter
    EmitterQueryRecord(const Point3f &ref) : ref(ref) { }

    /**
     * \brief Create a query record that can be used to query the
     * sampling density after having intersected an area emitter
     */
    EmitterQueryRecord(const Point3f &ref, const Point3f &p, const Normal3f &n) :
        ref(ref), p(p), n(n) {
        wi = (p - ref).normalized();
    }
};

/**
 * \brief Superclass of all emitters
 */
class Emitter : public NoriObject {
public:

    /**
     * \brief Sample the emitter and return the importance weight (i.e. the
     * value of the Emitter divided by the probability density
     * of the sample with respect to solid angles).
     *
     * \param lRec    An emitter query record (only ref is needed)
     * \param sample  A uniformly distributed sample on \f$[0,1]^2\f$
     *
     * \return The emitter value divided by the probability density of the sample.
     *         A zero value means that sampling failed.
     */
    virtual Color3f sample(EmitterQueryRecord &lRec, const Point2f &sample) const = 0;

    /**
     * \brief Evaluate the emitter
     *
     * \param lRec
     *     A record with detailed information on the emitter query
     * \return
     *     The emitter value, evaluated for each color channel
     */
    virtual Color3f eval(const EmitterQueryRecord &lRec) const = 0;

    /**
     * \brief Compute the probability of sampling \c lRec.p.
     *
     * This method provides access to the probability density that
     * is realized by the \ref sample() method.
     *
     * \param lRec
     *     A record with detailed information on the emitter query
     *
     * \return
     *     A probability/density value
     */
    virtual float pdf(const EmitterQueryRecord &lRec) const = 0;


    /// Sample a photon
    virtual Color3f samplePhoton(Ray3f &ray, const Point2f &sample1, const Point2f &sample2) const {
        throw NoriException("Emitter::samplePhoton(): not implemented!");
    }


    /**
     * \brief Virtual destructor
     * */
    virtual ~Emitter() {}

    /**
     * \brief Return the type of object (i.e. Mesh/Emitter/etc.)
     * provided by this instance
     * */
    virtual EClassType getClassType() const override { return EEmitter; }

    /**
     * \brief Set the shape if the emitter is attached to a shape
     * */
    void setShape(Shape * shape) { m_shape = shape; }

protected:
    /// Pointer to the shape if the emitter is attached to a shape
    Shape * m_shape = nullptr;

};

NORI_NAMESPACE_END

#endif /* __NORI_EMITTER_H */
