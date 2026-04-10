# adlib

> author: AD <ad-ios334@outlook.com>    
> maintenance: AD <ad-ios334@outlook.com>    
> version: 1.2

###### this is header-only dev lib
### AD 的 C++ 庫
AD's C++ dev library, 簡稱 `ADCDL` 庫 或 `AD` 庫  
下把本項目簡稱為`庫`  

## 修改  
- 標準庫命名空間修改:  
ADCDL 修改了 std 命名空間(具體請看 fix_std.hpp)  
如果不使用 ADCDL 不推薦引入 fix_std.hpp 頭文檔, 除非你有特殊需求  
fix_std.hpp 主要添加了 std::fs 即在 std 命名空間中添加了 fs  
並且為了確保 clang14(可能包括以下)編譯器可以使用 std::filesystem, 我們給 std 命名空間中添加了以下程式碼(包括添加的 fs 程式碼)  

```c++
#if defined(_LIBCPP_VERSION) && _LIBCPP_VERSION <= 14000
    namespace fs = __fs::filesystem;
    namespace filesystem = __fs::filesystem;
#else
    namespace fs = filesystem;
#endif
```

如果你沒有特殊用途的話, 你應該使用`std::filesystem`或者`namespace fs = std::filesystem`而不是使用`std::fs`庫是極其不推薦更不建議使用`std::fs`的    

## 提供
庫提供了 mkdir, touch, mkfile(touch 的別名), read, ... , archive 等等 api  

## 以下是依賴的程式庫
[libarchive](https://github.com/libarchive/libarchive)  
[curl](https://github.com/curl/curl)  
[openssl](https://github.com/openssl/openssl)  
[boost](https://github.com/boostorg/boost)  


## 其他
你可以在程式碼中定義`#define ad_io_use_std`, 這樣子`ad::cout ...`就等於`std::cout ...`  
但是庫不允許同時定義`#define ad_io_use_std`和`#define ad_output`或`#define ad_io`, 因為這會亂套(衝突)  
庫絕不推薦也絕不建議使用`using namespace AD;`否則極易導致衝突  
`std::fs`是庫內部使用的, 極其不建議也不應該使用它

## 聯繫
如果無法從`ad-ios334@outlook.com`電子郵件聯繫上我(AD)請嘗試以下電子郵件
<br>
**ad-ios335@outlook.com**
<br>
**ad-ios336@outlook.com**
<br>
**ad-ios337@outlook.com**

---

