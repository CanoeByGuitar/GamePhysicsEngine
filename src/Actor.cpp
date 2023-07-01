//
// Created by 王晨辉 on 2023/6/1.
//

#include "Actor.h"

AabbActor::AabbActor(GeoAabbPtr cube, std::string name)
    : m_geometry(std::move(cube)), Actor(std::move(name)) {
    m_renderComponent = std::make_shared<RenderComponent>();
    m_geometryCopy = m_geometry.get();
}

void AabbActor::InitRenderObject() {
    m_renderComponent->objects.push_back(std::make_shared<renderer::Cube>(
        m_name.c_str(), m_geometry, m_renderComponent->drawMode,
        m_renderComponent->primitiveType));
}
void AabbActor::InitPhysicsObject() {
    m_physicsComponent = std::make_shared<PhysicsComponent>();
    m_physicsComponent->object = std::make_shared<RigidBodyBox>(m_geometry);
}

ModelActor::ModelActor(GeoModelPtr model, std::string name)
    : m_geometry(std::move(model)), Actor(std::move(name)) {
    m_renderComponent = std::make_shared<RenderComponent>();
    m_geometryCopy = m_geometry.get();
}

void ModelActor::InitRenderObject() {
    for (auto& mesh : m_geometry->m_meshes) {
        m_renderComponent->objects.push_back(std::make_shared<renderer::Mesh>(
            m_name.c_str(), std::shared_ptr<geo::Mesh>(&mesh),
            m_renderComponent->drawMode, m_renderComponent->primitiveType));
    }
}

void ModelActor::InitPhysicsObject() {
    m_physicsComponent = std::make_shared<PhysicsComponent>();
    m_physicsComponent->object = std::make_shared<Cloth>(m_geometry);
}

SphereActor::SphereActor(GeoSpherePtr sphere, std::string name)
    : m_geometry(std::move(sphere)), Actor(std::move(name)) {
    m_renderComponent = std::make_shared<RenderComponent>();
    m_geometryCopy = m_geometry.get();
}
void SphereActor::InitRenderObject() {
    m_renderComponent->objects.push_back(
        std::make_shared<renderer::Sphere>(
            m_name.c_str(),
            m_geometry,
            m_renderComponent->drawMode,
            m_renderComponent->primitiveType
            )
        );
}
void SphereActor::InitPhysicsObject() {
    m_physicsComponent = std::make_shared<PhysicsComponent>();
    m_physicsComponent->object =
        std::make_shared<RigidBodySphere>(m_geometry);
}

ParticlesActor::ParticlesActor(const GeoParticlesPtr& particle, std::string name)
    : m_geometry(particle), Actor(std::move(name)) {
    m_renderComponent = std::make_shared<RenderComponent>();
    m_geometryCopy = m_geometry.get();
}

void ParticlesActor::InitRenderObject() {
    m_renderComponent->objects.push_back(
        std::make_shared<renderer::Particles>(
            m_name.c_str(),
            m_geometry,
            m_renderComponent->drawMode,
            m_renderComponent->primitiveType
            )
    );
}
void ParticlesActor::InitPhysicsObject() {}
