//
// Created by 王晨辉 on 2023/6/8.
//

#include "RigidBody.h"

void RigidBodyBox::Move(float dt) {
    m_box->position += (vel * dt);
}

void RigidBodySphere::Move(float dt) {

}
