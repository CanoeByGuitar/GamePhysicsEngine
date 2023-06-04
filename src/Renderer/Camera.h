//
// Created by 王晨辉 on 2023/5/28.
//

#ifndef GAMEPHYSICSINONEWEEKEND_CAMERA_H
#define GAMEPHYSICSINONEWEEKEND_CAMERA_H

#include <Base/Log.h>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include "Input.h"

class Camera{
public:
    Camera();
    void Update(const double deltaTime);
    void SetNear(const float near);
    void SetFar(const float far);
    void SetSpeed(const float speed);
    auto GetViewMatrix() const { return lookAt(m_position, m_position + m_front, m_up); }
    auto GetProjMatrix(const float width, const float height) const { return glm::perspective(m_FOV, width / height, m_near, m_far); }
    auto GetPosition() const { return m_position; }

private:
    enum class Direction {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
        UP,
        DOWN
    };

    void updateView(const bool constrainPitch = true);
    void processKeyboard(const Direction direction, const double deltaTime);
    void updateVectors();


    vec3 m_position = vec3(0.f);
    vec3 m_front = vec3(0.f, 0.f, -1.0f);
    vec3 m_up = vec3(0.f, 1.f, 0.f);
    vec3 m_right;
    const vec3 m_worldUp = vec3(0.f, 1.f, 0.f);
    float m_near = 1.0f, m_far = 100.0f;

    float m_yaw = -90.0f;
    float m_pitch = 0.0f;

    float m_speed = 5.0f;
    const float m_sensitivity = 0.3f;
    const float m_FOV = 70.0f;

    bool m_firstMouse = true;
    float m_prevX = 0.f;
    float m_prevY = 0.f;

    bool m_dirty = true;
};


#endif //GAMEPHYSICSINONEWEEKEND_CAMERA_H
