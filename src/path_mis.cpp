//
// Created by Niklaus on 11.11.19.
//
#include <nori/integrator.h>
#include <nori/scene.h>
#include <nori/bsdf.h>
#include <nori/sampler.h>


NORI_NAMESPACE_BEGIN

class PathMisIntegrator : public Integrator {
public:
    PathMisIntegrator(const PropertyList &props) {}

    Color3f Li(const Scene *scene, Sampler *sampler, const Ray3f &ray) const override {
        Color3f color = 0;
        Color3f t = 1;
        Ray3f rayRecursive = ray;
        float probability;
        float w_mats = 1.0f;

        Intersection its;
        if (!scene->rayIntersect(rayRecursive, its))
            return color;

        while (true) {
            // emitted
            if (its.mesh->isEmitter())
            {
                EmitterQueryRecord lRecE(rayRecursive.o, its.p, its.shFrame.n);
                color += t * w_mats * its.mesh->getEmitter()->eval(lRecE);
            }

            //EMS
            const Emitter *light = scene->getRandomEmitter(sampler->next1D());
            EmitterQueryRecord lRec(its.p);
            Color3f Li = light->sample(lRec, sampler->next2D()) * scene->getLights().size();
            float pdf_em = light->pdf(lRec);
            if (!scene->rayIntersect(lRec.shadowRay)) //intersection point is occluded
            {
                //cosine
                float cosTheta = std::max(0.f, Frame::cosTheta(its.shFrame.toLocal(lRec.wi)));

                //BSDF
                BSDFQueryRecord bRec(its.toLocal(-rayRecursive.d), its.toLocal(lRec.wi), ESolidAngle);
                bRec.uv = its.uv;
                Color3f f = its.mesh->getBSDF()->eval(bRec);
                float pdf_mat = its.mesh->getBSDF()->pdf(bRec);

                float w_ems = pdf_mat + pdf_em > 0.f ? pdf_em / (pdf_mat + pdf_em) : pdf_em;

                color += Li * f * cosTheta * w_ems * t;
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

            //New mats weight
            float pdf_mat = its.mesh->getBSDF()->pdf(bRec);

            Point3f origin = its.p;
            if (!scene->rayIntersect(rayRecursive, its))
                return color;

            if (its.mesh->isEmitter()) {
                EmitterQueryRecord lRec = EmitterQueryRecord(origin, its.p, its.shFrame.n);
                float pdf_em = its.mesh->getEmitter()->pdf(lRec);
                w_mats = pdf_mat + pdf_em > 0.f ? pdf_mat / (pdf_mat + pdf_em) : pdf_mat;
            }

            if (bRec.measure == EDiscrete)
                w_mats = 1.0f;
        }

        return color;
    }

    std::string toString() const {
        return "PathMisIntegrator[]";
    }
};

NORI_REGISTER_CLASS(PathMisIntegrator, "path_mis");
NORI_NAMESPACE_END


