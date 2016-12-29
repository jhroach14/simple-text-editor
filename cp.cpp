#include <iostream>
#include <string>
#include <map>
#include <dirent.h>
#include "library.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>

void fileToFile(char* sourceName,char* targetName);

void fileToDir(char* sourceName,char* targetName);


void dirToDir(char* sourceName,char* targetName);

struct file{
    char* fileName;
    char* fileContent;
};
struct directory;
struct dirLevel{

    char* parentDir;
    int parentMode;
    file *files[100];
    directory *dirs[100];
};

struct directory{
    char* dirName;
    dirLevel level;
};

file* fileDump[100000];
directory* dirDump[100000];
char* charDump[100000];
int dumpIndex=0;
int charDumpindex=0;
dirLevel constructDirTree(char* target);


void writeDirTree(dirLevel level);
int slashFlag = 0;
int main(int argc, char *argv[]) {
//use stat to check if file or directory
    int rFlag = 0;
    char* sourceName;
    char* targetName;

    if(strcmp(argv[1],"-R")==0){
        rFlag=1;
    }
    if((argc)!=(3+rFlag)){
        exitProgram("number of arguments is wrong");
    }
    sourceName=argv[1+rFlag];
    targetName=argv[2+rFlag];
    if(strcmp(sourceName,targetName)==0){
        exitProgram("cannot copy file to itself");
    }
    struct stat buf;
    if(stat(targetName,&buf)==-1){
    	if(mkdir(targetName,493)==-1){
	   exitProgram(strerror(errno));
	}
	stat(targetName,&buf);
    }
    struct stat buf2;
    if(stat(sourceName,&buf2)==-1){
        exitProgram(strerror(errno));
    }
    if(S_ISDIR (buf.st_mode)&&S_ISDIR (buf2.st_mode)&&rFlag==1){
        //cout<<"entered dirTodir"<<'\n';

        if(sourceName[strlen(sourceName)-1]=='/'){
            slashFlag=1;
           // cout<<"slashflag set"<<'\n';
        }
        dirToDir(sourceName,targetName);
    }else
    if(S_ISDIR (buf.st_mode)&&S_ISDIR (buf2.st_mode)){
        exitProgram("for directory to directory copying please use the -r option");
    }else
    if(S_ISDIR (buf.st_mode)){
        fileToDir(sourceName,targetName);
    }
    else{
        fileToFile(sourceName,targetName);
    }
    for(int i=0;i<=dumpIndex;i++){
        delete fileDump[i];
        delete dirDump[i];
    }
    for(int i=0;i<=charDumpindex;i++){
        delete[] charDump[i];
    }
    return 0;
}
void dirToDir(char* sourceName,char* targetName){
    dirLevel tree = constructDirTree(sourceName);
   // cout<<"TREE constructed"<<'\n';
    if(chdir(targetName)==-1){
         exitProgram(strerror(errno));
    }
    writeDirTree(tree);
}

void writeDirTree(dirLevel level){
    //cout<<"entering writeDirTree from parent "<<level.parentDir<<'\n';
    if(slashFlag!=1) {
        if (mkdir(level.parentDir, (mode_t) level.parentMode)) {
            exitProgram(strerror(errno));
        }
        chdir(level.parentDir);
    }else{
        slashFlag=0;
    }
    for(int i =0;i<100;i++){
        if(strcmp(level.files[i]->fileName,"empty")!=0) {
            char *fileName = level.files[i]->fileName;
            char *fileContent = level.files[i]->fileContent;

           // cout << "File map includes:" << fileName << '\n';
            int fd = open(fileName, O_WRONLY | O_TRUNC | O_CREAT,511);
            if (fd == -1) {
                //cout << "er2" << '\n';
                exitProgram(strerror(errno));
            }

            if (write(fd, fileContent, strlen(fileContent)) == -1) {
               // cout << "er3" << '\n';
                exitProgram(strerror(errno));
            }
            if (close(fd) == -1) {
               // cout << "er4" << '\n';
                exitProgram(strerror(errno));
            }
        }
    }

    for(int i = 0;i<100;i++){
        if(strcmp(level.dirs[i]->dirName,"empty")!=0) {
            //cout << "Dir map includes:" << level.dirs[i]->dirName << '\n';
            writeDirTree(level.dirs[i]->level);
        }
    }
    chdir("..");
}




