#ifndef ASTRONOMICALOBJECT_H
#define ASTRONOMICALOBJECT_H

#include <glm/glm.hpp>

#include <string>

#ifndef SUN_MASS
#define SUN_MASS 1.989e30f
#endif

#ifndef G_CONST
#define G_CONST 6.674e-11f
#endif

using namespace std;

class AstronomicalObject
{
public:
    AstronomicalObject(std::string name, float mass, float radius, float semiMajorAxis, float eccentricity, float orbitalPeriod, float currentAngle, glm::vec3 position, glm::vec3 velocity);

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
};

#endif