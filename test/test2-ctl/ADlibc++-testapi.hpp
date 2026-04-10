// ADlibc++-testapi.hpp

# include <limits.h>

namespace AD {
    namespace aux {
        std::string resolve_path(const std::string& path) {
            char resolved_path[PATH_MAX];
            if (realpath(path.c_str(), resolved_path) != nullptr) {
                return std::string(resolved_path);
            }
            return path;  // 解析失敗返回原路徑
        }
        constexpr auto& realpath = resolve_path;
    } // namespace aux
} // namespace AD