dirLevel constructDirTree(char* target){
    //cout<<"entered construct tree with target "<<target<<'\n';
    struct dirLevel level;
    int fileCount = 0;
    int dirCount = 0;

    for(int i=0; i<100;i++){
        level.dirs[i]=new directory;//TODO dumpster
        level.dirs[i]->dirName = (char *) "empty";

        level.files[i]=new file;
        level.files[i]->fileName = (char *) "empty";

        dirDump[dumpIndex]=level.dirs[i];
        fileDump[dumpIndex]=level.files[i];
        dumpIndex++;

    }


    level.parentDir = target;
    struct stat fileStat1;
    if(stat(target, &fileStat1 )==-1){
        exitProgram(strerror(errno));
    }
    level.parentMode = fileStat1.st_mode;


    if(chdir(target)==-1){
        exitProgram(strerror(errno));
    }

    DIR * currentDirectory = opendir(".");
    if(!currentDirectory){
        exitProgram(strerror(errno));
    }
    struct dirent *file;
    while((file=readdir(currentDirectory))) {
        if(strcmp(file->d_name,".")!=0&&strcmp(file->d_name,"..")!=0){
            struct stat fileStat;
            if (stat(file->d_name, &fileStat) == -1) {
                exitProgram(strerror(errno));
            }

            if(S_ISREG (fileStat.st_mode)){
                //cout << "adding to files array " << file->d_name << '\n';
                int fd = open(file->d_name, O_RDONLY);
                if (fd == -1) {
                    //cout<<"er1"<<'\n';
                    exitProgram(strerror(errno));
                }
                char * buf = new char[256];
                charDump[charDumpindex]=buf;
                charDumpindex++;
                strcpy(buf,file->d_name);
                level.files[fileCount]->fileName=buf;
                char * buf2 = new char[100000];
                charDump[charDumpindex]=buf2;
                charDumpindex++;
                strcpy(buf2,(char *) getFileContent(fd).c_str());
                level.files[fileCount]->fileContent = buf2;
                fileCount++;
            }else
            if (S_ISDIR (fileStat.st_mode)) {
                char * buf = new char[256];
                charDump[charDumpindex]=buf;
                charDumpindex++;
                strcpy(buf,file->d_name);
                level.dirs[dirCount]->dirName = buf;
                //cout << "adding to dir array " << level.dirs[dirCount]->dirName <<" at index "<<dirCount<<'\n';
                dirCount++;
            }
        }
    }
    //cout<<"dirs Found "<<dirCount<<'\n';
    //cout<<"files Found "<<fileCount<<'\n';
    for(int i=0;i<100;i++){
        if(strcmp(level.dirs[i]->dirName,"empty") != 0){
           // cout<<"dir: "<<level.dirs[i]->dirName<<" at index "<<i<< " \n";
        }
    }
    for(int i=0;i<100;i++){
        if(strcmp(level.files[i]->fileName,"empty") != 0){
            //cout<<"file: "<<level.files[i]->fileName<<" at index "<<i<< " \n";
        }
    }
    for(int i=0;i<100;i++){
        if(strcmp(level.dirs[i]->dirName,"empty") != 0){
           // cout<<"Entering level "<<level.dirs[i]->dirName<<'\n';
            level.dirs[i]->level = constructDirTree(level.dirs[i]->dirName);
            //cout<<"level "<<level.dirs[i]->dirName<< " written \n";
        }
    }
    chdir("..");
    //cout<<"leaving "<<target<<"\n \n";
    return level;
}

void fileToDir(char* sourceName,char* targetName){
    int sourceFd;
    int targetFd;
    string sourceContent;
    sourceFd = open(sourceName,O_RDONLY);
    if(sourceFd==-1){
        exitProgram(strerror(errno));
    }
    sourceContent = getFileContent(sourceFd);

    if(chdir(targetName)==-1){
        exitProgram(strerror(errno));
    }

    targetFd = open(sourceName,O_WRONLY|O_TRUNC|O_CREAT);
    if(targetFd==-1){
        exitProgram(strerror(errno));
    }
    if(write(targetFd,sourceContent.c_str(),strlen(sourceContent.c_str()))==-1){
        exitProgram(strerror(errno));
    }
    if(close(targetFd) == -1){
        exitProgram(strerror(errno));
    }
    if(chdir("..")==-1){
        exitProgram(strerror(errno));
    }
}

void fileToFile(char* sourceName,char* targetName){
    int sourceFd;
    int targetFd;
    string sourceContent;
    sourceFd = open(sourceName,O_RDONLY);
    if(sourceFd==-1){
        exitProgram(strerror(errno));
    }
    targetFd = open(targetName,O_WRONLY|O_TRUNC|O_CREAT);
    if(targetFd==-1){
        exitProgram(strerror(errno));
    }

    sourceContent = getFileContent(sourceFd);

    if(write(targetFd,sourceContent.c_str(),strlen(sourceContent.c_str()))==-1){
        exitProgram(strerror(errno));
    }
    if(close(targetFd) == -1){
        exitProgram(strerror(errno));
    }
}
