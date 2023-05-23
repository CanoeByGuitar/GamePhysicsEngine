//
// Created by 王晨辉 on 2023/4/30.
//


#include "Loader/PmxReader.h"

#include <iostream>

class A{
public:
    A(){
        p = new int[10];
    }

    ~A(){
        delete[] p;
    }
private:
    int* p;
};

class B{
public:
    B(){
        a = std::make_shared<A>();
    }
private:
    std::shared_ptr<A> a;
};

int main(){
    PmxReader pmxReader("/Users/wangchenhui/Desktop/games202/RTRTemplate_OpenGL/resources/models/37/37.pmx");
    pmxReader.PrintInfo();
//    B b;
}

