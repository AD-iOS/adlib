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
 *  AD_output.hpp
 *  Created by AD on 16/11/25
 *  Copyright (c) 2025-2026 AD All rights reserved.
 */
/*
 * by AD in 21:27/11/1/26 add std::fixed、std::setprecision Hold out
 * by AD in 21:26/11/1/26 repaired some error
**/

#ifndef _AD_output_HPP_
#define _AD_output_HPP_

#include <string>
#include <cstdio>
#if defined(_WIN32) || defined(_WIN64)
    #include <io.h>
    #define write _write
#else
    #include <unistd.h>
#endif
#include <limits>
#include <cfloat>
#include <cmath>
#include <cstdarg>

namespace AD {

class output_format {
private:
    int _precision;
    int _width;
    int _flags;
    char _fill;
    
public:
    // 格式标志位
    inline static const int fixed      = 0x01;
    inline static const int scientific = 0x02;
    inline static const int hexfloat   = 0x04;
    inline static const int showpoint  = 0x08;
    inline static const int showpos    = 0x10;
    inline static const int uppercase  = 0x20;
    inline static const int boolalpha  = 0x40;
    inline static const int showbase   = 0x80;
    inline static const int hex        = 0x100;
    inline static const int oct        = 0x200;
    inline static const int dec        = 0x400;

    int get_base() const {
        if (_flags & hex) return 16;
        if (_flags & oct) return 8;
        return 10;
    }

    output_format() : 
        _precision(6), 
        _width(0), 
        _flags(0), 
        _fill(' ') 
    {}
    
    void set_precision(int p) { _precision = p; }
    int get_precision() const { return _precision; }
    
    void set_width(int w) { _width = w; }
    int get_width() const { return _width; }
    
    void set_fill(char c) { _fill = c; }
    char get_fill() const { return _fill; }
    
    void setf(int flags) { _flags |= flags; }
    void unsetf(int flags) { _flags &= ~flags; }
    int flags() const { return _flags; }
    
    bool is_fixed() const { return (_flags & fixed) != 0; }
    bool is_scientific() const { return (_flags & scientific) != 0; }
    bool is_hexfloat() const { return (_flags & hexfloat) != 0; }
    bool is_showpoint() const { return (_flags & showpoint) != 0; }
    bool is_showpos() const { return (_flags & showpos) != 0; }
    bool is_uppercase() const { return (_flags & uppercase) != 0; }
    bool is_boolalpha() const { return (_flags & boolalpha) != 0; }
    bool is_showbase() const { return (_flags & showbase) != 0; }
};

// 2. 修改 basic_ostream 类,添加格式控制
class basic_ostream {
protected:
    static const unsigned int BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];
    unsigned int pos = 0;
    bool valid = true;
    int fd;
    
    // 添加格式控制成员
    output_format format;
    
    basic_ostream(int file_descriptor) : fd(file_descriptor) {}
    
    void flush_impl() {
        if (pos > 0 && valid) {
            unsigned long count = pos;
            #if defined(__aarch64__) || defined(__arm64__) || defined(__APPLE__)
                __asm__ __volatile__(
                    "mov x0, %0\n"
                    "mov x1, %1\n"
                    "mov x2, %2\n"
                    "mov x16, #4\n"
                    "svc #0x80\n"
                    :
                    : "r"((long)fd), "r"(buffer), "r"(count)
                    : "x0", "x1", "x2", "x16", "memory", "cc"
                );
            #else
                ssize_t result = write(fd, buffer, count);
                // ssize_t result = write(fd, buffer, count);
                (void)result;
            #endif
            pos = 0;
        }
    }
    
    void direct_write(const char* str, unsigned int len) {
        unsigned long length = len;
        #if defined(__aarch64__) || defined(__arm64__) || defined(__APPLE__)
            __asm__ __volatile__(
                "mov x0, %0\n"
                "mov x1, %1\n"
                "mov x2, %2\n"
                "mov x16, #4\n"
                "svc #0x80\n"
                :
                : "r"((long)fd), "r"(str), "r"(length)
                : "x0", "x1", "x2", "x16", "memory", "cc"
            );
        #else
            ssize_t result = write(fd, str, len); // 4
            // ssize_t result = write(fd, buffer, count);
            (void)result;
        #endif
    }
    
