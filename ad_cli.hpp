/*    AD-DEV Public General License
 *       Version 1.0.0, December 2026
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
 * ad_cli.hpp
 * AD's C++ private standard library
 * Created by AD on 20/3/26
 * Copyright (c) 2025-2026 AD All rights reserved.
 */

#ifndef AD_ARGS_AND_OPTS_HPP
#define AD_ARGS_AND_OPTS_HPP

#include <string>
#include <vector>
#include <iostream>
#include <cstring>
#include <optional>
#include <memory>
#include <cstdlib>

/* added by AD time: 2026-3-20:19:9 */
namespace AD {
    namespace cli {
        // 命令行參數類
        class Arg {
        private:
            int m_argc;
            std::vector<std::string> m_args;
        public:
            Arg(int argc, char* argv[]) : m_argc(argc) {
                for (int i = 0; i < argc; i++) {
                    m_args.push_back(std::string(argv[i]));
                }
            }

            int count() const { return m_argc; }
            std::string program_name() const { 
                return m_args.empty() ? "" : m_args[0]; 
            }

            std::string operator[](int index) const {
                if (index >= 0 && index < m_argc) {
                    return m_args[index];
                }
                return "";
            }
            // 實現 <= 操作符
            bool operator<=(int n) const { return m_argc <= n; }
            bool operator>=(int n) const { return m_argc >= n; }
            bool operator==(int n) const { return m_argc == n; }
            bool operator<(int n) const { return m_argc < n; }
            bool operator>(int n) const { return m_argc > n; }
            // 實現 == 操作符（用於匹配選項）
            bool operator==(const char* opt) const {
                for (size_t i = 1; i < m_args.size(); i++) {
                    if (m_args[i] == opt) {
                        return true;
                    }
                }
                return false;
            }

            bool operator==(const std::string& opt) const {
                return operator==(opt.c_str());
            }

            std::optional<std::string> value_of(const std::string& opt) const {
                for (size_t i = 1; i < m_args.size() - 1; i++) {
                    if (m_args[i] == opt) {
                        return m_args[i + 1];
                    }
                }
                return std::nullopt;
            }

            bool has(const std::string& opt) const {
                return operator==(opt);
            }
        };
        // CLI 管理類
        class CLIInput {
        private:
            Arg m_arg;
        public:
            CLIInput(int argc, char* argv[]) : m_arg(argc, argv) {}
            
            const Arg& arg() const { return m_arg; }
        };
        // 全局實例
        static std::unique_ptr<CLIInput> g_cli = nullptr;
        // 初始化
        inline void init(int argc, char* argv[]) { g_cli = std::make_unique<CLIInput>(argc, argv); }
        // 獲取 arg 對象
        inline const Arg& get_arg() {
            if (!g_cli) {
                throw std::runtime_error("CLI not initialized");
            }
            return g_cli->arg();
        }
        // 全局 arg 函數
        inline const Arg& arg() { return get_arg(); }

        constexpr auto& arg_impl = arg;

        # define arg arg_impl()
/*
        inline const Arg& arg_impl() {
            return get_arg();
        }
*/
        // getinput
        inline void __getinput(bool condition) {
            if (!condition) {
                std::cerr << "Error: argument count condition not satisfied" << std::endl;
              #ifdef ad_cli_auto_exit
                exit(1);
              #endif
            }
        }

        /* added by AD time: 2026-3-20:19:14 */
        // 無參數版本, 用於向後兼容
        inline void __getinput() { /* 什麼都不做 */ }
        /* added by AD time: 2026-3-22:13:20 */
        template<typename... Args>
        inline void require(Args&&... args) {
            __getinput(std::forward<Args>(args)...);
        }

        // inline constexpr auto& require = __getinput;
    } // namespace cli
} // namespace AD

#endif // AD_ARGS_AND_OPTS_HPP
