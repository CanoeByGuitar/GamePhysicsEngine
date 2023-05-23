//
// Created by 王晨辉 on 2023/4/29.
//

#ifndef GAMEPHYSICSINONEWEEKEND_BINREADER_H
#define GAMEPHYSICSINONEWEEKEND_BINREADER_H

#include <vector>
#include <Base/Log.h>
#include <fstream>

class BinReader {
public:
    using Data = std::vector<uint8_t>;

    BinReader(void *pBuffer, size_t len) : m_pBuffer(pBuffer), m_length(len), m_offset(0) {}

    explicit BinReader(const std::string& filePath){
        std::ifstream file(filePath.c_str(), std::ios::binary);
        PHY_ASSERT(file, "Cannot open file!");
        file.seekg(0, std::ios::end);
        m_length = file.tellg();
        file.seekg(0, std::ios::beg);

        m_pBuffer = static_cast<void*>(new char[m_length]);
        file.read(static_cast<char*>(m_pBuffer), (long)m_length);

        file.close();
        m_offset = 0;
        PHY_INFO("Correctly open file.");
    }

    ~BinReader(){
        delete[] static_cast<char*>(m_pBuffer);
        m_pBuffer = nullptr;
    }

    template<typename T>
    T Read() {
        PHY_ASSERT(m_offset + sizeof(T) < m_length, "out of file index");
        auto ret = reinterpret_cast<T*>(static_cast<char*>(m_pBuffer) + m_offset) ;
        m_offset += sizeof(T);
        return *ret;
    }

    void Read2String(size_t len, std::string &str) {
        PHY_ASSERT(m_offset + len < m_length, "out of file index");
        str.assign(static_cast<char *>(m_pBuffer) + m_offset, len);
        m_offset += len;
    }

    void Read2String(std::string &str) {
        char ch;
        str = "";
        while (0 != (ch = Read<char>())) {
            str.push_back(ch);
        }
    }

    void Read2Data(size_t len, Data &data) {
        PHY_ASSERT(m_offset + len < m_length, "out of file index");
        char *p = static_cast<char *>(m_pBuffer) + m_offset;
        data.assign(p, p + len);
        m_offset += len;
    }

    void Skip(size_t len) {
        PHY_ASSERT(m_offset + len < m_length, "out of file index");
        m_offset += len;
    }

    void Rewind() {
        m_offset = 0;
    }


private:
    void *m_pBuffer;
    size_t m_length;
    size_t m_offset;
};


#endif //GAMEPHYSICSINONEWEEKEND_BINREADER_H
