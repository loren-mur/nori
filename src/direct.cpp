#include <nori/integrator.h>
#include <nori/scene.h>
#include <nori/bsdf.h>


NORI_NAMESPACE_BEGIN

class DirectIntegrator : public Integrator {
public:
    DirectIntegrator(const PropertyList &props) {}

    Color3f Li(const Scene *scene, Sampler *sampler, const Ray3f &ray) const override {
        Intersection its;
        Color3f color = 0;

        if (!scene->rayIntersect(ray, its))
            return color;

        std::vector<Emitter *> lights = scene->getLights();

        Vector2f sample;

        for (auto light : lights)
        {
            EmitterQueryRecord lRec;
            lRec.ref = its.p;
            Color3f eColor = light->sample(lRec, sample);
            if (scene->rayIntersect(lRec.shadowRay)) //intersection point is occluded
                continue;

            float angle = Frame::cosTheta(its.shFrame.toLocal(lRec.wi));
            BSDFQueryRecord bRec(its.shFrame.toLocal(lRec.wi), its.shFrame.toLocal(-ray.d), ESolidAngle);
            bRec.uv = its.uv;
            color += its.mesh->getBSDF()->eval(bRec) * angle * eColor;
        }

        return color;
    }

    std::string toString() const {
        return "DirectIntegrator[]";
    }
};

NORI_REGISTER_CLASS(DirectIntegrator, "direct");
NORI_NAMESPACE_END