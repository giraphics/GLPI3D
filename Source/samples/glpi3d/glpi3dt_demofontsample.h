#ifndef _FONT_SAMPLE_H
#define _FONT_SAMPLE_H

#include "GLPi3D__Glutils.h"
#include "GLPi3D__Renderer.h"
#include "GLPi3D__Model.h"
using namespace GLPi3D;

class FontSample : public Model
{
public:
    FontSample( ModelType typeOfModel, Model* parent = NULL);
    ~FontSample();
    void Render();
    void InitModel();
};

#endif
