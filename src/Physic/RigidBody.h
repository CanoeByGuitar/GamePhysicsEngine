//
// Created by 王晨辉 on 2023/6/8.
//

#ifndef GAMEPHYSICSINONEWEEKEND_RIGIDBODY_H
#define GAMEPHYSICSINONEWEEKEND_RIGIDBODY_H

#include <Geometry/Geometry.h>
#include "MovableObject.h"


class RigidBody : public MovableObject{
public:
    RigidBody(){}
    virtual ~RigidBody(){};

    void Init() override {

    }

    void Finish() override {

    }

    void Update(float dt) override {

    }

};


class RigidBodyBox : public RigidBody{
public:
    RigidBodyBox(const std::shared_ptr<geo::AABB>& box)
    :m_box(box)
    {

    }

    void Move(float dt) override;
private:
    std::shared_ptr<geo::AABB> m_box;
};








#endif //GAMEPHYSICSINONEWEEKEND_RIGIDBODY_H
