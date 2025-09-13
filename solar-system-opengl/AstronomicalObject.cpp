#include "AstronomicalObject.h"

AstronomicalObject::AstronomicalObject(
    std::string name, float mass, float radius, 
    float semiMajorAxis, float eccentricity, float orbitalPeriod, 
    float currentAngle, glm::vec3 position, glm::vec3 velocity) :
    name(name), mass(mass), radius(radius), 
    semiMajorAxis(semiMajorAxis), eccentricity(eccentricity), orbitalPeriod(orbitalPeriod), 
    currentAngle(currentAngle), position(position), velocity(velocity)
{

}

void AstronomicalObject::updateOrbitalPositions(float deltaTime)
{
    if (this->name == "Sun")
    {
        return;
    }

    float orbitalVelocity = sqrt(G_CONST * SUN_MASS / this->semiMajorAxis);
    float angularVelocity = orbitalVelocity / this->semiMajorAxis;

    this->currentAngle += angularVelocity * deltaTime;

    this->position.x = this->semiMajorAxis * cos(this->currentAngle);
    this->position.z = this->semiMajorAxis * sin(this->currentAngle);
    this->position.y = 0.0f;
}