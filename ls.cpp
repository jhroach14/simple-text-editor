#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <iostream>
#include "library.h"
#include <iostream>
#include <sys/stat.h>
void wholeDirectory();

void singleFile(char*);

char* getMonth(int);

string* alphabetize(string*,int);

using namespace std;
int main(int argc, char *argv[]){
   if(argc==2){
       singleFile(argv[1]);
   }if(argc==1){
       wholeDirectory();
   }else{
        exitProgram("incorrect number of arguments");
    }
    return EXIT_SUCCESS;
}
//method for whole directory listing
void wholeDirectory(){
    DIR * currentDirectory = opendir(".");
    string* fileNames= new string[100];

    int blockCount=0;
    int fileCount=0;
    struct dirent *file;
    while((file=readdir(currentDirectory))) {
        if(strcmp(file->d_name,".")!=0&&strcmp(file->d_name,"..")!=0) {
            fileNames[fileCount] = file->d_name;
            fileCount++;
            struct stat buf;
            stat(file->d_name,&buf);
            blockCount+=buf.st_blocks;
        }
    }

    fileNames=alphabetize(fileNames,fileCount);
    cout<<"total "<<blockCount<<"\n";
    for (int i = 0; i < fileCount; i++) {
        singleFile((char *) fileNames[i].c_str());
        cout<<'\n';
    }


}
//method to sort alphabetically
string* alphabetize(string* fileNames,int fileCount){
    int alphabetized = 0;
    while(alphabetized!=2) {
        for (int i = 0; i < fileCount - 1; i++) {
            int r = strcasecmp(fileNames[i].c_str(), fileNames[i + 1].c_str());
            if (r > 0) {
                string temp = fileNames[i];
                fileNames[i] = fileNames[i + 1];
                fileNames[i + 1] = temp;
                alphabetized=1;
            }
        }
        if(alphabetized==0){
            alphabetized=2;
        }else{
            alphabetized=0;
        }
    }
    return fileNames;
}
//method for one file listing
void singleFile(char* filename){
    struct stat buf;
    stat(filename,&buf);
    char fileType = getFileType(buf);

    char* modeString = constructModeString(buf);
    char modeStr[9];
    for(int i=0;i<9;i++){
        modeStr[i] = modeString[i];
    }
    string directoryThing;
    if(S_ISDIR (buf.st_mode)) {
        directoryThing= "+";
    }else{
        directoryThing= " ";
    }
    int numHardlinks = buf.st_nlink;

    struct passwd *pw = getpwuid(buf.st_uid);
    struct group  *gr = getgrgid(buf.st_gid);

    char * userName= pw->pw_name;
    char * groupName= gr->gr_name;

    int size = (int) buf.st_size;

    struct tm *tm;
    tm = localtime(&buf.st_mtime);
    char* month = getMonth(tm->tm_mon);
    int day = tm->tm_mday;
    int hour = tm->tm_hour;
    int sec = tm->tm_sec;

    char* fileName = filename;
    cout<<fileType<<modeStr[0]<<modeStr[1]<<modeStr[2]<<modeStr[3]<<modeStr[4]<<modeStr[5]<<modeStr[6]<<modeStr[7]<<modeStr[8]<<". "<<numHardlinks<<" "<<userName;
    cout<<" "<<groupName<<" "<<size<<" "<<month<<"  "<<day;
    cout<<" "<<hour<<":"<<sec<<" "<<fileName;
}
//maps num to string month
char* getMonth(int monthNum){
    char * month;
    switch(monthNum+1){
        case 1:
            month = (char *) "Jan";
            break;
        case 2:
            month = (char *) "Feb";
            break;
        case 3:
            month = (char *) "Mar";
            break;
        case 4:
            month = (char *) "Apr";
            break;
        case 5:
            month = (char *) "May";
            break;
        case 6:
            month = (char *) "Jun";
            break;
        case 7:
            month = (char *) "Jul";
            break;
        case 8:
            month = (char *) "Aug";
            break;
        case 9:
            month = (char *) "Sep";
            break;
        case 10:
            month = (char *) "Oct";
            break;
        case 11:
            month = (char *) "Nov";
            break;
        case 12:
            month = (char *) "Dec";
            break;
        default:break;
    }
    return month;
}


