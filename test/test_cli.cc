#define ad_cli
#include <iostream>
#include <AD/ad.h>
#include <cstdio>

int main(int argc, char* argv[]) {
    AD::cli::init(argc, argv);
    AD::cli::getinput(AD::cli::arg() <= 2);
    if (AD::cli::arg() == "--help" || AD::cli::arg() == "-h") {
        std::string help_text = "usage: %s [option] [command]\n"
        "  --help  // show help\n"
        "  --version  // show version\n";
        printf(help_text.c_str(), argv[0]);
    } else if (AD::cli::arg() == "--version" || AD::cli::arg() == "-v") {
        printf("%s: version 0.0.1", argv[0]);
    } else {
        std::cerr << "E: unknown option and command, use --help, check help" << std::endl;
    }
    
    return 0;
}
