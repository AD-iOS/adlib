#ifndef _AD_OUTPUT_USE_STD_HPP_
#define _AD_OUTPUT_USE_STD_HPP_

#ifdef ad_io_use_std
    #include <iostream>
    #include <iomanip>
    #include <cstdio>
    #include <stdio.h>
    namespace AD {
        using std::cout;
        using std::cerr;
        using std::endl;
        using std::flush;
        using std::fixed;
        using std::scientific;
        using std::setprecision;
        using std::setw;
        using std::setfill;
        using std::printf;
      #if __cplusplus >= 202302L
        using std::print;
      #endif
        inline constexpr auto& perrm = perror;
    }
#endif // ad_io_use_std
#endif