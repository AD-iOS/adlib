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
 * ad_md5.hpp
 * MD5 hash implementation using OpenSSL
 * Created by AD on 2026-04-09
 */

# ifndef _AD_MD5_HPP_
# define _AD_MD5_HPP_

# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wdeprecated-declarations"

# include <openssl/md5.h>
# include <string>
# include <vector>
# include <fstream>
# include <iomanip>
# include <sstream>
# include <cstring>
# include "AD_output.hpp"

namespace AD::ssl::md5 {

// 將二進制 MD5 轉換為十六進制字符串
inline std::string to_hex(unsigned char* digest, size_t len = MD5_DIGEST_LENGTH) {
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for (size_t i = 0; i < len; ++i) {
        ss << std::setw(2) << static_cast<int>(digest[i]);
    }
    return ss.str();
}

// 計算字符串的 MD5
inline std::string hash(const std::string& input) {
    unsigned char digest[MD5_DIGEST_LENGTH];
    MD5(reinterpret_cast<const unsigned char*>(input.c_str()), input.size(), digest);
    return to_hex(digest);
}

// 計算字節數組的 MD5
inline std::string hash(const unsigned char* data, size_t len) {
    unsigned char digest[MD5_DIGEST_LENGTH];
    MD5(data, len, digest);
    return to_hex(digest);
}

// 計算文件的 MD5
inline std::string hash_file(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        ad::cerr << "[MD5 Error] Cannot open file: " << path << ad::endl;
        return "";
    }

    MD5_CTX ctx;
    MD5_Init(&ctx);

    char buffer[8192];
    while (file.read(buffer, sizeof(buffer))) {
        MD5_Update(&ctx, buffer, file.gcount());
    }
    // 處理最後一部分
    if (file.gcount() > 0) {
        MD5_Update(&ctx, buffer, file.gcount());
    }
    file.close();

    unsigned char digest[MD5_DIGEST_LENGTH];
    MD5_Final(digest, &ctx);
    return to_hex(digest);
}

// 計算多個輸入的 MD5(按順序組合)
inline std::string hash_combine(const std::vector<std::string>& inputs) {
    MD5_CTX ctx;
    MD5_Init(&ctx);
    
    for (const auto& input : inputs) {
        MD5_Update(&ctx, input.c_str(), input.size());
    }
    
    unsigned char digest[MD5_DIGEST_LENGTH];
    MD5_Final(digest, &ctx);
    return to_hex(digest);
}

// 驗證字符串是否匹配給定的 MD5
inline bool verify(const std::string& input, const std::string& expected_md5) {
    return hash(input) == expected_md5;
}

// 驗證文件是否匹配給定的 MD5
inline bool verify_file(const std::string& path, const std::string& expected_md5) {
    return hash_file(path) == expected_md5;
}

// 獲取 MD5 的二進制形式(16字節)
inline std::vector<unsigned char> hash_binary(const std::string& input) {
    std::vector<unsigned char> digest(MD5_DIGEST_LENGTH);
    MD5(reinterpret_cast<const unsigned char*>(input.c_str()), input.size(), digest.data());
    return digest;
}

// 將十六進制 MD5 字符串轉換為二進制
inline std::vector<unsigned char> hex_to_binary(const std::string& hex) {
    std::vector<unsigned char> result;
    if (hex.length() != MD5_DIGEST_LENGTH * 2) {
        return result;
    }
    result.reserve(MD5_DIGEST_LENGTH);
    for (size_t i = 0; i < hex.length(); i += 2) {
        result.push_back(static_cast<unsigned char>(std::stoi(hex.substr(i, 2), nullptr, 16)));
    }
    return result;
}

} // namespace AD::ssl::md5

# pragma GCC diagnostic pop

# endif