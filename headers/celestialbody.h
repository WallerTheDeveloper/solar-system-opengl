#ifndef CELESTIALBODY_H
#define CELESTIALBODY_H

#include <glm/glm.hpp>
#include <string>

#include "engine.h"
#include "shader.h"

#ifndef SUN_MASS
#define SUN_MASS 1.989e30f
#endif

#ifndef G_CONST
#define G_CONST 6.674e-11f
#endif

using namespace std;

class CelestialBody
{
public:
    CelestialBody(
        Engine* engine, 
        std::string name, 
        float mass,
        float radius, 
        float semiMajorAxis, 
        float eccentricity, 
        float orbitalPeriod, 
        float currentAngle, 
        glm::vec3 position, 
        glm::vec3 velocity);

    std::string name;
    float mass;           // kg
    float radius;         // meters (for rendering scale)
    float semiMajorAxis;  // meters (orbital radius)
    float eccentricity;   // 0 = circle, <1 = ellipse
    float orbitalPeriod;  // seconds
    float currentAngle;   // current position in orbit (radians)
    glm::vec3 position;
    glm::vec3 velocity;

    void updateOrbitalPositions(float deltaTime);
    virtual void create(const char* texturePath) = 0;
    virtual void render(glm::mat4 model, glm::mat4 view,
                        glm::mat4 projection) = 0;
   protected:
    Engine* engine;
};

#endif