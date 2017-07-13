	
#include "extend_fl.H"

extern MessageBuffer exceptions;
extern MessageBuffer warnings;
extern DebugMessageBuffer debugging;

typedef std::pair<std::string, Subdirectory> subdirectoryItem;

Subdirectory::Subdirectory(std::string _subdirectoryPath, Fl_Tree* _parentTree, Fl_Tree_Item* _treeItem, bool scoutDeeper = false, std::string _subdirectoryLabel = ""):
			subdirectoryPath_(_subdirectoryPath),parentTree_(_parentTree),treeItem_(_treeItem),subdirectoryLabel_(_subdirectoryLabel) {
	scouted = false;
	if (scoutDeeper) {
		if (get_dir_exists(_subdirectoryPath)) {
			scoutDirectory();
			scouted = true;
		} else {
			LOG(warnings,"Nonexistent subdirectory created: " << _subdirectoryPath);
		}
	}
	scenarioIcon = new Fl_Pixmap(sml);
}

Subdirectory::~Subdirectory() {}

void Subdirectory::scoutDirectory() {
	
	char s[512];
	//std::string escapedDir = find_replace(" ","\\ ",find_replace("\\ "," ",subdirectoryPath()));
	std::string spaceEscapedDir = subdirectoryPath();
	DEBUG(debugging,"::: " << subdirectoryPath());
	std::string lsCommand = "cd \"" + spaceEscapedDir + "\"; ls -d */ 2>/dev/null";
	//std::string lsCommand = "cd \"" + spaceEscapedDir + "\"; find * -maxdepth 0 -type d";
	
	std::string caseDir1 = "constant/";
	std::string caseDir2 = "system/";
	std::string caseDir3 = "0/";
	int caseDirs = 0;
	
	//DEBUG(debugging,lsCommand);
	if (true || get_dir_exists(subdirectoryPath() + "*")) {
		//DEBUG(debugging,"opening popen");
		FILE *fp = popen(lsCommand.c_str(), "r");
		//DEBUG(debugging,"opened popen" << lsCommand);
		for ( int r=0; fgets(s, sizeof(s)-1, fp); r++ ) {
			//DEBUG(debugging,"abc" << r);
			char *ss;
			//const char *delim = " \t\n";
			const char *delim = "\t\n";
			for(int t=0; (t==0)?(ss=strtok(s,delim)):(ss=strtok(NULL,delim)); t++) {
				if (true || ss[0] != '/') {
					if (caseDirs < 2) {
						char* newSubdirectoryChar = strdup(ss);
						std::string newSubdirectoryString = std::string(newSubdirectoryChar);
						trim(newSubdirectoryString);
						DEBUG(debugging, "checking the string '" << newSubdirectoryString << "'");
						if (newSubdirectoryString == caseDir1 ||
							newSubdirectoryString == caseDir2 ||
							newSubdirectoryString == caseDir3)
							caseDirs++;
						//DEBUG(debugging, "caseDirs is now " << caseDirs);
						//std::string a;
						//std::cin >> a;
						std::string newLabel = find_replace("/","/",newSubdirectoryString);
						std::string newSubdirectoryPath = subdirectoryPath();
						ensure_ending(newSubdirectoryPath,"/");
						newSubdirectoryPath += newSubdirectoryString;
						Subdirectory newSubdirectory(newSubdirectoryPath,parentTree_,NULL,false,newLabel);
						subdirectoryItem newSubdirectoryItem(newSubdirectoryPath,newSubdirectory);
						contents_.push_back(newSubdirectoryItem);
						//scenarioDirectory->add(scenarioDirectory->prefs(),strdup(ss),(Fl_Tree_Item*)(new SubdirectoryItem()));
						//GUI_->tree_scenario_browser->add(scenarioDirectory,strdup(ss));
					}
				}
			}
			// Keep track of max # columns
			//if ( (int)rc.size() > cols() ) {
			//    cols((int)rc.size());
			//}
		}
		pclose(fp);
	}
	treeItem()->user_data(new scenarioFolder(subdirectoryPath()));
	if (caseDirs < 2) {
		for (std::vector<subdirectoryItem>::iterator it = contents_.begin(); it != contents_.end(); it++) {
			it->second.insertItem(treeItem());
			it->second.scoutDirectory();
		}
	} else {
		treeItem()->usericon(scenarioIcon);
		((scenarioFolder*)treeItem()->user_data())->isScenario = true;
	}
}

void Subdirectory::insertItem(Fl_Tree_Item* _parentItem) {
	treeItem(parentTree_->add(_parentItem,subdirectoryLabel_.c_str()));
	treeItem()->close();
}
