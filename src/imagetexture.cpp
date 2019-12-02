//
// Created by Alessia Paccagnella on 30/11/2019.
//
#include <nori/lodepng.h>
//#include <nori/object.h>
#include <nori/texture.h>
#include <filesystem/resolver.h>

NORI_NAMESPACE_BEGIN

//png images to rgb vector
class ImageTexture : public Texture<Color3f> {
public:

    ImageTexture(const PropertyList &props) {
        m_name = props.getString("fileName", "");
        m_scale = props.getVector2("scale", Vector2f(1));
        //resolve from name to filepath
        filesystem::path filePath = getFileResolver()->resolve(m_name);
        //decode from https://raw.githubusercontent.com/lvandeve/lodepng/master/examples/example_decode.cpp
        //row wise. so first we have first row, then second row...
        unsigned error = lodepng::decode(m_image, m_width, m_height, filePath.str());
        //if there's an error, display it
        if(error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
    }


    //this function should return a rgb value depending on the coordinates UV passed
    Color3f eval(const Point2f & uv) {
        //get delta u and delta v
        float du = (uv.x() * m_width) / m_scale.x(); //da 01 ottengo la coordinata riscalata
        float dv = (uv.y() * m_height) / m_scale.y(); //da 01 ottengo la coordinata riscalata
        int u = (int)du; //prendo il valore piu vicino
        int v = (int)dv; //prendo il valore piu vicino

        /*if (u < 0) u = 0;
        if (v < 0) v = 0;
        if (u > m_width - 1) u = m_width - 1;
        if (v > m_height - 1) v = m_height - 1;*/
        //i find the index by multiplying the v for the width. this way i know in which row I am. then i add u for the cell
        long index = (v * m_width +u )*4;
        index = index % m_image.size(); //MODULO DIMENSIONE DELLA IMMAGINE (numero di pixel, every pixel has rgb values). in modo da avere unindice compreso tra 0 e dimensione immagine (RGBA)

        //http://viclw17.github.io/2019/04/12/raytracing-uv-mapping-and-texturing/
        // RGBA RGBA RGBA RGBA......
        float r = m_image[index] / 255.0;
        float g = m_image[index + 1] / 255.0;
        float b = m_image[index + 2] / 255.0;

        return Color3f(r, g, b);
    }

    std::string toString() const {
        return tfm::format(
                "ImageTexture[\n"
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

NORI_REGISTER_CLASS(ImageTexture, "imagetexture")
NORI_NAMESPACE_END
