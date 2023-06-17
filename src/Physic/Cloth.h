//
// Created by 王晨辉 on 2023/6/14.
//

#ifndef GAMEPHYSICSINONEWEEKEND_CLOTH_H
#define GAMEPHYSICSINONEWEEKEND_CLOTH_H

#include <Geometry/Geometry.h>
#include "MovableObject.h"




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

private:
    std::shared_ptr<geo::Model> m_model;


    std::vector<vec3> m_vertices; // x
    std::vector<vec3> m_velocities; // v
    
    std::vector<unsigned int> m_indices;
    std::vector<std::pair<int, int>> m_edgeList;
    std::vector<float> m_L; // initial length of spring
    float m_dt = 1.2e-3; // substep
    int m_iter = 8; // newton max iters
    float m_epsilon = 1e-6; // newton delta x threshould
    float m_springK = 200; // spring force
};

#endif //GAMEPHYSICSINONEWEEKEND_CLOTH_H
