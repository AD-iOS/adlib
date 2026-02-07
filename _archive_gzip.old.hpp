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
 * _archive_gzip.old.hpp
 * Created by AD on 3/1/26
 * Copyright (c) 2025-2026 AD All rights reserved.
**/

/* 舊實現 */
inline bool _archive_gzip_file_(const std::string& input_file, const std::string& output_file) {
    if (!fs::exists(input_file)) {
        AD::cerr << "[Error] Input file not found: " << input_file << AD::endl;
        return false;
    }
    
    if (!fs::is_regular_file(input_file)) {
        AD::cerr << "[Error] Input is not a regular file: " << input_file << AD::endl;
        return false;
    }
    
    // 打開輸入文件
    std::ifstream in(input_file, std::ios::binary);
    if (!in.is_open()) {
        AD::cerr << "[Error] Failed to open input file: " << input_file << AD::endl;
        return false;
    }
    
    // 獲取文件大小
    in.seekg(0, std::ios::end);
    size_t file_size = in.tellg();
    in.seekg(0, std::ios::beg);
    
    // 讀取文件內容
    std::vector<unsigned char> input_data(file_size);
    in.read(reinterpret_cast<char*>(input_data.data()), file_size);
    in.close();
    
    // 設置 GZIP 壓縮參數
    z_stream stream;
    memset(&stream, 0, sizeof(stream));
    
    if (deflateInit2(&stream, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                    15 + 16, // 15 = window bits, 16 = gzip header
                    8, // memory level
                    Z_DEFAULT_STRATEGY) != Z_OK) {
        AD::cerr << "[Error] Failed to initialize gzip compression" << AD::endl;
        return false;
    }
    
    // 設置輸入數據
    stream.next_in = input_data.data();
    stream.avail_in = static_cast<uInt>(input_data.size());
    
    // 計算最大輸出大小
    size_t max_output_size = deflateBound(&stream, stream.avail_in);
    std::vector<unsigned char> output_data(max_output_size);
    
    stream.next_out = output_data.data();
    stream.avail_out = static_cast<uInt>(max_output_size);
    
    // 執行壓縮
    int result = deflate(&stream, Z_FINISH);
    
    if (result != Z_STREAM_END) {
        AD::cerr << "[Error] Gzip compression failed: " << result << AD::endl;
        deflateEnd(&stream);
        return false;
    }
    
    size_t compressed_size = max_output_size - stream.avail_out;
    
    // 寫入輸出文件
    std::ofstream out(output_file, std::ios::binary);
    if (!out.is_open()) {
        AD::cerr << "[Error] Failed to open output file: " << output_file << AD::endl;
        deflateEnd(&stream);
        return false;
    }
    
    out.write(reinterpret_cast<const char*>(output_data.data()), compressed_size);
    out.close();
    
    deflateEnd(&stream);
    
    AD::cout << "Gzip compression successful: " << input_file 
             << " -> " << output_file 
             << " (" << file_size << " -> " << compressed_size << " bytes)" << AD::endl;
    
    return true;
}

/**
 * @brief 解壓縮 gz
 * @warn 這個函數已經在內部被廢棄了,他隨時可能會被刪除掉或者指向 _unarchive_gzip_stream bool 類
 */
inline bool _unarchive_gzip_file_(const std::string& input_file, const std::string& output_file) {
    if (!fs::exists(input_file)) {
        AD::cerr << "[Error] Input file not found: " << input_file << AD::endl;
        return false;
    }
    
    // 打開輸入文件
    std::ifstream in(input_file, std::ios::binary);
    if (!in.is_open()) {
        AD::cerr << "[Error] Failed to open input file: " << input_file << AD::endl;
        return false;
    }
    
    // 獲取文件大小
    in.seekg(0, std::ios::end);
    size_t file_size = in.tellg();
    in.seekg(0, std::ios::beg);
    
    // 讀取壓縮數據
    std::vector<unsigned char> compressed_data(file_size);
    in.read(reinterpret_cast<char*>(compressed_data.data()), file_size);
    in.close();
    
    // 設置 GZIP 解壓參數
    z_stream stream;
    memset(&stream, 0, sizeof(stream));
    
    if (inflateInit2(&stream, 15 + 16) != Z_OK) { // 15 + 16 for gzip header
        AD::cerr << "[Error] Failed to initialize gzip decompression" << AD::endl;
        return false;
    }
    
    // 設置輸入數據
    stream.next_in = compressed_data.data();
    stream.avail_in = static_cast<uInt>(compressed_data.size());
    
    // 分配緩衝區
    size_t output_size = compressed_data.size() * 6;
    std::vector<unsigned char> output_data(output_size);
    
    stream.next_out = output_data.data();
    stream.avail_out = static_cast<uInt>(output_size);
    
    // 執行解壓
    int result = inflate(&stream, Z_FINISH);
    
    if (result != Z_STREAM_END && result != Z_OK) {
        AD::cerr << "[Error] Gzip decompression failed: " << result 
                 << " - " << (stream.msg ? stream.msg : "Unknown error") << AD::endl;
        inflateEnd(&stream);
        return false;
    }
    
    size_t decompressed_size = output_size - stream.avail_out;
    
    // 寫入輸出文件
    std::ofstream out(output_file, std::ios::binary);
    if (!out.is_open()) {
        AD::cerr << "[Error] Failed to open output file: " << output_file << AD::endl;
        inflateEnd(&stream);
        return false;
    }
    
    out.write(reinterpret_cast<const char*>(output_data.data()), decompressed_size);
    out.close();
    
    inflateEnd(&stream);
    
    AD::cout << "Gzip decompression successful: " << input_file 
             << " -> " << output_file 
             << " (" << file_size << " -> " << decompressed_size << " bytes)" << AD::endl;
    
    return true;
}
