#include <iostream>
#include "library.h"

using namespace std;

int main(int argc, char *argv[]) {

    for(int i =1;i<argc;i++){

        int fd = open(argv[i],O_RDONLY);
        if(fd == -1){
            exitProgram(strerror(errno));
        }

        cout<<getFileContent(fd)<<'\n';
    }

    return 0;
}

