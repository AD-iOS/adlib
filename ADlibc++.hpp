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
 *
 * ADlibc++.hpp
 * AD‘s C++ private standard library
 * Created by AD on 9/12/25
 * Copyright (c) 2025-2026 AD All rights reserved.
 *
 */
/*
 *
 * This is a header document containing all header documents of the AD private C++ standard library.
 * Using this header document will contain all the header documents of the AD library.
 *
**/

#ifndef _AD_LIBCXX_HPP_
#define _AD_LIBCXX_HPP_

#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <memory>
#include <iostream>

#include "fix_std.hpp"

#ifdef ad_rm
    #include "_rm_.hpp"
#endif

#if defined(ad_output) || defined(ad_io)
    #include "AD_output.hpp"
#endif

#ifdef ad_system
    #include "ad_system.hpp"
#endif

#ifdef ad_file
    #include "_file.hpp"
#endif

#ifdef ad_func
    #include "AD_func.hpp"
#endif

#ifdef ad_null
    #include "AD_null.hpp"
    /* Added by AD Time: 22:11/20/12/25 */
#endif

#ifdef ad_read
    #include "_read.hpp"
#endif

#ifdef ad_string
    #include "AD_string.hpp"
#endif

#ifdef ad_time
    #include "AD_time.hpp"
    /* Added by AD Time: 22:37/20/12/25 */
#endif

#ifdef ad_write
    #include "_write.hpp"
    /* Added by AD Time: 23:48/28/12/25 */
#endif

#ifdef ad_filename
    #include "_filename.hpp"
    /* Added by AD Time: 22:46/2/1/26 */
#endif

#ifdef ad_archive
    #include <archive.h>
    #include <archive_entry.h>
    #include "_archive.hpp"
    /* Added by AD Time: 15:58/1/11/26 */
#endif

#ifdef ad_other
    #include "ad_system.hpp"
    #include "AD_func.hpp"
    #include "AD_time.hpp"
#endif

#if defined(ad_user) || defined(ad_chown)
    #include "_chown.hpp"
    #include "AD_user.hpp"
#endif

#ifdef ad_fs
    #include "_rm_.hpp"
    #include "_file.hpp"
    #include "AD_null.hpp"
    #include "_read.hpp"
    #include "AD_string.hpp"
    #include "_write.hpp"
    #include "_filename.hpp"
    #include "_archive.hpp"
#endif

#ifdef ad_all
    #include "AD_func.hpp"
    #include "AD_null.hpp"
    #include "AD_output.hpp"
    #include "AD_string.hpp"
    #include "AD_time.hpp"
    #include "AD_user.hpp"
    #include "_archive.hpp"
    #include "_chown.hpp"
    #include "_file.hpp"
    #include "_filename.hpp"
    #include "_read.hpp"
    #include "_rm_.hpp"
    #include "_write.hpp"
    #include "ad_system.hpp"
    #include <archive.h>
    #include <archive_entry.h>
#endif

/* Added by AD Time: 23:32/12/1/26 */
namespace AD {
    /* Added by AD Time: 19:47/21/1/26 */
  #ifdef ad_fs
    namespace fs {
        /* Added by AD Time: 22:13/20/12/25 */
        inline bool mkdir(const std::filesystem::path& path) {
            return _AD_mkdir(path);
        }
        
        inline bool touch(const std::filesystem::path& filename) {
            return _AD_touch(filename);
        }
        
        inline std::string read_all(const std::string& path) {
            return _AD_read_all(path);
        }
        
        inline std::vector<std::string> read_lines(const std::string& path) {
            return _AD_read_lines(path);
        }
        
        inline bool readable(const std::string& path) {
            return _AD_readable(path);
        }
        
        inline int rm(const std::filesystem::path& path) {
            return _AD_rm(path);
        }
        
        inline int rmdir(const std::filesystem::path& path) {
            return _AD_rmdir(path);
        }
        
        inline int rmdirf(const std::filesystem::path& path) {
            return _AD_rmdirf(path);
        }
        
        inline int rm_safe(const std::filesystem::path& path) {
            return _AD_rm_safe(path);
        }

        /* Added by AD Time: 22:38/20/12/25 */
        inline bool write_all(const std::string& path, const std::string& content) {
             return _AD_write_all(path, content);
        }

        inline bool append(const std::string& path, const std::string& content) {
            return _AD_append(path, content);
        }

