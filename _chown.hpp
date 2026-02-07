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
 * _chown.hpp
 * Created by AD on 3/1/26
 * Copyright (c) 2025-2026 AD All rights reserved.
 */

#ifndef _AD_CHOWN_HPP_
#define _AD_CHOWN_HPP_

#include "AD_output.hpp"
#include <filesystem>
#include <fstream>
#include <vector>
#include <string>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <cstring>

namespace fs = std::filesystem;

namespace AD {

namespace internal {
    
    // 获取用户ID
    inline uid_t getUid(const std::string& username) {
        struct passwd* pwd = getpwnam(username.c_str());
        if (!pwd) {
            if (username.empty()) {
                return getuid();  // 返回当前用户ID
            }
            AD::cerr << "Error: User '" << username << "' not found" << AD::endl;
            return static_cast<uid_t>(-1);
        }
        return pwd->pw_uid;
    }
    
    // 获取组ID
    inline gid_t getGid(const std::string& groupname) {
        struct group* grp = getgrnam(groupname.c_str());
        if (!grp) {
            if (groupname.empty()) {
                return getgid();  // 返回当前组ID
            }
            AD::cerr << "Error: Group '" << groupname << "' not found" << AD::endl;
            return static_cast<gid_t>(-1);
        }
        return grp->gr_gid;
    }
    
    // 获取当前用户名
    inline std::string getCurrentUsername() {
        struct passwd* pwd = getpwuid(getuid());
        return pwd ? pwd->pw_name : "";
    }
    
