//
// Created by 王晨辉 on 2023/6/8.
//

#ifndef GAMEPHYSICSINONEWEEKEND_MOVABLEOBJECT_H
#define GAMEPHYSICSINONEWEEKEND_MOVABLEOBJECT_H

#include <Base/Log.h>


class MovableObject{
public:
    MovableObject() = default;
    virtual ~MovableObject() = default;
    virtual void Move(float dt) = 0;
    virtual void Init() = 0;
    virtual void Finish() = 0;
    virtual void Update(float dt) = 0;
    virtual void Restart(){};
protected:
    vec3 vel = vec3(0.f, -0.5f, 0.f);
};

#endif //GAMEPHYSICSINONEWEEKEND_MOVABLEOBJECT_H
