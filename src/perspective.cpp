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

#include <nori/camera.h>
#include <nori/rfilter.h>
#include <nori/warp.h>
#include <Eigen/Geometry>
#include <nori/sampler.h>
#include <nori/texture.h>
#include <iostream>
#include <algorithm>
#include <fstream>


NORI_NAMESPACE_BEGIN

/**
 * \brief Perspective camera with depth of field
 *
 * This class implements a simple perspective camera model. It uses an
 * infinitesimally small aperture, creating an infinite depth of field.
 */

class PerspectiveCamera : public Camera {
public:
    PerspectiveCamera(const PropertyList &propList) {
        /* Width and height in pixels. Default: 720p */
        m_outputSize.x() = propList.getInteger("width", 1280);
        m_outputSize.y() = propList.getInteger("height", 720);
        m_invOutputSize = m_outputSize.cast<float>().cwiseInverse();

        /* Specifies an optional camera-to-world transformation. Default: none */
        m_cameraToWorld = propList.getTransform("toWorld", Transform());

        /* Horizontal field of view in degrees */
        m_fov = propList.getFloat("fov", 30.0f);

        /* Near and far clipping planes in world-space units */
        m_nearClip = propList.getFloat("nearClip", 1e-4f);
        m_farClip = propList.getFloat("farClip", 1e4f);

        lensRadius = propList.getFloat("lensRadius", 0);
        focalDistance = propList.getFloat("focalDistance", 12);
        m_distortion = propList.getInteger("distortion", 0);
        k1 = propList.getFloat("change1", 0.0f);
        k2 = propList.getFloat("change2", 0.0f);
        hasaberration = propList.getInteger("hasaberration", 0);



        weightvector = propList.getVector3("aberration", Vector3f(0));

        m_rfilter = NULL;

        //focalDistance = propList.getFloat("focalDistance", 10.0f);
        //lensRadius = propList.getFloat("lensRadius", 0.0f); // per default, no effect
    }

    virtual void activate() override {
        float aspect = m_outputSize.x() / (float) m_outputSize.y();

        /* Project vectors in camera space onto a plane at z=1:
         *
         *  xProj = cot * x / z
         *  yProj = cot * y / z
         *  zProj = (far * (z - near)) / (z * (far-near))
         *  The cotangent factor ensures that the field of view is 
         *  mapped to the interval [-1, 1].
         */

        float recip = 1.0f / (m_farClip - m_nearClip),
                cot = 1.0f / std::tan(degToRad(m_fov / 2.0f));


        Eigen::Matrix4f perspective;
        perspective <<
            cot, 0,   0,   0,
            0, cot,   0,   0,
            0,   0,   m_farClip * recip, -m_nearClip * m_farClip * recip,
            0,   0,   1,   0;

        /**
         * Translation and scaling to shift the clip coordinates into the
         * range from zero to one. Also takes the aspect ratio into account.
         */
        m_sampleToCamera = Transform( 
            Eigen::DiagonalMatrix<float, 3>(Vector3f(0.5f, -0.5f * aspect, 1.0f)) *
            Eigen::Translation<float, 3>(1.0f, -1.0f/aspect, 0.0f) * perspective).inverse();

        /* If no reconstruction filter was assigned, instantiate a Gaussian filter */
        if (!m_rfilter) {
            m_rfilter = static_cast<ReconstructionFilter *>(
                    NoriObjectFactory::createInstance("gaussian", PropertyList()));
            m_rfilter->activate();
        }
    }


