//
// Created by 王晨辉 on 2023/4/28.
//
#include <Base/Log.h>

int main(){
    PHY_LEVEL_DEBUG;
    PHY_INFO("123{}{}",4,5);
    PHY_ASSERT(1 < 2, "wrong!");
    PHY_DEBUG("debug");
    PHY_ERROR("error");
    PHY_WARN("warn");
}