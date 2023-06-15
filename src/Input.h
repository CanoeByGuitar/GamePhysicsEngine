//
// Created by 王晨辉 on 2023/6/2.
//

#ifndef GAMEPHYSICSINONEWEEKEND_INPUT_H
#define GAMEPHYSICSINONEWEEKEND_INPUT_H

#include <functional>
#include <array>
#include <Base/Log.h>
#include <GLFW/glfw3.h>





class Input{
public:

    Input(const Input&) = delete;
    Input& operator=(const Input&) = delete;

    static Input& GetInstance(){
        static Input instance;
        return instance;
    };

    void Update(){
        m_mouseMoved = false;
        m_shouldResize = false;
        std::copy(m_keys.cbegin(), m_keys.cend(), m_prevKeys.begin());

    }

    bool IsKeyPressed(const std::size_t key) const {
        PHY_ASSERT(key < 1024, "Key out of bound!");
        return m_keys[key] && !m_prevKeys[key];
    }

    bool IsKeyHeld(const std::size_t key) const{
        PHY_ASSERT(key < 1024, "Key out of bound!");
        return m_keys[key];
    }

    bool IsMouseClicked(const std::size_t key) const {
        return m_buttons[key];
    }

    bool MouseMoved() const {return m_mouseMoved;}
    float GetMouseX() const {return m_xPos;}
    float GetMouseY() const {return m_yPos;}

    bool ShouldResize() const {return m_shouldResize;}
    bool GetWidth() const {return m_width;}
    bool GetHeight() const {return m_height;}

public:
    std::function<void(double, double)> mouseMoved = [&](auto xPos, auto yPos){
        this->m_mouseMoved = true;
        this->m_xPos = xPos;
        this->m_yPos = yPos;
    };

    std::function<void(int, int, int)> mouseClicked = [&](auto button, auto action, auto mode){
        if(m_mouseClickEnabled && button >= 0 && button < 128){
            switch (action){
                case GLFW_PRESS:
//                    PHY_DEBUG("press");
                    m_buttons[button] = true;
                    break;
                case GLFW_RELEASE:
                    m_buttons[button] = false;
//                    PHY_DEBUG("release");
                    break;
            }
        }
    };

    std::function<void()> enableMouse = [&](){
        m_mouseClickEnabled = true;
    };

    std::function<void()> disableMouse = [&](){
        m_mouseClickEnabled = false;
    };

    std::function<void(int, int, int, int)> keyPressed
        = [&](auto key, auto scancode, auto action, auto mode){
        if(key >= 0 && key < 1024){
            switch (action) {
                case GLFW_PRESS:
                    this->m_keys[key] = true;
//                    PHY_DEBUG("press: {}", key);
                    break;
                case GLFW_RELEASE:
                    this->m_keys[key] = false;
//                    PHY_DEBUG("release: {}", key);
                    break;
            }
        }
    };



    std::function<void(int, int)> windowResized
        = [&](auto width, auto height){
        this->m_shouldResize = true;
        this->m_width = width;
        this->m_height = height;
    };

private:
    Input(){
        std::fill(m_keys.begin(), m_keys.end(), false);
        std::fill(m_prevKeys.begin(), m_prevKeys.end(), false);
        std::fill(m_buttons.begin(), m_buttons.end(), false);
    }



private:
    std::array<bool, 1024> m_keys;
    std::array<bool, 1024> m_prevKeys;

    std::array<bool, 128> m_buttons;

    bool m_mouseMoved = false;
    float m_xPos, m_yPos;

    bool m_shouldResize = false;
    std::size_t m_width, m_height;

    bool m_mouseClickEnabled = true;

};


#endif //GAMEPHYSICSINONEWEEKEND_INPUT_H
