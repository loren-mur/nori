//
// Created by Niklaus on 28.10.19.
//

#include <nori/integrator.h>
#include <nori/scene.h>
#include <nori/bsdf.h>
#include <nori/sampler.h>


NORI_NAMESPACE_BEGIN

class DirectEmsIntegrator : public Integrator {
public:
    DirectEmsIntegrator(const PropertyList &props) {}

    Color3f Li(const Scene *scene, Sampler *sampler, const Ray3f &ray) const override {
        Intersection its;
        Color3f color = 0;

        if (!scene->rayIntersect(ray, its))
            return color;

        //emitted
        if (its.mesh->isEmitter())
        {
            EmitterQueryRecord lRecE(ray.o, its.p, its.shFrame.n);
            color += its.mesh->getEmitter()->eval(lRecE);
        }


        std::vector<Emitter *> lights = scene->getLights();

        for (auto light : lights)
        {
            //reflected
            EmitterQueryRecord lRec;
            lRec.ref = its.p;
            Color3f Li = light->sample(lRec, sampler->next2D());
            if (scene->rayIntersect(lRec.shadowRay)) //intersection point is occluded
                Li = 0;

            //cosine
            float cosTheta = Frame::cosTheta(its.shFrame.toLocal(lRec.wi));

            //BSDF
            BSDFQueryRecord bRec(its.toLocal(-ray.d), its.toLocal(lRec.wi), ESolidAngle);
            bRec.uv = its.uv;
            Color3f f = its.mesh->getBSDF()->eval(bRec);

            color += Li * f * cosTheta;
        }

        return color;
    }

    std::string toString() const {
        return "DirectEmsIntegrator[]";
    }
};

NORI_REGISTER_CLASS(DirectEmsIntegrator, "direct_ems");
NORI_NAMESPACE_END