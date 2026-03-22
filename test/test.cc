// clang++ test.cc -o test1

// #define ad_io
#define ad_system
// #define _ad_cancel_std_system

#include <AD/ADlibc++.hpp>

// #define ad_fs

int main() {
    // AD::printf("hello\n");
    // AD::perrm("[Error]: usage xxx xxx\n");
    ad::system("echo 'hello'");
    return 0;
}