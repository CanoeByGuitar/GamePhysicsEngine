//
// Created by 王晨辉 on 2023/6/1.
//

#ifndef GAMEPHYSICSINONEWEEKEND_ACTOR_H
#define GAMEPHYSICSINONEWEEKEND_ACTOR_H

#include <Base/Log.h>
#include <Geometry/Geometry.h>
#include <Geometry/ParticleSystem.h>
#include <Renderer/Objects/ObectBase.h>

#include <utility>
#include "Physic/Cloth.h"
#include "Physic/RigidBody.h"
#include "Renderer/Objects/Cube.h"
#include "Renderer/Objects/Mesh.h"
#include "Renderer/Objects/Particles.h"
#include "Renderer/Objects/Sphere.h"

struct RenderComponent {
    std::vector<RenderObjPtr> objects;
    renderer::DrawMode drawMode;
    renderer::PrimitiveType primitiveType;

};

struct PhysicsComponent {
    std::shared_ptr<MovableObject> object;
};

class Actor {
   public:
    Actor(std::string name):m_name(std::move(name)){};
    virtual void InitRenderObject() = 0;
    virtual void InitPhysicsObject() = 0;

   public:
    std::shared_ptr<RenderComponent> m_renderComponent;
    std::shared_ptr<PhysicsComponent> m_physicsComponent;
    std::string m_name;
    void* m_geometryCopy = nullptr;
    
};

class AabbActor : public Actor {
   public:
    AabbActor(GeoAabbPtr cube, std::string name);
    void InitRenderObject() override;
    void InitPhysicsObject() override;

   private:
    GeoAabbPtr m_geometry;
};

class ModelActor : public Actor{
   public:
    ModelActor(GeoModelPtr model, std::string name);
    void InitRenderObject() override;
    void InitPhysicsObject() override;
   private:
    GeoModelPtr m_geometry;
};

class SphereActor : public Actor{
   public:
    SphereActor(GeoSpherePtr sphere, std::string name);
    void InitRenderObject() override;
    void InitPhysicsObject() override;
   private:
    GeoSpherePtr m_geometry;
};

class ParticlesActor : public Actor{
   public:
    ParticlesActor(GeoParticlesPtr particle, std::string name);
    void InitRenderObject() override;
    void InitPhysicsObject() override;
   private:
    GeoParticlesPtr m_geometry;
};

#endif  //GAMEPHYSICSINONEWEEKEND_ACTOR_H
