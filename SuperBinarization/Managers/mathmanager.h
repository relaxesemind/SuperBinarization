#ifndef MATHMANAGER_H
#define MATHMANAGER_H

#include <QRgb>

class MathManager
{
public:
    MathManager() = default;
    void rgb2lab( float R, float G, float B, float & l_s, float &a_s, float &b_s );
    void lab2rgb( float l_s, float a_s, float b_s, float& R, float& G, float& B );
    void rgb2hsv(QRgb q, float &h, float &s, float &v);
};

#endif // MATHMANAGER_H
