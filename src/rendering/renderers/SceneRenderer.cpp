#include "SceneRenderer.h"

#include <rendering/RenderContext.h>
#include <rendering/renderables/scene/CelestialBody.h>

#include <celestialbody/CelestialBodyFactory.h>

#include "glm/detail/func_trigonometric.hpp"
#include "glm/detail/type_mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"

SceneRenderer::SceneRenderer() = default;
SceneRenderer::~SceneRenderer() = default;

void SceneRenderer::render(const std::deque<std::unique_ptr<ISceneRenderable>>& renderables, const RenderContext& context) const {
  glm::mat4 view = context.camera.getViewMatrix();
  glm::mat4 projection =
      glm::perspective(glm::radians(context.camera.Zoom),
                       static_cast<float>(context.screenWidth) /
                           static_cast<float>(context.screenHeight),
                       0.1f, 10000.0f);

  for (const auto& renderablePtr : renderables) {
    auto renderable = renderablePtr.get();

    // If IRenderable is a type of CelestialBody
    if (const CelestialBody* body = dynamic_cast<CelestialBody*>(renderable)) {
      glm::mat4 model = calculateModelMatrix(*body, context.currentTime);
      renderable->render(model, view, projection);
    } else {
      glm::mat4 model = glm::mat4(1.0f);
      renderable->render(model, view, projection);
    }
  }
}

glm::mat4 SceneRenderer::calculateModelMatrix(const CelestialBody& body,
                                              float currentTime) const {
  glm::mat4 model = glm::mat4(1.0f);

  // Translation
  model = glm::translate(model, body.getBodyProps().position);

  // Rotation
  float rotationSpeed = CelestialBodyFactory::getRotationSpeed(body.getBodyProps().type);
  glm::vec3 rotationAxis = CelestialBodyFactory::getRotationAxis(body.getBodyProps().type);
  model = glm::rotate(model, currentTime * glm::radians(rotationSpeed),
                      rotationAxis);

  // Scale
  glm::vec3 scale = CelestialBodyFactory::getScale(body.getBodyProps().type);
  model = glm::scale(model, scale);

  return model;
}