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
 * ad_internet.hpp
 * Created by AD on 2026-04-09
 * Copyright (c) 2025-2026 AD All rights reserved.
 */
#ifndef _AD_INTERNET_HPP_
#define _AD_INTERNET_HPP_

#include <curl/curl.h>
#include <string>
#include <vector>
#include <functional>
#include <memory>
#include <fstream>
#include "AD_output.hpp"

namespace AD::internet {

// 進度回調類型
using progress_callback = std::function<bool(double dl_total, double dl_now, 
                                             double ul_total, double ul_now)>;

// HTTP 響應結構
struct http_response {
    long code = 0;
    std::string body;
    std::vector<std::string> headers;
    double download_speed = 0;

    bool success() const { return code >= 200 && code < 300; }
    bool redirect() const { return code >= 300 && code < 400; }
};

// HTTP 請求類
class http_request {
private:
    CURL* curl = nullptr;
    std::string url;
    std::string response_data;
    std::string output_file;
    progress_callback progress_cb;
    std::vector<std::string> custom_headers;
    long timeout_sec = 30;
    long connect_timeout_sec = 10;
    bool follow_redirect = true;
    bool verbose = false;

    static size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp) {
        ((std::string*)userp)->append((char*)contents, size * nmemb);
        return size * nmemb;
    }

    static size_t header_callback(char* buffer, size_t size, size_t nitems, void* userp) {
        ((std::vector<std::string>*)userp)->push_back(std::string(buffer, size * nitems));
        return size * nitems;
    }

    static int progress_callback_func(void* clientp, curl_off_t dltotal, curl_off_t dlnow,
                                  curl_off_t ultotal, curl_off_t ulnow) {
        auto* cb = (progress_callback*)clientp;
        if (cb && *cb) {
            return (*cb)(dltotal, dlnow, ultotal, ulnow) ? 0 : 1;
        }
        return 0;
    }
public:
    http_request() {
        curl = curl_easy_init();
    }

    ~http_request() {
        if (curl) curl_easy_cleanup(curl);
    }

    http_request& set_url(const std::string& u) { url = u; return *this; }
    http_request& set_timeout(long sec) { timeout_sec = sec; return *this; }
    http_request& set_connect_timeout(long sec) { connect_timeout_sec = sec; return *this; }
    http_request& set_follow_redirect(bool enable) { follow_redirect = enable; return *this; }
    http_request& set_verbose(bool enable) { verbose = enable; return *this; }
    http_request& set_output_file(const std::string& path) { output_file = path; return *this; }
    http_request& set_progress(progress_callback cb) { progress_cb = cb; return *this; }
    
    http_request& set_header(const std::string& header) {
        custom_headers.push_back(header);
        return *this;
    }

    http_request& set_user_agent(const std::string& ua) {
        custom_headers.push_back("User-Agent: " + ua);
        return *this;
    }

    http_request& set_authorization(const std::string& token) {
        custom_headers.push_back("Authorization: " + token);
        return *this;
    }

    http_response get() {
        if (!curl) return {};
        
        struct curl_slist* headers = nullptr;
        for (const auto& h : custom_headers) {
            headers = curl_slist_append(headers, h.c_str());
        }

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout_sec);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, connect_timeout_sec);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, follow_redirect ? 1L : 0L);
        curl_easy_setopt(curl, CURLOPT_VERBOSE, verbose ? 1L : 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);

        if (!output_file.empty()) {
            std::ofstream* ofs = new std::ofstream(output_file, std::ios::binary);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, ofs);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, +[](void* ptr, size_t sz, size_t n, void* f) -> size_t {
                ((std::ofstream*)f)->write((char*)ptr, sz * n);
                return sz * n;
            });
        } else {
            response_data.clear();
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        }
        http_response resp;
        if (progress_cb) {
            curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, progress_callback_func);
            curl_easy_setopt(curl, CURLOPT_XFERINFODATA, &progress_cb);
            curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
        }

        CURLcode res = curl_easy_perform(curl);

        if (res == CURLE_OK) {
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &resp.code);
            double speed;
            curl_easy_getinfo(curl, CURLINFO_SPEED_DOWNLOAD, &speed);
            resp.download_speed = speed;
            resp.body = response_data;
        } else {
            ad::cerr << "[Error] curl error: " << curl_easy_strerror(res) << ad::endl;
        }
        curl_slist_free_all(headers);
        return resp;
    }

    http_response post(const std::string& data = "", const std::string& content_type = "application/x-www-form-urlencoded") {
        if (!curl) return {};
        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, ("Content-Type: " + content_type).c_str());
        for (const auto& h : custom_headers) {
            headers = curl_slist_append(headers, h.c_str());
        }

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, data.size());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout_sec);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, follow_redirect ? 1L : 0L);
        response_data.clear();
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        http_response resp;
        CURLcode res = curl_easy_perform(curl);

        if (res == CURLE_OK) {
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &resp.code);
            resp.body = response_data;
        }

        curl_slist_free_all(headers);
        return resp;
    }
};
// 全局初始化
inline void __global_init() {
    curl_global_init(CURL_GLOBAL_DEFAULT);
}

// constexpr auto& init = __global_init;

inline void global_cleanup() {
    curl_global_cleanup();
}

} // namespace AD::internet;

#endif
