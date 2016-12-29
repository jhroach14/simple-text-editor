#include <iostream>
#include <getopt.h>
#include <string>
#include <cstdlib>
#include <errno.h>
#include <cstdio>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <math.h>
#include <sstream>
#include <string.h>
#include "library.h"
#include <sys/stat.h>


static int isOctFlag;
static char* octalStr;
static char* path;
static char* symString;
static int isEquFlag = 0;
static int xFlag = 0;
static int uFlag=0;
static int gFlag=0;
int* octMasks = new int[20];

char* opArray = new char[20];
string* statements=new string[20];

void parseOpts(int argc, char *argv[]);

void handleOct();

void handleSym();

void divideByComma();

int * parseStatements();

int constructBitMask(char ref[4], char op, char modes[6]);

int combineMasks(int masks[20]);

void handleEqu(int mask);

int* convertMasks(int masks[20]);

int getMode(int fd);


using namespace std;
int main(int argc, char *argv[]) {
    for(int i =0;i<20;i++){
        statements[i]="empty";
    }
    parseOpts(argc,argv);

    switch(isOctFlag){
        case 0:
            //cout<<"Entering octal"<<'\n';
            handleOct();
            break;

        case 1:
            //cout<<"Entering symbolic"<<'\n';
            handleSym();
            break;
    }
    delete[] statements;
    delete[] octMasks;
    delete[] opArray;
    return 0;
}
//method to handle symbolic modes
void handleSym(){
    divideByComma();
    int* masks = parseStatements();
    int fd = open(path,O_RDONLY);
    if(fd==-1){
        exitProgram(strerror(errno));
    }

    for(int i =0;i<20;i++){

        int existing = getMode(fd);
        int hundreds;
        int thousands;
        if(opArray[i]=='='){
            int ones = toOctal(masks[i])%10;
            int tens = (toOctal(masks[i])/10)%10;
            if(masks[i]/1000==0){
                hundreds = (toOctal(masks[i])/100)%10;
            }else{
                hundreds = ((toOctal(masks[i])%1000)/100)%10;
            }
            if(masks[i]/10000==0){
                thousands = (toOctal(masks[i])/1000)%10;
            }else{
                thousands= ((toOctal(masks[i])%10000)/1000)%10;
            }

            //cout<<path<<" has mode: "<<(existing)<<'\n';
            if(ones!=0){
                existing &= ~(7);
            }
            if(tens!=0){
                existing &= ~(56);
            }
            if(hundreds!=0){
                existing &= ~(455);
            }
            if(thousands!=0){
                existing &= ~(3647);
            }

            //cout<<"Changing mode of "<<path<<" to "<<(masks[i]|existing)<<'\n';
            if(chmod(path, (mode_t) masks[i]|existing)== -1){
                exitProgram(strerror(errno));
            }
        }else
        if(opArray[i]=='+'){
            if(xFlag==1){
                if(((existing>>2)&1)==1){
                    //cout<<path<<" has mode: "<<(existing)<<'\n';
                    //cout<<"Changing mode of "<<path<<" to "<<(masks[i]|existing)<<'\n';
                    chmod(path, (mode_t) (masks[i] | existing));
                }
            }else {
                //cout<<path<<" has mode: "<<(existing)<<'\n';
                //cout<<"Changing mode of "<<path<<" to "<<(masks[i]|existing)<<'\n';
                chmod(path, (mode_t) (masks[i] | existing));
            }
        }else
        if(opArray[i]=='-'){
            //cout<<path<<" has mode: "<<(existing)<<'\n';
            //cout<<"Changing mode of "<<path<<" to "<<(existing&(~masks[i]))<<'\n';
            chmod(path, (mode_t) (existing&(~masks[i])));
        }
        getMode(fd);
    }
}
//gets the mode of the fd file
int getMode(int fd){

    struct stat buf;
    fstat(fd,&buf);
    mode_t existing = buf.st_mode;
    //cout<<"The mode is: "<<existing<<'\n';
    return existing;
}
//parses through the symobls
int* parseStatements(){
    int masks[20];
    int maskIndex=0;
    int* maskArray;

    for(int i=0;i<20;i++){
        masks[i]=0;
        opArray[i]=0;
    }

    for(int i=0;i<20;i++) {
        string str = statements[i];
        if (str != "empty") {
            int tFlag=0;
            char ref[4];
            int refIndex = 0;
            char op;
            char modes[6];
            int refFlag = 0;
            int opFlag = 0;
            for (int j = 0; j < 6; j++) {
                modes[j] = '0';
            }
            int mdIndex = 0;
            for (unsigned int j = 0; j < str.length(); j++) {
                char ch = str.at(j);
                if (ch == 'u' || ch == 'o' || ch == 'g' || ch == 'a') {
                    ref[refIndex] = ch;
                    refIndex++;
                    refFlag=1;
                } else if (ch == '+' || ch == '-' || ch == '=') {
                    op = ch;
                    opFlag=1;
                } else if (ch == 'r' || ch == 'w' || ch == 'x' || ch == 'X' || ch == 's' || ch == 't') {
                    if(ch=='t'){
                        tFlag=1;
                    }
                    if(ch=='s'&&ref[refIndex-1]=='u'){
                        uFlag=1;
                    }else
                    if(ch=='s'&&ref[refIndex-1]=='g'){
                        gFlag=1;
                    }
                    modes[mdIndex] = ch;
                    mdIndex++;
                } else {
                    exitProgram("unexpected character in symbolic mode");
                }
            }
            if(refFlag==0&&tFlag==0){
                exitProgram("missing reference(u or g or o or a)");
            }
            if(opFlag==0){
                exitProgram("missing operator");
            }
            masks[maskIndex]=constructBitMask(ref, op, modes);
            opArray[maskIndex]=op;
            maskIndex++;
        }
    }
    maskArray =convertMasks(masks);
    return maskArray;
}
//converts the octal mask representations into the correct decimal for use in chmod
int* convertMasks(int masks[20]){
    int octMasksIndex=0;

    for(int i =0;i<20;i++){
        int octal;
        stringstream strs;
        strs << masks[i];
        string temp = strs.str();

        char* charMask = new char[temp.length()];
        strcpy(charMask,temp.c_str());
        octal =baseConverter(charMask);
        octMasks[octMasksIndex]= octal;
        octMasksIndex++;
        delete[] charMask;
        //cout<<"Converted octal "<<masks[i]<<" to decimal "<<octal<<'\n';
    }

    return octMasks;
}
//constructs an octal version of what the mask should be
int constructBitMask(char ref[4], char op, char modes[6]) {
    int o = 0;
    int g = 0;
    int u = 0;
    int thousanths=0;
    int thousanthsMode=0;
    int myOp =1;
    int mode = 0;
    int mask = 0;

    for(int i = 0;i<4;i++){
        char ch =ref[i];
        if(ch=='o'){
            o=1;
        }else
        if(ch=='g'){
            g=10;
        }else
        if(ch=='u'){
            u=100;
        }else
         if(ch=='a'){
             o=1;
             g=10;
             u=100;
        }
    }

    if(op=='-'){
        myOp= 1;
        isEquFlag = 2;
    }else
    if(op=='+'){
        myOp=1;
        isEquFlag=3;
    }else
    if(op=='='){
        isEquFlag=1;
    }

    for(int i =0;i<6;i++){
        char ch = modes[i];
        if(ch=='r'){
            mode+=4;
        }else
        if(ch=='w'){
            mode+=2;
        }else
        if(ch=='x'||ch=='X'){
            if(ch=='X'){
                xFlag = 1;
            }
            mode+=1;
        }else
        if(ch=='t'){
            thousanths=1000;
            thousanthsMode+=1;
        }else
        if(ch=='s'){
            thousanths=1000;
            if(uFlag==1){
                thousanthsMode+=4;
            }
            if(gFlag==1){
                thousanthsMode+=2;
            }
        }
    }
    o*=mode;
    g*=mode;
    u*=mode;
    thousanths*=thousanthsMode;

    mask= mask+((thousanths+o+g+u)*myOp);
    //cout<<"Constructed octal bit mask: "<<mask<<'\n';
    return mask;
}
//divides each statments by commas
void divideByComma(){
    string str(symString);
    //TODO make dumpster

    int count = 0;
    for(int i=0,prev=0,cur=0;((unsigned)i)<str.length();i++,cur++){
        if(str.at((unsigned long) i)==','){
            statements[count] = str.substr((unsigned long) prev, (unsigned long) cur);
            prev = i+1;
            count++;
            cur= -1;
        } else
        if((unsigned(i+1))==str.length()){
            statements[count] = str.substr((unsigned long) prev, (unsigned long) i+1);
        }
        if(strcmp(statements[count].c_str(),"empty")!=0){
            //cout << "Found statment: " << statements[count] << '\n';
        }
    }

}
//handles octal
void handleOct(){

    int octal = baseConverter(octalStr);
    //cout<<"Octal in decimal: "<<octal<<'\n';

    int fd = open(path,O_RDONLY);
    if(fd==-1){
        exitProgram(strerror(errno));
    }
    struct stat buf;
    fstat(fd,&buf);
    //mode_t existing = buf.st_mode;
    //cout<<"Existing mode: "<<existing<<'\n';

    if(chmod(path, (mode_t) octal)== -1){
        exitProgram(strerror(errno));
    }
    fstat(fd,&buf);
    //cout<<"New Mode: "<<buf.st_mode<<'\n';
}
//parses through the options setting flags for those it finds
void parseOpts(int argc, char *argv[]){

    int optionIndex = 0;
    int c = 0;

    static struct option long_options[]= {
            {"oct",required_argument,0,'c'},
            {"sym",required_argument,0,'s'}
    };

    while(c!=-1){
        opterr = 1;
        c = getopt_long_only( argc, argv, "", long_options,&optionIndex);

        switch(c){
            case 'c':
               // cout<<"Found option Octal"<<'\n';
                isOctFlag = 0;
                octalStr = optarg;
               // cout<<"Octal string: "<<octalStr<<'\n';
                break;

            case 's':
                //cout<<"Found option symbolic"<<'\n';
                isOctFlag = 1;
                symString = optarg;
                //cout<<"Symbolic string: "<<symString<<'\n';
                break;
        }
    }
    path=argv[optind];
    //cout<<"Target: "<<path<<'\n';
}




