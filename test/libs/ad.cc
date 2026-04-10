// this is .so file
/*
c++ -fobjc-arc -fPIC -shared \
    -framework UIKit -framework WebKit -framework JavaScriptCore -framework CoreGraphics \
    -lroothide -lboost_json \
    -lssl -lcrypto -lz -larchive \
    -lnghttp2 -lssh2 -lrtmp -lzstd \
    -lbrotlidec -lbrotlicommon \
    -lcares -lidn2 -lunistring \
    -lcurl -llzma -liosexec \
    -Wl,-rpath,/usr/lib \
    -Wl,-rpath,/lib \
    -Wl,-rpath,/usr/local/lib \
    -Wl,-rpath,/var/jb/usr/lib \
    -Wl,-rpath,/var/jb/lib \
    -Wl,-rpath,/var/jb/usr/local/lib \
    -Wl,-rpath,/var/jb/opt/local/lib \
    -Wl,-rpath,/opt/local/lib \
    ad.cc -o libad.so.1 && ldid -S libad.so.1
--- 靜態:
c++ -fobjc-arc -fPIC -shared \
    -framework UIKit -framework WebKit -framework JavaScriptCore -framework CoreGraphics \
    -lroothide -liosexec -larchive -lz -lgnutls -lnettle -lresolv \
    $jb/usr/lib/libboost_json.a \
    $jb/usr/lib/libssl.a \
    $jb/usr/lib/libcrypto.a \
    $jb/usr/lib/liblz4.a \
    $jb/usr/lib/libnghttp2.a \
    $jb/usr/lib/libssh2.a \
    $jb/usr/lib/librtmp.a \
    $jb/usr/lib/libzstd.a \
    $jb/usr/lib/libbrotlidec.a \
    $jb/usr/lib/libbrotlicommon.a \
    $jb/usr/lib/libcares.a \
    $jb/usr/lib/libidn2.a \
    $jb/usr/lib/libcurl.a \
    $jb/usr/lib/liblzma.a \
    $jb/usr/lib/libgmp.a \
    $jb/usr/lib/libgmpxx.a \
    $jb/usr/lib/libintl.a \
    -Wl,-rpath,/usr/lib \
    -Wl,-rpath,/lib \
    -Wl,-rpath,/usr/local/lib \
    -Wl,-rpath,/var/jb/usr/lib \
    -Wl,-rpath,/var/jb/lib \
    -Wl,-rpath,/var/jb/usr/local/lib \
    -Wl,-rpath,/var/jb/opt/local/lib \
    -Wl,-rpath,/opt/local/lib \
    ad.cc -o libad.so.1 && ldid -S libad.so.1
: -- old option
    -Wl,-force_load,$jb/usr/lib/libunistring.a \
    -Wl,-mllvm,-allow-multiple-definition \
    -Wl,-undefined,dynamic_lookup \
*/

# define ad_all
# include <AD/ad.h>