#include "../headers/camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
    Position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
    : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
    Position = glm::vec3(posX, posY, posZ);
    WorldUp = glm::vec3(upX, upY, upZ);
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix()
{
    return Camera::lookAt(Position, Position + Front, Up);
    //return glm::lookAt(Position, Position + Front, Up);
}

glm::mat4 Camera::lookAt(glm::vec3 from, glm::vec3 to, glm::vec3 up)
{
    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 cameraDirection = glm::normalize(from - to);

    float cameraMatrix[4][4] =
    {
        { Right.x, Right.y, Right.z, 0},
        { Up.x, Up.y, Up.z, 0 },
        { cameraDirection.x, cameraDirection.y, cameraDirection.z, 0},
        { 0, 0, 0, 1 }
    };

    float transformationMatrix[4][4] =
    {
        { 1, 0, 0, -Position.x },
        { 0, 1, 0, -Position.y },
        { 0, 0, 1, -Position.z },
        { 0, 0, 0, 1}
    };

    float result[4][4] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            result[row][col] = 0.0f;
            for (int k = 0; k < 4; k++) {
                result[row][col] += cameraMatrix[row][k] * transformationMatrix[k][col];
            }
        }
    }

    return glm::mat4(
        result[0][0], result[1][0], result[2][0], result[3][0],
        result[0][1], result[1][1], result[2][1], result[3][1],
        result[0][2], result[1][2], result[2][2], result[3][2],
        result[0][3], result[1][3], result[2][3], result[3][3] 
    );
}

void Camera::processKeyboard(Camera_Movement direction, float deltaTime, float speedMultiplier) 
{
    float velocity = MovementSpeed * deltaTime * speedMultiplier;
    if (direction == FORWARD) 
    {
        Position += Front * velocity;
    }
    if (direction == BACKWARD) 
    {
        Position -= Front * velocity;
    }
    if (direction == LEFT) 
    {
        Position -= Right * velocity;
    }
    if (direction == RIGHT) 
    {
        Position += Right * velocity;
    }
    if (direction == UP) {
        Position += WorldUp * velocity;
    }
    if (direction == DOWN) {
      Position -= WorldUp * velocity;
    }
}

void Camera::processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    if (constrainPitch)
    {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }

    updateCameraVectors();
}

void Camera::processMouseScroll(float yoffset)
{
    Zoom -= (float)yoffset;
    if (Zoom < 1.0f)
        Zoom = 1.0f;
    if (Zoom > 45.0f)
        Zoom = 45.0f;
}

void Camera::updateCameraVectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    
    Front = glm::normalize(front);
    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up = glm::normalize(glm::cross(Right, Front));
}   