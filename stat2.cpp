#include <iostream>
#include <pwd.h>
#include <grp.h>
#include "library.h"
#include <sstream>
#include <sys/stat.h>

string getFileTypeStr(struct stat buf);

string constructTime(time_t *pInt, long i);

using namespace std;

int main(int argc, char *argv[]) {

    char * fileName = argv[1];

    struct stat buf;
    lstat(fileName,&buf);

    int size = (int) buf.st_size;
    int blocks = (int) buf.st_blocks;
    int blockSize = buf.st_blksize;

    string fileType = getFileTypeStr(buf);
    string major = toHex(major(buf.st_dev));
    string minor = toHex(minor(buf.st_dev));
    long dec = fromHex(major+"0"+minor);

    ino_t inode = buf.st_ino;
    int numHardlinks = buf.st_nlink;
    int octalMode = toOctal(buf.st_mode);

    char fileTypeCh = getFileType(buf);
    char* modeString = constructModeString(buf);

    struct passwd *pw = getpwuid(buf.st_uid);
    struct group  *gr = getgrgid(buf.st_gid);
    char * userName= pw->pw_name;
    char * groupName= gr->gr_name;
    int userId =buf.st_uid;
    int groupId =buf.st_gid;

    string aTime= constructTime(&buf.st_atime, buf.st_atim.tv_nsec);
    string mTime= constructTime(&buf.st_mtime, buf.st_mtim.tv_nsec);
    string cTime= constructTime(&buf.st_ctime, buf.st_ctim.tv_nsec);

    cout<<"  File: '"<<fileName<<"'\n";
    cout<<"  Size: "<<size<<"    Blocks: "<<blocks<<"    IO Block: "<<blockSize<<"  "<<fileType<<'\n';
    cout<<"Device: "<<major<<"0"<<minor<<"h/"<<dec<<"d   Inode: "<<inode<<"  Links: "<<numHardlinks<<'\n';
    cout<<"Access: (0"<<(octalMode%1000)<<'/'<<fileTypeCh<<modeString<<")  Uid: ("<<userId<<'/'<<userName<<")   Gid:  ("<<groupId<<'/'<<groupName<<")\n";
    cout<<"Access: "<<aTime<<'\n';
    cout<<"Modify: "<<mTime<<'\n';
    cout<<"Change: "<<cTime<<'\n';
    return EXIT_SUCCESS;
}
//constructs time string
string constructTime(time_t *pInt, long nano) {
    std::ostringstream oss;
    struct tm *tm;
    tm = localtime(pInt);
    char  time[30];
    strftime(time,30,"%F %T",tm);
    string strTime;

    char timeZone[30];
    strftime(timeZone,30,"%z",tm);

    oss<<time<<'.'<<nano<<" "<<timeZone;
    strTime =oss.str();
    return strTime;
}
//gets the file type as a string
string getFileTypeStr(struct stat buf){
    string fileType;
    if(S_ISREG (buf.st_mode)){
        fileType="regular file";
    }else
    if(S_ISDIR (buf.st_mode)){
        fileType="directory";
    }else
    if(S_ISCHR (buf.st_mode)){
        fileType="character device";
    }else
    if(S_ISBLK (buf.st_mode)){
        fileType="block device";
    }else
    if(S_ISFIFO (buf.st_mode)){
        fileType="named pipe";
    }else
    if(S_ISLNK (buf.st_mode)){
        fileType="symbolic link";
    }else
    if(S_ISSOCK (buf.st_mode)){
        fileType="socket";
    }else{
        fileType="?";
    }
    return fileType;
}
