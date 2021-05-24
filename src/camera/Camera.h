#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <iostream>

enum CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    ROTATE_LEFT,
    ROTATE_RIGHT
};


const float YAW = -90.0f;
const float PITCH = -65.0f; //-65
const float SPEED = 4.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;


class Camera
{
public:
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    float Yaw;
    float Pitch;
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    glm::mat4 getViewMatrix()
    {
        return glm::lookAt(Position, Position + Front, Up);
    }


    void processKeyboard(CameraMovement direction, float deltaTime)
    {
	    const float velocity = MovementSpeed * deltaTime;
        if (direction == FORWARD)
            Position += glm::vec3(Front.x, 0.0f, Front.z) * velocity;
        if (direction == BACKWARD)
            Position -= glm::vec3(Front.x, 0.0f, Front.z) * velocity;
        if (direction == LEFT)
            Position -= Right * velocity;
        if (direction == RIGHT)
            Position += Right * velocity;
        if (direction == ROTATE_LEFT) {
            Yaw += velocity * 10.0f;
            updateCameraVectors();
        }
        if (direction == ROTATE_RIGHT) {
            Yaw -= velocity * 10.0f;
            updateCameraVectors();
        }





        //Position.y = 0.0f; 
    }

    void processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        //std::cout << xoffset << " " << yoffset << std::endl;
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

    void processMouseScroll(float yoffset, float deltaTime)
    {
//        if (Zoom >= 1.0f && Zoom <= 45.0f)
//            Zoom -= yoffset;
//        if (Zoom <= 1.0f)
//            Zoom = 1.0f;
//        if (Zoom >= 45.0f)
//            Zoom = 45.0f;
        const float mouseSensitivity = this->MouseSensitivity * 4.0f;
        const float velocity = mouseSensitivity;
        //std::cout << velocity << " " << deltaTime << " " << mouseSensitivity << " " << yoffset << " " << yoffset * velocity << std::endl;

        if (Position.y < 11.0f && yoffset < 0) {
            Position += glm::vec3(0.0f, abs(yoffset), 0.0f) * velocity;

            Pitch += yoffset * velocity * 4.0f;
            if (Pitch > 10.0f)
                Pitch = 10.0f;
            if (Pitch < -65.0f)
                Pitch = -65.0f;
        }
        if (Position.y > 1.0f && yoffset > 0) {
            Position -= glm::vec3(0.0f, abs(yoffset), 0.0f) * velocity;

            Pitch += yoffset * velocity * 4.0f;
            if (Pitch > -10.0f)
                Pitch = -10.0f;
            if (Pitch < -65.0f)
                Pitch = -65.0f;
        }

        updateCameraVectors();
    }

    void processMouse(int w, int h, double x, double y, float deltaTime) {
        const float velocity = MovementSpeed * deltaTime;
        if (y < 10)
            Position += glm::vec3(Front.x, 0.0f, Front.z) * velocity;
        if (y > h - 10)
            Position -= glm::vec3(Front.x, 0.0f, Front.z) * velocity;
        if (x < 10)
            Position -= Right * velocity;
        if (x > w - 10)
            Position += Right * velocity;
    }

private:
    void updateCameraVectors()
    {
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        Right = glm::normalize(glm::cross(Front, WorldUp));
        Up = glm::normalize(glm::cross(Right, Front));
    }
};

