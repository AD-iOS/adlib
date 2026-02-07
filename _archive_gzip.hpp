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
 * _archive_gzip.hpp
 * Created by AD on 2/1/26
 * Copyright (c) 2026-2026 AD All rights reserved.
 */
/*
 * _archive.hpp 的補丁
 * 添加 gzip or ungzip 支持
**/

#ifndef _AD_ARCHIVE_GZIP_HPP_
#define _AD_ARCHIVE_GZIP_HPP_

#include "AD_output.hpp"
#include <archive.h>
#include <archive_entry.h>
#include <zlib.h>
#include <filesystem>
#include <vector>
#include <string>
#include <fstream>
#include <memory>
#include <iomanip>

namespace fs = std::filesystem;

namespace AD {
// 前向聲明 _archive 的 api
bool _archive_files_(const std::vector<std::string>& files, 
                         const std::string& output_path, 
                         const std::string& format);
    
bool _unarchive_(const std::string& archive_path,
                     const std::string& extract_to);

/* 
 * 向前聲明
 * 因為定義在後面
 */
inline bool _archive_gzip_stream(
    const std::string& input_file, 
    const std::string& output_file);
inline bool _unarchive_gzip_stream(
    const std::string& input_file, 
    const std::string& output_file);

/**
 * @brief 壓縮單個文件為 gz(已废弃,请使用 _archive_gzip_stream)
 * @deprecated 使用 _archive_gzip_stream 替代
 */
inline bool _archive_gzip_file_(const std::string& input_file, const std::string& output_file) {
    static bool warned = false;
    if (!warned) {
        AD::cout << "[Deprecated] _archive_gzip_file_ is deprecated. "
                 << "Please use _archive_gzip_stream instead." << AD::endl;
        warned = true;
    }
    return _archive_gzip_stream(input_file, output_file);
}

/**
 * @brief 解壓縮 gz(已废弃,请使用 _unarchive_gzip_stream)
 * @deprecated 使用 _unarchive_gzip_stream 替代
 */
inline bool _unarchive_gzip_file_(const std::string& input_file, const std::string& output_file) {
    static bool warned = false;
    if (!warned) {
        AD::cout << "[Deprecated] _unarchive_gzip_file_ is deprecated. "
                 << "Please use _unarchive_gzip_stream instead." << AD::endl;
        warned = true;
    }
    return _unarchive_gzip_stream(input_file, output_file);
}

inline bool _archive_gzip_stream(
    const std::string& input_file, 
    const std::string& output_file) {
    
    // 1. 打開文件
    std::ifstream in(input_file, std::ios::binary);
    if (!in.is_open()) {
        AD::cerr << "[Error] Failed to open input file: " << input_file << AD::endl;
        return false;
    }
    
    std::ofstream out(output_file, std::ios::binary);
    if (!out.is_open()) {
        AD::cerr << "[Error] Failed to open output file: " << output_file << AD::endl;
        return false;
    }
    
    // 2. 初始化 zlib
    z_stream stream = {0};
    int ret = deflateInit2(&stream, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                          15 + 16, 8, Z_DEFAULT_STRATEGY);
    if (ret != Z_OK) {
        AD::cerr << "[Error] Failed to initialize gzip compression: " 
                 << (stream.msg ? stream.msg : "Unknown error") << AD::endl;
        return false;
    }
    
    // 3. RAII 確保清理
    struct StreamCleanup {
        z_stream* s;
        ~StreamCleanup() { if (s) deflateEnd(s); }
    } cleanup = {&stream};
    
    // 4. 流式處理
    const size_t BUFFER_SIZE = 64 * 1024; // 64KB
    std::vector<char> in_buf(BUFFER_SIZE);
    std::vector<unsigned char> out_buf(BUFFER_SIZE);
    
    size_t total_in = 0, total_out = 0;
    bool success = true;
    
    do {
        in.read(in_buf.data(), in_buf.size());
        stream.avail_in = static_cast<uInt>(in.gcount());
        stream.next_in = reinterpret_cast<Bytef*>(in_buf.data());
        total_in += stream.avail_in;
        
        int flush = in.eof() ? Z_FINISH : Z_NO_FLUSH;
        
        do {
            stream.avail_out = static_cast<uInt>(out_buf.size());
            stream.next_out = out_buf.data();
            
            ret = deflate(&stream, flush);
            if (ret == Z_STREAM_ERROR) {
                AD::cerr << "[Error] Gzip compression stream error: " 
                         << (stream.msg ? stream.msg : "Unknown error") << AD::endl;
                success = false;
                break;
            }
            
            size_t have = out_buf.size() - stream.avail_out;
            total_out += have;
            
            if (have > 0) {
                out.write(reinterpret_cast<const char*>(out_buf.data()), have);
                if (!out.good()) {
                    AD::cerr << "[Error] Failed to write compressed data" << AD::endl;
                    success = false;
                    break;
                }
            }
        } while (stream.avail_out == 0);
        
        if (!success) break;
        
    } while (ret != Z_STREAM_END && !in.eof());
    
    // 5. 檢查結果
    if (ret != Z_STREAM_END) {
        AD::cerr << "[Error] Gzip compression incomplete: " 
                 << (stream.msg ? stream.msg : "Unknown error") << AD::endl;
        success = false;
    }
    
    if (success) {
        AD::cout << "[Success] Gzip compression: " << input_file 
                 << " -> " << output_file 
                 << " (" << total_in << " -> " << total_out 
                 << " bytes, ratio: " 
                 << AD::fixed << AD::setprecision(1)
                 << (total_out * 100.0 / total_in) << "%)" << AD::endl;
    }
    
    return success;
}

inline bool _unarchive_gzip_stream(
    const std::string& input_file, 
    const std::string& output_file) {
    
    // 1. 打開文件
    std::ifstream in(input_file, std::ios::binary);
    if (!in.is_open()) {
        AD::cerr << "[Error] Failed to open input file: " << input_file << AD::endl;
        return false;
    }
    
    std::ofstream out(output_file, std::ios::binary);
    if (!out.is_open()) {
        AD::cerr << "[Error] Failed to open output file: " << output_file << AD::endl;
        return false;
    }
    
    // 2. 檢查 gzip 魔數
    unsigned char magic[2];
    in.read(reinterpret_cast<char*>(magic), 2);
    if (!in.good() || magic[0] != 0x1F || magic[1] != 0x8B) {
        AD::cerr << "[Error] Not a valid gzip file: " << input_file << AD::endl;
        return false;
    }
    in.seekg(0, std::ios::beg); // 重置位置
    
    // 3. 初始化 zlib
    z_stream stream = {0};
    int ret = inflateInit2(&stream, 15 + 16); // 15 + 16 for gzip
    if (ret != Z_OK) {
        AD::cerr << "[Error] Failed to initialize gzip decompression: " 
                 << (stream.msg ? stream.msg : "Unknown error") << AD::endl;
        return false;
    }
    
    // 4. RAII 確保清理
    struct StreamCleanup {
        z_stream* s;
        ~StreamCleanup() { if (s) inflateEnd(s); }
    } cleanup = {&stream};
    
    // 5. 流式處理
    const size_t BUFFER_SIZE = 64 * 1024; // 64KB
    std::vector<char> in_buf(BUFFER_SIZE);
    std::vector<unsigned char> out_buf(BUFFER_SIZE);
    
    size_t total_in = 0, total_out = 0;
    bool success = true;
    
    do {
        in.read(in_buf.data(), in_buf.size());
        stream.avail_in = static_cast<uInt>(in.gcount());
        stream.next_in = reinterpret_cast<Bytef*>(in_buf.data());
        total_in += stream.avail_in;
        
        int flush = in.eof() ? Z_FINISH : Z_NO_FLUSH;
        
        do {
            stream.avail_out = static_cast<uInt>(out_buf.size());
            stream.next_out = out_buf.data();
            
            ret = inflate(&stream, flush);
            if (ret < 0 && ret != Z_BUF_ERROR) {
                AD::cerr << "[Error] Gzip decompression error (" << ret << "): " 
                         << (stream.msg ? stream.msg : "Unknown error") << AD::endl;
                success = false;
                break;
            }
            
            size_t have = out_buf.size() - stream.avail_out;
            total_out += have;
            
            if (have > 0) {
                out.write(reinterpret_cast<const char*>(out_buf.data()), have);
                if (!out.good()) {
                    AD::cerr << "[Error] Failed to write decompressed data" << AD::endl;
                    success = false;
                    break;
                }
            }
        } while (stream.avail_out == 0);
        
        if (!success) break;
        
    } while (ret != Z_STREAM_END && !in.eof());
    
    // 6. 檢查結果
    if (ret != Z_STREAM_END) {
        AD::cerr << "[Error] Gzip decompression incomplete: " 
                 << (stream.msg ? stream.msg : "Unknown error") << AD::endl;
        success = false;
    }
    
    if (success) {
        AD::cout << "[Success] Gzip decompression: " << input_file 
                 << " -> " << output_file 
                 << " (" << total_in << " -> " << total_out 
                 << " bytes)" << AD::endl;
    }
    
    return success;
}

/**
 * @brief 使用 libarchive 的 GZIP 壓縮(支持多文件)
 */
inline bool _archive_gzip_libarchive_(const std::vector<std::string>& files, const std::string& output_path) {
    auto writer = std::unique_ptr<archive, decltype(&archive_write_free)>(
        archive_write_new(), archive_write_free);
    
    if (!writer) {
        AD::cerr << "[Error] Failed to create archive writer" << AD::endl;
        return false;
    }
    
    // 設置為 raw gzip 格式(單文件壓縮)
    archive_write_set_format_raw(writer.get());
    archive_write_add_filter_gzip(writer.get());
    
    if (archive_write_open_filename(writer.get(), output_path.c_str()) != ARCHIVE_OK) {
        AD::cerr << "[Error] Failed to open archive for writing: " 
                 << archive_error_string(writer.get()) << AD::endl;
        return false;
    }
    
    // gzip 只能壓縮單個文件
    if (files.size() != 1) {
        AD::cerr << "[Error] GZIP format only supports single file compression. " 
                 << "Use TAR.GZ for multiple files." << AD::endl;
        return false;
    }
    
    const std::string& file_path = files[0];
    
    if (!fs::exists(file_path)) {
        AD::cerr << "[Error] File not found: " << file_path << AD::endl;
        return false;
    }
    
    std::ifstream file(file_path, std::ios::binary);
    if (!file.is_open()) {
        AD::cerr << "[Error] Failed to open file: " << file_path << AD::endl;
        return false;
    }
    
    // 獲取文件大小
    file.seekg(0, std::ios::end);
    size_t file_size = file.tellg();
    file.seekg(0, std::ios::beg);
    
    // 讀取文件內容
    std::vector<char> buffer(file_size);
    file.read(buffer.data(), file_size);
    file.close();
    
    // 寫入壓縮數據
    la_ssize_t written = archive_write_data(writer.get(), buffer.data(), file_size);
    
    if (written < 0) {
        AD::cerr << "[Error] Failed to write compressed data: " 
                 << archive_error_string(writer.get()) << AD::endl;
        return false;
    }
    
    archive_write_close(writer.get());
    
    AD::cout << "GZIP archive created: " << output_path 
             << " (" << file_size << " bytes compressed)" << AD::endl;
    
    return true;
}

/**
 * @brief gz 解壓
 */
inline bool _unarchive_gzip_libarchive_(const std::string& archive_path, const std::string& output_file) {
    auto reader = std::unique_ptr<archive, decltype(&archive_read_free)>(
        archive_read_new(), archive_read_free);
    
    if (!reader) {
        AD::cerr << "[Error] Failed to create archive reader" << AD::endl;
        return false;
    }
    
    archive_read_support_filter_gzip(reader.get());
    archive_read_support_format_raw(reader.get());
    
    if (archive_read_open_filename(reader.get(), archive_path.c_str(), 10240) != ARCHIVE_OK) {
        AD::cerr << "[Error] Failed to open archive: " 
                 << archive_error_string(reader.get()) << AD::endl;
        return false;
    }
    
    struct archive_entry* entry;
    if (archive_read_next_header(reader.get(), &entry) != ARCHIVE_OK) {
        AD::cerr << "[Error] Failed to read archive header: " 
                 << archive_error_string(reader.get()) << AD::endl;
        return false;
    }
    
    // 打開輸出文件
    std::ofstream out(output_file, std::ios::binary);
    if (!out.is_open()) {
        AD::cerr << "[Error] Failed to open output file: " << output_file << AD::endl;
        return false;
    }
    
    // 讀取並寫入解壓數據
    const void* buff;
    size_t size;
    int64_t offset;
    
    while (archive_read_data_block(reader.get(), &buff, &size, &offset) == ARCHIVE_OK) {
        out.write(static_cast<const char*>(buff), size);
    }
    
    out.close();
    archive_read_close(reader.get());
    
    AD::cout << "GZIP archive extracted: " << archive_path 
             << " -> " << output_file << AD::endl;
    
    return true;
}

/**
 * @brief 智能 GZIP 壓縮(自動選擇實現)
 */
inline bool _archive_gzip_(const std::vector<std::string>& files, const std::string& output_path) {
    if (files.size() == 1) {
        return _archive_gzip_file_(files[0], output_path);
    } else {
        // 多個文件壓縮會創建 tar.gz
        AD::cout << "[Info] Multiple files detected. Creating TAR.GZ archive instead." << AD::endl;
        return _archive_files_(files, output_path, "gzip");
    }
}

/**
 * @brief 智能 gz 解壓
 */
inline bool _unarchive_gzip_(const std::string& archive_path, const std::string& extract_to = ".") {
    // 檢查文件擴展名
    std::string ext = fs::path(archive_path).extension().string();
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    
    if (ext == ".gz" || ext == ".gzip") {
        // 對於 .gz 文件，使用 libarchive 解壓
        if (fs::is_directory(extract_to)) {
            // 如果 extract_to 是目錄，使用默認文件名
            std::string default_name = fs::path(archive_path).stem().string();
            if (default_name.empty() || default_name == fs::path(archive_path).filename()) {
                default_name = "decompressed_file";
            }
            return _unarchive_gzip_libarchive_(archive_path, fs::path(extract_to) / default_name);
        } else {
            // 如果 extract_to 是文件路徑，直接使用
            return _unarchive_gzip_libarchive_(archive_path, extract_to);
        }
    } else if (ext == ".tgz" || ext == ".tar.gz") {
        // 對於 tar.gz 文件，使用通用解壓
        return _unarchive_(archive_path, extract_to);
    } else {
        AD::cerr << "[Error] Unrecognized gzip file extension: " << ext << AD::endl;
        return false;
    }
}

/**
 * @brief 壓縮目錄為 tar.gz
 */
inline bool _archive_gzip_directory_(const std::string& directory_path, const std::string& output_path) {
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
    
    return _archive_files_(files, output_path, "gzip");
}

/**
 * @brief 檢測是否為 gz
 */
inline bool _is_gzip_file_(const std::string& file_path) {
    std::ifstream file(file_path, std::ios::binary);
    if (!file.is_open()) {
        return false;
    }
    
    unsigned char magic[2];
    file.read(reinterpret_cast<char*>(magic), 2);
    
    // GZIP 魔數: 0x1F 0x8B
    return (magic[0] == 0x1F && magic[1] == 0x8B);
}

} // namespace AD

#endif // _AD_ARCHIVE_GZIP_HPP_