//
// Created by 王晨辉 on 2023/6/5.
//

#ifndef GAMEPHYSICSINONEWEEKEND_TIMER_H
#define GAMEPHYSICSINONEWEEKEND_TIMER_H

#include <functional>
#include <utility>

class Timer{
    using TimerCallbackFunc = std::function<void()>;

public:
    Timer(const float secondsBetweenCallbackTriggers, TimerCallbackFunc callback)
    :m_secondsBetweenCallbackTriggers(secondsBetweenCallbackTriggers),
    m_callbackFunc(callback)
    {}

    void Update(const float time){
        if(time - m_prevTriggerTime >= m_secondsBetweenCallbackTriggers){
            m_callbackFunc;
            m_prevTriggerTime += m_secondsBetweenCallbackTriggers;
        }
        m_delta = time - m_prevTime;
        m_prevTime = time;
    }

    auto GetDelta() const{
        return m_delta;
    }
private:
    float m_prevTriggerTime = 0.f;
    float m_prevTime = 0.f;
    float m_delta = 0.f;
    const float m_secondsBetweenCallbackTriggers;
    const TimerCallbackFunc  m_callbackFunc;

};


#endif //GAMEPHYSICSINONEWEEKEND_TIMER_H
