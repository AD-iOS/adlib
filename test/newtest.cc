
#define ad_io
#define ad_func

#include <AD/ad.h>
#include <iostream>

static int counter = 0;

struct InitTracker {
    InitTracker() {
        std::cout << "InitTracker constructor called" << std::endl;
        counter++;
    }
};

static InitTracker tracker;

adfunc (testfunc)() -> void {
    std::cout << "testfunc lambda body executed" << std::endl;
    ad::printf("hello, world!\n");
};

int main() {
    std::cout << "main() called, counter = " << counter << std::endl;
    adfunc_call(testfunc, void());
    return 0;
}