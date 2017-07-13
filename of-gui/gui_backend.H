#ifndef GUI_BACKEND_H
#define GUI_BACKEND_H

#include "messaging.H"
#include "utilities.H"
#include "exceptions.H"
#include "file_handler.H"
#include "extend_fl.H"
#include "timing.H"
#include "tf-gui.H"
#include "fvCFD.H"
#include "wordList.H"
#include "JobInfo.H"

extern MessageBuffer warnings;

class gui_backend {
private:
	UserInterface *GUI_;
	
	//~ Fl_Text_Buffer BRunFileName,
		//~ BRunFileContents,
		//~ BModelDirectory,
		//~ BModelName,
		//~ BSettingsDirectory,
		//~ BSettingsName,
		//~ BOutputDirectory,
		//~ BOutputName,
		//~ BDataDirty;
	
	gui_status_bar_messages BStatusBar;
	
	gui_console_messages gui_console;
	
	MessageBuffer gui_status;
	MessageBuffer gui_alerts;
	MessageBuffer gui_actions;
	
	file_handler* scenario_browser_config;
	bool scenario_browser_is_prepared;
	
private:
	// actions
	void open_test_case();
	// building GUI
	void prepare_scenario_browser();
	void add_scenario_directory(std::string dir_);
	void insert_subdirectory(std::string dir_, std::string sub_);
	
public:
	gui_backend(UserInterface *gui);
	~gui_backend();
	inline UserInterface &GUI(){ return *GUI_; }
	void action(Fl_Widget *sender);
};

#endif
