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
 * adlibcxx-makeupforit.hpp
 * AD‘s C++ private standard library
 * Created by AD on 10/3/26
 * Copyright (c) 2025-2026 AD All rights reserved.
 */
/*
 * ~~~ the undisclosed expansion test function of the ad dev library ~~~
 * this is the complete library of the AD library (extra library)
**/

# ifndef AD_LIBCXX_MAKEUPFORIT_HPP
# define AD_LIBCXX_MAKEUPFORIT_HPP

#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <memory>
#include <iostream>
#include <cstring>

#include "fix_std.hpp"

#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>


namespace AD {
    namespace user {
        // get user uid
        inline uid_t current_uid() { return getuid(); }

        int get_user_uid() { return current_uid(); }

        constexpr auto& getuseruid = get_user_uid;
    } /* namespace user */

    // aux::ad_exist and is_dir and is_file
    // return 0, it exists
    // return 1, it doesn't exist
    namespace aux {
        int exist(const char *path) {
            struct stat st;
            return (stat(path, &st) != 0); /* 0 if exists, 1 if not exists */
        }

        int is_dir(const char *path) {
            struct stat st;
            if (stat(path, &st) != 0) { return 1; /* doesn't exist or error */ }
            return (S_ISDIR(st.st_mode) ? 0 : 1);  /* 0 if is directory, 1 if not */
        }

        int is_file(const char *path) {
            struct stat st;
            if (stat(path, &st) != 0) { return 1; /* doesn't exist or error */ }
            return (S_ISREG(st.st_mode) ? 0 : 1);  // 0 if is file, 1 if not
        }

        constexpr auto& path_exist = exist;
    } /* namespace aux */

    namespace auxiliary = aux;
} /* namespace AD */
#endif /* AD_LIBCXX_MAKEUPFORIT_HPP */
