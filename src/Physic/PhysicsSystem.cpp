//
// Created by 王晨辉 on 2023/6/8.
//

#include "PhysicsSystem.h"

void PhysicsSystem::AddObject(const std::shared_ptr<MovableObject> &object) {
    m_objects.push_back(object);
}

void PhysicsSystem::Update(float dt) {
    for(auto& body : m_objects){
        body->Move(dt);
        body->Update(dt);
    }
}

void PhysicsSystem::Init() {
    for(auto& object : m_objects){
        object->Init();
    }
}

void PhysicsSystem::Finish() {
    for(auto& object : m_objects){
        object->Finish();
    }
}
