//
// Created by Niklaus on 28.10.19.
//

#include <nori/integrator.h>
#include <nori/scene.h>
#include <nori/bsdf.h>
#include <nori/sampler.h>


NORI_NAMESPACE_BEGIN

class PathMatsIntegrator : public Integrator {
public:
    PathMatsIntegrator(const PropertyList &props) {}

    Color3f Li(const Scene *scene, Sampler *sampler, const Ray3f &ray) const override {
        Color3f color = 0;
        Color3f t = 1;
        Ray3f rayRecursive = ray;
        float probability;

        while (true)
        {
            Intersection its;
            if (!scene->rayIntersect(rayRecursive, its))
                return color;

            //contribute emitted
            if (its.mesh->isEmitter())
            {
                EmitterQueryRecord lRecE(rayRecursive.o, its.p, its.shFrame.n);
                color += t * its.mesh->getEmitter()->eval(lRecE);
            }

            //Russian roulettio
            probability = std::min(t.x(), 0.99f);
            if (sampler->next1D() > probability)
                return color;

            t /= probability;


            //BSDF
            BSDFQueryRecord bRec(its.shFrame.toLocal(-rayRecursive.d));
            bRec.uv = its.uv;
            Color3f f = its.mesh->getBSDF()->sample(bRec, sampler->next2D());
            t *= f;

           //continue recursion
           rayRecursive = Ray3f(its.p, its.toWorld(bRec.wo));
        }


        return color;
    }

    std::string toString() const {
        return "PathMatsIntegrator[]";
    }
};

NORI_REGISTER_CLASS(PathMatsIntegrator, "path_mats");
NORI_NAMESPACE_END


