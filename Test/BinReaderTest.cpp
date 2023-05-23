//
// Created by 王晨辉 on 2023/4/30.
//


#include "Loader/PmxReader.h"
#include <iostream>



int main(){
    int test_case = 3;


    if(test_case == 1){
        BinReader reader("/Users/wangchenhui/Desktop/games202/RTRTemplate_OpenGL/resources/models/37/37.pmx");
        std::string s;
        reader.Read2String(1024,s);
        for(int i = 0; i < s.size(); i++){
            if(s[i] >= 32 && s[i] <= 126){
                // printable
                std::cout << s[i];
            }else{
                std::cout << ".";
            }
            if(i % 16 == 15) std::cout << "\n";
        }
    }

    if(test_case == 2){
        unsigned char buffer[] = { 0x00, 0x00, 0x00, 0x40 };
        //error
//    float* p1 = static_cast<float*>(buffer);

        // C-style: correct
        float* p2 = (float *)buffer;

        // Cpp-style: correct
        float* p3 = reinterpret_cast<float*>(buffer);

        std::cout << *p2 << " " << *p3;
    }



    if(test_case == 3){
        unsigned char buffer[] = {0x09, 0x4E, 0x08, 0x67, 0x03, 0x4E};
//        unsigned char buffer[] = { 0x4E, 0x09, 0x67, 0x08, 0x4E, 0x03 };
        size_t bufferSize = sizeof(buffer);
        auto str = bin_to_string(buffer, bufferSize);
        std::cout << str;
    }

    if(test_case == 4){

    }


}

