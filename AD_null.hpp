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
 * AD_null.hpp
 * AD‘s C++ private standard library
 * Created by AD on 5/11/25
 * Copyright (c) 2025-2026 AD All rights reserved.
 *
**/

#ifndef _AD_NULL_HPP_
#define _AD_NULL_HPP_

#include <iostream>
#include <fstream>
#include <string>

namespace AD {

/**
 * @Brief RAII class is used to temporarily redirect std::cout or std::cout/std::cerr to /dev/null
 *
 * The constructor function executes redirection, and the deconstructor function restores the original stream.
 * Ensure automatic recovery at the end of the scope
**/

class AD_null {
public:

    explicit AD_null(bool redirect_cerr = false)
        : redirect_cerr_(redirect_cerr),
          old_cout_buf_(nullptr),
          old_cerr_buf_(nullptr),
          is_redirected_(false)
    {
        /* Select the path of /dev/null according to the operating system. */

#ifdef _WIN32
        const std::string dev_null_path = "NUL";
#else
        const std::string dev_null_path = "/dev/null";
#endif

        dev_null_file_.open(dev_null_path);
        if (dev_null_file_.is_open()) {
            old_cout_buf_ = std::cout.rdbuf();
            std::cout.rdbuf(dev_null_file_.rdbuf());

            if (redirect_cerr_) {
                old_cerr_buf_ = std::cerr.rdbuf();
                std::cerr.rdbuf(dev_null_file_.rdbuf());
            }
            is_redirected_ = true;
        } else {
            std::cerr << "Warning: Failed to open " << dev_null_path << " for redirection. Output will not be suppressed." << std::endl;
        }
    }

    ~AD_null() {
        if (is_redirected_) {
            std::cout.rdbuf(old_cout_buf_);
            if (redirect_cerr_) {
                std::cerr.rdbuf(old_cerr_buf_);
            }
            dev_null_file_.close();
        }
    }

    AD_null(const AD_null&) = delete;
    AD_null& operator=(const AD_null&) = delete;
    AD_null(AD_null&&) = delete;
    AD_null& operator=(AD_null&&) = delete;

private:
    bool redirect_cerr_;
    std::streambuf* old_cout_buf_;
    std::streambuf* old_cerr_buf_;
    std::ofstream dev_null_file_;
    bool is_redirected_;
};

} /* namespace AD */

#endif // AD_NULL_HPP_