#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>

using std::cout;
using std::endl;

int main(const int argc, const char * argv []) {
    char * pwd = nullptr;
    if ((pwd = get_current_dir_name()) != nullptr) {
        cout << pwd << endl;
        free(pwd);
        return EXIT_SUCCESS;
    } else {
        perror(argv[0]);
        return EXIT_FAILURE;
    } // if
} // main
