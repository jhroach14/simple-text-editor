#include <iostream>
#include "library.h"
#include <unistd.h>
#include <stdio.h>

#include <ftw.h>

int checkPermissions(int mode);

void parseOpts(int argc, char *argv[]);
static int rm(const char *fpath, const struct stat *sb,int tflag, struct FTW *ftwbuf);

int rFlag=0;
using namespace std;

int main(int argc, char *argv[]) {
    parseOpts(argc,argv);
    char* files[100];
    int fileCount = 0;

    for(int i =1+rFlag;i<argc;i++){
        files[fileCount] = argv[i];
        fileCount++;
    }
    if(rFlag==0){
        for (int i = 0; i < fileCount; i++) {
            struct stat buf;
            stat(files[i], &buf);
            if (checkPermissions(buf.st_mode) == 0) {
                cerr << files[i] << " is write protected are you sure you wish to remove?(y/n)" << "\n";
                int response = getchar();
                switch (response) {
                    case 'y':
                        unlink(files[i]);
                        break;
                    case 'n':
                        break;
                    default:
                        break;
                }
            }else{
                unlink(files[i]);
            }
        }
    }else
    if(rFlag==1){
        for (int i = 0; i < fileCount; i++) {
            int flags =FTW_DEPTH;
            if (nftw(files[i], rm, 20, flags) == -1) {
                exitProgram(strerror(errno));
            }
        }
    }
    return 0;
}
static int rm(const char *fpath, const struct stat *sb,int tflag, struct FTW *ftwbuf) {
    if(tflag==FTW_F){
        unlink(fpath);
    }else
    if(tflag==FTW_DP){
        rmdir(fpath);
    }
    return 0;
}

int checkPermissions(int mode){
    int hasPermission=1;

    if(((mode>>7)&1)==0){
        hasPermission=0;
    }
    return hasPermission;
}
void parseOpts(int argc, char *argv[]){
    int opt;
    while((opt = getopt(argc, argv, "R")) != -1){
        switch(opt){
            case 'R':
                rFlag=1;
                break;
            default:
                exitProgram(strerror(errno));
        }
    }
}


