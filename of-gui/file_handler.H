#ifndef FILE_HANDLER_HH
#define FILE_HANDLER_HH

#define DEFAULT_FILE_KEY_WIDTH 20

#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <set>
#include "messaging.H"

enum file_purpose {
	WriteToFile,
	AppendToFile,
	ReadFromFile,
	StringOnly
};
enum file_buffer_mode {
	Flush = 1, // The file stays open because you push data into it as soon as you find it
	Step = 1,
	Open = 1,
	Hold = 2, // The file is closed after any reading/writing (you hold the data until ready to write)
	Wait = 2,
	Closed = 2
};

typedef std::vector<std::pair<std::string,std::string>>::iterator entry_it;

class file_handler {
private:
	std::string file_name_;
	std::fstream file_;
	file_purpose purpose_;
	file_buffer_mode buffer_mode_;
	int key_width_;
	std::vector<std::pair<std::string, std::string>> entries;
	
	int furthest_line; // this contains the furthest (new) line read/written
	int line_n;
	entry_it line_it; // this 
	bool file_complete; // this indicates whether the entire file was written/read yet
	bool file_unavailable; // this indicates if this file_handler has encountered exceptions
	
private:
	void initialise();
	void pick_file_up();
	void put_file_down();
	void read_line();
	void read_to_line(int n);
	void read_to_end();
	void write_line();
	void write_to_line(int n);
	void write_to_end();
	void close_file();
	void expand_key_width(std::string new_key);
public:
	file_handler(file_purpose _purpose, std::string _file_name="", file_buffer_mode _buffer_mode=Flush, int _key_width=DEFAULT_FILE_KEY_WIDTH);
	~file_handler();
	inline std::string file_name() { return file_name_; }
	inline file_purpose purpose() { return purpose_; }
	inline int lines_read() { return furthest_line; }
	inline bool file_completed() { return file_complete; }
	
	const std::pair<std::string,std::string> line(int n);
	const std::string line_str(int n);
	const std::string key(int n);
	const std::string value(int n);

	// these public functions which change the values in entries are only
	// for use with WriteTo- or AppendTo- File.
	bool line(int n, std::pair<std::string,std::string> _p);
	bool line(int n, std::string _line);
	bool line(int n, std::string _key, std::string _value);
	bool key(int n, std::string _key);
	bool value(int n, std::string _value);
	void read();
	void write();
};

#endif