    // 获取当前组名
    inline std::string getCurrentGroupname() {
        struct group* grp = getgrgid(getgid());
        return grp ? grp->gr_name : "";
    }
}

/**
 * @brief 设置文件/目录的所有者（用户和组）
 * 
 * @param path 文件或目录路径
 * @param username 用户名（空字符串表示当前用户）
 * @param groupname 组名（空字符串表示当前组）
 * @param recursive 是否递归设置目录内容（默认为false）
 * @return true 设置成功
 * @return false 设置失败
 */
inline bool _AD_chown(const fs::path& path, 
                     const std::string& username = "", 
                     const std::string& groupname = "", 
                     bool recursive = false) {
    
    // 检查路径是否存在
    if (!fs::exists(path)) {
        AD::cerr << "Error: Path '" << path << "' does not exist" << AD::endl;
        return false;
    }
    
    // 获取用户ID和组ID
    uid_t uid = internal::getUid(username);
    gid_t gid = internal::getGid(groupname);
    
    if (uid == static_cast<uid_t>(-1) || gid == static_cast<gid_t>(-1)) {
        return false;
    }
    
    // 设置所有权
    if (chown(path.c_str(), uid, gid) != 0) {
        AD::cerr << "Error: Failed to set ownership for '" << path 
                  << "': " << strerror(errno) << AD::endl;
        return false;
    }
    
    std::string user = username.empty() ? internal::getCurrentUsername() : username;
    std::string group = groupname.empty() ? internal::getCurrentGroupname() : groupname;
    
    AD::cout << "Set ownership of '" << path << "' to " << user << ":" << group << AD::endl;
    
    // 如果需要递归处理目录
    if (recursive && fs::is_directory(path)) {
        try {
            for (const auto& entry : fs::recursive_directory_iterator(path)) {
                if (chown(entry.path().c_str(), uid, gid) != 0) {
                    AD::cerr << "Warning: Failed to set ownership for '" 
                              << entry.path() << "': " << strerror(errno) << AD::endl;
                }
            }
        } catch (const fs::filesystem_error& e) {
            AD::cerr << "Error during recursive ownership setting: " << e.what() << AD::endl;
            return false;
        }
    }
    
    return true;
}

/**
 * @brief 仅设置文件所有者（用户）
 * 
 * @param path 文件或目录路径
 * @param username 用户名（空字符串表示当前用户）
 * @param recursive 是否递归设置目录内容
 * @return true 设置成功
 * @return false 设置失败
 */
inline bool _AD_chown_user(const fs::path& path, 
                          const std::string& username = "", 
                          bool recursive = false) {
    return _AD_chown(path, username, "", recursive);
}

/**
 * @brief 仅设置文件所属组
 * 
 * @param path 文件或目录路径
 * @param groupname 组名（空字符串表示当前组）
 * @param recursive 是否递归设置目录内容
 * @return true 设置成功
 * @return false 设置失败
 */
inline bool _AD_chown_group(const fs::path& path, 
                           const std::string& groupname = "", 
                           bool recursive = false) {
    
    if (!fs::exists(path)) {
        AD::cerr << "Error: Path '" << path << "' does not exist" << AD::endl;
        return false;
    }
    
    gid_t gid = internal::getGid(groupname);
    if (gid == static_cast<gid_t>(-1)) {
        return false;
    }
    
    // 只设置组，保持所有者不变
    struct stat st;
    if (stat(path.c_str(), &st) != 0) {
        AD::cerr << "Error: Cannot get file status for '" << path 
                  << "': " << strerror(errno) << AD::endl;
        return false;
    }
    
    if (chown(path.c_str(), st.st_uid, gid) != 0) {
        AD::cerr << "Error: Failed to set group for '" << path 
                  << "': " << strerror(errno) << AD::endl;
        return false;
    }
    
    std::string group = groupname.empty() ? internal::getCurrentGroupname() : groupname;
    AD::cout << "Set group of '" << path << "' to " << group << AD::endl;
    
    // 递归处理
    if (recursive && fs::is_directory(path)) {
        try {
            for (const auto& entry : fs::recursive_directory_iterator(path)) {
                if (stat(entry.path().c_str(), &st) == 0) {
                    if (chown(entry.path().c_str(), st.st_uid, gid) != 0) {
                        AD::cerr << "Warning: Failed to set group for '" 
                                  << entry.path() << "': " << strerror(errno) << AD::endl;
                    }
                }
            }
        } catch (const fs::filesystem_error& e) {
            AD::cerr << "Error during recursive group setting: " << e.what() << AD::endl;
            return false;
        }
    }
    
    return true;
}

/**
 * @brief 获取文件的所有者信息
 * 
 * @param path 文件或目录路径
 * @param[out] username 返回的用户名
 * @param[out] groupname 返回的组名
 * @return true 获取成功
 * @return false 获取失败
 */
inline bool _AD_get_ownership(const fs::path& path, 
                            std::string& username, 
                            std::string& groupname) {
    
    if (!fs::exists(path)) {
        AD::cerr << "Error: Path '" << path << "' does not exist" << AD::endl;
        return false;
    }
    
    struct stat st;
    if (stat(path.c_str(), &st) != 0) {
        AD::cerr << "Error: Cannot get file status for '" << path 
                  << "': " << strerror(errno) << AD::endl;
        return false;
    }
    
    // 获取用户名
    struct passwd* pwd = getpwuid(st.st_uid);
    username = pwd ? pwd->pw_name : std::to_string(st.st_uid);
    
    // 获取组名
    struct group* grp = getgrgid(st.st_gid);
    groupname = grp ? grp->gr_name : std::to_string(st.st_gid);
    
    return true;
}

/**
 * @brief 打印文件的所有者信息
 * 
 * @param path 文件或目录路径
 */
inline void _AD_print_ownership(const fs::path& path) {
    std::string username, groupname;
    
    if (_AD_get_ownership(path, username, groupname)) {
        AD::cout << "Path: " << path << AD::endl;
        AD::cout << "Owner: " << username << AD::endl;
        AD::cout << "Group: " << groupname << AD::endl;
    }
}

/**
 * @brief 设置符号链接的所有者（不跟随链接）
 * 
 * @param symlink_path 符号链接路径
 * @param username 用户名
 * @param groupname 组名
 * @return true 设置成功
 * @return false 设置失败
 */
inline bool _AD_lchown(const fs::path& symlink_path, 
                      const std::string& username = "", 
                      const std::string& groupname = "") {
    
    if (!fs::exists(symlink_path) || !fs::is_symlink(symlink_path)) {
        AD::cerr << "Error: '" << symlink_path << "' is not a valid symlink" << AD::endl;
        return false;
    }
    
    uid_t uid = internal::getUid(username);
    gid_t gid = internal::getGid(groupname);
    
    if (uid == static_cast<uid_t>(-1) || gid == static_cast<gid_t>(-1)) {
        return false;
    }
    
    if (lchown(symlink_path.c_str(), uid, gid) != 0) {
        AD::cerr << "Error: Failed to set symlink ownership for '" << symlink_path 
                  << "': " << strerror(errno) << AD::endl;
        return false;
    }
    
    std::string user = username.empty() ? internal::getCurrentUsername() : username;
    std::string group = groupname.empty() ? internal::getCurrentGroupname() : groupname;
    
    AD::cout << "Set symlink ownership of '" << symlink_path << "' to " 
              << user << ":" << group << AD::endl;
    
    return true;
}

} // namespace AD

#endif // _AD_CHOWN_HPP_