    // 辅助函数: 在缓冲前添加填充字符
    void add_padding(int width, int actual_len) {
        if (width > actual_len) {
            int padding = width - actual_len;
            for (int i = 0; i < padding; ++i) {
                if (pos >= BUFFER_SIZE) {
                    flush_impl();
                }
                buffer[pos++] = format.get_fill();
            }
        }
    }
    
    // 新的无符号整数输出(带格式控制)
    basic_ostream& output_unsigned(unsigned long long value, int base = 10) {
        char temp[64];  // 足够大以容纳最大整数
        char* p = temp + 63;
        *p = '\0';
        
        const char* digits = format.is_uppercase() ? "0123456789ABCDEF" : "0123456789abcdef";
        
        if (format.is_showbase()) {
            if (base == 16) {
                *--p = format.is_uppercase() ? 'X' : 'x';
                *--p = '0';
            } else if (base == 8) {
                *--p = '0';
            }
        }
        
        do {
            *--p = digits[value % base];
            value /= base;
        } while (value > 0);
        
        // 处理宽度和填充
        int len = (temp + 63) - p;
        add_padding(format.get_width(), len);
        
        return *this << p;
    }
    
    // 新的有符号整数输出
    basic_ostream& output_signed(long long value) {
        if (value < 0) {
            *this << '-';
            if (value == (-9223372036854775807LL - 1)) {
                return output_unsigned(9223372036854775808ULL, 10);
            }
            return output_unsigned(-value, 10);
        } else {
            if (format.is_showpos()) {
                *this << '+';
            }
            return output_unsigned(value, 10);
        }
    }
    
    // 检查浮点数特殊值(增强版)
    const char* check_special_double(double value) {
        if (value != value) {
            return format.is_uppercase() ? "NAN" : "nan";
        }
        
        unsigned long long bits = *(unsigned long long*)&value;
        if ((bits & 0x7FFFFFFFFFFFFFFFULL) == 0x7FF0000000000000ULL) {
            if (bits & 0x8000000000000000ULL) {
                return format.is_uppercase() ? "-INF" : "-inf";
            } else {
                return format.is_uppercase() ? "INF" : "inf";
            }
        }
        
        return nullptr;
    }

    basic_ostream& output_fixed(double value, int precision) {
        const char* special = check_special_double(value);
        if (special) return *this << special;
        bool is_negative = value < 0;
        if (is_negative) {
            *this << "-";
            value = -value;
        } else if (format.is_showpos()) {
            *this << "+";
        }
        if (precision > 0) {
            double round_factor = 0.5;
            for (int i = 0; i < precision; ++i) {
                round_factor /= 10.0;
            }
            value += round_factor;
        }
        unsigned long long int_part = (unsigned long long)value;
        double fractional = value - int_part;
        *this << int_part;
        if (precision > 0 || format.is_showpoint()) {
            *this << ".";
            double frac = fractional;
            for (int i = 0; i < precision; ++i) {
                frac *= 10.0;
                int digit = (int)frac;
                if (digit >= 10) {
                    digit = 9;
                }
                *this << (char)('0' + digit);
                frac -= digit;
                if (frac < 1e-12) {
                    for (int j = i + 1; j < precision; ++j) {
                        *this << "0";
                    }
                    break;
                }
            }
        }
        return *this;
    }

    // 完整的科学计数法输出
    basic_ostream& output_scientific(double value, int precision) {
        if (value == 0.0) {
            *this << "0";
            if (precision > 0 || format.is_showpoint()) {
                *this << ".";
                for (int i = 0; i < precision; ++i) {
                    *this << "0";
                }
            }
            *this << (format.is_uppercase() ? "E+00" : "e+00");
            return *this;
        }
        
        int exponent = 0;
        double abs_value = std::fabs(value);
        
        // 规范化: 1.0 <= value < 10.0
        while (abs_value >= 10.0) {
            abs_value /= 10.0;
            exponent++;
        }
        while (abs_value < 1.0 && abs_value > 0.0) {
            abs_value *= 10.0;
            exponent--;
        }
        
        if (value < 0) {
            *this << "-";
            value = -value;
        } else if (format.is_showpos()) {
            *this << "+";
        }
        
        // 输出尾数
        unsigned long long int_part = (unsigned long long)abs_value;
        double fractional = abs_value - int_part;
        
        *this << int_part;
        
        if (precision > 0 || format.is_showpoint()) {
            *this << ".";
            
            double frac = fractional;
            for (int i = 0; i < precision; ++i) {
                frac *= 10.0;
                int digit = (int)frac;
                *this << (char)('0' + digit);
                frac -= digit;
                
                if (frac < 1e-12) {
                    for (int j = i + 1; j < precision; ++j) {
                        *this << "0";
                    }
                    break;
                }
            }
        }
        
        // 输出指数
        *this << (format.is_uppercase() ? "E" : "e");
        if (exponent >= 0) {
            *this << "+";
        } else {
            *this << "-";
            exponent = -exponent;
        }
        // 2
        if (exponent < 10) {
            *this << "0";
        }
        *this << exponent;
        
        return *this;
    }
    
