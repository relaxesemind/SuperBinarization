#include "mathmanager.h"
#include <cmath>
#include <algorithm>

void MathManager::rgb2lab(float R, float G, float B, float &l_s, float &a_s, float &b_s)
{
     float X = 0.4124 * R + 0.3576 * G + 0.1805 * B;
     float Y = 0.2126 * R + 0.7152 * G + 0.0722 * B;
     float Z = 0.0193 * R + 0.1192 * G + 0.9504 * B;


//     Observer	2° (CIE 1931)	10° (CIE 1964)	Note
//     Illuminant	X2      Y2      Z2      X10     Y10 	Z10
//     A            109.850	100.000	35.585	111.144	100.000	35.200	Incandescent/tungsten
//     B            99.0927	100.000	85.313	99.178;	100.000	84.3493	Old direct sunlight at noon
//     C            98.074	100.000	118.232	97.285	100.000	116.145	Old daylight
//     D50          96.422	100.000	82.521	96.720	100.000	81.427	ICC profile PCS
//     D55          95.682	100.000	92.149	95.799	100.000	90.926	Mid-morning daylight
//     D65          95.047	100.000	108.883	94.811	100.000	107.304	Daylight, sRGB, Adobe-RGB
//     D75          94.972	100.000	122.638	94.416	100.000	120.641	North sky daylight
//     E            100.000	100.000	100.000	100.000	100.000	100.000	Equal energy
//     F1           92.834	100.000	103.665	94.791	100.000	103.191	Daylight Fluorescent
//     F2           99.187	100.000	67.395	103.280	100.000	69.026	Cool fluorescent
///     F3           103.754	100.000	49.861	108.968	100.000	51.965	White Fluorescent
//     F4           109.147	100.000	38.813	114.961	100.000	40.963	Warm White Fluorescent
//     F5           90.872	100.000	98.723	93.369	100.000	98.636	Daylight Fluorescent
//     F6           97.309	100.000	60.191	102.148	100.000	62.074	Lite White Fluorescent
//     F7           95.044	100.000	108.755	95.792	100.000	107.687	Daylight fluorescent, D65 simulator
//     F8           96.413	100.000	82.333	97.115	100.000	81.135	Sylvania F40, D50 simulator
//     F9           100.365	100.000	67.868	102.116	100.000	67.826	Cool White Fluorescent
//     F10          96.174	100.000	81.712	99.001	100.000	83.134	Ultralume 50, Philips TL85
//     F11          100.966	100.000	64.370	103.866	100.000	65.627	Ultralume 40, Philips TL84
//     F12      	108.046	100.000	39.228	111.428	100.000	40.353	Ultralume 30, Philips TL83

     float Xr = 103.754;
     float Yr = 100.000;
     float Zr = 49.861;

     float xr = X/Xr;
     float yr= Y/Yr;
     float zr = Z/Zr;

     const float e  = 0.008856;
     const float k = 903.3;

     auto f = [e,k](float comp)->float
     {
         return comp > e ? std::pow(comp,1./3.) : (k * comp + 16) / 116.f;
     };

     float fx = f(xr);
     float fy = f(yr);
     float fz = f(zr);

     l_s = 116.f * fy - 16;
     a_s = 500.f * (fx - fy);
     b_s = 200 * (fy -fz);
}

void MathManager::lab2rgb(float l_s, float a_s, float b_s, float &R, float &G, float &B)
{

}

void MathManager::rgb2hsv(QRgb q, float &h, float &s, float &v)
{
    float r = qRed(q) / 255.f;
    float g = qGreen(q) / 255.f;
    float b = qBlue(q) / 255.f;
    float max = std::max(std::max(r,g),b);
    float min = std::min(std::min(r,g),b);

    v = max;

    if (max == min)
    {
        h = 0;
    }else if (max == r and g >= b)
    {
        h = 60*(g-b)/(max-min);
    }else if (max == r and g < b)
    {
        h = 360 + 60*(g-b)/(max-min);
    }else if (max == g)
    {
        h = 120 + 60*(b - r)/(max-min);
    }else if (max == b)
    {
        h = 240 + 60*(r - g)/(max-min);
    }

    s = max == 0 ? 0 : 1 - min / max;
}