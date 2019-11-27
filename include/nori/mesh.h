/*
    This file is part of Nori, a simple educational ray tracer

    Copyright (c) 2015 by Wenzel Jakob, Romain Prévost

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

#if !defined(__NORI_MESH_H)
#define __NORI_MESH_H

#include <nori/shape.h>
#include <nori/dpdf.h>

NORI_NAMESPACE_BEGIN

/**
 * \brief Triangle mesh
 *
 * This class stores a triangle mesh object and provides numerous functions
 * for querying the individual triangles. Subclasses of \c Mesh implement
 * the specifics of how to create its contents (e.g. by loading from an
 * external file)
 */
class Mesh : public Shape {
public:
    /// Initialize internal data structures (called once by the XML parser)
    virtual void activate() override;

    /// Return the total number of triangles in this shape
    virtual uint32_t getPrimitiveCount() const override { return (uint32_t) m_F.cols(); }

    //// Return an axis-aligned bounding box containing the given triangle
    virtual BoundingBox3f getBoundingBox(uint32_t index) const override;

    //// Return the centroid of the given triangle
    virtual Point3f getCentroid(uint32_t index) const override;

    /** \brief Ray-triangle intersection test
     *
     * Uses the algorithm by Moeller and Trumbore discussed at
     * <tt>http://www.acm.org/jgt/papers/MollerTrumbore97/code.html</tt>.
     *
     * Note that the test only applies to a single triangle in the mesh.
     * An acceleration data structure like \ref BVH is needed to search
     * for intersections against many triangles.
     *
     * \param index
     *    Index of the triangle that should be intersected
     * \param ray
     *    The ray segment to be used for the intersection query
     * \param t
     *    Upon success, \a t contains the distance from the ray origin to the
     *    intersection point,
     * \param u
     *   Upon success, \c u will contain the 'U' component of the intersection
     *   in barycentric coordinates
     * \param v
     *   Upon success, \c v will contain the 'V' component of the intersection
     *   in barycentric coordinates
     * \return
     *   \c true if an intersection has been detected
     */
    virtual bool rayIntersect(uint32_t index, const Ray3f &ray, float &u, float &v, float &t) const override;

    /// Set intersection information: hit point, shading frame, UVs
    virtual void setHitInformation(uint32_t index, const Ray3f &ray, Intersection & its) const override;

    /// Return the total number of vertices in this shape
    uint32_t getVertexCount() const { return (uint32_t) m_V.cols(); }

    /**
     * \brief Uniformly sample a position on the mesh with
     * respect to surface area.
     */
    virtual void sampleSurface(ShapeQueryRecord & sRec, const Point2f & sample) const override;
    virtual float pdfSurface(const ShapeQueryRecord & sRec) const override;

    /// Return the surface area of the given triangle
    float surfaceArea(uint32_t index) const;

    Point3f getInterpolatedVertex(uint32_t index, const Vector3f & bc) const;
    Normal3f getInterpolatedNormal(uint32_t index, const Vector3f & bc) const;

    /// Return a pointer to the vertex positions
    const MatrixXf &getVertexPositions() const { return m_V; }

    /// Return a pointer to the vertex normals (or \c nullptr if there are none)
    const MatrixXf &getVertexNormals() const { return m_N; }

    /// Return a pointer to the texture coordinates (or \c nullptr if there are none)
    const MatrixXf &getVertexTexCoords() const { return m_UV; }

    /// Return a pointer to the triangle vertex index list
    const MatrixXu &getIndices() const { return m_F; }


    /// Return the name of this mesh
    const std::string &getName() const { return m_name; }

    /// Return a human-readable summary of this instance
    virtual std::string toString() const override;

protected:
    /// Create an empty mesh
    Mesh();

protected:
    std::string m_name;                  ///< Identifying name
    MatrixXf      m_V;                   ///< Vertex positions
    MatrixXf      m_N;                   ///< Vertex normals
    MatrixXf      m_UV;                  ///< Vertex texture coordinates
    MatrixXu      m_F;                   ///< Faces

    DiscretePDF m_pdf;
};

NORI_NAMESPACE_END

#endif /* __NORI_MESH_H */
