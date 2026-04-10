/*
c++ test.cc -o test
*/

# define ad_comuse
# define load_ad_system
# include <AD/ad.h>

std::string get_jbarch() {
    return ad::system("printf \"%s\" $(dpkg --print-architecture) || echo 'unknown'", 1);
}

int main() {
    ad::cout << get_jbarch() << ad::endl;
    return 0;
}
