//
// Created by 王晨辉 on 2023/4/30.
//

#ifndef GAMEPHYSICSINONEWEEKEND_PMXREADER_H
#define GAMEPHYSICSINONEWEEKEND_PMXREADER_H

#include "BinReader.h"
#include "Utils.h"

using vec3 = glm::vec3;
using vec2 = glm::vec2;
using vec4 = glm::vec4;



struct PmxVertex{
    vec3 Position, Normal, C0, R0, R1, RW0, RW1;
    vec2 UV;
    vec4 UVA[4];

};


class PmxReader{
#define watch(x) spdlog::info("{} is {}", #x, x);

public:
    explicit PmxReader(const std::string& filePath)
    :m_binReader(std::make_shared<BinReader>(filePath)){
        ReadHeader();
        ReadModelInfo();
    }

    void PrintInfo(){
        watch(m_version)
        watch(m_textEncoding)
        watch(m_UVACount)
        watch(m_vertexSize)
        watch(m_texSize)
        watch(m_materialSize)
        watch(m_boneSize)
        watch(m_morphSize)
        watch(m_bodySize)

        watch(m_modelName)
        watch(m_modelNameE)
        watch(m_comment)
        watch(m_commentE)
    }

private:
    void ReadHeader(){
        static std::string PmxKey = "PMX ";
        std::string str;
        m_binReader->Read2String(4, str);
        PHY_ASSERT(str == PmxKey, "Not support pmx version!")
        m_version = m_binReader->Read<float>();
        PHY_ASSERT(abs(m_version - 2.0) < 1e-6, "Not support pmx version!")
        ReadElementFormat();
    }


    void ReadElementFormat(){
        int szRead = ReadElementInt32(1, true);
        BinReader::Data buffer;
        m_binReader->Read2Data(szRead, buffer);

        int idx = 0;

        // example in 37.pmx
        m_textEncoding = buffer[idx++]; // 0
        m_UVACount = buffer[idx++];  // 0
        m_vertexSize = buffer[idx++]; // 2
        m_texSize = buffer[idx++]; // 1
        m_materialSize = buffer[idx++]; // 1
        m_boneSize = buffer[idx++]; // 2
        m_morphSize = buffer[idx++]; // 1
        m_bodySize = buffer[idx++]; // 2
    }

    void ReadStringUtf16(std::string& str){
        int count = m_binReader->Read<int>();
        BinReader::Data dat;
        m_binReader->Read2Data(count, dat);
        str = bin_to_string(dat.data(), count);
    }

    void ReadModelInfo(){
        ReadStringUtf16(m_modelName);
        ReadStringUtf16(m_modelNameE);
        ReadStringUtf16(m_comment);
        ReadStringUtf16(m_commentE);
    }

    void ReadVertexInfo(){
        int num = ReadElementInt32(4, true);

    }



private:
    int ReadElementInt32(int len, int hasSigned = true){
        int num = 0;
        BinReader::Data buffer;
        m_binReader->Read2Data(len, buffer);

        switch(len){
            case 1:
                if(!hasSigned){
                    // unsigned uint8_t
                    return buffer[0];
                }else{
                    // char
                    return static_cast<char>(buffer[0]);
                }

            case 2:
                if(!hasSigned){
                    return *(uint16_t*)buffer.data();
                }else{
                    return *(int16_t*)buffer.data();
                }

            case 4:
                return *(int32_t*)buffer.data();

            default:
                return num;
        }
    }



private:
    std::shared_ptr<BinReader> m_binReader;
    float m_version = -1;
    int m_textEncoding = -1; // 0 for utf16LE; 1 for utf8
    int m_UVACount = -1;
    int m_vertexSize = -1;
    int m_texSize = -1;
    int m_materialSize = -1;
    int m_boneSize = -1;
    int m_morphSize = -1;
    int m_bodySize = -1;

    std::string m_modelName;
    std::string m_modelNameE;
    std::string m_comment;
    std::string m_commentE;

};

#endif //GAMEPHYSICSINONEWEEKEND_PMXREADER_H
