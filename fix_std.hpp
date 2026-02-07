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
 * fix_std_fs.hpp
 * Created by AD 21/1/26
 * Copyright (c) 2025-2026 AD All rights reserved.
 *
 */
/*
 * @provide: why std add/fix some func/namespace/..., to ensure,the largest compatibility
 */
/*
 * ====------------ fix ------------====
 * fix@change: change fs(std::filesystem) to std::fs
 * fix@change: remove fs define
 * fix@add: add namespace cc
 * fix@add: add add string to namespace cc
 * fix@change: ``std_system = ::system`` to std_system = std::system, add cstdlib include
**/

#ifndef _AD_STD_FIX_HPP__
#define _AD_STD_FIX_HPP__

#ifdef _ad_cc__
  #ifdef _ad_std_string_cc
    #include <string>
    #include <cstring>
  #endif
#endif

// C++ std alias
#ifdef _ad_cxx__
  #ifdef _ad_std_alias_cxx
  #endif
#endif

#ifdef _ad_cancel_std_system
  #include <cstdlib>
  static constexpr auto& std_system = std::system;
#endif

namespace std {
  #ifdef _LIBCPP_VERSION
    namespace fs = std::__fs::filesystem;
  #else
    namespace fs = std::filesystem;
  #endif
    // namespace fs = std::filesystem;
    #ifdef _ad_cc_
      namespace cc {
        #ifdef _ad_std_string_cc
          namespace string {
            inline int strcpy(...);
            inline int strcmp(...);            inline int strcat(...);            inline int strlen(...);            inline int strstr(...);            inline int memcpy(...);            inline int memset(...);
          } /* namespace cc::string */
          #endif
      } /* namespace cc */
      #endif
  #ifdef _ad_cancel_std_system
    inline int command(const std::string& cmd) {
        return std_system(cmd.c_str());
    }
  #endif
} /* namespace std */

#ifdef _ad_std_alias_cxx
  namespace cxx = std;
  namespace standard = std;
#endif /* _ad_std_alias_cxx */
#endif /* _AD_STD_FIX_HPP__ */

