//
// Created by James Roach on 10/29/2015.
//

#ifndef PROJ2_LIBRARY_H
#define PROJ2_LIBRARY_H
#include <string.h>
#include <sys/unistd.h>
#include <sys/errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <math.h>
#endif //PROJ2_LIBRARY_H
using namespace std;

string getFileContent(int fd);
void exitProgram(const char *message);
int baseConverter(char* num);
int toOctal(int n);
char getFileType(struct stat buf);
char* constructModeString(struct stat buf);
string toHex(int n);
long fromHex(string hex);