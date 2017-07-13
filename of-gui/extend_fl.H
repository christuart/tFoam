#ifndef EXTEND_FL_H
#define EXTEND_FL_H

#include <FL/Fl.H>
#include <FL/Fl_Tree.H>
#include "utilities.H"
#include "messaging.H"
#include <FL/Fl_Image.H>
#include <FL/Fl_Pixmap.H>

struct scenarioFolder {
		scenarioFolder(std::string _fullPath):isScenario(false),
		               fullPath(_fullPath) {}
		bool isScenario;
		std::string fullPath;
	};

class Subdirectory;

typedef std::pair<std::string, Subdirectory> subdirectoryItem;

static const char *sml[] = {                       // XPM
  "9 11 5 1",
  ".  c None",
  "@  c #000000",
  "+  c #808080",
  "r  c #802020",
  "#  c #ff8080",
  ".........",
  ".........",
  "@+.......",
  "@@@+.....",
  "@@r@@+...",
  "@@##r@@+.",
  "@@####r@@",
  "@@##r@@+.",
  "@@r@@+...",
  "@@@+.....",
  "@+.......",
};

class Subdirectory {
private:
	std::string subdirectoryPath_;
	std::string subdirectoryLabel_;
	Fl_Tree* parentTree_;
	Fl_Tree_Item* treeItem_;
	std::vector<subdirectoryItem> contents_;
	bool scouted;
	Fl_Image* scenarioIcon;
	
private:
	void insertItem(Fl_Tree_Item* _parentItem);
public:
	Subdirectory(std::string, Fl_Tree*, Fl_Tree_Item*, bool, std::string);
	virtual ~Subdirectory();
	
	inline std::string subdirectoryPath() { return subdirectoryPath_; }
	inline Fl_Tree_Item* treeItem() { return treeItem_; }
	inline void treeItem(Fl_Tree_Item* _newItem) { treeItem_ = _newItem; }
	
	void scoutDirectory();
	
};

#endif
