//
// Created by 王晨辉 on 2023/5/29.
//

#include <Base/IOUtils.h>
#include <iostream>


int main(){

    auto [vs, fs] = GetShaderFiles("/Users/wangchenhui/Dev/GamePhysicsInOneWeekend/resource/shader/test");
    std::cout << vs << " " << fs << std::endl;
}