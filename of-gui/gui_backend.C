#include "gui_backend.H"

extern MessageBuffer exceptions;
extern MessageBuffer warnings;
extern DebugMessageBuffer debugging;
extern standard_cout_listener console_out;
extern standard_cerr_listener console_err;
extern error_log_listener error_log;
extern Foam::argList args;

gui_backend::gui_backend(UserInterface *gui):GUI_(gui),scenario_browser_is_prepared(false){
	// Initialise buffers
	gui_status = MessageBuffer(MBGuiStatus,"STATUS      ");
	gui_alerts = MessageBuffer(MBGuiStatus,"ALERTS      ");
	gui_actions = MessageBuffer(MBGuiActions,"GUI         ");
	
	GUI_->txdsp_status_bar->buffer(BStatusBar); // just while testing don't kill me!
	
	gui_console.browser(GUI_->brwsr_run_output);

	// Prepare the tds_display messaging systemsc
	//   - those stored in tds_display
	gui_status.add_listener(&BStatusBar);
	gui_status.add_listener(&console_out);
	gui_status.add_listener(&gui_console);
	gui_alerts.add_listener(&console_out);
	gui_alerts.add_listener(&gui_console);
	gui_actions.add_listener(&console_out);
	
	// Prepare the scenario browser
	GUI_->tree_scenario_browser->showroot(0);
	GUI_->tree_scenario_browser->add("Press Load to read scenario directories");
	
	// Construct config file handlers
	scenario_browser_config = new file_handler(file_purpose::ReadFromFile,"config/scenariobrowser",file_buffer_mode::Closed);
	
	scenarioIsLoaded = false;
}

gui_backend::~gui_backend() {
}

void gui_backend::prepare_scenario_browser() {
	// get a reference to the scenario browser tree widget
	Fl_Tree* tree = GUI_->tree_scenario_browser;
	tree->showroot(0); // don't show the root of the tree
	tree->item_reselect_mode(Fl_Tree_Item_Reselect_Mode::FL_TREE_SELECTABLE_ALWAYS);
	
	// read the config file for the scenario browser to see which
	// directories should be read
	try {
		if (scenario_browser_config->file_completed()) {
			// DEBUG(debugging,"Reopening scenariobrowser config.");
			delete(scenario_browser_config);
			scenario_browser_config = new file_handler(file_purpose::ReadFromFile,"config/scenariobrowser",file_buffer_mode::Closed);
		}
		scenario_browser_config->read();
		// DEBUG(debugging,"Managed to read " << scenario_browser_config->lines_read() << " lines from scenariobrowser config.");
		tree->clear();
		// DEBUG(debugging,"Tree cleared.");
		for (int i=1; i <= scenario_browser_config->lines_read(); i++) {
			// DEBUG(debugging,"i = " << i);
			// DEBUG(debugging,"key[i] = " << scenario_browser_config->key(i));
			// DEBUG(debugging,"value[i] = " << scenario_browser_config->value(i));
			// DEBUG(debugging,i << " - " << scenario_browser_config->key(i) << " - " << scenario_browser_config->value(i));
			if (scenario_browser_config->key(i) == "scenario-directory") {
				LOG(gui_status,"Adding scenario directory: " << scenario_browser_config->value(i));
				add_scenario_directory(scenario_browser_config->value(i));
			}
		}
	} catch(Errors::FileHandlerException e) {
		LOG(warnings,"Could not open scenario browser config file: " << e.what());
	}
	// now prepare the 'loaded scenario' fields
	//GUI_->output_scenario_browser_name->buffer(BLoadedScenarioName);
	//GUI_->output_scenario_browser_path->buffer(BLoadedScenarioPath);
	
	// All done
	scenario_browser_is_prepared = true;
}
void gui_backend::add_scenario_directory(std::string dir_) {
	if (!get_dir_exists(dir_)) {
		LOG(warnings,"Tried to open nonexistent directory in scenario browser: " << dir_);
		return;
	}
	Fl_Tree* scenBrowser = GUI_->tree_scenario_browser;
	Fl_Tree_Item* scenarioDirectory;
	Subdirectory* sd;
	DEBUG(debugging,"abc");
	ensure_ending(dir_,"/");
	std::string parsedDir = find_replace("/","\\/",dir_);
	if (scenBrowser->find_item(parsedDir.c_str()) == NULL) {
		
		scenarioDirectory = scenBrowser->add(parsedDir.c_str());
		scenarioDirectory->user_data(new scenarioFolder(dir_));
		
		sd = new Subdirectory(dir_,scenBrowser,scenarioDirectory,true,"");
		
	}
    // How many rows we loaded
    //rows((int)_rowdata.size()); 
	// now use ls -d to find the subdirectories, and for each one check for a "0/", "constant/" and "system/" to see if it is a scenario
	// do this by making a subclass of fl_tree_item so that it can store the info/work it out itself
}
	
