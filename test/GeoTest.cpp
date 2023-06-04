//
// Created by chenhui on 23-5-23.
//

#include <Geometry/Geometry.h>
#include <glm/gtc/random.hpp>
#include <Actor.h>

using namespace std;

using namespace geo;

vec3 RandomVec3(){
    return {
            glm::linearRand(-1.f, 1.f),
            glm::linearRand(-1.f, 1.f),
            glm::linearRand(-1.f, 1.f)
            };
}

int main(){
    Mesh mesh;
    vector<Triangle*> triangleList;
    triangleList.reserve(1000);
    for(int i = 0; i < 1000; i++){
        triangleList.push_back(
                new Triangle(RandomVec3(), RandomVec3(), RandomVec3()));
    }
    mesh.bound = AABB(vec3(0), vec3(1));
    mesh.triangles = triangleList;
    AccelerateMesh(mesh);

    int c = 1;

    Actor a{};
}




