//
// Created by 王晨辉 on 2023/4/28.
//
#include <Base/Log.h>

struct A{
    A(std::vector<int> &&vec):m_v(vec){};
    ~A(){
        PHY_INFO("destroy A");
    }
    std::vector<int> m_v;
};

void f(){
    std::vector<int> temp = {1, 2, 3};
    A* a = new A(std::move(temp));
    delete a;
}

int main(){
//    PHY_LEVEL_DEBUG;
//    PHY_INFO("123{}{}",4,5);
//    PHY_ASSERT(1 < 2, "wrong!");
//    PHY_DEBUG("debug");
//    PHY_ERROR("error");
//    PHY_WARN("warn");

    f();

    PHY_INFO("exit");
}