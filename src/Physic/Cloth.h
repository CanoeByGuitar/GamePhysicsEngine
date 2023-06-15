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

private:
    std::shared_ptr<geo::Model> m_model;


    std::vector<vec3> m_vertices;
    std::vector<unsigned int> m_indices;
    std::vector<geo::Line> m_edgeList;
};

#endif //GAMEPHYSICSINONEWEEKEND_CLOTH_H
