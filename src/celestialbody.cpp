#include "../headers/celestialbody.h"

CelestialBody::CelestialBody(Engine* engine, BodyType bodyType, float mass,
                             float radius, float semiMajorAxis,
                             float eccentricity, float orbitalPeriod,
                             float currentAngle, glm::vec3 position,
                             glm::vec3 velocity)
    : engine(engine),
      bodyType(bodyType),
      mass(mass),
      radius(radius),
      semiMajorAxis(semiMajorAxis),
      eccentricity(eccentricity),
      orbitalPeriod(orbitalPeriod),
      currentAngle(currentAngle),
      position(position),
      velocity(velocity) {}

void CelestialBody::updateOrbitalPositions(float deltaTime) {
    if (this->bodyType == Sun) {
        return;
    }

    if (this->orbitalPeriod > 0.0f) {
        float angularVelocity = (2.0f * M_PI) / this->orbitalPeriod;

        this->currentAngle += angularVelocity * deltaTime;

        if (this->currentAngle > 2.0f * M_PI) {
            this->currentAngle -= 2.0f * M_PI;
        }

        this->position.x = this->semiMajorAxis * cos(this->currentAngle);
        this->position.z = this->semiMajorAxis * sin(this->currentAngle);
        this->position.y = 0.0f; // Assuming all orbits are in the same plane

        float speed = angularVelocity * this->semiMajorAxis;
        this->velocity.x = -speed * sin(this->currentAngle);
        this->velocity.z = speed * cos(this->currentAngle);
        this->velocity.y = 0.0f;
    }
}