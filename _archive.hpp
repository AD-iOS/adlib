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
 * _archive.hpp
 * Created by AD on 2/1/26
 * Copyright (c) 2025-2026 AD All rights reserved.
 */

#ifndef _AD_ARCHIVE_HPP_
#define _AD_ARCHIVE_HPP_

#include "AD_output.hpp"
#include <archive.h>
#include <archive_entry.h>
#include <filesystem>
#include <vector>
#include <string>
#include <fstream>
#include <memory>

/* 補丁文件 */
#include "_archive_gzip.hpp"

namespace fs = std::filesystem;

namespace AD {

/**
 * @brief 創建歸檔對象
 */
inline std::unique_ptr<archive, decltype(&archive_write_free)> create_writer(const std::string& format) {
    archive* ar = archive_write_new();
    
    if (format == "zip") {
        archive_write_set_format_zip(ar);
    } else if (format == "tar") {
        archive_write_set_format_pax_restricted(ar); // 現代 tar 格式
    } else if (format == "gzip" || format == "gz") {
        archive_write_set_format_gnutar(ar);
        archive_write_add_filter_gzip(ar);
    } else if (format == "bzip2" || format == "bz2") {
        archive_write_set_format_gnutar(ar);
        archive_write_add_filter_bzip2(ar);
    } else if (format == "xz") {
        archive_write_set_format_gnutar(ar);
        archive_write_add_filter_xz(ar);
    } else if (format == "lzma") {
        archive_write_set_format_gnutar(ar);
        archive_write_add_filter_lzma(ar);
    } else if (format == "zstd") {
        archive_write_set_format_gnutar(ar);
        archive_write_add_filter_zstd(ar);
    } else if (format == "7zip" || format == "7z") {
        archive_write_set_format_7zip(ar);
    } else if (format == "ar") {
        archive_write_set_format_ar_bsd(ar);
    } else if (format == "cpio") {
        archive_write_set_format_cpio(ar);
    } else if (format == "iso") {
        archive_write_set_format_iso9660(ar);
    } else {
        AD::cerr << "[Error] Unsupported archive format: " << format << AD::endl;
        archive_write_free(ar);
        return {nullptr, archive_write_free};
    }
    
    return {ar, archive_write_free};
}

/**
 * @brief 創建解壓對象
 */
inline std::unique_ptr<archive, decltype(&archive_read_free)> create_reader() {
    archive* ar = archive_read_new();
    archive_read_support_format_all(ar);
    archive_read_support_filter_all(ar);
    return {ar, archive_read_free};
}

/**
 * @brief 通用歸檔函數
 */
inline bool _archive_files_(const std::vector<std::string>& files, 
                           const std::string& output_path, 
                           const std::string& format = "zip") {
    
    auto writer = create_writer(format);
    if (!writer) {
        return false;
    }
    
    if (archive_write_open_filename(writer.get(), output_path.c_str()) != ARCHIVE_OK) {
        AD::cerr << "[Error] Failed to open archive for writing: " << archive_error_string(writer.get()) << AD::endl;
        return false;
    }
    
    char buff[8192];
    
    for (const auto& file_path : files) {
        if (!fs::exists(file_path)) {
            AD::cerr << "[Warning] File not found: " << file_path << AD::endl;
            continue;
        }
        
        // 創建歸檔條目
        struct archive_entry* entry = archive_entry_new();
        archive_entry_set_pathname(entry, fs::path(file_path).filename().c_str());
        archive_entry_set_size(entry, fs::file_size(file_path));
        archive_entry_set_filetype(entry, S_IFREG);
        archive_entry_set_perm(entry, 0644);
        
        if (archive_write_header(writer.get(), entry) != ARCHIVE_OK) {
            AD::cerr << "[Error] Failed to write header for: " << file_path 
                     << " - " << archive_error_string(writer.get()) << AD::endl;
            archive_entry_free(entry);
            continue;
        }
        
        // 寫入文件內容
        std::ifstream file(file_path, std::ios::binary);
        if (!file.is_open()) {
            AD::cerr << "[Error] Failed to open file: " << file_path << AD::endl;
            archive_entry_free(entry);
            continue;
        }
        
        while (file.read(buff, sizeof(buff))) {
            archive_write_data(writer.get(), buff, file.gcount());
        }
        
        if (file.gcount() > 0) {
            archive_write_data(writer.get(), buff, file.gcount());
        }
        
        file.close();
        archive_entry_free(entry);
    }
    
    archive_write_close(writer.get());
    return true;
}

/**
 * @brief 歸檔目錄
 */
inline bool _archive_directory_(const std::string& directory_path,
                               const std::string& output_path,
                               const std::string& format = "zip") {
    
    if (!fs::exists(directory_path) || !fs::is_directory(directory_path)) {
        AD::cerr << "[Error] Directory not found or not a directory: " << directory_path << AD::endl;
        return false;
    }
    
    std::vector<std::string> files;
    for (const auto& entry : fs::recursive_directory_iterator(directory_path)) {
        if (fs::is_regular_file(entry)) {
            files.push_back(entry.path().string());
        }
    }
    
    return _archive_files_(files, output_path, format);
}

/**
 * @brief 通用解壓函數
 */
inline bool _unarchive_(const std::string& archive_path,
                       const std::string& extract_to = ".") {
    
    if (!fs::exists(archive_path)) {
        AD::cerr << "[Error] Archive not found: " << archive_path << AD::endl;
        return false;
    }
    
    auto reader = create_reader();
    if (!reader) {
        return false;
    }
    
    if (archive_read_open_filename(reader.get(), archive_path.c_str(), 10240) != ARCHIVE_OK) {
        AD::cerr << "[Error] Failed to open archive: " << archive_error_string(reader.get()) << AD::endl;
        return false;
    }
    
    struct archive_entry* entry;
    char buff[8192];
    int flags = ARCHIVE_EXTRACT_TIME | ARCHIVE_EXTRACT_PERM | ARCHIVE_EXTRACT_ACL | ARCHIVE_EXTRACT_FFLAGS;
    
    while (archive_read_next_header(reader.get(), &entry) == ARCHIVE_OK) {
        const char* filename = archive_entry_pathname(entry);
        fs::path full_path = fs::path(extract_to) / filename;
        
        // 確保目錄存在
        fs::create_directories(full_path.parent_path());
        
        archive_entry_set_pathname(entry, full_path.c_str());
        
        if (archive_read_extract(reader.get(), entry, flags) != ARCHIVE_OK) {
            AD::cerr << "[Error] Failed to extract: " << filename 
                     << " - " << archive_error_string(reader.get()) << AD::endl;
        } else {
            AD::cout << "Extracted: " << filename << AD::endl;
        }
    }
    
    archive_read_close(reader.get());
    return true;
}

/* _archive_ */

// ZIP 格式
inline bool _archive_zip_(const std::vector<std::string>& files, const std::string& output_path) {
    return _archive_files_(files, output_path, "zip");
}

inline bool _unarchive_zip_(const std::string& archive_path, const std::string& extract_to = ".") {
    return _unarchive_(archive_path, extract_to);
}

// TAR 格式
inline bool _archive_tar_(const std::vector<std::string>& files, const std::string& output_path) {
    return _archive_files_(files, output_path, "tar");
}

inline bool _unarchive_tar_(const std::string& archive_path, const std::string& extract_to = ".") {
    return _unarchive_(archive_path, extract_to);
}

// GZIP 壓縮的 TAR (.tar.gz 或 .tgz)
inline bool _archive_tgz_(const std::vector<std::string>& files, const std::string& output_path) {
    return _archive_files_(files, output_path, "gzip");
}

inline bool _unarchive_tgz_(const std::string& archive_path, const std::string& extract_to = ".") {
    return _unarchive_(archive_path, extract_to);
}

/*
 * inline bool _archive_gzip_(const std::vector<std::string>& files, const std::string& output_path) {
 *    return _archive_tgz_(files, output_path);
 * }
 * 
 * inline bool _unarchive_gzip_(const std::string& archive_path, const std::string& extract_to = ".") {
 *    return _unarchive_(archive_path, extract_to);
 * }
 */

// BZIP2 壓縮的 TAR (.tar.bz2)
inline bool _archive_bz2_(const std::vector<std::string>& files, const std::string& output_path) {
    return _archive_files_(files, output_path, "bzip2");
}

inline bool _unarchive_bz2_(const std::string& archive_path, const std::string& extract_to = ".") {
    return _unarchive_(archive_path, extract_to);
}

// XZ 壓縮的 TAR (.tar.xz)
inline bool _archive_xz_(const std::vector<std::string>& files, const std::string& output_path) {
    return _archive_files_(files, output_path, "xz");
}

inline bool _unarchive_xz_(const std::string& archive_path, const std::string& extract_to = ".") {
    return _unarchive_(archive_path, extract_to);
}

// ZSTD 壓縮的 TAR (.tar.zst)
inline bool _archive_zstd_(const std::vector<std::string>& files, const std::string& output_path) {
    return _archive_files_(files, output_path, "zstd");
}

inline bool _unarchive_zstd_(const std::string& archive_path, const std::string& extract_to = ".") {
    return _unarchive_(archive_path, extract_to);
}

// 7ZIP 格式
inline bool _archive_7zip_(const std::vector<std::string>& files, const std::string& output_path) {
    return _archive_files_(files, output_path, "7zip");
}

inline bool _unarchive_7zip_(const std::string& archive_path, const std::string& extract_to = ".") {
    return _unarchive_(archive_path, extract_to);
}

// AR 歸檔格式
inline bool _archive_ar_(const std::vector<std::string>& files, const std::string& output_path) {
    return _archive_files_(files, output_path, "ar");
}

inline bool _unarchive_ar_(const std::string& archive_path, const std::string& extract_to = ".") {
    return _unarchive_(archive_path, extract_to);
}

// CPIO 格式
inline bool _archive_cpio_(const std::vector<std::string>& files, const std::string& output_path) {
    return _archive_files_(files, output_path, "cpio");
}

inline bool _unarchive_cpio_(const std::string& archive_path, const std::string& extract_to = ".") {
    return _unarchive_(archive_path, extract_to);
}

// ISO 格式
inline bool _archive_iso_(const std::vector<std::string>& files, const std::string& output_path) {
    return _archive_files_(files, output_path, "iso");
}

inline bool _unarchive_iso_(const std::string& archive_path, const std::string& extract_to = ".") {
    return _unarchive_(archive_path, extract_to);
}


/**
 * @brief 自動檢測輸入類型
 */
inline bool _archive_smart_(const std::string& input_path, const std::string& output_path, const std::string& format = "zip") {
    if (fs::is_directory(input_path)) {
        return _archive_directory_(input_path, output_path, format);
    } else if (fs::is_regular_file(input_path)) {
        return _archive_files_({input_path}, output_path, format);
    }
    
    AD::cerr << "[Error] Input path not found: " << input_path << AD::endl;
    return false;
}

/**
 * @brief 檢測歸檔格式
 */
inline std::string _detect_archive_format_(const std::string& archive_path) {
    if (!fs::exists(archive_path)) {
        return "";
    }
    
    auto reader = create_reader();
    if (!reader) {
        return "";
    }
    
    if (archive_read_open_filename(reader.get(), archive_path.c_str(), 10240) != ARCHIVE_OK) {
        return "";
    }
    
    const char* format = archive_format_name(reader.get());
    std::string result = format ? format : "";
    
    archive_read_close(reader.get());
    return result;
}

/**
 * @brief 列出歸檔內容
 */
inline std::vector<std::string> _list_archive_(const std::string& archive_path) {
    std::vector<std::string> files;
    
    if (!fs::exists(archive_path)) {
        AD::cerr << "[Error] Archive not found: " << archive_path << AD::endl;
        return files;
    }
    
    auto reader = create_reader();
    if (!reader) {
        return files;
    }
    
    if (archive_read_open_filename(reader.get(), archive_path.c_str(), 10240) != ARCHIVE_OK) {
        AD::cerr << "[Error] Failed to open archive: " << archive_error_string(reader.get()) << AD::endl;
        return files;
    }
    
    struct archive_entry* entry;
    while (archive_read_next_header(reader.get(), &entry) == ARCHIVE_OK) {
        files.push_back(archive_entry_pathname(entry));
    }
    
    archive_read_close(reader.get());
    return files;
}

/* 來自 _archive_gzip 補丁文件 */
/**
 * @brief GZIP 單文件壓縮
 */
inline bool _archive_gzip_single_(const std::string& input_file, const std::string& output_file) {
    return _archive_gzip_stream(input_file, output_file);
}

/**
 * @brief GZIP 單文件解壓
 */
inline bool _unarchive_gzip_single_(const std::string& input_file, const std::string& output_file) {
    return _unarchive_gzip_stream(input_file, output_file);
}

} // namespace AD

#endif // _AD_ARCHIVE_HPP_