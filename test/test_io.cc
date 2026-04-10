#define ad_io
#define ad_func

#include <AD/ad.h>
// #include <iostream>

static int call_count = 0;

adfunc (testfunc)() -> void {
    call_count++;
    ad::cout << "testfunc called, count = " << call_count << ad::endl;
    ad::printf("hello, world!\n");
};

adfunc (add)(int a, int b) -> int {
    ad::cout << "add called" << ad::endl;
    return a + b;
};

int main() {
    ad::cout << "=== main start ===" << ad::endl;
    
    ad::cout << "Calling testfunc..." << ad::endl;
    adfunc_call(testfunc, void());
    
    ad::cout << "After testfunc" << ad::endl;
    ad::cout << "==== test ====" << ad::endl;
    
    ad::cout << "Calling add..." << ad::endl;
    int x = adfunc_call(add, int(int, int), 1, 2);
    ad::cout << "1 + 2 = " << x << ad::endl;
    
    ad::cout << "=== main end ===" << ad::endl;
    return 0;
}
