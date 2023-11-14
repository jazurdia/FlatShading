#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // Ensure this header is included for transformation functions

float a = 3.14f / 3.0f;

glm::mat4 createModelMatrix() {
    glm::mat4 translate = glm::translate(glm::mat4(1), glm::vec3(0.0f, 0.0f, 0.0f));
    glm::mat4 scale = glm::scale(glm::mat4(1), glm::vec3(1.0f, 1.0f, 1.0f));
    glm::mat4 rotation = glm::rotate(glm::mat4(1), glm::radians(a++), glm::vec3(0.0f, 1.0f, 0.0f));

    return translate * scale * rotation;
}

glm::mat4 createViewMatrix() {
    glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 targetPosition = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);

    return glm::lookAt(cameraPosition, targetPosition, upVector);
}

