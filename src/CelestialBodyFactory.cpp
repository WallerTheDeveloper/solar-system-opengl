//
// Created by Daniel on 30-Sep-25.
//
#include "CelestialBodyFactory.h"

std::vector<CelestialBodyConfig> CelestialBodyFactory::getSolarSystemConfig() {
    return {
        // Sun
        {
            CelestialBody::Sun, 1.989e30f, 696340000.0f,
            0.0f, 0.0f, 0.0f, 0.0f,
            glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f),
            "../textures/sun.jpg"
        },
        // Mercury
        {
            CelestialBody::Mercury, 3.301e23f, 2439700.0f,
            4.0f, 0.205f, 20.0f, 0.0f,
            glm::vec3(4.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f),
            "../textures/mercury.jpg"
        },
        // Venus
        {
            CelestialBody::Venus, 4.867e24f, 6051800.0f,
            6.0f, 0.007f, 35.0f, 0.0f,
            glm::vec3(6.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f),
            "../textures/venus.jpg"
        },
        // Earth
        {
            CelestialBody::Earth, 5.972e24f, 6371000.0f,
            8.0f, 0.017f, 50.0f, 0.0f,
            glm::vec3(8.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f),
            "../textures/earth.jpg"
        },
        // Mars
        {
            CelestialBody::Mars, 6.417e23f, 3389500.0f,
            10.5f, 0.094f, 75.0f, 0.0f,
            glm::vec3(10.5f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f),
            "../textures/mars.jpg"
        },
        // Jupiter
        {
            CelestialBody::Jupiter, 1.898e27f, 69911000.0f,
            16.0f, 0.049f, 120.0f, 0.0f,
            glm::vec3(16.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f),
            "../textures/jupiter.jpg"
        },
        // Saturn
        {
            CelestialBody::Saturn, 5.683e26f, 58232000.0f,
            20.0f, 0.057f, 180.0f, 0.0f,
            glm::vec3(20.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f),
            "../textures/saturn.jpg"
        },
        // Uranus
        {
            CelestialBody::Uranus, 8.681e25f, 25362000.0f,
            25.0f, 0.046f, 250.0f, 0.0f,
            glm::vec3(25.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f),
            "../textures/uranus.jpg"
        },
        // Neptune
        {
            CelestialBody::Neptune, 1.024e26f, 24622000.0f,
            30.0f, 0.009f, 350.0f, 0.0f,
            glm::vec3(30.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f),
            "../textures/neptune.jpg"
        }
    };
}

std::vector<CelestialBody> CelestialBodyFactory::createSolarSystem(Engine* engine) {
    std::vector<CelestialBody> celestialBodies;
    auto configs = getSolarSystemConfig();

    celestialBodies.reserve(configs.size());

    for (const auto& config : configs) {
        celestialBodies.emplace_back(
            engine, config.type, config.mass, config.radius,
            config.orbitalRadius, config.eccentricity, config.orbitalPeriod,
            config.rotationAngle, config.position, config.velocity
        );

        celestialBodies.back().create(config.texturePath);
    }

    return celestialBodies;
}

float CelestialBodyFactory::getRotationSpeed(CelestialBody::BodyType type) {
    switch (type) {
        case CelestialBody::Sun: return 2.0f;
        case CelestialBody::Mercury: return 10.0f;
        case CelestialBody::Venus: return -5.0f;
        case CelestialBody::Earth: return 15.0f;
        case CelestialBody::Mars: return 14.0f;
        case CelestialBody::Jupiter: return 45.0f;
        case CelestialBody::Saturn: return 38.0f;
        case CelestialBody::Uranus: return -20.0f;
        case CelestialBody::Neptune: return 25.0f;
        default: return 0.0f;
    }
}

glm::vec3 CelestialBodyFactory::getScale(CelestialBody::BodyType type) {
    switch (type) {
        case CelestialBody::Sun: return glm::vec3(5.0f);
        case CelestialBody::Mercury: return glm::vec3(0.4f);
        case CelestialBody::Venus: return glm::vec3(0.9f);
        case CelestialBody::Earth: return glm::vec3(1.0f);
        case CelestialBody::Mars: return glm::vec3(0.5f);
        case CelestialBody::Jupiter: return glm::vec3(2.5f);
        case CelestialBody::Saturn: return glm::vec3(2.2f);
        case CelestialBody::Uranus: return glm::vec3(1.8f);
        case CelestialBody::Neptune: return glm::vec3(1.7f);
        default: return glm::vec3(1.0f);
    }
}

glm::vec3 CelestialBodyFactory::getRotationAxis(CelestialBody::BodyType type) {
    if (type == CelestialBody::Uranus) {
        return glm::vec3(0.8f, 0.2f, 0.1f); // Tilted rotation
    }
    return glm::vec3(0.0f, 1.0f, 0.1f); // Standard rotation
}