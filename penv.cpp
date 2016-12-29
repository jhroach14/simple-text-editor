#include <iostream>
#include <cstdlib>

using std::cout;
using std::endl;

int main(const int argc, const char * argv [], const char * envp []) {
    for (const char ** env = envp; *env != nullptr; ++env) {
        cout << *env << endl;
    } // for
    return EXIT_SUCCESS;
} // main