//
// Created by 王晨辉 on 2023/4/30.
//

#ifndef GAMEPHYSICSINONEWEEKEND_UTILS_H
#define GAMEPHYSICSINONEWEEKEND_UTILS_H

#include <string>
#include <codecvt>
#include <locale>

inline std::string utf16_to_utf8(const std::wstring& wstr) {
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> converter;
    return converter.to_bytes(wstr);
}

inline std::wstring bin_to_wstring(const unsigned char* buffer, size_t size) {
    std::wstring result;
    result.reserve(size / sizeof(wchar_t)); // Reserve space for wchar_t elements
    for (size_t i = 0; i < size; i += 2) {
        wchar_t ch = static_cast<wchar_t>((buffer[i + 1] << 8) | buffer[i]);
        result.push_back(ch);
    }
    return result;
}

inline std::string bin_to_string(const unsigned char* buffer, size_t size){
    return utf16_to_utf8(bin_to_wstring(buffer, size));
}

#endif //GAMEPHYSICSINONEWEEKEND_UTILS_H