        inline bool write_lines(const std::string& path, const std::vector<std::string>& lines) {
            return _AD_write_lines(path, lines);
        }
        /* Added by AD Time: 21:23/28/12/25 */
        inline std::string filename(const std::filesystem::path& path) {
            return _AD_filename(path);
        }

        inline std::string filename_base(const std::filesystem::path& path) {
            return _AD_filename_base(path);
        }
    } /* namespace fs */
  #endif

    /*
     * @update Added by AD Time: 23:32/25/12/25
     * @Prompt It is not recommended to use using namespace sys
     */
  #ifdef ad_system
    namespace command {
        /* Added by AD Time: 23:32/25/12/25 */
        inline int bash(const char* command) {
            return ad_bash_system(command);
        }
        /* Added by AD Time: 23:40/25/12/25 */
        inline int zsh(const char* command) {
            return ad_zsh_system(command);
        }

        inline int fish(const char* command) {
            return ad_fish_system(command);
        }

        inline int dash(const char* command) {
            return ad_dash_system(command);
        }

    } /* namespace system */
    namespace sys = command;
  #endif

    /* Added by AD Time: 22:46/2/1/26 */
    /*
     * @adjust: changed the API name!
     * reason: semantic conflict/meaning conflict
     * adjusted to 'archive::zip', which is at least better than 'archive::arch_zip(compression::compression_zip)'(semantically)
     */
  #ifdef ad_archive
    namespace archive {
        inline bool zip(const std::vector<std::string>& files, const std::string& output_path) {
            return _archive_zip_(files, output_path);
        }

        inline bool tar(const std::vector<std::string>& files, const std::string& output_path) {
            return _archive_tar_(files, output_path);
        }

        inline bool tgz(const std::vector<std::string>& files, const std::string& output_path) {
            return _archive_tgz_(files, output_path);
        }

        /* Added by AD Time: 23:01/2/1/26 */
        inline bool gzip(const std::vector<std::string>& files, const std::string& output_path) {
            return _archive_gzip_(files, output_path);
        }

    } /* namespace archive */

    /* Added by AD Time: 22:46/2/1/26 */
    /* unarchive same as archive namespace */
    namespace unarchive {
        inline bool zip(const std::string& archive_path, const std::string& extract_to = ".") {
            return _unarchive_zip_(archive_path, extract_to);
        }

        inline bool tar(const std::string& archive_path, const std::string& extract_to = ".") {
            return _unarchive_tar_(archive_path, extract_to);
        }

        inline bool tgz(const std::string& archive_path, const std::string& extract_to = ".") {
            return _unarchive_tgz_(archive_path, extract_to);
        }

        /* Added by AD Time: 23:01/2/1/26 */
        inline bool gzip(const std::string& archive_path, const std::string& extract_to = ".") {
            return _unarchive_gzip_(archive_path, extract_to);
        }

    } /* namespace unarchive */

    /* Added by AD Time: 22:46/2/1/26 */

    namespace archive_method {
        inline bool arch_smart(const std::string& input_path, const std::string& output_path, const std::string& format = "zip") {
            return _archive_smart_(input_path, output_path, format);
        }

        inline std::vector<std::string> list_arch(const std::string& archive_path) {
            return _list_archive_(archive_path);
        }

        inline std::string detect_arch_fmt(const std::string& archive_path) {
            return _detect_archive_format_(archive_path);
        }

        inline bool unarch_gzip_single(const std::string& input_file, const std::string& output_file) {
            return _unarchive_gzip_file_(input_file, output_file);
        }

        inline bool is_gzip_file(const std::string& file_path) {
            return _is_gzip_file_(file_path);
        }

        inline bool arch_gzip_single(const std::string& input_file, const std::string& output_file) {
            return _archive_gzip_file_(input_file, output_file);
        }

        inline bool arch_gzip_dir(const std::string& directory_path, const std::string& output_path) {
            return _archive_gzip_directory_(directory_path, output_path);
        }

        /* alias of function */
        /* Added by AD Time: 22:58/11/1/26 */
        constexpr auto& gzip_dir = arch_gzip_dir;
        constexpr auto& gz_d = arch_gzip_dir;
        constexpr auto& gzip_single = arch_gzip_single;
        constexpr auto& gz_single = arch_gzip_single;
        constexpr auto& is_gz_file = is_gzip_file;
        constexpr auto& unarch_gz_single = unarch_gzip_single;
        constexpr auto& fmt_detection = detect_arch_fmt;
        constexpr auto& smart = arch_smart;
        constexpr auto& list = list_arch;

    } /* namespace archive_method */

