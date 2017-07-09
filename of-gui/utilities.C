#include "utilities.hh"

/*===========================================================================*/
void cSeparateValues(std::vector<std::string>& keys, std::string& text, std::string sep, std::string no){
int S0=0, S=0, i, ni; // values separated by 'sep' (exclusing '[no]sep')
 while ((i=text.find(sep,S))!=std::string::npos){
  if (!no.empty() && (ni=text.rfind(no,i))!=std::string::npos)
   if(i==no.length()+ni) goto skip; keys.push_back(text.substr(S0,i-S0));
  S0=i+sep.length(); skip:S=i+sep.length(); // guides.
 } keys.push_back(text.substr(S0,text.length()-S0));
}

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/*                                 cCommandLine                              */
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*===========================================================================*/
// debug of flags should be done by the user.
void cCommandLine::FClassify(std::string name){ // int pos;
 if (name.length()>0 && name[0]=='-'){ // flag(s)
  if (name.length()>1 && name[1]=='-') addFlag(name.substr(2));
  else  for (unsigned i=1; i<name.length(); ++i) addFlag(name.substr(i,1));
 } else addArg(name); // argument
}

/*===========================================================================*/
void cCommandLine::addArg(std::string name){ if (!name.empty()){
 FPList.push_back(clParameter(cltArgument,name)); ++ FnArgs;
} }

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void cCommandLine::addFlag(std::string name){ if (!name.empty()){
 FPList.push_back(clParameter(cltFlag,name)); ++ FnFlags;
} }

