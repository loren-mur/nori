//
// Created by Alessia Paccagnella on 03/12/2019.
//

#include <nori/object.h>
#include <nori/texture.h>

NORI_NAMESPACE_BEGIN

class ProceduralTexture : public Texture<Color3f> {
public:

    ProceduralTexture(const PropertyList &props) {
        m_delta = props.getPoint2("delta", Point2f(0));
        scale = props.getVector2("scale", Vector2f(1));
        color1 = props.getColor("color1", Color3f(0));
        color2 = props.getColor("color2", Color3f(1));
    }

    std::string toString() const {
        return tfm::format(
                "ImgTexture[\n"
                "  scale = %s,\n"
                "  color 1 = %s,\n"
                "  color 2 = %s,\n"
                "]",
                scale.toString(),
                color1.toString(),
                color2.toString());
    }

    virtual Color3f eval(const Point2f & uv) override {

        Point2f coord = uv;
        double tx = coord.x() / scale.x() - m_delta.x();

        if ((int)tx % 2 == 0)
            return color1;
        else
            return color2;
    }

protected:
    Color3f color1;
    Color3f color2;
    Vector2f scale;
    Point2f m_delta;

};

NORI_REGISTER_CLASS(ProceduralTexture, "proceduraltexture")
NORI_NAMESPACE_END


