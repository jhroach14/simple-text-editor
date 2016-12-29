#include <iostream>
#include "library.h"
#include <sys/stat.h>
#include <stdio.h>


using namespace std;
void fileToDir(int argc, char *argv[]);
void fileToFile(int argc, char *argv[]);
int main(int argc, char *argv[]) {
    char* last = argv[argc-1];
    struct stat fileStat;
    if(stat(last, &fileStat )==-1){
        exitProgram(strerror(errno));
    }

    if(S_ISDIR (fileStat.st_mode)){
        //cout<<"entered fileToDir"<<'\n';
        fileToDir(argc,argv);

    }else{
        //cout<<"entered file to file"<<'\n';
        fileToFile(argc,argv);
    }

    return 0;
}
void fileToDir(int argc, char *argv[]){
    char* files[100];
    string last= argv[argc-1];

    for(int i=0;i<100;i++){
        files[i] = (char *)"empty";
    }

    for(int i =1;i<argc-1;i++){
        files[i] = argv[i];
        //cout<<"found file "<<files[i]<<'\n';
    }

    for(int i=0;i<100;i++){
        if(strcmp(files[i],"empty") != 0){
            string path;
           // cout<<last.at(last.length()-1)<<'\n';
            if(last.at(last.length()-1)=='/'){
                 path = last + files[i];
            }else {
                 path = last + '/' + files[i];
            }
           // cout<<"constructed path "<<path<<'\n';
            if(rename(files[i],path.c_str())==-1){
                exitProgram(strerror(errno));
            }
        }
    }
}
void fileToFile(int argc, char *argv[]){
    if(rename(argv[1],argv[2])==-1){
        exitProgram(strerror(errno));
    }
}
