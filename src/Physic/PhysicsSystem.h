//
// Created by 王晨辉 on 2023/6/8.
//

#ifndef GAMEPHYSICSINONEWEEKEND_PHYSICSSYSTEM_H
#define GAMEPHYSICSINONEWEEKEND_PHYSICSSYSTEM_H

#include "RigidBody.h"
class PhysicsSystem{
public:
    virtual ~PhysicsSystem(){}

    void AddObject(const std::shared_ptr<MovableObject>& object);

    virtual void Update(float dt);


protected:
    std::vector<std::shared_ptr<MovableObject>> m_objects;
};

class ThreeBodySystem : public PhysicsSystem{
    virtual void Update(float dt){
        for(auto& body : m_objects){
            body->Move(-dt);
        }
    }
};

#endif //GAMEPHYSICSINONEWEEKEND_PHYSICSSYSTEM_H