void gui_backend::insert_subdirectory(std::string dir_, std::string sub_) {
}

void gui_backend::open_test_case() {
	int argc = 3;
	char** argv;
	argv = new char*[argc];
	char* thisWord; int i = 0;
	thisWord = "tritFoamGUI"; argv[i++] = thisWord;
	thisWord = "-case"; argv[i++] = thisWord;
	thisWord = "/home/cstuart/OpenFOAM/cstuart-4.0/run/tRegionFoam/openfoam-045/"; argv[i++] = thisWord;
	Foam::argList args(argc, argv);
	if (!args.checkRootCase())
	{
		Foam::FatalError.exit();
	} else {
		Foam::Time runTime(Foam::Time::controlDictName, args);
		Foam::fvMesh mesh
			(
				Foam::IOobject
				(
					Foam::fvMesh::defaultRegion,
					runTime.timeName(),
					runTime,
					Foam::IOobject::MUST_READ
				)
			);
		Foam::wordList dbNames = mesh.thisDb().names();
		/*
		DEBUG(debugging,dbNames.size());
		for (int i = 0; i < dbNames.size(); ++i) {
			DEBUG(debugging,dbNames[i]);
		}
		dbNames = mesh.cellZones().names();
		DEBUG(debugging,dbNames.size());
		for (int i = 0; i < dbNames.size(); ++i) {
			DEBUG(debugging,dbNames[i]);
		}
		DEBUG(debugging,mesh.cellZones()[mesh.cellZones().findZoneID(dbNames[0])].name());
		*/
		Foam::jobInfo.clear();
	}
}

void gui_backend::select_scenario_from_browser() {
	
	// First, we check whether the selected item was a case or not
	scenarioFolder* selectedScenarioFolder = (scenarioFolder*)GUI_->tree_scenario_browser->callback_item()->user_data();
	if (!selectedScenarioFolder->isScenario) {
		// in that case it must be a folder, so we simply toggle open
		GUI_->tree_scenario_browser->callback_item()->open_toggle();
		return;
	}
	
	std::string selectedScenarioPath = selectedScenarioFolder->fullPath;
	
	if (scenarioIsLoaded) {
		// the current loaded scenario path is stored in scenarioInUse
		if (scenarioInUse == selectedScenarioPath) {
			LOG(gui_alerts,"Scenario selected but is already loaded.");
			return;
		} else {
			Foam::jobInfo.clear();
		}
	}
	
	
	// We now load the case using OpenFOAM classes. To do this, we have
	// to provide what they would normally receive from the commandline:
	int thisArgc = 3;
	
	std::vector<char*> argsVector;
	argsVector.push_back("tritFoamGUI");
	argsVector.push_back("-case");
	argsVector.push_back((char*)selectedScenarioPath.c_str());
	
	char** thisArgv;
	thisArgv = argsVector.data();
	
	LOG(gui_alerts,"Loading scenario: " << selectedScenarioPath);
	// the Foam class spits out output without new lines that my message
	// handling class expected, so I add some here
	cout << std::endl << std::endl;
	Foam::argList thisArgs(thisArgc, thisArgv);
	
	if (!thisArgs.checkRootCase())
	{
		LOG(gui_alerts,"Failed to load scenario into GUI: " << selectedScenarioPath);
	} else {
		// We're going to load the case, but first we'll populate a
		// couple of fields we already know:
		size_t lastSlash = selectedScenarioPath.find_last_of('/');
		size_t secondLastSlash = selectedScenarioPath.find_last_of('/',lastSlash-1);
		
		
		GUI_->output_scenario_browser_name->value(selectedScenarioPath.substr(secondLastSlash+1,lastSlash-1).c_str());
		GUI_->output_scenario_browser_path->value(selectedScenarioPath.c_str());
		
		DEBUG(debugging,"oainsd");
		Foam::Time runTime(Foam::Time::controlDictName, thisArgs);
		DEBUG(debugging,"oiw3ho3t");
		
		fileNameList libNames(runTime.controlDict().lookup("libs"));
		DEBUG(debugging,"892th");
        forAll(libNames, i) {
			DEBUG(debugging,"a " << i);
            runTime.libs().close(libNames[i]);
			DEBUG(debugging,"b " << i);
        }
		DEBUG(debugging,"cccccc");
        
		Foam::fvMesh mesh
			(
				Foam::IOobject
				(
					Foam::fvMesh::defaultRegion,
					runTime.timeName(),
					runTime,
					Foam::IOobject::MUST_READ
				)
			);
		scalar deltaT = runTime.deltaTValue();
		scalar endTime = runTime.endTime().value();
		std::ostringstream oss;
		oss << deltaT;
		GUI_->output_scenario_browser_timestep->value(oss.str().c_str());
		oss.str(""); oss << endTime;
		GUI_->output_scenario_browser_final_time->value(oss.str().c_str());
		
		GUI_->brwsr_scenario_browser_mesh_regions->clear();
		Foam::wordList dbNames = mesh.thisDb().names();
		dbNames = mesh.cellZones().names();
		for (int i = 0; i < dbNames.size(); ++i) {
			GUI_->brwsr_scenario_browser_mesh_regions->add(dbNames[i].c_str());
		}
		/*
		DEBUG(debugging,dbNames.size());
		for (int i = 0; i < dbNames.size(); ++i) {
			DEBUG(debugging,dbNames[i]);
		}
		dbNames = mesh.cellZones().names();
		DEBUG(debugging,dbNames.size());
		for (int i = 0; i < dbNames.size(); ++i) {
			DEBUG(debugging,dbNames[i]);
		}
		DEBUG(debugging,mesh.cellZones()[mesh.cellZones().findZoneID(dbNames[0])].name());
		*/
		//Foam::jobInfo.clear();
	}
	scenarioIsLoaded = true;
	scenarioInUse = selectedScenarioPath;
}

