/*    AD-DEV Public General License
 *       Version 1.0.0, December 2025
 *
 *  Copyright (c) 2025-2026 AD-iOS (1107154510@qq.com) All rights reserved.
 *
 *  (Note: AD, AD-dev, and AD-iOS refer to the same person. Email: ad-ios@qq.com)
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
 * _rm_.hpp
 * AD‘s C++ private standard library
 * Created by AD on 5/11/25
 * Copyright (c) 2025-2026 AD All rights reserved.
 * 
*/

#ifndef _AD_RM_HPP_
#define _AD_RM_HPP_

#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

namespace AD {

/**
 *
 * @brief Delete a single file
 * @param filePath File path
 * @return int Success returns 1, failure returns 0
 *
**/

int _AD_rm(const fs::path& filePath) {
    try {
        if (!fs::exists(filePath)) {
            std::cout << "Document does not exist: " << filePath << std::endl;
            return 0;
        }
        
        if (!fs::is_regular_file(filePath)) {
            std::cout << "Path is not a regular file: " << filePath << std::endl;
            return 0;
        }
        
        if (fs::remove(filePath)) {
            std::cout << "File deleted successfully: " << filePath << std::endl;
            return 1;
        } else {
            std::cout << "Failed to delete file: " << filePath << std::endl;
            return 0;
        }
        
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Delete file error: " << e.what() << std::endl;
        return 0;
    }
}

/**
 *
 * @brief Delete empty folders
 * @param dirPath Folder Path
 * @return int Success returns 1, failure returns 0
 *
**/

int _AD_rmdir(const fs::path& dirPath) {
    try {
        if (!fs::exists(dirPath)) {
            std::cout << "Directory does not exist: " << dirPath << std::endl;
            return 0;
        }
        
        if (!fs::is_directory(dirPath)) {
            std::cout << "Path is not a directory: " << dirPath << std::endl;
            return 0;
        }
        
        // 檢查目錄是否為空
        if (!fs::is_empty(dirPath)) {
            std::cout << "Directory is not empty, use _AD_rmdirf instead: " << dirPath << std::endl;
            return 0;
        }
        
        if (fs::remove(dirPath)) {
            std::cout << "Empty directory deleted successfully: " << dirPath << std::endl;
            return 1;
        } else {
            std::cout << "Failed to delete empty directory: " << dirPath << std::endl;
            return 0;
        }
        
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Delete directory error: " << e.what() << std::endl;
        return 0;
    }
}

/**
 *
 * @brief Recursively delete the folder and all its contents
 * @param dirPath Folder path
 * @return int Success returns the number of items deleted, failure returns 0
 *
**/

int _AD_rmdirf(const fs::path& dirPath) {
    try {
        if (!fs::exists(dirPath)) {
            std::cout << "Directory does not exist: " << dirPath << std::endl;
            return 0;
        }
        
        if (!fs::is_directory(dirPath)) {
            std::cout << "Path is not a directory: " << dirPath << std::endl;
            return 0;
        }
        
        std::uintmax_t count = fs::remove_all(dirPath);
        if (count > 0) {
            std::cout << "Directory and its contents deleted successfully: " << dirPath << std::endl;
            std::cout << "Total items deleted: " << count << std::endl;
            return static_cast<int>(count);
        } else {
            std::cout << "Failed to delete directory: " << dirPath << std::endl;
            return 0;
        }
        
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Recursive deletion error: " << e.what() << std::endl;
        return 0;
    }
}

/**
 *
 * @brief Automatically select the deletion method according to the type
 * @param path Way
 * @return int Success returns 1, failure returns 0
 *
**/

int _AD_rm_safe(const fs::path& path) {
    try {
        if (!fs::exists(path)) {
            std::cout << "Path does not exist: " << path << std::endl;
            return 0;
        }
        
        if (fs::is_directory(path)) {
            return _AD_rmdirf(path) > 0 ? 1 : 0;
        } else if (fs::is_regular_file(path)) {
            return _AD_rm(path);
        } else {
            std::cout << "Unsupported file type: " << path << std::endl;
            return 0;
        }
        
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Safe deletion error: " << e.what() << std::endl;
        return 0;
    }
}

} // namespace AD

#endif