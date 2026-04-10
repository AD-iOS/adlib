/*    AD-DEV Public General License
 *       Version 1.0.0, December 2025
 *
 *  Copyright (c) 2025-2026 AD-iOS (ad-ios334@outlook.com) All rights reserved.
 */
/*
 * _find.hpp
 * Created by AD on 2026-04-06
 * File content search and parsing utilities
 */

#ifndef _AD_FIND_HPP_
#define _AD_FIND_HPP_

#include <AD/AD_output.hpp>
#include <AD/AD_string.hpp>
#include <fstream>
#include <string>
#include <vector>
#include <optional>

namespace AD {

/**
 * @brief 在文件中查找包含指定字符串的行
 * @param filepath 文件路径
 * @param search_str 要搜索的字符串
 * @return 返回包含该字符串的完整行，如果未找到返回空字符串
 */
inline std::string find_line_containing(const std::string& filepath, const std::string& search_str) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        AD::cerr << "[Error] Cannot open file: " << filepath << AD::endl;
        return "";
    }
    
    std::string line;
    while (std::getline(file, line)) {
        if (line.find(search_str) != std::string::npos) {
            return line;
        }
    }
    return "";
}

/**
 * @brief 检查文件中是否存在指定字符串
 * @param filepath 文件路径
 * @param search_str 要搜索的字符串
 * @return true 如果找到，false 如果未找到
 */
inline bool find_string(const std::string& filepath, const std::string& search_str) {
    return !find_line_containing(filepath, search_str).empty();
}

/**
 * @brief 重载: 支持 C 风格字符串
 */
inline bool find_string(const std::string& filepath, const char* search_str) {
    return find_string(filepath, std::string(search_str));
}

/**
 * @brief 获取文件中指定行的内容
 * @param filepath 文件路径
 * @param line_num 行号（从1开始）
 * @return 该行的内容，如果行号无效返回空字符串
 */
inline std::string get_line(const std::string& filepath, int line_num) {
    if (line_num < 1) {
        AD::cerr << "[Error] Line number must be >= 1" << AD::endl;
        return "";
    }
    
    std::ifstream file(filepath);
    if (!file.is_open()) {
        AD::cerr << "[Error] Cannot open file: " << filepath << AD::endl;
        return "";
    }
    
    std::string line;
    int current_line = 1;
    while (std::getline(file, line)) {
        if (current_line == line_num) {
            return line;
        }
        current_line++;
    }
    return "";
}

/**
 * @brief 从一行文本中提取分隔符后面的值
 * @param line 文本行
 * @param delimiter 分隔符（如 '=', ',', ':', 空格等）
 * @return 分隔符后面的值（自动 trim 空白），如果找不到分隔符返回空字符串
 */
inline std::string extract_value(const std::string& line, char delimiter) {
    size_t pos = line.find(delimiter);
    if (pos == std::string::npos) {
        return "";
    }
    std::string value = line.substr(pos + 1);
    return AD::str.trim(value);
}

/**
 * @brief 从文件中读取环境变量格式的值（按行号）
 * @param filepath 文件路径
 * @param line_num 行号（从1开始）
 * @param delimiter 分隔符（默认 '='）
 * @return 提取的值，如果失败返回空字符串
 */
inline std::string read_get_env_val(const std::string& filepath, int line_num, char delimiter = '=') {
    std::string line = get_line(filepath, line_num);
    if (line.empty()) {
        return "";
    }
    return extract_value(line, delimiter);
}

/**
 * @brief 从文件中读取环境变量格式的值（按关键字搜索）
 * @param filepath 文件路径
 * @param key 关键字（如 "export Loop_Sleep_Interval"）
 * @param delimiter 分隔符（默认 '='）
 * @return 提取的值，如果找不到返回空字符串
 */
inline std::string read_get_env_val(const std::string& filepath, const std::string& key, char delimiter = '=') {
    std::string line = find_line_containing(filepath, key);
    if (line.empty()) {
        AD::cerr << "[Warn] Key '" << key << "' not found in " << filepath << AD::endl;
        return "";
    }
    return extract_value(line, delimiter);
}

/**
 * @brief 重载: 支持 C 风格字符串作为 key
 */
inline std::string read_get_env_val(const std::string& filepath, const char* key, char delimiter = '=') {
    return read_get_env_val(filepath, std::string(key), delimiter);
}

/**
 * @brief 查找包含指定字符串的所有行
 * @param filepath 文件路径
 * @param search_str 要搜索的字符串
 * @return 包含该字符串的所有行的 vector
 */
inline std::vector<std::string> find_all_lines(const std::string& filepath, const std::string& search_str) {
    std::vector<std::string> results;
    std::ifstream file(filepath);
    if (!file.is_open()) {
        AD::cerr << "[Error] Cannot open file: " << filepath << AD::endl;
        return results;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        if (line.find(search_str) != std::string::npos) {
            results.push_back(line);
        }
    }
    return results;
}

/**
 * @brief 查找并提取所有匹配行的值
 * @param filepath 文件路径
 * @param key_pattern 关键字模式
 * @param delimiter 分隔符
 * @return 所有匹配值的 vector
 */
inline std::vector<std::string> find_all_values(const std::string& filepath, 
                                                  const std::string& key_pattern, 
                                                  char delimiter = '=') {
    std::vector<std::string> values;
    std::ifstream file(filepath);
    if (!file.is_open()) {
        AD::cerr << "[Error] Cannot open file: " << filepath << AD::endl;
        return values;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        if (line.find(key_pattern) != std::string::npos) {
            std::string val = extract_value(line, delimiter);
            if (!val.empty()) {
                values.push_back(val);
            }
        }
    }
    return values;
}

/**
 * @brief 获取匹配行号（第一个匹配）
 * @param filepath 文件路径
 * @param search_str 要搜索的字符串
 * @return 行号（从1开始），未找到返回 -1
 */
inline int find_line_number(const std::string& filepath, const std::string& search_str) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        AD::cerr << "[Error] Cannot open file: " << filepath << AD::endl;
        return -1;
    }
    
    std::string line;
    int line_num = 1;
    while (std::getline(file, line)) {
        if (line.find(search_str) != std::string::npos) {
            return line_num;
        }
        line_num++;
    }
    return -1;
}

} // namespace AD

#endif // _AD_FIND_HPP_