void gui_backend::action(Fl_Widget *sender){
	//LOG(gui_actions, "Action received...");
	if (sender == GUI_->btn_open_recent) {
		LOG(gui_actions, "Opening recent. (Actually a test first button: opening a case!)");
		open_test_case();
	} else if (sender == GUI_->tab_main_tabs) {
		LOG(gui_actions, "Tab changed.");
		action(GUI_->tab_main_tabs->value());
	} else if (sender == GUI_->grp_scenario_browser) {
		LOG(gui_actions, "Opening scenario browser tab.");
		if (!scenario_browser_is_prepared) {
			prepare_scenario_browser();
		}
	} else if (sender == GUI_->btn_load_refresh_scenarios) {
		LOG(gui_actions, "Refreshing browser tab.");
		prepare_scenario_browser();
	} else if (sender == GUI_->tree_scenario_browser) {
		if (GUI_->tree_scenario_browser->callback_reason() == FL_TREE_REASON_RESELECTED) {
			LOG(gui_actions, "Double-clicked on scenario browser.");
			select_scenario_from_browser();
		}
	}
	
	 /*else if (sender == GUI_->btn_save_run_file) {
		LOG(gui_actions, "Saving run file.");
		save_run_file();
	} else if (sender == GUI_->btn_preview_run_file) {
		LOG(gui_actions, "Generating run file preview.");
		preview_run_file();
	} else if (sender == GUI_->btn_revert_run_file) {
		LOG(gui_actions, "Reverting to run file.");
		revert_run_file();
	} else if (sender == GUI_->btn_new_files) {
		LOG(gui_actions, "Changing files/directories.");
		change_files();
	} else if (sender == GUI_->btn_revert_new_files) {
		LOG(gui_actions, "Reverting files/directories.");
		restore_files_memento();
	} else if (sender == GUI_->btn_save_new_files) {
		LOG(gui_actions, "Accepting new files/directories.");
		finish_changing_files();
	} else if (sender == GUI_->btn_run_simulation) {
		LOG(gui_actions, "Running the loaded simulation.");
		run_loaded_simulation();
	} else if (sender == GUI_->main_window) {
		LOG(gui_actions, "Closing application.");
		close_application();
	}*/
}
