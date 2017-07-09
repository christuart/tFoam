#include <time.h>
#include <string>
#include "tf-gui.h"
#include <iostream>
#include <iomanip>
#include "exceptions.hh"
#include "utilities.hh"
//#include "tds.hh"

UserInterface* UI=NULL;
//tds_display* tds=NULL;
//tds_batch* tds_b=NULL;
//tds_run* tds_t=NULL;
//tds_run* tds_r=NULL;
void clear_pointers();

// Global Message Buffers
//MessageBuffer exceptions = MessageBuffer(MBUnhandledException, "EXCEPTION   ");
//MessageBuffer warnings = MessageBuffer(MBWarnings, "WARNING     ");
//DebugMessageBuffer debugging = DebugMessageBuffer(MBDebug, "DEBUG       ");
// Global Message Listeners
//standard_cout_listener console_out;
//standard_cerr_listener console_err;
//error_log_listener error_log;

/*===========================================================================*/
// GUI (FLUID) INTERFACE PROCEDURES.
//void userAction(Fl_Widget* sender){ tds->action(sender); }
//void userAction(selection sel, Ca_Canvas *sender){ tds->action(sel,sender); }
//selection as xmin,xmax,ymin,ymax
//void mark_data_dirty(){ tds->mark_data_dirty(); }
/*===========================================================================*/

void show_preamble();
void show_usage();

int main(int nArg, char** vArg){
	try {
		show_preamble();

		int argsize = nArg;
		std::string textfile("config_file.conf"); // currently unused, could maybe set defaults in here?
		std::string prefix;
		cCommandLine cl(nArg,vArg);// decompose the command line.
	
		// std::cout << "Number of arguments: " << cl.size() << std::endl;
		// for (unsigned i=0; i < cl.size(); ++i) {
		// 	std::cout << "Argument " << i+1 << ": " << cl[i].arg << std::endl;
		// }

		//console_out.error_listener(&console_err);
		//console_err.out_listener(&console_out);
		
		//debugging.add_listener(&console_out);
		//exceptions.add_listener(&console_err);
		//warnings.add_listener(&console_err);
		
		int t = cl.flag("t|test");
		int b = cl.flag("b|batch");
		int v = cl.flag("v|viewer");
		int version_flag = cl.flag("version");
		int run = cl.flag("run");

		if (version_flag >= 0) {
			return 0;
		} else if (run >= 0) {
			return 0;
		} else {
			if (v >= 0) {
				try {
					UI = new UserInterface(); UI->start_showing_window();
//					tds = new tds_display(UI);
					int err=Fl::run();
					clear_pointers();
					return err;
				}
//				catch (Errors::UIException& e) {
				catch (std::runtime_error& e) {
					std::cerr << e.what() << std::endl;
				}
				clear_pointers();
				return 0;
			} else if (t>=0) {
				// looks like we're testing functionality rather than using the display or batch processing
				return 0;
			} else if (b >= 0) {
				return 0;
			} else {
				show_usage();
			}
		}
	}
	catch (std::exception& e){
//		LOG(exceptions,"(unhandled exception) " << e.what());
	}
	clear_pointers();
};

void clear_pointers() {
//	delete tds; tds=NULL;
//	delete tds_r; tds_r=NULL;
//	delete tds_t; tds_t=NULL;
//	delete tds_b; tds_b=NULL;
	delete UI; UI=NULL;
	std::cout << std::endl << std::endl; // ensures that next console line displays cleanly
}

void show_preamble() {
	
	std::cout << std::endl;
	std::cout << "***" << std::endl;
	std::cout << "************************************************************" << std::endl;
	std::cout << "***" << std::endl;
	std::cout << "***  trit-dif program written by Chris Stuart" << std::endl;
	std::cout << "***  Supervised by Anthony Hollingsworth at CCFE" << std::endl;
	std::cout << "***" << std::endl;
	std::cout << "***  This version: " << VERSION << std::endl;
	std::cout << "***" << std::endl;
	std::cout << "***  Find out more on the GitHub repository at " << std::endl;
	std::cout << "***  http://github.com/christuart/trit-dif/" << std::endl;
	std::cout << "***" << std::endl;
	std::cout << "************************************************************" << std::endl;
	std::cout << "***" << std::endl;
	std::cout << std::endl;

}
void show_usage() {

	std::cout << std::endl;
	std::cout << "Usage: trit-dif [[--run <instruction file> ] | [-v|--viewer] | [-h|--help ]]" << std::endl;
	std::cout << std::endl << std::left;
	std::cout << std::setw(4) << "" << std::setw(12) << "--run" << "Provide the program with a set of instructions in a '.run' file" << std::endl;
	std::cout << std::setw(4) << "-v" << std::setw(12) <<  "--viewer" << "Open the graphical user interface" << std::endl;
	std::cout << std::setw(4) << "-h" << std::setw(12) <<  "--help" << "Show this usage information" << std::endl;
	std::cout << "Alternative, older ways of using the program may be found in the documentation or the source." << std::endl;
	std::cout << std::endl;

}
