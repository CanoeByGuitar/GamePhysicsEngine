//
// Created by 王晨辉 on 2023/4/28.
//

#include <glm/glm.hpp>
#include <Base/Log.h>
#include <iostream>

using vec3 = glm::vec3;
using vec4 = glm::vec4;
using mat3 = glm::mat3;
using mat2 = glm::mat2;
using mat4 = glm::mat4;

int main(){
//    vec3 v(0);
//    PHY_INFO("{}", v);
//
//    mat3 m = mat3(1);
//    PHY_INFO("{}", m);
////    PHY_INFO("{}",m[0][0]);
//
//    // column vec
//    vec3 v1(1, 2 ,3);
//    vec3 v2(4, 5 ,6);
//    vec3 v3(7, 8 ,9);
//    mat3 A(v1, v2, v3);
//    PHY_INFO("{}",A);
//    auto p = glm::value_ptr(A);
//    for(int i = 0; i < 9; i++){
//        std::cout << p[i] << " ";
//    }
//
//    vec4 a1(1 , 2, 3, 4);
//    vec4 a2(5, 6, 7, 8);
//    PHY_INFO("{}", mat4(a1, a2, a1, a2));
//
//    mat2 b = mat2(1);
//    b[0][1] = 3;
//    PHY_INFO("{}", b);
//
//
//    // 2 col x 3 row
//    glm::mat2x3 lhs(v1, v2);
//    // 3 col x 2 row
//    auto rhs = glm::mat3x2(1);
//    PHY_INFO("{}", lhs * rhs);
//
//    auto v4 = glm::vec2(1, 2);
//    PHY_INFO("{}", lhs * v4);


    vec3 a(1, 1, 1);
    vec3 b(2, 2, 2);
    PHY_INFO("{}", a * b);
    PHY_INFO("{}", 1.f / b);
}