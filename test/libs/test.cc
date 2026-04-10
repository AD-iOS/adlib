/*
c++ -L. -lad test.cc -o test && ldid -S$jb/opt/ad/share/ens/ens.xml -Hsha256 -Hsha1 -M test
*/

# define ad_io
# include <AD/ad.h>

int main() {
    ad::cout << "hello, this is ad dev lib cout" << ad::endl;
    return 0;
}