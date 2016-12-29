#include <iostream>
#include <getopt.h>
#include "library.h"
#include <sys/stat.h>
int pFlag=0;
int mode;
int mFlag=0;
int targetDepth =-1;

void noOpts(int argc,char *argv[]);

void parseOpts(int argc, char *argv[]);

void pOpt(int argc, char* argv[]);

string * parseDirs(char* dir);

using namespace std;

int main(int argc, char *argv[]) {

    parseOpts(argc,argv);

    if(pFlag==1){
        pOpt(argc,argv);
    }else{
        noOpts(argc,argv);
    }

    return 0;
}
//for when the p flag is set
void pOpt(int argc, char* argv[]){
    ////cout<<"Entered pOpt"<<'\n';
    char* dirs[100];
    for(int i=0;i<100;i++){
        dirs[i] = (char *) "empty";
    }
    for(int i=2+mFlag;i<argc;i++){
        dirs[i] = argv[i];
    }

    int depth=0;
    for(int i =0;i<100;i++){
        if(strcmp(dirs[i],"empty")!=0&&strcmp(dirs[i],"-p")!=0){
            string* seperatedDirs =parseDirs(dirs[i]);
            for(int j=0;j<100;j++){
                if(strcmp( seperatedDirs[j].c_str(),"empty")!=0){
                    //cout<<"found entry "<<seperatedDirs[j]<<" at index "<<j<<'\n';
                    //cout<<"making directory "<<seperatedDirs[j]<<'\n';
                    //cout<<"depth="<<depth<<" target depth="<<targetDepth<<'\n';
                    if(depth == targetDepth){
                        //cout<<"mode="<<mode<<'\n';
                        if(mkdir(seperatedDirs[j].c_str(), (mode_t) mode)==-1){
                            exitProgram(strerror(errno));
                        }
			chmod(seperatedDirs[j].c_str(),(mode_t) mode);
                    }else{
                        //cout<<"entering trans make"<<'\n';
                        //cout<<"mode=493"<<'\n';
                        if(mkdir(seperatedDirs[j].c_str(), 493)==-1){
                            exitProgram(strerror(errno));
                        }
                    }
                    chdir(seperatedDirs[j].c_str());
                    depth++;
                }
            }
            for(int j=0; j<depth;j++){
                chdir("..");
            }
            delete[] seperatedDirs;
        }
        depth=0;
        targetDepth=-1;
    }
}
//gets all the intermediate directories
string* parseDirs(char* dir){
    //cout<<"Entered parseDirs with argument "<<dir<<'\n';
    string* directories = new string[100];
    for(int i =0;i<100;i++){
        directories[i]= "empty";
    }
    int dirIndex = 0;
    unsigned int prev = 0;

    for(unsigned int i =0;i<strlen(dir);i++){
        char ch = dir[i];
        if(ch=='/'){
            directories[dirIndex]="";
            for(unsigned int j = prev;j<i;j++){
                directories[dirIndex]+=dir[j];
            }
            dirIndex++;
            targetDepth++;
            prev=i+1;
        }else
        if(i == (strlen(dir)-1)){
            directories[dirIndex]="";
            for(unsigned int j = prev;j<i+1;j++){
                directories[dirIndex]+=dir[j];
            }
            dirIndex++;
            targetDepth++;
            prev=i+1;
        }
    }
    /*for(int i =0;i<100;i++){
        if(strcmp(directories[i].c_str(),"empty")!=0) {
            //cout << "found Dir " << directories[i] << '\n';
        }
    }*/
    //cout<<"target depth"<<targetDepth<<'\n';
    return directories;
}
//for when there is no pflag
void noOpts(int argc,char *argv[]){
    for(int i =1+mFlag;i<argc;i++){
        if(mkdir(argv[i], (mode_t) mode)==-1){
            exitProgram(strerror(errno));
        }
    }
}
//to get the options specified on the command line
void parseOpts(int argc, char *argv[]){
    int opt;
    while((opt = getopt(argc, argv, "m:p")) != -1){
        switch(opt){
            case 'm':
                mode = baseConverter(optarg);
                mFlag =2;
                break;
            case 'p':
                pFlag = 1;
                break;
            default:
                exitProgram(strerror(errno));
        }
    }
    if(mFlag==0){
        mode = 493;
    }

}
