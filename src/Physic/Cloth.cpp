//
// Created by 王晨辉 on 2023/6/14.
//

#include "Cloth.h"
#include <cmath>
#include <algorithm>

static std::vector<vec3> last_x;
static std::vector<vec3> init_x;
static std::vector<vec3> last_v;
static float mass = 1;
void Cloth::SetupEdgeList() {
    for (const auto &mesh: m_model->m_meshes) {
        m_indices.reserve(m_indices.size() + mesh.indices.size());
        auto tempSize = m_vertices.size();
        for(int i = 0; i < mesh.indices.size(); i++){
            m_indices.push_back(mesh.indices[i] + tempSize);
        }
        m_vertices.reserve(m_vertices.size() + mesh.vertices.size());
        m_vertices.insert(m_vertices.end(), mesh.vertices.begin(), mesh.vertices.end());
    }
    m_velocities = std::vector<vec3>(m_vertices.size(), vec3(0));
    PHY_INFO("Models have {} meshes, there are totally {} vertices and {} indices.",
             m_model->m_meshes.size(), m_vertices.size(), m_indices.size());


    // {[vert0, vert1, triangleID]}
    std::vector<std::vector<unsigned int>> TripleList(m_indices.size(), std::vector<unsigned int>(3));
    for(int i = 0; i < m_indices.size(); i = i + 3){
        TripleList[i] = {std::min(m_indices[i], m_indices[i + 1]),
                         std::max(m_indices[i], m_indices[i + 1]),
                         (unsigned int)(i / 3)};
        TripleList[i + 1] = {std::min(m_indices[i + 1], m_indices[i + 2]),
                             std::max(m_indices[i + 1], m_indices[i + 2]),
                             (unsigned int)(i / 3)};
        TripleList[i + 2] = {std::min(m_indices[i + 2], m_indices[i]),
                             std::max(m_indices[i + 2], m_indices[i]),
                             (unsigned int)(i / 3)};
    }

    // sort
    sort(TripleList.begin(), TripleList.end(), [](std::vector<unsigned int> lhs, std::vector<unsigned int> rhs){
        if(lhs[0] == rhs[0]){
            if(lhs[1] == rhs[1]){
                return lhs[2] < rhs[2];
            }else{
                return lhs[1] < rhs[1];
            }
        }else{
            return lhs[0] < rhs[0];
        }
    });

    // gen edegList from triple list
    int i = 0;
    unsigned int temp0 = 0;
    unsigned int temp1 = 0;
    m_edgeList.reserve(TripleList.size());
    while(i < TripleList.size()){
        if(TripleList[i][0] == temp0 && TripleList[i][1] == temp1){

        }else{
            m_edgeList.emplace_back(TripleList[i][0],
                                    TripleList[i][1]);
            temp0 = TripleList[i][0];
            temp1 = TripleList[i][1];
        }
        i++;
    }

    // spring initial length
    auto a = m_vertices[m_indices[0]];
    auto b = m_vertices[m_indices[1]];

    m_L.resize(m_edgeList.size());
    for(int idx = 0; idx < m_L.size(); idx++){
        m_L[idx] = glm::distance(m_vertices[m_edgeList[idx].first],
                                 m_vertices[m_edgeList[idx].second]);
    }
}

void Cloth::Init() {
    this->SetupEdgeList();
    int c = 0;
}

void Cloth::Finish() {
    m_model->m_meshes[0].vertices = m_vertices;
//    auto& triVec = m_model->m_meshes[0].triangles;
//    triVec.clear();
//    triVec.reserve(m_indices.size() / 3);
//    for(int i = 0; i < m_indices.size(); i += 3){
//        auto tri = new geo::Triangle(
//                m_vertices[m_indices[i]],
//                m_vertices[m_indices[i + 1]],
//                m_vertices[m_indices[i + 2]]
//        );
//        triVec.push_back(tri);
//    }
    {
        //    static int finish_counter = 0;
//    if(finish_counter == 0){
//        for(int i = 0; i < triVec.size(); i++){
//        PHY_INFO("tri({}): {}, {}, {}",
//                 i, triVec[i]->a, triVec[i]->b, triVec[i]->c);
//        }
//        finish_counter++;
//    }
    }


//    PHY_INFO("Transfer data to renderer, there are totally {} vertices, {} indices and {} triangles.",
//             m_vertices.size(), m_indices.size(), m_model->m_meshes[0].triangles.size());

}

void Cloth::Update(float dt) {
    dt = 0.016;
    int iteration = std::ceil(dt / m_dt);
    PHY_INFO("physic update: {} iters, {:.5f}s per iter", iteration, m_dt);
    for(int i = 0; i < iteration; i++){
//        PHY_INFO("step {}", i);
        TICK(substep)
        AdvanceOneSubstep();
        TOCK(substep)
    }
}


void Cloth::AdvanceOneSubstep(){
    // nenton iteration argmin F(x)
    // initial guess(x0 = x0 or x0 = x0 + dt * v0)
    last_x = m_vertices;
    last_v = m_velocities;
    for(int i = 0; i < m_vertices.size(); i++){
        m_velocities[i] *= 0.99; //damping
    }

    for(int k = 0; k < m_iter; k++){
//        PHY_INFO("Newton iter: {}", k);
        auto dx = SolveLinearSystem();
        for(int i = 0; i < m_vertices.size(); i++){
            if(i == 41 || i == 440 ) continue;
            m_vertices[i] += dx[i];
        }
    }
    for(int i = 0; i < m_vertices.size(); i++){
        m_velocities[i] = (m_vertices[i] - last_x[i]) / m_dt;
    }
}

std::vector<vec3> Cloth::SolveLinearSystem() {
    // g : gredient * (-1)
    std::vector<vec3> g(m_vertices.size());
    for(int i = 0; i < m_vertices.size(); i++){

        g[i] = - mass * (m_vertices[i] - (last_x[i] + m_dt * last_v[i])) / (m_dt * m_dt)
                + mass * vec3(0, -9.8, 0);

        for(int e = 0; e < m_edgeList.size(); e++){
            auto a = m_edgeList[e].second;
            auto b = m_edgeList[e].first;
            auto pa = m_vertices[m_edgeList[e].second];
            auto pb = m_vertices[m_edgeList[e].first];

            vec3 f = -m_springK * (glm::length(pa - pb) - m_L[e]) * glm::normalize(pa - pb);
            // fa = fa + f  fb = fb - f
            // ga = ga + fa  gb = gv - fb
            g[a] = g[a] + f;
            g[b] = g[b] - f;
        }
    }

    // simplified by taking H as diagnol elsewise should use jacobbi
    for(int i = 0; i < m_vertices.size(); i++){
        g[i] /= (mass / (m_dt * m_dt) + 4.f * m_springK);
    }

    return g;
}