/*===========================================================================*/
cCommandLine::cCommandLine(std::string line):FnFlags(0),FnArgs(0){
std::vector<std::string> val; cSeparateValues(val,line," ","\\");
 for (unsigned i=0; i<val.size(); ++i) // multiple spaces.
  if (!val[i].empty()) FClassify(val[i]);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
cCommandLine::cCommandLine(int nArg, char** vArg):FnFlags(0),FnArgs(0){
 for (unsigned i=0; i<nArg; ++i) FClassify(vArg[i]);
}

/*===========================================================================*/
int cCommandLine::arg(unsigned count, unsigned start){
 for (unsigned i=start, N=1; i<FPList.size(); ++i) // all.
  if (FPList[i].type==cltArgument){ if (N==count) return i; else ++N; }
 return -1; // not found;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
int cCommandLine::flag(unsigned count, unsigned start){
 for (unsigned i=start, N=1; i<FPList.size(); ++i) // all.
  if (FPList[i].type==cltFlag){ if (N==count) return i; else ++N; }
 return -1; // not found;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
int cCommandLine::flag(std::string name, unsigned start){
std::vector<std::string> mul; cSeparateValues(mul,name,"|");
 for (unsigned i=start; i<FPList.size(); ++i) // all.
  if (FPList[i].type==cltFlag && cContains(mul,FPList[i].arg)>=0)
   return i; return -1; // not found.
}

// /*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
// /*                                 cPipeOut                                  */
// /*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

// /*===========================================================================*/
// // Put 'cmd' into pipe.
// void cPipeOut::Execute(const std::string cmd){
//  fputs(cmd.c_str(),FPipe); fflush(FPipe);
// }

// /*===========================================================================*/
// // Constructor: open pipe to 'application' with 'parameters'.
// // Example 1: sPipeOut("gnuplot -persist");
// // Example 2: sPipeout("pawX11");
// cPipeOut::cPipeOut(const std::string commandline){
//  FPipe = popen(commandline.c_str(),"w");
//  if (FPipe==NULL) throw Exception("popen","cPipeOut::cPipeOut(const std::string)",
//   "Cannot open pipe for application ("+commandline+")");
// }

// /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
// cPipeOut::~cPipeOut(){ // Destructor.
//  if (pclose(FPipe) == -1) throw Exception("pclose","cPipeOut::~cPipeOut()",
//   "Problem(s) closing pipe for application");
// }

// /*===========================================================================*/
// // Put 'Arg' control flag into pipe.
// cPipeOut &cPipeOut::operator<<(cFlag flag){
//  switch (flag){ // Use cFlag definitions.
//   case cTab:  Execute("\t"); break;
//   case cEndl: Execute("\n"); break;
//   default: throw Exception("cFlag","cPipeOut::operator<<(cFlag)","Unknown type");
// } return *this; }


// /*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
// /*                                   cPipe                                   */
// /*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

// /*===========================================================================*/
// void cPipe::OnChild(std::string &command, std::string &args){
//   if (dup2(FOUT[mWRITE],fileno(stdout))<0) throw Exception(
//    "Invalid Operation","cPipe::OnChild(std::string)","Fail to redirect stdout");
//   close(FOUT[mWRITE]); close(FOUT[mREAD]); // unused end of pipe...............
//   if (dup2(FIN[mREAD],fileno(stdin))<0) throw Exception(
//    "Invalid Operation","cPipe::OnChild(std::string)","Fail to redirect stdin");
//   close(FIN[mREAD]); close(FIN[mWRITE]); // unused end of pipe.................
//   if (dup2(FERR[mWRITE],fileno(stderr))<0) throw Exception(
//    "Invalid Operation","cPipe::OnChild(std::string)","Fail to redirect stderr");
//   close(FERR[mWRITE]); close(FERR[mREAD]); // unused end of pipe...............
//   fflush(stdout); fflush(stdin); fflush(stderr);
//   if (execlp(command.c_str(),args.c_str(),(char*)NULL)<0) // replace exe image.
//    throw Exception("Invalid Operation","cPipe::OnChild(std::string)",
//     "Fail to run command: "+command);
// }

// /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
// void cPipe::OnParent(){ // close unused end of read, write, err pipes.
//  close(FOUT[mWRITE]); close(FIN[mREAD]); close(FERR[mWRITE]);
// }

// /*===========================================================================*/
// cPipe::cPipe(std::string command, std::string args){
//  if (pipe(FOUT)<0 || pipe(FIN)<0 || pipe(FERR)) throw Exception(
//   "Invalid Operation","cPipe::cPipe(std::string)","Fail to open pipes");
//  if ((FPID=fork())<0) throw Exception("Invalid Operation",
//   "cPipe::cPipe(std::string)","Fail to create a child - fork");
//  else if (FPID==0) OnChild(command,args); // child process.
//  else OnParent(); // parent process.
// }

// /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
// cPipe &cPipe::operator>>(ostringstream &arg){ char bff;
//  while (read(FOUT[mREAD],&bff,1)>0) arg << bff; return *this;
// }

// /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
// cPipe &cPipe::operator>>(std::string &arg){ ostringstream o;
//  this->operator>>(o); arg=o.str(); return *this;
// }

// /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
// cPipe &cPipe::operator<<(cFlag arg){ // cEof for child.
//  switch (arg){ // Use cFlag definitions.
//   case cEof: close(FIN[mWRITE]); break;
//   case cTab:  this->operator<<("\t"); break;
//   case cEndl: this->operator<<("\n"); break;
//   default: throw Exception("cFlag","cPipe::operator<<(cFlag)","Unknown type");
// } return *this; }

std::string find_replace(const std::string& pattern,const std::string& rep,const std::string& str) {
	size_t matched = 0;
	size_t match_start = 0;
	std::string out = "";
	while (match_start <= (str.size() - pattern.size())){
		// Try to find a new match
		match_start = str.find(pattern, match_start);
		if (match_start == std::string::npos)
			break;
     
		// Replace 'pattern' by 'rep'
		out = out + str.substr(matched,match_start-matched) + rep;
		matched = match_start + pattern.size();
		++match_start;
	}
	out = out + str.substr(matched);
	return out;
}

void ensure_ending(std::string& str, const std::string& suffix) {
        if ((str.length() < suffix.length()) || (str.rfind(suffix) != str.length()-suffix.length())) str += suffix;
}

void trim(std::string& str) {
	size_t endpos = str.find_last_not_of(" \t");
	if (std::string::npos != endpos) {
		// trim leading spaces
		size_t startpos = str.find_first_not_of(" \t");
		if (std::string::npos != startpos) {
			str = str.substr(startpos, endpos+1);
		} else {
			str = str.substr(0, endpos+1);
		}
	} else {
		size_t startpos = str.find_first_not_of(" \t");
		if (std::string::npos != startpos) {
			str = str.substr(startpos);
		}
	}
}

// Function by bnbertha from http://www.cplusplus.com/forum/unices/2259/#msg8505
std::string get_timestamp() {
   time_t now;
   char the_date[MAX_DATE];
   the_date[0] = '\0';
   now = time(NULL);
   if (now != -1)
   {
      strftime(the_date, MAX_DATE, "%a-%d-%b-%y_%H-%M-%S", gmtime(&now));
   }

   return std::string(the_date) + "-GMT";
}

/// Check whether file exists without opening it
/** Method courtesy of Rico: http://stackoverflow.com/a/6296808/2569197
    */
bool get_file_exists(const std::string& filename)
{
    struct stat buf;
    return (stat(filename.c_str(), &buf) != -1);
}
