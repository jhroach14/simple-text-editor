#include <iostream>
#include "library.h"

int sFlag=0;
void parseOpts(int, char *argv[]);
using namespace std;

int main(int argc, char *argv[]) {

    parseOpts(argc,argv);

    if(sFlag==0){
        link(argv[1],argv[2]);
    }else
    if(sFlag==1){
        symlink(argv[2],argv[3]);
    }

    return EXIT_SUCCESS;
}
//parses the opts. gotta love dat parsin
void parseOpts(int argc, char *argv[]){
    int opt;
    while((opt = getopt(argc, argv, "S")) != -1){
        switch(opt){
            case 'S':
                sFlag=1;
                break;
            default:
                exitProgram("unknown option");
        }
    }
}
