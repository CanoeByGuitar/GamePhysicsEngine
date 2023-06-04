//
// Created by chenhui on 23-5-23.
//

#include "Geometry.h"
#include "algorithm"

namespace geo{
    void AccelerateMesh(Mesh &mesh) {
        if (mesh.accelerator) return;
        mesh.accelerator = new BVHNode(mesh.bound, mesh.triangles);
        RecursiveBuildBVH(mesh.accelerator, 5);

    }

    std::pair<BVHNode*, BVHNode*> SplitBVHNode(BVHNode* node){
        // x-alias partition
        auto bound = node->bound;
        std::vector<Triangle*> leftTriangles, rightTriangles;
        leftTriangles.reserve(node->triangles.size());
        rightTriangles.reserve(node->triangles.size());
        for(const auto& tri : node->triangles){
            if(GetMax(GetBound(*tri)).x < GetMin(bound).x){
                leftTriangles.push_back(tri);
            }else{
                rightTriangles.push_back(tri);
            }
        }
        auto left = new BVHNode(GetBound(leftTriangles), std::move(leftTriangles));
        auto right = new BVHNode(GetBound(rightTriangles), std::move(rightTriangles));
        return {left, right};
    }

    void RecursiveBuildBVH(BVHNode* node, int kThreshold) {
        // Boundary Condition
        if(!node) return;
        if(node->triangles.size() < kThreshold){
            return;
        }

        // Split
        auto [left, right] = SplitBVHNode(node);
        if(!left->triangles.empty() || !right->triangles.empty()){
            std::vector<Triangle*>().swap(node->triangles);
        }
        if(!left->triangles.empty()){
            node->left = left;
        }
        if(!right->triangles.empty()){
            node->right = right;
        }

        // Recursive
        RecursiveBuildBVH(node->left, kThreshold);
        RecursiveBuildBVH(node->right, kThreshold);
    }

    void TraverseBVH(BVHNode *node) {

    }


}

