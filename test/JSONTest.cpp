//
// Created by 王晨辉 on 2023/6/11.
//
#include <Loader/json.hpp>
#include <Base/Log.h>
#include <fstream>
#include <cstdio>

using json = nlohmann::json;

int main(){
    std::ifstream f("/Users/wangchenhui/Dev/GamePhysicsInOneWeekend/resource/config/example.json");
    json data = json::parse(f);
    for(const auto& obj : data["objects"]){
        printf("\n");
        for(auto it = obj.begin(); it != obj.end(); it++){
            vec3 v;
            if(it.value().is_array() && it.value().size() == 3){
                v.x = it.value()[0];
                v.y = it.value()[1];
                v.z = it.value()[2];
            }else{
                PHY_INFO("{} : {}", it.key(), it.value());
                continue;
            }
            PHY_INFO("{} : {}", it.key(), v);

//            PHY_INFO("{} : {}", it.key(), it.value());
        }

    }



    int c = 1;
}