    namespace arch = archive;
    namespace unarch = unarchive;
    namespace auxarch = archive_method;

  #endif

    /* Added by AD Time: 16:00/11/1/26 */
  #ifdef ad_user
    namespace set {
        inline bool chown(const std::fs::path& path, 
                          const std::string& username = "", 
                          const std::string& groupname = "", 
                          bool recursive = false) {
            return _AD_chown(path, username, groupname, recursive);
        }

        inline bool user(const std::fs::path& path, 
                         const std::string& username = "", 
                         bool recursive = false) {
            return _AD_chown_user(path, username, recursive);
        }

        inline bool group(const std::fs::path& path, 
                          const std::string& groupname = "", 
                          bool recursive = false) {
            return _AD_chown_group(path, groupname, recursive);
        }

        inline bool symlink(const std::fs::path& symlink_path, 
            const std::string& username = "", 
            const std::string& groupname = "") {
            return _AD_lchown(symlink_path, username, groupname);
        }

    /* Added by AD Time: 21:58/11/1/26 */
    /* this is a new alias of  function in set namespace */
    constexpr auto& uid = user;
    constexpr auto& gid = group;
    constexpr auto& link = symlink;

    /* Added by AD Time: 21:59/11/1/26 */
    /* new alias, from user namespace of get namespace */

    constexpr auto& uidname = user::get::uidname;
    constexpr auto& gidname = user::get::gidname;

    // constexpr auto& uidname = ::AD::user::get::uidname;
    // constexpr auto& gidname = ::AD::user::get::gidname;

    } /* namespace set */
  #endif

    /* Added by AD Time: 16:8/11/1/26 */
  #if defined(ad_chown) || defined(ad_user)
    namespace chown {
        inline void powner(const std::fs::path& path) {
            _AD_print_ownership(path);
        }

        inline bool usrex(const std::string& username) {
            return getpwnam(username.c_str()) != nullptr;
        }

        inline bool grpex(const std::string& groupname) {
            return getgrnam(groupname.c_str()) != nullptr;
        }

        /* Added by AD Time: 21:59/11/1/26 */
        /* this is new alias of func in the chown namespace */
        constexpr auto& all = set::chown;
        constexpr auto& uid = set::uid;
        constexpr auto& gid = set::gid;
        constexpr auto& link = set::link;
        constexpr auto& user = set::user;
        constexpr auto& group = set::group;

    } /* namespace chown */

    /* Added by AD Time: 16:26/11/1/26 */
    namespace get {
        inline std::pair<std::string, std::string> all(const std::fs::path& path) {
            std::string username, groupname;
            _AD_get_ownership(path, username, groupname);
            return {username, groupname};
        }

        inline std::string user(const std::fs::path& path) {
            std::string username, groupname;
            _AD_get_ownership(path, username, groupname);
            return username;
        }

        inline std::string group(const std::fs::path& path) {
            std::string username, groupname;
            _AD_get_ownership(path, username, groupname);
            return groupname;
        }

        inline uid_t uid(const std::fs::path& path) {
            if (!std::fs::exists(path)) return static_cast<uid_t>(-1);
            struct stat st;
            if (stat(path.c_str(), &st) != 0) return static_cast<uid_t>(-1);
            return st.st_uid;
        }

        inline gid_t gid(const std::fs::path& path) {
            if (!std::fs::exists(path)) return static_cast<gid_t>(-1);
            struct stat st;
            if (stat(path.c_str(), &st) != 0) return static_cast<gid_t>(-1);
            return st.st_gid;
        }

    } /* namespace get */
  #endif

    /* Added by AD Time: 22:16/22/1/26 */
  #ifdef ad_fs
    namespace mk {
        inline bool dir(const std::fs::path& path) {
            return _AD_mkdir(path);
        }
        
        inline bool file(const std::fs::path& filename) {
            return _AD_touch(filename);
        }

        inline bool directory(const std::fs::path& path) {
            return _AD_mkdir(path);
        }
    } /* namespace mk */
  #endif

  #ifdef ad_system
    inline int system(const std::string& command) {
        return _ad_system(command);
    }
  #endif

} /* namespace AD */

namespace ad = AD;

#endif
