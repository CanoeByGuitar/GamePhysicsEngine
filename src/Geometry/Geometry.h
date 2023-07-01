//
// Created by chenhui on 23-5-23.
//

#ifndef GAMEPHYSICSINONEWEEKEND_GEOMETRY_H
#define GAMEPHYSICSINONEWEEKEND_GEOMETRY_H

#include <Base/Log.h>
#include <cmath>

namespace geo{

    enum BVHSplitStrategy : int{
        MID_X_ALIAS,
        MID_TRIANGLE_PARTITION
    };


    struct Line{
        vec3 start;
        vec3 end;
        Line() = default;
        Line(const vec3& s, const vec3& e): start(s), end(e){}
    };

    struct Ray{
        vec3 origin;
        vec3 direction;
        Ray() = default;
        Ray(const vec3& o, const vec3& d) : origin(o), direction(glm::normalize(d)){}
    };

    struct Sphere{
        vec3 position;
        float radius;
        Sphere():radius(0.f){}
        Sphere(const vec3& p, float r):position(p), radius(r){}
    };

    struct AABB{
        vec3 position;
        vec3 halfSize;
        AABB() = default;
        AABB(const vec3& p, const vec3& s): position(p), halfSize(s){}
    };

    struct OBB{
        // oriented OBB
        vec3 position;
        vec3 halfSize;
        mat3 orientation;
        OBB() = default;
        OBB(const vec3& p, const vec3& s):position(p), halfSize(s), orientation(mat3()) {}
        OBB(const vec3& p, const vec3& s, const mat3& o):position(p), halfSize(s), orientation(o) {}

    };

    struct Plane{
        vec3 normal;
        float distance;
        Plane():normal(vec3(0, 1, 0)), distance(0.f){}
        Plane(const vec3& n, float d) : normal(n), distance(d) {}
    };

    struct Triangle{
        union{
            struct{
                vec3 a;
                vec3 b;
                vec3 c;
            };
            struct{
                vec3 p1;
                vec3 p2;
                vec3 p3;
            };
            vec3 points[3];
        };
        Triangle():a(vec3()), b(vec3()), c(vec3()){}
        Triangle(const vec3& a, const vec3& b, const vec3& c): a(a), b(b), c(c){}
    };

    struct BVHNode{
        AABB bound;
        std::vector<Triangle*> triangles;
        BVHNode* left;
        BVHNode* right;

        BVHNode():left(nullptr), right(nullptr) {}

        BVHNode(const AABB& aabb, std::vector<Triangle*> &&tri)
                :bound(aabb), triangles(tri), left(nullptr), right(nullptr)
        {}

        BVHNode(const AABB& aabb, const std::vector<Triangle*> &tri)
                :bound(aabb), triangles(tri), left(nullptr), right(nullptr)
        {}


    };


    struct Mesh{
        // TODO: simplify data structure
        std::vector<Triangle*> triangles;
        std::vector<vec3> vertices;
        std::vector<unsigned int> indices;
        BVHNode* accelerator;
        AABB bound;

        Mesh():accelerator(nullptr){}
    };


    struct Model{
        std::vector<Mesh> m_meshes;

        Model() = default;
    };





    void AccelerateMesh(Mesh &mesh);

    void RecursiveBuildBVH(BVHNode* node, int kThreshold);

    void TraverseBVH(BVHNode *node, std::unordered_map<int, std::vector<BVHNode*>> &boundMap);

    inline vec3 GetMin(const AABB& aabb){
        return aabb.position - aabb.halfSize;
    }

    inline vec3 GetMax(const AABB& aabb){
        return aabb.position + aabb.halfSize;
    }

    inline AABB UnionBounds(const AABB& lhs, const AABB& rhs){
        vec3 min = glm::min(GetMin(lhs), GetMin(rhs));
        vec3 max = glm::max(GetMax(lhs), GetMax(rhs));
        return {0.5f * (min + max), 0.5f * (max - min)};
    }


    inline AABB GetBound(const Triangle& tri){
        vec3 min = tri.points[0];
        vec3 max = tri.points[0];
        for(int i = 1; i < 3; i++){
            min = glm::min(min, tri.points[i]);
            max = glm::max(max, tri.points[i]);
        }
        return {0.5f * (min + max), 0.5f * (max - min)};
    }

    inline AABB GetBound(const std::vector<Triangle*> &vec){
        if(vec.empty()) return {};
        auto ret = GetBound(*vec[0]);
        for(const auto tri : vec){
            ret = UnionBounds(ret, GetBound(*tri));
        }
        return ret;
    }

    inline AABB GetBound(const Mesh& mesh){
        vec3 min = mesh.vertices[0];
        vec3 max = mesh.vertices[0];
        for(int i = 1; i < mesh.vertices.size(); i++){
            min = glm::min(min, mesh.vertices[i]);
            max = glm::max(max, mesh.vertices[i]);
        }
        return {0.5f * (min + max), 0.5f * (max - min)};
    }

    inline AABB GetBound(const Model& model){
        auto ret = GetBound(model.m_meshes[0]);
        for(int i = 1; i < model.m_meshes.size(); i++){
            ret = UnionBounds(ret, GetBound(model.m_meshes[i]));
        }
        return ret;
    }
}


using GeoAabbPtr = std::shared_ptr<geo::AABB>;
using GeoModelPtr = std::shared_ptr<geo::Model>;
using GeoSpherePtr = std::shared_ptr<geo::Sphere>;
using GeoMeshPtr = std::shared_ptr<geo::Mesh>;














#endif //GAMEPHYSICSINONEWEEKEND_GEOMETRY_H
