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
 * _read.hpp
 * Created by AD on 20/12/25
 * Copyright (c) 2025-2026 AD All rights reserved.
**/

#ifndef _AD_READ_HPP_
#define _AD_READ_HPP_

#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <filesystem>

#include "AD_output.hpp"

namespace AD {

inline std::string _AD_read_all(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        AD::cerr << "[Error] Cannot open file: " << path << AD::endl;
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

inline std::vector<std::string> _AD_read_lines(const std::string& path) {
    std::vector<std::string> lines;
    std::ifstream file(path);
    if (!file.is_open()) {
        AD::cerr << "[Error] Cannot open file: " << path << AD::endl;
        return lines;
    }
    std::string line;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }
    return lines;
}

inline bool _AD_readable(const std::string& path) {
    std::ifstream file(path);
    return file.good();
}

inline std::string _AD_read_lines(const std::string& path, size_t line_number) {
    std::ifstream file(path);
    if (!file.is_open()) {
        AD::cerr << "[Error] Cannot open file: " << path << AD::endl;
        return "";
    }
    
    std::string line;
    for (size_t i = 0; i <= line_number; ++i) {
        if (!std::getline(file, line)) {
            AD::cerr << "[Error] Line " << line_number << " not found in: " << path << AD::endl;
            return "";
        }
    }
    return line;
}

inline std::vector<std::string> _AD_read_lines(const std::string& path, 
                                                size_t start_line, 
                                                size_t end_line) {
    std::vector<std::string> lines;
    std::ifstream file(path);
    
    if (!file.is_open()) {
        AD::cerr << "[Error] Cannot open file: " << path << AD::endl;
        return lines;
    }
    
    if (start_line > end_line) {
        AD::cerr << "[Error] Invalid line range: start > end" << AD::endl;
        return lines;
    }
    
    std::string line;
    size_t current_line = 0;
    
    while (std::getline(file, line)) {
        if (current_line >= start_line && current_line <= end_line) {
            lines.push_back(line);
        }
        if (current_line > end_line) break;
        current_line++;
    }
    
    if (lines.empty() && start_line <= end_line) {
        AD::cerr << "[Error] No lines found in range [" << start_line << ", " 
                 << end_line << "] in: " << path << AD::endl;
    }
    
    return lines;
}

} // namespace AD

#endif
