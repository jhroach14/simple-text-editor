//
// Created by James Roach on 10/29/2015.
//
#include <string>
#include <iostream>
#include <stdlib.h>
#include <sstream>
#include "library.h"
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
using namespace std;
string getFileContent(int fd){

    int n;
    char buffer [256];
    string content = "";

    while((n = read(fd,buffer,256)) > 0){
        if(n != -1){
            buffer[n] = '\0';
            content += buffer;
        }
        else
        if(n == -1){
            exitProgram(strerror(errno));
        }
    }
    if(close(fd) == -1){
       exitProgram(strerror(errno));
    }
    return content;
}

void exitProgram(const char *message){
    cerr<<"ERROR: "<<message<<'\n';
    exit(EXIT_FAILURE);
}
int baseConverter(char* num){
    string str(num);
    int ans = 0;
    int size = (int) str.length();
    long x = 0u;

    for (long i = size-1; i >=0; i--,x++) {
        int numInt = str.at((unsigned long) i) - '0';
        ans += numInt * (pow(8, x));
    }

    return ans;
}
int toOctal(int n) {
    int rem;
    int i=1;
    int octal=0;

    while (n!=0)
    {
        rem=n%8;
        n/=8;
        octal+=rem*i;
        i*=10;
    }
    return octal;
}
string toHex(int n) {
    std::stringstream stream;
    stream << std::hex << n;
    std::string result( stream.str() );
    return result;
}

long fromHex(string hexNum) {
    int dec;
    stringstream stream;
    stream<<hexNum;
    stream>>std::hex>>dec;
    return dec;
}

char getFileType(struct stat buf){
    char fileType;
    if(S_ISREG (buf.st_mode)){
        fileType='-';
    }else
    if(S_ISDIR (buf.st_mode)){
        fileType='d';
    }else
    if(S_ISCHR (buf.st_mode)){
        fileType='c';
    }else
    if(S_ISBLK (buf.st_mode)){
        fileType='b';
    }else
    if(S_ISFIFO (buf.st_mode)){
        fileType='p';
    }else
    if(S_ISLNK (buf.st_mode)){
        fileType='l';
    }else
    if(S_ISSOCK (buf.st_mode)){
        fileType='s';
    }else{
        fileType='?';
    }
    return fileType;
}

char* constructModeString(struct stat buf){
    int mode = toOctal(buf.st_mode);
    int ones = mode%10;
    int tens = (mode/10)%10;
    int hundreds = (mode/100)%10;

    char other[3];
    char group[3];
    char user[3];

    if((ones-4)>=0){
        other[0]='r';
        ones-=4;
    }else{
        other[0]='-';
    }
    if((ones-2)>=0){
        other[1]='w';
        ones-=2;
    }else{
        other[1]='-';
    }
    if((ones-1)>=0){
        other[2]='x';
    }else{
        other[2]='-';
    }

    if((tens-4)>=0){
        group[0]='r';
        tens-=4;
    }else{
        group[0]='-';
    }
    if((tens-2)>=0){
        group[1]='w';
        tens-=2;
    }else{
        group[1]='-';
    }
    if((tens-1)>=0){
        group[2]='x';
    }else{
        group[2]='-';
    }

    if((hundreds-4)>=0){
        user[0]='r';
        hundreds-=4;
    }else{
        group[0]='-';
    }
    if((hundreds-2)>=0){
        user[1]='w';
        hundreds-=2;
    }else{
        user[1]='-';
    }
    if((hundreds-1)>=0){
        user[2]='x';
    }else{
        user[2]='-';
    }

    char* modeString = new char[9];
    for(int i =0;i<3;i++){
        modeString[i] = user[i];
        modeString[i+3] = group[i];
        modeString[i+6] = other[i];
    }

    return modeString;
}