    Color3f sampleRay(Ray3f &ray,const Point2f &samplePosition,const Point2f &apertureSample, int index) const {
        /* Compute the corresponding position on the 
           near plane (in local camera space) */

       Point3f nearP;
       Color3f color;

        Sampler *sampler = static_cast<Sampler*>(
                NoriObjectFactory::createInstance("independent", PropertyList()));
        sampler->activate();

        nearP = m_sampleToCamera * Point3f(samplePosition.x() * m_invOutputSize.x(),samplePosition.y() * m_invOutputSize.y(), 0.0f);


        if(m_distortion) {
            float distort = calculateDistortion(Vector2f(nearP.x() / nearP.z(), nearP.y() / nearP.z()).norm());
            nearP.x() *= distort;
            nearP.y() *= distort;
        }
        /* Turn into a normalized ray direction, and
               adjust the ray interval accordingly */

        float weight = 0;
        if(hasaberration){
            weight = weightvector[index];
            //cout << "   " << weight;
            color = Color3f(0);
            color[index] = 1.0;
        }
        else{
            color =  Color3f(1.0f);
        }

        Vector3f d = nearP.normalized();
        float invZ = 1.0f / d.z();
        ray.o = m_cameraToWorld * Point3f(0, 0, 0);
        ray.d = m_cameraToWorld * d;

        if((hasaberration)||(lensRadius > 0)){
            //aberration
            //compute point on plane of focus
            //calculate t
            Point2f pLens = lensRadius * Warp::squareToUniformDisk(apertureSample);
            float tf = focalDistance / d.z();
            Point3f pFocus = d * tf;
            Point2f changedSamplePosition = Point2f(samplePosition.x() - 0.5*m_outputSize.x(), samplePosition.y() - 0.5*m_outputSize.y()); //zero centered
            changedSamplePosition /= m_outputSize.maxCoeff(); //the maximum between the two sizes (the same)

            Point2f delta = changedSamplePosition * changedSamplePosition.squaredNorm() * weight;
            pFocus = pFocus + Point3f(-delta.x(), delta.y(), 0);


            //dof
            //update ray for effect of lens
            Point3f o(pLens.x(), pLens.y(), 0);
            ray.o = m_cameraToWorld * o;
            d = (pFocus - o).normalized();
            ray.d = m_cameraToWorld * d;
        }


        ray.mint = m_nearClip * invZ;
        ray.maxt = m_farClip * invZ;
        ray.update();

        return color;
    }


    int hasChromatic() const {
        return hasaberration;
    }




    float calculateDistortion(float y) const{
        int iteration = 0;

        float r = y;
        while (true) {
            float r_squared = pow(r,2);
            float f  = r *(1+ k1 * r_squared + k2 * pow(r_squared,2)) - y;
            float df = 1 + 3 * k1 * r_squared + 5 * k2 * pow(r_squared,2);

            r = r - f / df;
            if (std::abs(f) < 1e-6 || iteration++ > 4)
                break;
        }
        return r/y;
    }

    virtual void addChild(NoriObject *obj) override {
        switch (obj->getClassType()) {
            case EReconstructionFilter:
                if (m_rfilter)
                    throw NoriException("Camera: tried to register multiple reconstruction filters!");
                m_rfilter = static_cast<ReconstructionFilter *>(obj);
                break;

            default:
                throw NoriException("Camera::addChild(<%s>) is not supported!",
                    classTypeName(obj->getClassType()));
        }
    }

    /// Return a human-readable summary
    virtual std::string toString() const override {
        return tfm::format(
            "PerspectiveCamera[\n"
            "  cameraToWorld = %s,\n"
            "  outputSize = %s,\n"
            "  fov = %f,\n"
            "  clip = [%f, %f],\n"
            "  rfilter = %s\n"
            "  focalDistance = %s,\n"
            "  lensRadius = %s\n"
            "]",
            indent(m_cameraToWorld.toString(), 18),
            m_outputSize.toString(),
            m_fov,
            m_nearClip,
            m_farClip,
            indent(m_rfilter->toString()),
            focalDistance,
            lensRadius
        );
    }


    //method: has chromatic
private:
    Vector2f m_invOutputSize;
    Transform m_sampleToCamera;
    Transform m_cameraToWorld;
    float m_fov;
    float m_nearClip;
    float m_farClip;
    //dof parameters
    float lensRadius;
    float focalDistance;
    //distorsion parameters
    bool m_distortion;
    int hasaberration;
    float k1,k2;
    //chromatic aberration parameters
    //std::shared_ptr<Texture> _aperture;
    Vector3f m_aberration;
    Vector3f weightvector;

    //std::shared_ptr<Texture> _aperture;
    //float _apertureSize;
};

NORI_REGISTER_CLASS(PerspectiveCamera, "perspective");
NORI_NAMESPACE_END
