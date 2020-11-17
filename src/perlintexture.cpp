//
// Created by Alessia Paccagnella on 03/12/2019.
//
#include <nori/object.h>
#include <nori/texture.h>
#include <nori/sampler.h>
#include "math.h"

#define maxPrimeIndex 10

NORI_NAMESPACE_BEGIN

class PerlinTexture : public Texture<Color3f> {
public:

    PerlinTexture(const PropertyList &propList) {
        m_width = propList.getInteger("width");
        m_height = propList.getInteger("height");
        m_levels = propList.getInteger("levels");
        //primeIndex = 0;
        //m_texture.resize(m_height, m_width);
    }


//ok
    float interpolatedNoise(float x, float y){
        int intX = int(x);
        float dx = x - intX;

        int intY = int(y);
        float dy = y - intY;

        float v1 = smoothedNoise(intX,intY);
        float v2 = smoothedNoise(intX + 1,intY);
        float v3 = smoothedNoise(intX,intY + 1);
        float v4 = smoothedNoise(intX + 1,intY + 1);

        float i1 = cosineinterpolate(v1 , v2 , dx);
        float i2 = cosineinterpolate(v3 , v4 , dx);

        return cosineinterpolate(i1 , i2 , dy);
    }

    //cosine interpolation, not linear
    float cosineinterpolate(float a, float b, float x){
        double ft = x * M_PI;
        double f = (1 - cos(ft)) * 0.5;
        return  a*(1-f) + b*f;
    }

    //gradient for the corners, gradient for the sides, gradient for the center. then sum
    float smoothedNoise(int x, int y){
        float corners = ( noise(x-1, y-1)+noise(x+1, y-1)+noise(x-1, y+1)+noise(x+1, y+1) ) / 16;
        float sides   = ( noise(x-1, y)  +noise(x+1, y)  +noise(x, y-1)  +noise(x, y+1) ) /  8;
        float center  =  noise(x, y) / 4;
        return corners + sides + center;
    }

    //gradient for 2 points
    float noise(int x, int y){
        //GOOD FUNCTION , NUMBER 1
        int n = x + y * 57;
        n = (n << 13) ^ n;
        return (1.0 - ((n * ((n * n * 15731) + 789221) +  1376312589) & 0x7fffffff) / 1073741824.0);


        /*int n = x + y * 2;
        n = (n << 8) ^ n;
        return (1.0 - ((n * ((n * n * 8432) + 87993) +  53642513) & 0x7fffffff) / 1073741824.0);*/

        //noise function number 2
        /*int v = 36969*(x & 65535) + (x >> 16);
        int u = 18000*(y & 65535) + (y >> 16);
        return ((v << 16) + (u & 65535))/16777216;*/

        //noise function number 3
        /*Point2f co(x,y);
        Point2f vec(12.9898,78.233);
        return ((sin(co.dot(vec))) * 43758.5453); //43758.5453*/


        /* ALMOST CONSTANT FUNCTION
         * x = x * 3266489917 + 374761393;
        x = (x << 17) | (x >> 15);

        mix around the bits in y and mix those into x:
        x += y * 3266489917;
        x *= 668265263;
        x ^= x >> 15;
        x *= 2246822519;
        x ^= x >> 13;
        x *= 3266489917;
        x ^= x >> 16;

        return (x & 0x00ffffff) * (1.0f / 0x10000000);*/
        }

    Color3f eval(const Point2f &uv) {
        /*int total = 0;
        float p = 0.25;
        int n = 6;
        int frequency;
        float amplitude = 1;

        for(int i = 0;i < n; i++){
            frequency = pow(2, i);
            amplitude = pow(p, i); //p * i;
            total = total + interpolatedNoise(uv.x() * frequency, uv.y() * frequency, i % maxPrimeIndex) * amplitude; //interpolatedNoise(uv.x() * frequency, uv.y() * frequency) * amplitude;
        }

        return total;*/

        //numero perfetto di ottave è 10, oltre 10 aggiunge troppa luce, di piu has too high a frequency to be displayable
        int i;
        float total = 0.0f;
        float frequency = 1.0f/(float)m_height;
        float amplitude = 1;
        float lacunarity = 2.0;
        int octaves = 9;

        //each noise function is twice the frequency of the previous one
        for (i = 0; i < octaves; ++i)
        {
            total += interpolatedNoise((float)uv.x() * frequency, (float)uv.y() * frequency) * amplitude;
            frequency = pow(2, i); //just like pseudocode
            amplitude = pow(lacunarity, (float)i);
        }

        return Color3f((total/255), (total/255), (total/255));
    }


    std::string toString() const {
        return "perlinTexture[]";

    }
protected:
    int m_width;
    int m_height;
    int m_levels;
};

NORI_REGISTER_CLASS(PerlinTexture, "perlintexture")
NORI_NAMESPACE_END


