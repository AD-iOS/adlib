/*
 * test2 測試文件
 * test2.cc
 * test12
 * Created by AD on 20/12/25
 * Copyright (c) 2025 AD All rights reserved.
 * xcode++ -e ../../entitlements.plist -I ./ -I ../ test2.cc -o test12 sdk=17.5 2>1.txt
 */

#include "../ADlibc++.hpp"

int main() {
    AD::stopwatch sw("Total Task");
    AD::cout << "help test" << AD::endl;
    AD::fs::mkdir("test122");
    AD::fs::touch("test122/test.cc");
    AD::cout << "完成" << AD::endl;
    AD::sys::bash("echo 'hello'");
    // AD::fs::rm_safe("test122");
    
    return 0;
}