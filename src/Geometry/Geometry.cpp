//
// Created by chenhui on 23-5-23.
//

#include "Geometry.h"
#include "algorithm"
#include <queue>
#include <unordered_map>
#include <algorithm>

namespace control{
    geo::BVHSplitStrategy bvh_strategy = geo::MID_TRIANGLE_PARTITION;
}





static int BVH_DEPTH = 0;
static const int MAX_DEPTH = 4;

namespace geo{

    void AccelerateMesh(Mesh &mesh) {
        if (mesh.accelerator) return;
        mesh.accelerator = new BVHNode(mesh.bound, mesh.triangles);
        RecursiveBuildBVH(mesh.accelerator, 20);

    }


    static inline bool compareTriangle(Triangle* a, Triangle* b){
        return GetBound(*a).position.x < GetBound(*b).position.x;
    }


    std::pair<BVHNode*, BVHNode*> SplitBVHNode(BVHNode* node){
        std::vector<Triangle*> leftTriangles, rightTriangles;
        if(control::bvh_strategy == MID_X_ALIAS){
            // x-alias partition
            auto bound = node->bound;
            leftTriangles.reserve(node->triangles.size());
            rightTriangles.reserve(node->triangles.size());
            for(const auto& tri : node->triangles){
                if(GetMax(GetBound(*tri)).x < bound.position.x){
                    leftTriangles.push_back(tri);
                }else{
                    rightTriangles.push_back(tri);
                }
            }
        }else if(control::bvh_strategy == MID_TRIANGLE_PARTITION){
            // mid triangle partition
            auto vec = node->triangles;
            std::sort(vec.begin(), vec.end(), compareTriangle);

            auto beginning = vec.begin();
            auto middling = vec.begin() + vec.size() / 2;
            auto ending = vec.end();
            leftTriangles = std::vector<Triangle*>(beginning, middling);
            rightTriangles = std::vector<Triangle*>(middling, ending);
        }

        auto left = new BVHNode(GetBound(leftTriangles), std::move(leftTriangles));
        auto right = new BVHNode(GetBound(rightTriangles), std::move(rightTriangles));
        return {left, right};
    }



    void RecursiveBuildBVH(BVHNode* node, int kThreshold) {
        // Boundary Condition
        if(node->triangles.size() < kThreshold){
            return;
        }
//        if(BVH_DEPTH > MAX_DEPTH) return;

        PHY_DEBUG("Node bound m_pos: {}, halfSize: {}, triNum:{}", node->bound.position, node->bound.halfSize, node->triangles.size());


        // Split
        auto [leftNode, rightNode] = SplitBVHNode(node);

        if(!leftNode->triangles.empty() || !rightNode->triangles.empty()){
            std::vector<Triangle*>().swap(node->triangles);
        }

        bool shouldReturn = false;
        if(!leftNode->triangles.empty()){
            node->left = leftNode;
        }else{
            shouldReturn = true;
        }

        if(!rightNode->triangles.empty()){
            node->right = rightNode;
        }else{
            shouldReturn = true;
        }
        if(shouldReturn) return;

        // Recursive
        BVH_DEPTH++;
        RecursiveBuildBVH(node->left, kThreshold);
        BVH_DEPTH--;

        BVH_DEPTH++;
        RecursiveBuildBVH(node->right, kThreshold);
        BVH_DEPTH--;
    }

    void TraverseBVH(BVHNode *node, std::unordered_map<int, std::vector<BVHNode*>> &boundMap) {
        if(!node) return;
        std::queue<BVHNode*> q;
        q.push(node);
        int level = 0;
        while(!q.empty()){
            int levelSize = q.size();
            PHY_DEBUG("Visit level: {}", level);
            for(int i = 0; i < levelSize; i++){
                auto front = q.front();
                q.pop();
                if(boundMap.find(level) != boundMap.end()){
                    boundMap[level].push_back(front);
                }else{
                    boundMap[level] = {front};
                }

                if(front->left) q.push(front->left);
                if(front->right) q.push(front->right);
            }
            level++;
        }
    }


}

