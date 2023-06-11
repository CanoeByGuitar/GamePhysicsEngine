//
// Created by 王晨辉 on 2023/6/1.
//

#ifndef GAMEPHYSICSINONEWEEKEND_ACTOR_H
#define GAMEPHYSICSINONEWEEKEND_ACTOR_H

#include <Base/Log.h>
#include <Geometry/Geometry.h>
#include <Renderer/Objects/ObectBase.h>
#include "Renderer/Objects/Cube.h"
#include "Renderer/Objects/Mesh.h"
#include "Physic/RigidBody.h"

struct RenderComponent{
    std::shared_ptr<renderer::Object> object;
    renderer::DrawMode drawMode;
    renderer::PrimitiveType primitiveType;
    vec3 color = vec3{-1};

    void SetDrawMode(renderer::DrawMode mode){
        object->SetDrawMode(mode);
    }

    void SetPrimitiveType(renderer::PrimitiveType type){
        object->SetPrimitiveType(type);
    }

    void SetColor(const vec3 &c){
        object->SetColor(c);
    }
};

struct PhysicsComponent{
    std::shared_ptr<MovableObject> object;

};


class Actor{
public:
    virtual void InitRenderObject() = 0;
    virtual void UpdateRenderObject() = 0;
    virtual void InitPhysicsObject() = 0;

public:
    std::shared_ptr<RenderComponent> m_renderComponent;
    std::shared_ptr<PhysicsComponent> m_physicsComponent;

};

template<typename T>
class ActorBase : public Actor{
public:
    void InitRenderObject() override {}
    void UpdateRenderObject() override {}
    virtual void InitPhysicsObject() override {};
public:
    std::shared_ptr<T> m_geometry;
};



template<>
class ActorBase<geo::AABB> : public Actor{
public:
    explicit ActorBase<geo::AABB>(const std::shared_ptr<geo::AABB>& cube)
            :m_geometry(cube){
        m_renderComponent = std::make_shared<RenderComponent>();
    }

    ActorBase<geo::AABB>(const std::string& name, const std::shared_ptr<geo::AABB>& cube)
            :m_name(name), m_geometry(cube){
        m_renderComponent = std::make_shared<RenderComponent>();

    }


    void InitRenderObject() override {
        m_renderComponent->object = std::make_shared<renderer::Cube>(
                m_name.c_str(),
                m_geometry,
                m_renderComponent->drawMode,
                m_renderComponent->primitiveType,
                m_renderComponent->color
                );
    }

    void InitPhysicsObject() override{
        m_physicsComponent = std::make_shared<PhysicsComponent>();
        m_physicsComponent->object = std::make_shared<RigidBodyBox>(
                    m_geometry);

    }

    void UpdateRenderObject() override {

    }

private:
    std::shared_ptr<geo::AABB> m_geometry;
    std::string m_name = "test";
};

template<>
class ActorBase<geo::Model> : public Actor{
public:
    explicit ActorBase<geo::Model>(const std::shared_ptr<geo::Model>& model)
            :m_geometry(model){
        m_renderComponent = std::make_shared<RenderComponent>();
    }

    ActorBase<geo::Model>(const std::string& name, const std::shared_ptr<geo::Model>& model)
            :m_geometry(model), m_name(name){
        m_renderComponent = std::make_shared<RenderComponent>();
    }


    void InitRenderObject() override {
        m_renderComponent->object = std::make_shared<renderer::BasicModel>(
                m_name.c_str(),
                m_geometry,
                m_renderComponent->drawMode,
                m_renderComponent->primitiveType,
                m_renderComponent->color
        );
    }

    void UpdateRenderObject() override {}

    void InitPhysicsObject() override{}

private:
    std::shared_ptr<geo::Model> m_geometry;
    std::string m_name = "test";
};



#endif //GAMEPHYSICSINONEWEEKEND_ACTOR_H
