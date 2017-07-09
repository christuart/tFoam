#ifndef UTILITIES_HH
#define UTILITIES_HH

#define MAX_DATE 25

//#include "exception.h"
//#include "mathematic.h"
#include <string>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <time.h>
#include <dirent.h>
#include <signal.h>
#include <math.h>
#include <vector>
#include <iostream>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/resource.h>
#include <pwd.h>
#include <pthread.h>

class cPipeOut;
class cPipe;
class cCommandLine;


/*===========================================================================*/
void cSeparateValues(std::vector<std::string>&, std::string& , std::string, std::string no="");

/*===========================================================================*/
template <class Type> int cContains(std::vector<Type>& tgt, Type arg){
 for (unsigned i=0; i<tgt.size(); ++i) if (tgt[i]==arg) return i; return -1;
}

//===========================================================================*/
  //Writes to 'std::string' through 'std::ostringstream'.
  //Use 'std::stringstream' instead of 'std::string' to read more than one field.
template <class inType> std::string &operator<<(std::string &str, inType Data){
std::ostringstream out; out << Data; return str+=out.str();
}

/*===========================================================================*/
// Writes to 'std::string' through 'std::ostringstream'.
template <class Type> std::string cString(Type Data){
std::ostringstream out; out << Data; return out.str();
}

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/*                                 cCommandLine                              */
/*          Francisco Neves @ 2008.03.13 ( Last modified 2008.03.14)         */
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
class cCommandLine {
public: enum clType { cltFlag=0, cltArgument=1 };
public: struct clParameter { clType type; std::string arg;
    clParameter(clType t, std::string a):type(t),arg(a){ } };
private: //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    unsigned FnFlags, FnArgs;
    std::vector<clParameter> FPList;
    void FClassify(std::string name);
protected: //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    void addArg(std::string name);
    void addFlag(std::string name);
public: //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    cCommandLine(std::string line);
    cCommandLine(int nArg, char** vArg);
    inline unsigned size(){ return FPList.size(); }
    inline clParameter& operator[](unsigned i){ return FPList[i]; }
    inline unsigned nArgs(){ return FnArgs; }
    int arg(unsigned count, unsigned start=0);
    inline unsigned nFlags(){ return FnFlags; }
    int flag(unsigned count, unsigned start=0);
    int flag(std::string name, unsigned start=0);
};

// /*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
// /*                                 cPipeOut                                  */
// /*          Francisco Neves @ 2006.02.07 ( Last modified 2006.02.08 )        */
// /*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
// class cPipeOut{ // Output pipe for a file/application.
// private:
// 	FILE *FPipe;
// protected:
// 	void Execute(const std::string cmd);
// public:
// 	cPipeOut(const std::string commandline);
// 	~cPipeOut();
// 	cPipeOut &operator<<(cFlag flag);
// 	template <class argType> cPipeOut &operator<<(argType Arg);
// };

// /*===========================================================================*/
// // Put 'Arg' into pipe.
// template <class argType> cPipeOut &cPipeOut::operator<<(argType Arg){
// std::ostringstream out; out << Arg;  Execute(out.str());
//  return *this;
// }

// /*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
// /*                                   cPipe                                   */
// /*          Francisco Neves @ 2008.02.12 ( Last modified 2008.02.20 )        */
// /*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
// class cPipe{ // // Output/Input pipe for a file/application.
// public: enum cMode {mREAD=0, mWRITE=1};
// private: //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// 	int FOUT[2], FIN[2], FERR[2], FPID;
// protected: //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//     void OnChild(std::string &comand, std::string &args);
//     void OnParent();
// public: //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//     cPipe(std::string command, std::string args="");
//     ~cPipe(){ close(FIN[mWRITE]); close(FOUT[mREAD]); close(FERR[mREAD]); }
//     void waitForChild(){ int status; int s; waitpid(FPID,&status,0); }
// 	template <class argType> cPipe &operator<<(argType arg);
//     cPipe &operator>>(std::ostringstream &arg);
//     cPipe &operator>>(std::string &arg);
// 	cPipe &operator<<(cFlag arg);
// };

// /*===========================================================================*/
// template <class argType> cPipe &cPipe::operator<<(argType arg){
// std::ostringstream bff; bff << arg; std::string out(bff.str());
//  write(FIN[mWRITE],out.c_str(),out.size());
//  return *this;
// }

std::string find_replace(const std::string& pattern,const std::string& rep,const std::string& str);
void ensure_ending(std::string& str, const std::string& suffix);
void trim(std::string& str);
std::string get_timestamp();
bool get_file_exists(const std::string& filename);

#endif
