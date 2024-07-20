#if 0
#include "Sprite.h"

#include <cmath>

#define FEQ(a, b) (fabs(a - b) < 0.0001)

void Sprite::SetOutputData(double thisx, double thisy)
{
    if (VBO == 0)
    {
        glGenBuffers(1, &VBO);
    }

    char it[6] = { 'a', 'b', 'c', 'a', 'c', 'd' };
    for (int i = 0; i < 6; i++)
    {
        char c = it[i];

        double _width = FEQ(0.0, width) ? 50 : width;
        double _height = FEQ(0.0, height) ? 50 : height;

        switch (c)
        {
        case 'a':
            // top right corner
            vertices[5*i] = E2GLX(thisx + offsetX + _width);
            vertices[5*i + 1] = E2GLY(thisy + offsetY + _height);
            vertices[5*i+2] = 0.0;
            break;
        case 'b':
            // top left corner
            vertices[5*i] = E2GLX(thisx + offsetX);
            vertices[5*i + 1] = E2GLY(thisy + offsetY + _height);
            vertices[5*i+2] = 0.0;
            break;
        case 'c':
            // bottom left corner
            vertices[5*i] = E2GLX(thisx + offsetX);
            vertices[5*i + 1] = E2GLY(thisy + offsetY);
            vertices[5*i+2] = 0.0;
            break;
        case 'd':
            // bottom right corner
            vertices[5*i] = E2GLX(thisx + offsetX + _width);
            vertices[5*i + 1] = E2GLY(thisy + offsetY);
            vertices[5*i+2] = 0.0;
            break;
        };
    }
}

void Sprite::DestroyComponent()
{
    if (VBO != 0)
        glDeleteBuffers(1, &VBO);
}
#endif
