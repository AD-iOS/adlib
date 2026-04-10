# include <AD/ad_internet.hpp>
# include <AD/ad_md5.hpp>
# include <AD/ad_json.hpp>
# define ad_comuse
# define ad_fs
# include <AD/ad.h>

namespace ad = AD;

// 下载单个文件
bool download_file(const std::string& url, const std::string& local_path) {
    ad::internet::http_request req;
    auto resp = req.set_url(url)
                   .set_output_file(local_path)
                   .set_follow_redirect(true)
                   .set_timeout(30)
                   .get();
    return resp.success();
}

// 同步函数
inline bool sync_web_assets(const std::string& www, const std::string& manifest_url, const std::string& manifest_path) {
    ad::fs::mkdir(www);
    // 1. 下载清单
    if (!download_file(manifest_url, manifest_path)) {
        ad::cerr << "Failed to download manifest" << ad::endl;
        return false;
    }
    // 2. 解析 manifest
    auto manifest_value = ad::json::parse_file(manifest_path);
    if (!manifest_value.is_object()) {
        ad::cerr << "Invalid manifest format" << ad::endl;
        return false;
    }

    auto manifest = manifest_value.as_object();
    // 3. 遍历文件列表, 按需下载
    auto files = ad::json::get_or(manifest, "files", ad::json::array());
    for (const auto& file_val : files.as_array()) {
        auto file = file_val.as_object();
        std::string name = ad::json::get_or(file, "name", std::string(""));
        std::string url = ad::json::get_or(file, "url", std::string(""));
        std::string remote_md5 = ad::json::get_or(file, "md5", std::string(""));
        if (name.empty() || url.empty()) continue;
        std::string local_path = www + "/" + name;
        // 确保父目录存在
        ad::fs::mkdir(std::filesystem::path(local_path).parent_path());
        // 本地文件不存在 或 MD5 不匹配
        bool need_download = false;
        if (!ad::fs::readable(local_path)) {
            need_download = true;
        } else {
            std::string local_md5 = ad::ssl::md5::hash_file(local_path);
            if (local_md5 != remote_md5) {
                need_download = true;
            }
        }

        if (need_download) {
            ad::cout << "Downloading: " << name << ad::endl;
            if (download_file(url, local_path)) {
                ad::cout << "  -> OK" << ad::endl;
            } else {
                ad::cerr << "  -> Failed!" << ad::endl;
            }
        }
    }
    // 4. 记录同步时间
    ad::fs::write_all(www + "/.last_sync", std::to_string(time(nullptr)));
    ad::cout << "Sync completed!" << ad::endl;
    return true;
}

inline bool sync_web_assets(const std::string& www, const std::string& manifest_url) {
    std::string manifest_path = www + "/manifest.json";
    return sync_web_assets(www, manifest_url, manifest_path);
}
