#include <iostream>
#include "library.h"
#include <map>
#include <sys/stat.h>
#include <signal.h>

int sFlag = 0;
string sig;
int signo;
char* pId;
void parseOpts(int argc, char *argv[]);
const map<string, int> signal_map {
#ifdef SIGHUP
        {"SIGHUP", SIGHUP},
#endif
#ifdef SIGINT
        {"SIGINT", SIGINT},
#endif
#ifdef SIGQUIT
        {"SIGQUIT", SIGQUIT},
#endif
#ifdef SIGILL
        {"SIGILL", SIGILL},
#endif
#ifdef SIGTRAP
        {"SIGTRAP", SIGTRAP},
#endif
#ifdef SIGABRT
        {"SIGABRT", SIGABRT},
#endif
#ifdef SIGEMT
        {"SIGEMT", SIGEMT},
#endif
#ifdef SIGFPE
        {"SIGFPE", SIGFPE},
#endif
#ifdef SIGKILL
        {"SIGKILL", SIGKILL},
#endif
#ifdef SIGBUS
        {"SIGBUS", SIGBUS},
#endif
#ifdef SIGSEGV
        {"SIGSEGV", SIGSEGV},
#endif
#ifdef SIGSYS
        {"SIGSYS", SIGSYS},
#endif
#ifdef SIGPIPE
        {"SIGPIPE", SIGPIPE},
#endif
#ifdef SIGALRM
        {"SIGALRM", SIGALRM},
#endif
#ifdef SIGTERM
        {"SIGTERM", SIGTERM},
#endif
#ifdef SIGURG
        {"SIGURG", SIGURG},
#endif
#ifdef SIGSTOP
        {"SIGSTOP", SIGSTOP},
#endif
#ifdef SIGTSTP
        {"SIGTSTP", SIGTSTP},
#endif
#ifdef SIGCONT
        {"SIGCONT", SIGCONT},
#endif
#ifdef SIGCHLD
        {"SIGCHLD", SIGCHLD},
#endif
#ifdef SIGCLD
        {"SIGCLD", SIGCLD},
#endif
#ifdef SIGTTIN
        {"SIGTTIN", SIGTTIN},
#endif
#ifdef SIGTTOU
        {"SIGTTOU", SIGTTOU},
#endif
#ifdef SIGIO
        {"SIGIO", SIGIO},
#endif
#ifdef SIGPOLL
        {"SIGPOLL", SIGPOLL},
#endif
#ifdef SIGXCPU
        {"SIGXCPU", SIGXCPU},
#endif
#ifdef SIGXFSZ
        {"SIGXFSZ", SIGXFSZ},
#endif
#ifdef SIGVTALRM
        {"SIGVTALRM", SIGVTALRM},
#endif
#ifdef SIGPROF
        {"SIGPROF", SIGPROF},
#endif
#ifdef SIGWINCH
        {"SIGWINCH", SIGWINCH},
#endif
#ifdef SIGLOST
        {"SIGLOST", SIGLOST},
#endif
#ifdef SIGPWR
        {"SIGPWR", SIGPWR},
#endif
#ifdef SIGUSR1
        {"SIGUSR1", SIGUSR1},
#endif
#ifdef SIGUSR2
        {"SIGUSR2", SIGUSR2},
#endif
};

using namespace std;

int main(int argc, char *argv[]) {
    parseOpts(argc,argv);
    if(sFlag==0){
        if(kill(atoi(pId),SIGTERM)==-1){
            if(kill(atoi(pId),SIGKILL)==-1){
                exitProgram(strerror(errno));
            }
        }
    }else{
        if(kill(atoi(pId),atoi(sig.c_str()))==-1){
            try {
                signo = signal_map.at(sig);
            } catch (exception & e) {
                exitProgram("invalid signal");
            }
            if(kill(atoi(pId),signo)==-1){
                exitProgram(strerror(errno));
            }
        }
    }

    return 0;
}
void parseOpts(int argc, char *argv[]){
    int opt;
    while((opt = getopt(argc, argv, "s:")) != -1){
        switch(opt){
            case 's':
                sFlag=1;
                sig=optarg;
                break;
            default:
                exitProgram(strerror(errno));
        }
    }
    pId = argv[optind];

}