    // 新的浮点数输出(支持多种格式)
    basic_ostream& output_double(double value) {
        const char* special = check_special_double(value);
        if (special) {
            return *this << special;
        }
        
        int precision = format.get_precision();
        
        if (format.is_hexfloat()) {
            // 十六进制浮点数格式
            return *this << "hexfloat not implemented yet";
        } else if (format.is_scientific()) {
            // 科学计数法
            return output_scientific(value, precision);
        } else if (format.is_fixed()) {
            return output_fixed(value, precision);
        } else {
            // 默认格式: 自动选择定点或科学计数法
            double abs_value = std::fabs(value);
            if (abs_value == 0.0 || 
                (abs_value >= 0.0001 && abs_value < 1e6)) {
                // 使用定点表示
                output_format saved = format;
                format.setf(output_format::fixed);
                output_double(value);
                format = saved;
            } else {
                // 使用科学计数法
                output_format saved = format;
                format.setf(output_format::scientific);
                output_double(value);
                format = saved;
            }
            return *this;
        }
    }
    
    basic_ostream& output_float(float value) {
        return output_double((double)value);
    }
    
public:
    ~basic_ostream() {
        if (valid) {
            flush_impl();
            valid = false;
        }
    }
    
    // 获取格式控制对象
    output_format& fmt() { return format; }
    const output_format& fmt() const { return format; }
    
    // 设置精度
    basic_ostream& precision(int p) {
        format.set_precision(p);
        return *this;
    }
    
    // 设置宽度
    basic_ostream& width(int w) {
        format.set_width(w);
        return *this;
    }
    
    // 设置填充字符
    basic_ostream& fill(char c) {
        format.set_fill(c);
        return *this;
    }
    
    // 设置格式标志
    basic_ostream& setf(int flags) {
        format.setf(flags);
        return *this;
    }
    
    basic_ostream& unsetf(int flags) {
        format.unsetf(flags);
        return *this;
    }
    
    basic_ostream& flush() {
        flush_impl();
        return *this;
    }
    
    basic_ostream& printf(const char* format, ...) {
        if (!valid) return *this;
        va_list args;
        va_start(args, format);
        va_list args_copy;
        va_copy(args_copy, args);
        int needed = vsnprintf(nullptr, 0, format, args_copy) + 1;
    va_end(args_copy);
        if (needed > 0) {
            char* temp_buffer = new char[needed];
            int len = vsnprintf(temp_buffer, needed, format, args);
            if (len > 0) {
                if ((unsigned int)len >= BUFFER_SIZE) {
                direct_write(temp_buffer, len);
                } else {
                    *this << temp_buffer;
                }
            }
            delete[] temp_buffer;
        }
        va_end(args);
        return *this;
    }
    
    template<typename... Args>
    basic_ostream& print(const char* format, Args... args) {
        return printf(format, args...);
    }
    
    basic_ostream& operator<<(const char* str) {
        if (!valid) return *this;
        
        unsigned int len = 0;
        while (str[len]) len++;
        
        if (len == 0) return *this;
        
        // 处理宽度
        add_padding(format.get_width(), len);
        
        if (len >= BUFFER_SIZE) {
            direct_write(str, len);
            return *this;
        }
        
        for (unsigned int i = 0; i < len; ++i) {
            if (pos >= BUFFER_SIZE) {
                flush_impl();
            }
            buffer[pos++] = str[i];
        }
        
        return *this;
    }
    
