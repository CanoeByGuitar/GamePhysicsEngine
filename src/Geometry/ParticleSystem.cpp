//
// Created by 王晨辉 on 2023/6/22.
//

#include "ParticleSystem.h"
#include <stack>
#include <algorithm>



bool isClockWise(const vec2& t, const vec2& s, const vec2& p){
    // p: p0
    // t: top point of stack
    // s: second top point of stack
    // detect if pt is on the right of ps
    auto st = t - s;
    auto sp = p - s;
    if(glm::cross(vec3(st, 0), vec3(sp, 0)).z < 0) return true;
    return false;
}

std::vector<int> geo::Particles2D::ComputeConvexHull() {
    if (m_pos.size() == 1) return {0};
    if (m_pos.size() == 2) return {0, 1};
    if (m_pos.size() == 3) return {0, 1, 2};
    auto points = this->m_pos;

    // find the lowest y-coordinate and leftmost point
    int minIdx = 0;
    vec2 p0 = points[0];
    for (int i = 1; i < points.size(); i++) {
        if (points[i].y < p0.y ||
            (points[i].y == p0.y && points[i].x < p0.x)) {
            p0 = points[i];
            minIdx = i;
        }
    }

    // Store the indices of the points in the original list
    std::vector<int> indices(points.size());
    for (int i = 0; i < points.size(); i++) {
        indices[i] = i;
    }

    std::swap(indices[0], indices[minIdx]);

    // sort points by polar angle with P0
    std::sort(indices.begin() + 1, indices.end(),[&](int i, int j){
        auto a = points[i];
        auto b = points[j];
        auto pa = (a - p0);
        auto pb = (b - p0);
        auto cos_pa = glm::normalize(pa);
        auto cos_pb = glm::normalize(pb);
        if(abs(cos_pa.x - cos_pb.x) < 1e-5){
            return glm::length(pa) < glm::length(pb);
        }
        return cos_pa.x > cos_pb.x;
    });

    std::stack<int> stk;
    stk.push(indices[0]);
    stk.push(indices[1]);
    stk.push(indices[2]);
    // p0 - p1 - p2 must be anti-clockwise for the vector has been sorted

    auto nextToTop = [&](){
        int t = -1, s = -1;
        if(stk.size() >= 3){
            t = stk.top();
            stk.pop();
            s = stk.top();
            stk.push(t);
        }
        return s;
    };
    for(int i = 3; i < points.size(); i++){
        while(stk.size() >= 3 && isClockWise(points[stk.top()], points[nextToTop()], points[indices[i]])){
            // pop the top element
            stk.pop();
        }
        stk.push(indices[i]);
    }

    std::vector<int> ret;
    while(!stk.empty()){
        ret.emplace_back(stk.top());
        stk.pop();
    }
    return ret;
}


geo::Particles3D geo::Particles2D::to3D(float z) {
    std::vector<vec3> vec(m_pos.size());
    for(int i = 0; i < m_pos.size(); i++){
        vec[i] = {m_pos[i], z};
    }
    Particles3D ret(vec);
    return ret;
}

std::vector<std::vector<vec3>> geo::Particles2D::ComputeMultiLayerConvexHull() {
    auto tempPos = m_pos;
    std::vector<std::vector<vec3>> ret;
    while(!m_pos.empty()){
        auto ch = ComputeConvexHull();
        std::vector<vec3> vec;
        vec.reserve(m_pos.size());
        for(int i : ch){
            vec.emplace_back(m_pos[i], 0);
        }
        ret.push_back(vec);

        std::sort(ch.begin(), ch.end(), [](int i, int j){
            return i > j;
        });
        for(auto idx : ch){
            m_pos.erase(m_pos.begin() + idx);
        }
    }
    m_pos = tempPos;
    return ret;
}

std::vector<std::vector<vec3>> geo::Particles2D::ComputeMultiLayerConvexHullOn2() {
    auto points = m_pos;
    std::sort(points.begin(), points.end(), [&](const vec2& a, const vec2& b){
        if(a.x == b.x){
            return a.y < b.y;
        }else{
            return a.x < b.x;
        }
    });
    int n = (int)points.size();
    std::vector<int> isVisit(n);
    std::fill(isVisit.begin(), isVisit.end(), false);

    int cnt = 0;
    std::vector<std::vector<vec3>> ret;
    while(cnt != n){
        std::vector<int> U, L;
        for(int i = 0; i < n; i++){
            if(isVisit[i]) continue;
            while(L.size() >= 2 && !isClockWise(points[L[L.size() - 1]],
                                                points[L[L.size() - 2]],
                                                points[i])){
                L.pop_back();
            }
            L.push_back(i);
        }
        for(int i = n - 1; i >= 0; i--){
            if(isVisit[i]) continue;
            while(U.size() >= 2 && !isClockWise(points[U[U.size() - 1]],
                                                points[U[U.size() - 2]],
                                                points[i])){
                U.pop_back();
            }
            U.push_back(i);
        }
        L.pop_back();
        U.pop_back();

        std::vector<vec3> temp;
        for(int i : L){
            isVisit[i] = true;
            temp.emplace_back(points[i], 0);
            cnt++;
        }
        for(int i : U){
            isVisit[i] = true;
            temp.emplace_back(points[i], 0);
            cnt++;
        }
        ret.push_back(temp);
    }
    return ret;
}
