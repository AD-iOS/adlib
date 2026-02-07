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
 *  AD_func.hpp
 *  Created by AD on 25/11/25
 *  Copyright (c) 2025-2026 AD All rights reserved.
**/

#ifndef _AD_FUNC_HPP_
#define _AD_FUNC_HPP_

#include <iostream>
#include <functional>
#include <unordered_map>
#include <string>
#include <any>
#include <typeindex>
#include <type_traits>
#include <vector>

namespace AD {

template<typename T> struct func_traits;

template<typename R, typename... Args>
struct func_traits<R(Args...)> {
    using return_type = R;
    using func_type = std::function<R(Args...)>;
    static constexpr const char* name = "function";
};

class DynamicSystem {
private:
    static std::unordered_map<std::string, std::any>& registry() {
        static std::unordered_map<std::string, std::any> instance;
        return instance;
    }

    static std::unordered_map<std::string, std::string>& debug_names() {
        static std::unordered_map<std::string, std::string> instance;
        return instance;
    }

public:
    template<typename F>
    static bool define(const std::string& name, F lambda) {
        try {
            auto func_wrapper = std::function(lambda); 
            registry()[name] = std::any(func_wrapper);
            debug_names()[name] = typeid(func_wrapper).name();
            return true;
        } catch (const std::exception& e) {
            std::cerr << "[Error] Failed to register function '" << name << "': " << e.what() << std::endl;
            return false;
        }
    }

    template<typename Sig, typename... CallArgs>
    static typename func_traits<Sig>::return_type invoke(const std::string& name, CallArgs&&... args) {
        using R = typename func_traits<Sig>::return_type;
        using FuncType = typename func_traits<Sig>::func_type;

        auto& reg = registry();
        auto it = reg.find(name);
        
        if (it == reg.end()) {
            std::cerr << "[Error] Function '" << name << "' not found!" << std::endl;
            if constexpr (!std::is_void_v<R>) return R{};
            else return;
        }

        try {
            auto f = std::any_cast<FuncType>(it->second);
            return f(std::forward<CallArgs>(args)...);
        } catch (const std::bad_any_cast& e) {
            std::cerr << "[Fatal] Type mismatch for '" << name << "'\n";
            std::cerr << "  Expected: " << typeid(FuncType).name() << "\n";
            std::cerr << "  Stored:   " << debug_names()[name] << "\n";
            if constexpr (!std::is_void_v<R>) return R{};
        }
    }
    
    static bool exists(const std::string& name) {
        return registry().find(name) != registry().end();
    }
    
    static std::vector<std::string> list_functions() {
        std::vector<std::string> names;
        names.reserve(registry().size());
        for (const auto& [name, _] : registry()) {
            names.push_back(name);
        }
        return names;
    }
    
    static void clear() {
        registry().clear();
        debug_names().clear();
    }
};

struct AutoRegHelper {
    std::string name;
    AutoRegHelper(const char* n) : name(n) {}

    template <typename F>
    int operator=(F&& f) {
        AD::DynamicSystem::define(name, std::forward<F>(f));
        return 0;
    }
};

} // namespace AD

#define ad_func(name) static int _reg_##name = AD::AutoRegHelper(#name) = [] 

#define ad_call(name, Sig, ...) \
    AD::DynamicSystem::invoke<Sig>(#name, ##__VA_ARGS__)

#define ad_safe_call(name, Sig, default_val, ...) \
    ([&]() -> typename AD::func_traits<Sig>::return_type { \
        if (AD::DynamicSystem::exists(#name)) { \
            try { \
                return AD::DynamicSystem::invoke<Sig>(#name, ##__VA_ARGS__); \
            } catch (...) { \
                std::cerr << "[Warn] Call failed for " << #name << ", using default.\n"; \
                return default_val; \
            } \
        } \
        std::cerr << "[Warn] Function " << #name << " not found, using default.\n"; \
        return default_val; \
    }())

#endif