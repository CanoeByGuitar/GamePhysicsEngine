//
// Created by 王晨辉 on 2023/5/28.
//

#include "Camera.h"

Camera::Camera() {
    updateVectors();
}

void Camera::Update(const double deltaTime) {
//    if (Input::GetInstance().IsKeyPressed(GLFW_KEY_TAB)) {
//        m_dirty = !m_dirty;
//        PHY_INFO("Click on TAB, m_dirty = {}", m_dirty);
//    }

    if (m_dirty) {
        // Update view from mouse movement
        updateView();

        // Update Keyboard
        if (Input::GetInstance().IsKeyHeld(GLFW_KEY_W)) { processKeyboard(Direction::FORWARD, deltaTime); }
        if (Input::GetInstance().IsKeyHeld(GLFW_KEY_S)) { processKeyboard(Direction::BACKWARD, deltaTime); }
        if (Input::GetInstance().IsKeyHeld(GLFW_KEY_A)) { processKeyboard(Direction::LEFT, deltaTime); }
        if (Input::GetInstance().IsKeyHeld(GLFW_KEY_D)) { processKeyboard(Direction::RIGHT, deltaTime); }
        if (Input::GetInstance().IsKeyHeld(GLFW_KEY_SPACE)) { processKeyboard(Direction::UP, deltaTime); }
        if (Input::GetInstance().IsKeyHeld(GLFW_KEY_LEFT_CONTROL)) { processKeyboard(Direction::DOWN, deltaTime); }
    }
}

void Camera::SetNear(const float near) {
    m_near = near;
}

void Camera::SetFar(const float far) {
    m_far = far;
}

void Camera::SetSpeed(const float speed) {
    m_speed = speed;
}


void Camera::updateView(const bool constrainPitch) {
    if(Input::GetInstance().MouseMoved()){
        const auto xPos = Input::GetInstance().GetMouseX();
        const auto yPos = Input::GetInstance().GetMouseY();

        if(m_firstMouse){
            m_prevX = xPos;
            m_prevY = yPos;
            m_firstMouse = false;
        }

        const auto xOffset = (xPos - m_prevX) * m_sensitivity;
        const auto yOffset = (yPos - m_prevY) * m_sensitivity;
        m_prevX = xPos;
        m_prevY = yPos;


        if(Input::GetInstance().IsMouseClicked(GLFW_MOUSE_BUTTON_1)){
            // only update camera info when mouse button 1 clicked
            m_yaw += xOffset;
            m_pitch -= yOffset;

            if(constrainPitch){
                if(m_pitch > 89.0) m_pitch = 89.0;
                if(m_pitch < -89.0) m_pitch = -89.0;
            }
            updateVectors();
        }

    }
}

void Camera::processKeyboard(const Camera::Direction direction, const double deltaTime) {
    const float velocity = m_speed * static_cast<float>(deltaTime);
    switch (direction) {
        case Direction::FORWARD:
            m_position += m_front * velocity;
            break;
        case Direction::BACKWARD:
            m_position -= m_front * velocity;
            break;
        case Direction::LEFT:
            m_position -= m_right * velocity;
            break;
        case Direction::RIGHT:
            m_position += m_right * velocity;
            break;
        case Direction::UP:
            m_position += m_worldUp * velocity;
            break;
        case Direction::DOWN:
            m_position -= m_worldUp * velocity;
            break;
    }
}

void Camera::updateVectors() {
    vec3 front = {
            glm::cos(glm::radians(m_yaw)) * glm::cos(glm::radians(m_pitch)),
            glm::sin(glm::radians(m_pitch)),
            glm::sin(glm::radians(m_yaw)) * glm::cos(glm::radians(m_pitch))
    };

    m_front = glm::normalize(front);
    m_right = glm::normalize(glm::cross(m_front, m_worldUp));
    m_up = glm::normalize(glm::cross(m_right, m_front));
}
