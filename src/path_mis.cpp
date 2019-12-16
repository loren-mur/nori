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

        Texture<Vector3f>* normalMap;
        //https://en.wikipedia.org/wiki/Normal_mapping
        if (its.mesh->getBSDF()->hasNormalMap(normalMap)) {
            Vector3f newNormal = normalMap->eval(its.uv);

            if (abs(newNormal.norm() - 1.f) < Epsilon)
            {
                Normal3f n = (its.shFrame.toWorld(newNormal)).normalized();
                Vector3f s = (its.shFrame.t - n * n.dot(its.shFrame.t)).normalized();
                Vector3f tan (n(1)*s(2) - n(2) * s(1),
                              n(2) * s(0) - n(0) * s(2),
                              n(0) * s(1) - n(1)* s(0));
                its.shFrame = Frame(s,tan,n);
            }
        }

        while (true) {
            // emitted
            if (its.mesh->isEmitter())
            {
                EmitterQueryRecord lRec(rayRecursive.o, its.p, its.shFrame.n);
                lRec.uv = its.uv;
                color += t * w_mats * its.mesh->getEmitter()->eval(lRec);
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

            //https://en.wikipedia.org/wiki/Normal_mapping
            if (its.mesh->getBSDF()->hasNormalMap(normalMap)) {
                Vector3f newNormal = normalMap->eval(its.uv);

                if (abs(newNormal.norm() - 1.f) < Epsilon)
                {
                    Normal3f n = (its.shFrame.toWorld(newNormal)).normalized();
                    Vector3f s = (its.shFrame.t - n * n.dot(its.shFrame.t)).normalized();
                    Vector3f tan (n(1)*s(2) - n(2) * s(1),
                                  n(2) * s(0) - n(0) * s(2),
                                  n(0) * s(1) - n(1)* s(0));
                    its.shFrame = Frame(s,tan,n);
                }
            }

            if (its.mesh->isEmitter()) {
                EmitterQueryRecord lRec = EmitterQueryRecord(origin, its.p, its.shFrame.n);
                lRec.uv = its.uv;
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