    basic_ostream& operator<<(char c) {
        if (!valid) return *this;
        
        // 处理宽度(单字符)
        add_padding(format.get_width(), 1);
        
        if (pos >= BUFFER_SIZE) {
            flush_impl();
        }
        buffer[pos++] = c;
        return *this;
    }
    
    // 布尔类型(支持 boolalpha)
    basic_ostream& operator<<(bool value) {
        if (format.is_boolalpha()) {
            return *this << (value ? 
                (format.is_uppercase() ? "TRUE" : "true") : 
                (format.is_uppercase() ? "FALSE" : "false"));
        } else {
            return *this << (value ? "1" : "0");
        }
    }
    
    // 整数类型
    basic_ostream& operator<<(short value) { 
        int base = format.get_base();
        if (base != 10) {
            return output_unsigned((unsigned short)value, base);
        }
        return output_signed(value); 
    }
    
    basic_ostream& operator<<(int value) {
        int base = format.get_base();
        if (base != 10) {
            return output_unsigned((unsigned int)value, base);
        }
        return output_signed(value);
    }
    // 5
    basic_ostream& operator<<(long value) { 
        int base = format.get_base();
        if (base != 10) {
            return output_unsigned((unsigned long)value, base);
        }
        return output_signed(value); 
    }
    
    basic_ostream& operator<<(long long value) { // 6
        int base = format.get_base();
        if (base != 10) {
            return output_unsigned((unsigned long long)value, base);
        }
        return output_signed(value); 
    }
    
    // 无符号整数类型
    basic_ostream& operator<<(unsigned short value) { 
        return output_unsigned(value, format.get_base()); 
    }
    
    basic_ostream& operator<<(unsigned int value) { 
        return output_unsigned(value, format.get_base());
    }
    
    basic_ostream& operator<<(unsigned long value) { 
        return output_unsigned(value, format.get_base());
    }
    
    basic_ostream& operator<<(unsigned long long value) {
        return output_unsigned(value, format.get_base());
    }
    
    // 浮点类型
    basic_ostream& operator<<(float value) { 
        return output_float(value); 
    }
    
    basic_ostream& operator<<(double value) { 
        return output_double(value); 
    }
    
    // 指针类型
    basic_ostream& operator<<(const void* ptr) {
        unsigned long value = (unsigned long)ptr;
        char temp[20];
        char* p = temp + 19;
        *p = '\0';
        
        if (value == 0) {
            return *this << "nullptr";
        }
        
        do {
            int digit = value & 0xF;
            *--p = digit < 10 ? '0' + digit : 
                  (format.is_uppercase() ? 'A' + (digit - 10) : 'a' + (digit - 10));
            value >>= 4;
        } while (value > 0);
        
        *--p = format.is_uppercase() ? 'X' : 'x';
        *--p = '0';
        
        return *this << p;
    }
    
    basic_ostream& operator<<(const std::string& str) {
        return *this << str.c_str();
    }
    
    // 流操作符重载(用于格式控制对象)
    basic_ostream& operator<<(basic_ostream& (*manip)(basic_ostream&)) {
        return manip(*this);
    }
};

// 定义 iomanip 操作符
class setprecision {
private:
    int _precision;
public:
    explicit setprecision(int p) : _precision(p) {}
    friend basic_ostream& operator<<(basic_ostream& os, const setprecision& manip) {
        os.precision(manip._precision);
        return os;
    }
};

class setw {
private:
    int _width;
public:
    explicit setw(int w) : _width(w) {}
    friend basic_ostream& operator<<(basic_ostream& os, const setw& manip) {
        os.width(manip._width);
        return os;
    }
};

class setfill {
private:
    char _fill;
public:
    explicit setfill(char c) : _fill(c) {}
    friend basic_ostream& operator<<(basic_ostream& os, const setfill& manip) {
        os.fill(manip._fill);
        return os;
    }
};

// 4. 定义格式标志操作符(类似 std::fixed, std::scientific 等)
inline basic_ostream& fixed(basic_ostream& os) {
    os.unsetf(output_format::scientific | output_format::hexfloat);
    os.setf(output_format::fixed);
    return os;
}

inline basic_ostream& scientific(basic_ostream& os) {
    os.unsetf(output_format::fixed | output_format::hexfloat);
    os.setf(output_format::scientific);
    return os;
}

