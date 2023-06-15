//
// Created by 王晨辉 on 2023/6/14.
//

#include "Cloth.h"
#include <cmath>
#include <algorithm>


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

    int i = 0;
    unsigned int temp0 = 0;
    unsigned int temp1 = 0;
    m_edgeList.reserve(TripleList.size());
    while(i < TripleList.size()){
        if(TripleList[i][0] == temp0 && TripleList[i][1] == temp1){

        }else{
            m_edgeList.emplace_back(m_vertices[TripleList[i][0]],
                                           m_vertices[TripleList[i][1]]);
            temp0 = TripleList[i][0];
            temp1 = TripleList[i][1];
        }
        i++;
    }
}

void Cloth::Init() {
    this->SetupEdgeList();
}

void Cloth::Finish() {
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

}



