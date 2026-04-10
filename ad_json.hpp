/*    AD-DEV Public General License
 *       Version 1.0.0, December 2025
 *
 *  Copyright (c) 2025-2026 AD-iOS (ad-ios334@outlook.com) All rights reserved.
 *
 *  (Note: AD, AD-dev, and AD-iOS refer to the same person. Email: ad-ios334@outlook.com)
 *
 *  Hereinafter, the AD-DEV Public General License is referred to as this Agreement or this License. The original source code, executable binaries, and related documentation are collectively referred to as the Software. Use for profit, including sales, leasing, advertising support, etc., is referred to as Commercial Use. Works modified or extended based on the Software are referred to as Derivative Products.
 *
 *  We hereby grant you the following rights, subject to the following conditions, a worldwide, royalty-free, non-exclusive, irrevocable license to:
 *    1. Use, copy, and modify the Software
 *    2. Create derivative works
 *    3. Distribute the Software and derivative works
 *
 *  Conditions:
 *    1. Retain the original author's license and copyright notices
 *    2. The original author assumes no liability
 *    3. Unauthorized commercial use is prohibited
 *    4. All modifications and derivative products based on the Software must use this License and disclose the original source code
 *    5. Disclosure of the original source code is not required when using the Software unmodified (e.g., via APIs, libraries, automatic linkers, etc.)
 *
 *  Anyone may distribute or copy this License file, but may not modify it.
 *  For commercial use, please contact the maintainer of the Software to obtain written and electronic authorization. Note that commercial licensing may involve fees; specific terms shall be negotiated separately.
 *  Violation of this Agreement will automatically terminate the license. Upon termination, all copies of the Software must be destroyed, use of the Software must cease, derivative products must be withdrawn from distribution channels, and distribution of derivative products must stop.
 *
 *  The software is provided "as is", without warranty of any kind, express or implied. In no event shall the authors or copyright holders be liable for any claim, damages, or other liability arising from the use of the software.
 *
 *  If any provision is invalid, it does not affect the validity of the other provisions.
 *  By using, distributing, or modifying the Software, you automatically agree to and intend to comply with this Agreement. If you cannot comply, you must stop using, distributing, or modifying the Software.
 *  This Agreement is governed by and construed in accordance with the laws of the People's Republic of China (without regard to conflict of law principles).
 */
/*
 * ad_json.hpp
 * JSON wrapper for Boost.JSON
 * Created by AD on 2026-04-09
 * Copyright (c) 2025-2026 AD All rights reserved.
 */

#ifndef _AD_JSON_HPP_
#define _AD_JSON_HPP_

#include <boost/json.hpp>
#include <string>
#include <optional>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include "AD_output.hpp"

namespace AD::json {

namespace json = boost::json;

// 類型別名
using value = json::value;
using object = json::object;
using array = json::array;
using string = json::string;
// fix: boost::json 在新版本中没有 number 类型，使用 value 代替
// using number = json::number;

// 解析 JSON
inline json::value parse(const std::string& str) {
    try {
        return json::parse(str);
    } catch (const std::exception& e) {
        ad::cerr << "[JSON Error] Parse failed: " << e.what() << ad::endl;
        return json::value();
    }
}

inline json::value parse_file(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        ad::cerr << "[JSON Error] Cannot open file: " << path << ad::endl;
        return json::value();
    }
    std::stringstream ss;
    ss << file.rdbuf();
    return parse(ss.str());
}

// 序列化 JSON
inline std::string serialize(const json::value& v, bool pretty = false) {
    if (pretty) {
        // fix: 明确使用 serialize_options
        return json::serialize(v, json::serialize_options{});
    }
    return json::serialize(v);
}

inline bool save_file(const std::string& path, const json::value& v, bool pretty = false) {
    std::ofstream file(path);
    if (!file.is_open()) {
        ad::cerr << "[JSON Error] Cannot write to file: " << path << ad::endl;
        return false;
    }
    file << serialize(v, pretty);
    return true;
}

// 便捷構造
inline json::object make_object() { return json::object(); }

inline json::array make_array() { return json::array(); }

// 輔助函式: 檢查 key 是否存在
inline bool has_key(const json::object& obj, const std::string& key) { return obj.contains(key); }

// 輔助函式: 安全獲取值(帶默認值)
template<typename T>
inline T get_or(const json::object& obj, const std::string& key, const T& default_value) {
    if (auto it = obj.find(key); it != obj.end()) {
        try {
            return json::value_to<T>(it->value());
        } catch (...) {
            return default_value;
        }
    }
    return default_value;
}

// 輔助函式: 將 JSON 轉換為字符串
inline std::string to_string(const json::value& v) {
    // fix: 明确指定参数，避免与标准库函数冲突
    return json::serialize(v);
}

// 輔助函式: 從字符串構造
inline json::value from_string(const std::string& str) {
    return parse(str);
}

} // namespace AD::json

#endif