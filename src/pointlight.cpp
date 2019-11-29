#include <nori/emitter.h>

NORI_NAMESPACE_BEGIN

class PointLight : public Emitter {
public:
	PointLight(const PropertyList &propList) {
		m_position = propList.getPoint3("position", Point3f());
		m_power = propList.getColor("power", Color3f());
	}

	virtual Color3f sample(EmitterQueryRecord &lRec, const Point2f &sample) const {
		lRec.wi = (m_position - lRec.ref).normalized();
		lRec.p = m_position;
		lRec.pdf = 1.f;
		lRec.shadowRay = Ray3f(lRec.ref, lRec.wi, Epsilon, (m_position - lRec.ref).norm() - Epsilon);
		return m_power / (4.f * M_PI * (m_position - lRec.ref).squaredNorm());
	}

	virtual Color3f eval(const EmitterQueryRecord &lRec) const {
		return m_power / (4.f * M_PI * (m_position - lRec.ref).squaredNorm());
	}

	virtual float pdf(const EmitterQueryRecord &lRec) const {
		return 1.f;
	}

	virtual std::string toString() const {
		return "PointLight[]";
	}

private:
	Color3f m_power;
	Point3f m_position;
};

NORI_REGISTER_CLASS(PointLight, "point");
NORI_NAMESPACE_END