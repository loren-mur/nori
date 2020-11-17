//
// Created by Niklaus on 11.11.19.
//
#include <nori/integrator.h>
#include <nori/medium.h>
#include <nori/warp.h>
#include <nori/PhaseFunction.h>


NORI_NAMESPACE_BEGIN

class VolPathIntegrator : public Integrator {
public:
    VolPathIntegrator(const PropertyList &props) {}

    Color3f Li(const Scene *scene, Sampler *sampler, const Ray3f &ray) const override {

        Color3f result(0);
        Ray3f rayRecursive = ray;
        Color3f t(1);

        float probability;
        Medium *medium = scene->getMedia()[0];
        float w_mats = 1.f;
        Intersection its;

        bool hasIntersected = scene->rayIntersect(rayRecursive, its);

        while (true) {
            float tmax;

            if (hasIntersected)
                tmax = (its.p - rayRecursive.o).norm();
            else
                tmax = its.t;

            //sample mean free path
            MediumInteractionQuery mi;
            mi.tMax = tmax;
            Color3f mediumColor = medium->sample(rayRecursive, sampler, mi);

            if (mi.isValid) {

                Vector3f wo;
                float pdf_mat = medium->get_phaseFunction()->sample_p(rayRecursive.d, wo, sampler->next2D());

                const Emitter *light = scene->getRandomEmitter(sampler->next1D());
                EmitterQueryRecord lRec(mi.p);
                lRec.uv = its.uv;
                Color3f Li = light->sample(lRec, sampler->next2D()) * scene->getLights().size();
                if (!scene->rayIntersect(lRec.shadowRay)) {
                    t *= mediumColor;
                    mi.tMax = lRec.shadowRay.maxt;
                    result += t * medium->Tr(lRec.shadowRay, sampler, mi) * Li * pdf_mat;
                }
                else
                    t *= mediumColor;

                probability = std::min(t.x(), .99f);
                if (sampler->next1D() >= probability)
                    return result;
                t /= probability;

                //continue recursion
                rayRecursive = Ray3f(mi.p, wo.normalized());
                hasIntersected = scene->rayIntersect(rayRecursive, its);
                if (hasIntersected) {
                    if (its.mesh->isEmitter()) {
                        EmitterQueryRecord lRec = EmitterQueryRecord(rayRecursive.o, its.p, its.shFrame.n);
                        lRec.uv = its.uv;
                        float pdf_em = its.mesh->getEmitter()->pdf(lRec);
                        w_mats = pdf_mat + pdf_em > 0.f ? pdf_mat / (pdf_mat + pdf_em) : pdf_mat;
                    }
                }
            } else if (!hasIntersected) {
                return result;
            } else {
                //Surface interaction

                // emitted
                if (its.mesh->isEmitter())
                {
                    EmitterQueryRecord lRec(rayRecursive.o, its.p, its.shFrame.n);
                    lRec.uv = its.uv;
                    result += t * w_mats * its.mesh->getEmitter()->eval(lRec) * medium->Tr(rayRecursive, sampler, mi);
                }

                //EMS
                const Emitter *light = scene->getRandomEmitter(sampler->next1D());
                EmitterQueryRecord lRec(its.p);
                lRec.uv = its.uv;
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
                    mi.tMax = lRec.shadowRay.maxt;
                    result += Li * f * cosTheta * w_ems * t * medium->Tr(lRec.shadowRay, sampler, mi);
                }

                //Russian roulettio
                probability = std::min(t.x(), 0.99f);
                if (sampler->next1D() > probability)
                    return result;

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

                hasIntersected = scene->rayIntersect(rayRecursive, its);

                if (hasIntersected) {

                    if (its.mesh->isEmitter()) {
                        EmitterQueryRecord lRec = EmitterQueryRecord(rayRecursive.o, its.p, its.shFrame.n);
                        lRec.uv = its.uv;
                        float pdf_em = its.mesh->getEmitter()->pdf(lRec);
                        w_mats = pdf_mat + pdf_em > 0.f ? pdf_mat / (pdf_mat + pdf_em) : pdf_mat;
                    }

                    if (bRec.measure == EDiscrete)
                        w_mats = 1.0f;
                }
            }

        }
        return result;
    }

    std::string toString() const {
        return "VolPathIntegrator[]";
    }
};

NORI_REGISTER_CLASS(VolPathIntegrator, "vol_path");
NORI_NAMESPACE_END


