//
// Created by Niklaus on 28.10.19.
//

#include <nori/integrator.h>
#include <nori/scene.h>
#include <nori/bsdf.h>
#include <nori/sampler.h>


NORI_NAMESPACE_BEGIN

class DirectMatsIntegrator : public Integrator {
public:
    DirectMatsIntegrator(const PropertyList &props) {}

    Color3f Li(const Scene *scene, Sampler *sampler, const Ray3f &ray) const override {
        Intersection its;
        Color3f color = 0;

        if (!scene->rayIntersect(ray, its))
            return color;

        //emitted
        Color3f Le = 0;
        if (its.mesh->isEmitter())
        {
            EmitterQueryRecord lRecE(ray.o, its.p, its.shFrame.n);
            Le = its.mesh->getEmitter()->eval(lRecE);
        }

        //BSDF
        BSDFQueryRecord bRec(its.shFrame.toLocal(-ray.d));
        bRec.uv = its.uv;
        Color3f f = its.mesh->getBSDF()->sample(bRec, sampler->next2D());


        //reflected
        Color3f Li = 0;
        Ray3f rayR = Ray3f(its.p, its.shFrame.toWorld(bRec.wo));
        Intersection itsR;
        if (scene->rayIntersect(rayR, itsR))
        {
            if (itsR.mesh->isEmitter())
            {
                EmitterQueryRecord lRec = EmitterQueryRecord(its.p, itsR.p, itsR.shFrame.n);
                Li = itsR.mesh->getEmitter()->eval(lRec);
            }
        }

        return Le + Li * f;
    }

    std::string toString() const {
        return "DirectMatsIntegrator[]";
    }
};

NORI_REGISTER_CLASS(DirectMatsIntegrator, "direct_mats");
NORI_NAMESPACE_END//
// Created by Niklaus on 28.10.19.
//

