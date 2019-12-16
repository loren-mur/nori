//
// Created by Niklaus on 14.12.19.
//

//
// Created by Alessia Paccagnella on 30/11/2019.
//
#include <nori/lodepng.h>
#include <nori/object.h>
#include <nori/texture.h>
#include <filesystem/resolver.h>

NORI_NAMESPACE_BEGIN

//png images to rgb vector
    class NormalTexture : public Texture<Vector3f> {
    public:

        NormalTexture(const PropertyList &props) {
            m_name = props.getString("fileName", "");
            m_scale = props.getVector2("scale", Vector2f(1));
            //resolve from name to filepath
            filesystem::path filePath = getFileResolver()->resolve(m_name);
            //decode from https://raw.githubusercontent.com/lvandeve/lodepng/master/examples/example_decode.cpp
            unsigned error = lodepng::decode(m_image, m_width, m_height, filePath.str());
            if(error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
        }


        //returns normal based on uv coordinate.
        Vector3f eval(const Point2f & uv) {
            //get delta u and delta v
            float du = (uv.x() * m_width) / m_scale.x();
            float dv = (uv.y() * m_height) / m_scale.y();
            int u = (int)du;
            int v = (int)dv;

            long index = (v * m_width + u ) * 4;
            index = index % m_image.size();

            //https://en.wikipedia.org/wiki/Normal_mapping
            float r = m_image[index] / 255.0;
            float g = m_image[index + 1] / 255.0;
            float b = m_image[index + 2] / 255.0;
            float x = 2 * r - 1;      // X: -1 to + 1: Red    0 to 255
            float y = 2 * g - 1;      // Y: -1 to + 1: Green: 0 to 255
            float z = 2 * b - 1;      //Z: 0 to -1:
            Vector3f result(x,-y,z);
            return result.normalized();
        }

        std::string toString() const {
            return tfm::format(
                    "NormalTexture[\n"
                    "  m_name = %s,\n"
                    "  scale = %s,\n"
                    "]",
                    m_name,
                    m_scale.toString()
            );
        }

    protected:
        Vector2f m_scale;
        std::string m_name;
        std::vector<unsigned char> m_image;
        unsigned m_width;
        unsigned m_height;
    };

NORI_REGISTER_CLASS(NormalTexture, "normaltexture")
NORI_NAMESPACE_END