inline basic_ostream& hexfloat(basic_ostream& os) {
    os.unsetf(output_format::fixed | output_format::scientific);
    os.setf(output_format::hexfloat);
    return os;
}

inline basic_ostream& defaultfloat(basic_ostream& os) {
    os.unsetf(output_format::fixed | output_format::scientific | output_format::hexfloat);
    return os;
}

inline basic_ostream& showpoint(basic_ostream& os) {
    os.setf(output_format::showpoint);
    return os;
}

inline basic_ostream& noshowpoint(basic_ostream& os) {
    os.unsetf(output_format::showpoint);
    return os;
}

inline basic_ostream& showpos(basic_ostream& os) {
    os.setf(output_format::showpos);
    return os;
}

inline basic_ostream& noshowpos(basic_ostream& os) {
    os.unsetf(output_format::showpos);
    return os;
}

inline basic_ostream& uppercase(basic_ostream& os) {
    os.setf(output_format::uppercase);
    return os;
}

inline basic_ostream& nouppercase(basic_ostream& os) {
    os.unsetf(output_format::uppercase);
    return os;
}

inline basic_ostream& boolalpha(basic_ostream& os) {
    os.setf(output_format::boolalpha);
    return os;
}

inline basic_ostream& noboolalpha(basic_ostream& os) {
    os.unsetf(output_format::boolalpha);
    return os;
}

inline basic_ostream& showbase(basic_ostream& os) {
    os.setf(output_format::showbase);
    return os;
}

inline basic_ostream& noshowbase(basic_ostream& os) {
    os.unsetf(output_format::showbase);
    return os;
}

inline basic_ostream& hex(basic_ostream& os) {
    os.setf(0x100);  // 十六进制标志
    os.unsetf(0x200); // 清除八进制标志
    return os;
}

inline basic_ostream& oct(basic_ostream& os) {
    os.setf(0x200);  // 八进制标志
    os.unsetf(0x100); // 清除十六进制标志
    return os;
}

inline basic_ostream& dec(basic_ostream& os) {
    os.unsetf(0x100 | 0x200); // 清除进制标志
    return os;
}

// 全局函数(类似 std::setprecision)
/*
 * already discarded
 * It will cycle itself infinitely.(possibly)
 *
 * inline setprecision setprecision(int p) {
 *     return setprecision(p);
 * }
 * 
 * inline setw setw(int w) {
 *     return setw(w);
 * }
 * 
 * inline setfill setfill(char c) {
 *     return setfill(c);
 * }
 * 
**/

inline ::AD::setprecision setprecision(int p) {
    return ::AD::setprecision(p);
}

inline ::AD::setw setw(int w) {
    return ::AD::setw(w);
}

inline ::AD::setfill setfill(char c) {
    return ::AD::setfill(c);
}

// 標準輸出類
class cout_t : public basic_ostream {
public:
    cout_t() : basic_ostream(1) {}  // stdout 文件描述符為 1
};

// 標準錯誤類
class cerr_t : public basic_ostream {
public:
    cerr_t() : basic_ostream(2) {}  // stderr 文件描述符為 2
};

class endl_t {
public:
    friend basic_ostream& operator<<(basic_ostream& os, const endl_t&) {
        os << '\n';
        os.flush();
        return os;
    }
};

inline cout_t cout;
inline cerr_t cerr;
inline endl_t endl;

inline endl_t& _endl = endl;
inline cout_t& _cout = cout;
inline cerr_t& _cerr = cerr;

template<typename... Args>
inline basic_ostream& printf(const char* format, Args... args) {
    return cout.print(format, args...);
}

template<typename... Args>
inline basic_ostream& printf_err(const char* format, Args... args) {
    return cerr.print(format, args...);
}

template<typename... Args>
inline basic_ostream& perrm(const char* format, Args... args) {
    return cerr.print(format, args...);
}

// for setprecision、setw、setfill, function templates can be created
template<typename... Args>
inline auto setprecision(Args&&... args) {
    return AD::setprecision(std::forward<Args>(args)...);
}

template<typename... Args>
inline auto setw(Args&&... args) {
    return AD::setw(std::forward<Args>(args)...);
}

template<typename... Args>
inline auto setfill(Args&&... args) {
    return AD::setfill(std::forward<Args>(args)...);
}

} // namespace AD

#endif
