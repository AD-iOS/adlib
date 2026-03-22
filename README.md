# adlib

> author: AD <ad-ios334@outlook.com>    
> maintenance: AD <ad-ios334@outlook.com>    
> version: 1.0

### AD 的 C++庫
AD's C++ dev library, 簡稱 ADCDL

## 修改
- 標準庫命名空間修改:
ADCDL 修改了 std 命名空間(具體請看 fix_std.hpp)
如果不使用 ADCDL 不推薦引入 fix_std.hpp 頭文檔, 除非你有特殊需求
fix_std.hpp 主要添加了 std::fs 即在 std 命名空間中添加了 fs
並且為了確保 clang14 (可能包括以下) 編譯器可以使用 std::filesystem, 我們給 std 命名空間中添加了以下程式碼(包括添加的 fs 程式碼)
如果你沒有特殊用途應該使用 std::filesystem 或者 namespace fs = std::filesystem 而不是使用 std::fs
```c++
#ifdef _LIBCPP_VERSION
    namespace fs = __fs::filesystem;
    namespace filesystem = __fs::filesystem;
#else
    namespace fs = filesystem;
#endif
```

- 提供
庫提供了 mkdir, touch, mkfile(touch 的別名), read, ... , archive 等等
如果使用 archive api 請安裝[libarchive](https://github.com/libarchive/libarchive)庫

## 其他
如果無法從`ad-ios334@outlook.com`電子郵件聯繫上我(AD)請嘗試以下電子郵件
<br>
**ad-ios335@outlook.com**
<br>
**ad-ios336@outlook.com**
<br>
**ad-ios337@outlook.com**

---

