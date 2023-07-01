//
// Created by 王晨辉 on 2023/6/14.
//

#ifndef GAMEPHYSICSINONEWEEKEND_CLOTH_H
#define GAMEPHYSICSINONEWEEKEND_CLOTH_H

#include <Geometry/Geometry.h>
#include "MovableObject.h"
#include <Base/DataStructure.h>

enum ClothScheme{ImplicitSolver, PBD};


class Cloth : public MovableObject{
public:
    Cloth(const std::shared_ptr<geo::Model>& model)
    :m_model(model){}

    virtual ~Cloth(){}

    void Move(float dt) override{};

    void Init() override;

    void Finish() override;

    void Update(float dt) override;

    void SetupEdgeList();

    void AdvanceOneSubstep();

    std::vector<vec3> SolveLinearSystem();

    void HandleCollision();

    void Restart() override;

   private:
    ClothScheme m_clothScheme = ClothScheme::PBD;

    GeoModelPtr m_model;


    std::vector<vec3> m_originVertices; // x at t0 used to replay
    std::vector<vec3> m_originVelocities; // x at t0 used to replay
    std::vector<vec3> m_vertices; // x
    std::vector<vec3> m_velocities; // v
    
    std::vector<unsigned int> m_indices;
    std::vector<std::pair<int, int>> m_edgeList;
    std::vector<float> m_L; // initial length of spring

    float m_mass = 1;
    float m_dt = 4.8e-3; // substep  1.2e-3 for implicit solver
    int m_iter = 16; // newton max iters
    float m_epsilon = 1e-2; // newton delta x threshould
    float m_springK = 200; // spring force

    vec3 m_gravity = vec3(0, -9.8, 0);
    vec3 m_wind = vec3(0, 0, 0);

    std::unordered_map<int, int> m_fixedIndices = {{41, 1}, {440, 1}};

};

#endif //GAMEPHYSICSINONEWEEKEND_CLOTH_H
