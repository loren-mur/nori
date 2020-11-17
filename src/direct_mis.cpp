//
// Created by Niklaus on 28.10.19.
//

#include <nori/integrator.h>
#include <nori/scene.h>
#include <nori/bsdf.h>
#include <nori/sampler.h>


NORI_NAMESPACE_BEGIN

    class DirectMisIntegrator : public Integrator {
    public:
        DirectMisIntegrator(const PropertyList &props) {}

        Color3f Li(const Scene *scene, Sampler *sampler, const Ray3f &ray) const override {
            Intersection its;

            if (!scene->rayIntersect(ray, its))
                return 0;

            //emitted
            Color3f Le = 0;
            if (its.mesh->isEmitter())
            {
                EmitterQueryRecord lRecE(ray.o, its.p, its.shFrame.n);
                Le = its.mesh->getEmitter()->eval(lRecE);
            }

            /*
             * Ems
             */
            Color3f Lems = 0;
            float pdf_mat, pdf_em = 0.f;

            std::vector<Emitter *> lights = scene->getLights();

            for (auto light : lights)
            {
                //reflected
                EmitterQueryRecord lRec(its.p);
                Color3f Li = light->sample(lRec, sampler->next2D());
                pdf_em = light->pdf(lRec);
                if (scene->rayIntersect(lRec.shadowRay)) //intersection point is occluded
                {
                    continue;
                }

                //cosine
                float cosTheta = Frame::cosTheta(its.shFrame.toLocal(lRec.wi));

                //BSDF
                BSDFQueryRecord bRec(its.toLocal(-ray.d), its.toLocal(lRec.wi), ESolidAngle);
                bRec.uv = its.uv;
                Color3f f = its.mesh->getBSDF()->eval(bRec);
                pdf_mat = its.mesh->getBSDF()->pdf(bRec);

                float w_ems = pdf_mat + pdf_em > 0.f ? pdf_em / (pdf_mat + pdf_em) : pdf_em;

                Lems += Li * f * cosTheta * w_ems;
            }

            /*
            * Mats
            */
            Color3f Lmats = 0;
            pdf_em = 0.f;

            //BSDF
            BSDFQueryRecord bRec_m(its.shFrame.toLocal(-ray.d));
            bRec_m.uv = its.uv;
            Color3f f_m = its.mesh->getBSDF()->sample(bRec_m, sampler->next2D());
            pdf_mat = its.mesh->getBSDF()->pdf(bRec_m);

            //reflected
            Color3f Li_m = 0;
            Ray3f rayR = Ray3f(its.p, its.shFrame.toWorld(bRec_m.wo));
            Intersection itsR;
            if (scene->rayIntersect(rayR, itsR))
            {
                if (itsR.mesh->isEmitter())
                {
                    EmitterQueryRecord lRec = EmitterQueryRecord(its.p, itsR.p, itsR.shFrame.n);
                    Li_m = itsR.mesh->getEmitter()->eval(lRec);
                    pdf_em = itsR.mesh->getEmitter()->pdf(lRec);
                }
            }

            float w_mats = pdf_mat + pdf_em > 0.f ? pdf_mat / (pdf_mat + pdf_em) : pdf_mat;
            Lmats = Li_m * f_m * w_mats;


            return Le + Lmats + Lems;
        }



        std::string toString() const {
            return "DirectMisIntegrator[]";
        }
    };

NORI_REGISTER_CLASS(DirectMisIntegrator, "direct_mis");
NORI_NAMESPACE_END


