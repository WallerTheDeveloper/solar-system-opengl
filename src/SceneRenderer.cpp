#include "../headers/renderers/SceneRenderer.h"

#include <glm/gtc/matrix_transform.hpp>

SceneRenderer::SceneRenderer(Skybox* skybox, Ring* saturnRing)
    : skybox_(skybox), saturnRing_(saturnRing) {}

void SceneRenderer::renderScene(const std::vector<std::unique_ptr<CelestialBody>>& celestialBodies,
                                const RenderContext& context) {
  glm::mat4 view = calculateViewMatrix(context.camera);
  glm::mat4 projection = calculateProjectionMatrix(context);

  // Render skybox first
  skybox_->render(view, projection);

  // Render all planets
  for (const auto& body : celestialBodies) {
    glm::mat4 model = calculateModelMatrix(body, context.currentTime);
    body->render(model, view, projection);

    // Special case: render Saturn's ring
    if (body->type == CelestialBody::Saturn) {
      saturnRing_->render(model, view, projection);
    }
  }
}

glm::mat4 SceneRenderer::calculateViewMatrix(const Camera& camera) const {
  return camera.getViewMatrix();
}

glm::mat4 SceneRenderer::calculateProjectionMatrix(
    const RenderContext& context) const {
  return glm::perspective(glm::radians(context.camera.Zoom),
                          static_cast<float>(context.screenWidth) /
                              static_cast<float>(context.screenHeight),
                          0.1f, 10000.0f);
}

glm::mat4 SceneRenderer::calculateModelMatrix(const std::unique_ptr<CelestialBody>& celestialBody  ,
                                              float currentTime) const {
  glm::mat4 model = glm::mat4(1.0f);

  // Translation
  model = glm::translate(model, celestialBody->position);

  // Rotation
  float rotationSpeed = CelestialBodyFactory::getRotationSpeed(celestialBody->type);
  glm::vec3 rotationAxis = CelestialBodyFactory::getRotationAxis(celestialBody->type);
  model = glm::rotate(model, currentTime * glm::radians(rotationSpeed),
                      rotationAxis);

  // Scale
  glm::vec3 scale = CelestialBodyFactory::getScale(celestialBody->type);
  model = glm::scale(model, scale);

  return model;
}