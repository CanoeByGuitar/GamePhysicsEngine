//
// Created by 王晨辉 on 2023/4/30.
//

#include "Loader/Utils.h"
#include "iostream"

int main(){
    std::wstring a = L"abcde";
    std::cout << utf16_to_utf8(a) << std::endl;
}