#ifndef Material_h
#define Material_h

#include "glutils.h"
#include <string>

typedef enum {
    MaterialNone,
    MaterialGold,
    MaterialCopper,
    MaterialPolishedCopper,
    MaterialPolishedGold,
    MaterialSilver,
    MaterialPewter,
    MaterialWhite
} MaterialType;

// Copper Material
const glm::vec4 CopperAmbient(0.19125f, 0.0735f, 0.0225, 1.0f);
const glm::vec4 CopperDiffuse(0.7038f, 0.27048f, 0.0828, 1.0f);
const glm::vec4 CopperSpecular(0.256777f, 0.137622f, 0.086014, 1.0f);
const GLfloat   CopperShiness = 2.8f;

// Polished Copper Material
const glm::vec4 PolishedCopperAmbient(0.2295f, 0.08825f, 0.0275f, 1.0f);
const glm::vec4 PolishedCopperDiffuse(0.5508f, 0.2118f, 0.066f, 1.0f);
const glm::vec4 PolishedCopperSpecular(0.580594f, 0.223257f, 0.0695701f, 1.0f);
const GLfloat   PolishedCopperShiness = 51.2f;

// Gold Material
const glm::vec4 GoldAmbient(0.24725f, 0.1995f, 0.0745f, 1.0f);
const glm::vec4 GoldDiffuse(0.75164f, 0.60648f, 0.22648f, 1.0f);
const glm::vec4 GoldSpecular(0.628281f, 0.555802f, 0.366065f, 1.0f);
const GLfloat   GoldShiness=51.2f;

// Polished Gold Material
const glm::vec4 PolishedGoldAmbient(0.24725f, 0.2245f, 0.0645f, 1.0f);
const glm::vec4 PolishedGoldDiffuse(0.34615f, 0.3143f, 0.0903f, 1.0f);
const glm::vec4 PolishedGoldSpecular(0.797357f, 0.723991f, 0.208006f, 1.0f);
const GLfloat   PolishedGoldShiness = 83.2f;

//Silver Material
const glm::vec4 SilverAmbient(0.19225f, 0.19225f, 0.19225f, 1.0f);
const glm::vec4 SilverDiffuse(0.50754f, 0.50754f, 0.50754f, 1.0f);
const glm::vec4 SilverSpecular(0.508273f, 0.508273f, 0.508273f, 1.0f);
const GLfloat   SilverShiness = 51.2f;

//Petwer Material
const glm::vec4 PewterAmbient(0.105882f, 0.058824f, 0.113725f, 1.0f);
const glm::vec4 PewterDiffuse(0.427451f, 0.470588f, 0.541176f, 1.0f);
const glm::vec4 PewterSpecular(0.333333f, 0.333333f, 0.521569f, 1.0f);
const GLfloat   PewterShiness = 9.84615f;

//Pure White
const glm::vec4 WhiteAmbient(1.0f,1.0f,1.0f,1.0f);
const glm::vec4 WhiteDiffuse(1.0f,1.0f,1.0f,1.0f);
const glm::vec4 WhiteSpecular(1.0f,1.0f,1.0f,1.0f);
const GLfloat   WhiteShiness = 10.0f;

class Material
{
public:
    glm::vec4 ambient, diffuse, specular;
    GLfloat shiness;
    std::string name;
    
    MaterialType typeOfMaterial;
    
    Material(glm::vec4  ambient, glm::vec4 diffuse, glm::vec4 specular, GLfloat shiness);
    
    Material(const Material & p);
    
    Material & operator = (const Material & p);
    
    Material(MaterialType type = MaterialNone);
};

#endif
