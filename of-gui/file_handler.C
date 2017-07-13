#include "file_handler.H"

extern MessageBuffer exceptions;
extern MessageBuffer warnings;
extern DebugMessageBuffer debugging;

file_handler::file_handler(file_purpose _purpose, std::string _file_name, file_buffer_mode _buffer_mode, int _key_width):purpose_(_purpose),file_name_(_file_name),buffer_mode_(_buffer_mode),key_width_(_key_width),file_(),file_complete(false),file_unavailable(false) {
	if (file_name() == "" && purpose() != StringOnly) {
		throw Errors::FileHandlerException("Missing file name when trying to instantiate a file_handler object.");
	}
	file_.exceptions(std::fstream::badbit);
	furthest_line = 0;
	line_n = 0;
	
}
file_handler::~file_handler() {
	close_file();
}

void file_handler::pick_file_up() {
	if (!file_unavailable && !file_.is_open()) {
		try {
			if (purpose_ == WriteToFile) {
				file_.open(file_name().c_str(),std::ios_base::out | std::ios_base::trunc);
			} else if (purpose_ == AppendToFile) {
				file_.open(file_name().c_str(),std::ios_base::out | std::ios_base::app);
			} else if (purpose_ == ReadFromFile) {
				file_.open(file_name().c_str(),std::ios_base::in);
			}
		} catch (std::fstream::failure& e) {
			if (purpose_ == WriteToFile || purpose_ == AppendToFile) {
				LOG(warnings, "File exception whilst accessing '" << file_name()
				    << "'. Try checking if the target folder is available.");
			} else if (purpose_ == ReadFromFile) {
				LOG(warnings, "File exception whilst accessing '" << file_name()
				    << "'. Check whether the file exists.");
			}
			file_unavailable = true;
		}
	}
}

void file_handler::put_file_down() {
	if (buffer_mode_ == Hold) {
		if (file_.is_open()) {
			file_.close();
		}
	}
}

void file_handler::read_line() {
	// Not to be called without pick_file_up called before it and put_file_down called after it
	if (purpose() != ReadFromFile) {
		throw Errors::FileHandlerException("Trying to read with the wrong type of file_handler.");
	}
	if (!file_unavailable && !file_complete) {
		std::string l; // full line
		if (std::getline(file_, l)) {
			std::istringstream iss(l);
			std::string k, v; // key and value
			DEBUG(debugging,"Doing a line read on '" << l << "'");
			if (iss >> k) {
				DEBUG(debugging,"k was '" << k << "'");
				if (!std::getline(iss,v)) {
					v = "";
				} else {
					DEBUG(debugging,"v was '" << v << "'");
				}
			} else {
				k = "";
				v="";
			}
			trim(k);
			trim(v);
			expand_key_width(k);
			entries.push_back(std::pair<std::string,std::string>(k,v));
			++furthest_line;
		} else {
			file_complete = true;
		}
	}
}
void file_handler::read_to_line(int n) {
	pick_file_up();
	while (!file_complete && !file_unavailable && furthest_line < n) {
		read_line();
	}
	put_file_down();
	if (furthest_line < n) {
		std::ostringstream oss;
		oss << "File stopped at line " << furthest_line << ".";
		throw Errors::FileHandlerLineUnavailableException(oss.str());
	}
}
void file_handler::read_to_end() {
	if (file_complete || file_unavailable) return;
	pick_file_up();
	while (!file_complete && !file_unavailable) {
		read_line();
	}
	put_file_down();
}
void file_handler::write_line() {
	// Not to be called without pick_file_up called before it and put_file_down called after it
	if ((purpose() != WriteToFile) && (purpose() != AppendToFile)) {
		throw Errors::FileHandlerException("Trying to write with the wrong type of file_handler.");
	}
	if (!file_unavailable && !file_complete) {
		++furthest_line;
		if (furthest_line < line_n) {
			line_n = 1;
			line_it = entries.begin();
		}
		while (line_n < furthest_line) {
			++line_n;
			++line_it;
		}
		std::ostringstream oss;
		oss << std::left << std::setw(key_width_) << (*line_it).first << (*line_it).second;
		std::string line_to_write = oss.str();
		trim(line_to_write);
		file_ << line_to_write << std::endl;
		if (furthest_line == entries.size())
			file_complete = true;
	}
}
void file_handler::write_to_line(int n) {
	pick_file_up();
	while (furthest_line < n && !file_unavailable && !file_complete) {
		write_line();
	}
	if (n > entries.size()) {
		std::ostringstream oss;
		oss << "Trying to write up to line " << n << " out of " << entries.size() << ".";
		throw Errors::FileHandlerLineUnavailableException(oss.str());
	}
	put_file_down();
}
void file_handler::write_to_end() {
	pick_file_up();
	while (!file_unavailable && !file_complete) {
		write_line();
	}
	put_file_down();
}
void file_handler::close_file() {
	file_.close();
}
void file_handler::expand_key_width(std::string new_key) {
	int w = new_key.length();
	key_width_ = std::max(w,key_width_);
}

const std::pair<std::string,std::string> file_handler::line(int n) {
	DEBUG(debugging,"line requested: " << n);
	if (purpose() == ReadFromFile && n > furthest_line) {
		DEBUG(debugging,"need to read more file");
		// We will need to read more of the file
		try {
			read_to_line(n);
		} catch (Errors::FileHandlerLineUnavailableException& e) {
			std::ostringstream oss;
			oss << "Whilst looking for line " << n << " encountered:" << std::endl << "\t" << e.what();
			throw Errors::FileHandlerLineUnavailableException(oss.str());
		}
	}
	// We know that if the line exists, we have it in memory
	if (n > entries.size() || n < 1) {
		std::ostringstream oss;
		oss << "Line " << n << " does not exist. (Lines are 1-indexed.)";
		throw Errors::FileHandlerLineUnavailableException(oss.str());
	}
	DEBUG(debugging,"Currently on line_n=" << line_n);
	// We know that the line exists and that we already have it in memory
	if (n < line_n || line_n < 1) {
		line_n = 1;
		line_it = entries.begin();
	}
	while (line_n < n) {
		++line_n;
		++line_it;
	}
	return *line_it;
}
const std::string file_handler::line_str(int n) {
	std::ostringstream oss;
	oss << std::left << std::setw(key_width_) << line(n).first << line(n).second;
	return oss.str();
}
const std::string file_handler::key(int n) {
	return line(n).first;
}
const std::string file_handler::value(int n) {
	return line(n).second;
}
// return value is whether or not the change was written to file
bool file_handler::line(int n, std::string _key, std::string _value) {
	return line(n, std::pair<std::string,std::string>(_key,_value));
}
bool file_handler::line(int n, std::string _line) {
	return line(n, std::pair<std::string,std::string>(_line,""));
}
bool file_handler::line(int n, std::pair<std::string,std::string> _p) {
	if (purpose() == ReadFromFile) {
		throw Errors::FileHandlerException("Trying to write to file opened for read.");
	}
	if (n < 1)
		n = entries.size()+1;
	if (n <= furthest_line) {
		throw Errors::FileHandlerException("Trying to rewrite lines already stored in file.");
	}
	if (n > entries.size()) {
		if (n == entries.size()+1) {
			entries.push_back(_p);
		} else {
			throw Errors::FileHandlerLineUnavailableException("Trying to write beyond one new line.");
		}
	} else {
		if (n < line_n) {
			line_n = 1;
			line_it = entries.begin();
		}
		while (line_n < n) {
			++line_n;
			++line_it;
		}
		*line_it = _p;
	}
	if (buffer_mode_ == Flush) {
		write_to_line(n);
		return true;
	} else {
		file_complete = false;
		return false;
	}
}
bool file_handler::key(int n, std::string _key) {
	if (purpose() == ReadFromFile) {
		throw Errors::FileHandlerException("Trying to write to file opened for read.");
	}
	if (n < 1)
		n = entries.size()+1;
	if (n <= furthest_line) {
		throw Errors::FileHandlerException("Trying to rewrite lines already stored in file.");
	}
	if (n > entries.size()) {
		if (n == entries.size()+1) {
			throw Errors::FileHandlerLineUnavailableException("Trying to write new line with only a key.");
		} else {
			throw Errors::FileHandlerLineUnavailableException("Trying to write beyond one new line.");
		}
	} else {
		if (n < line_n) {
			line_n = 1;
			line_it = entries.begin();
		}
		while (line_n < n) {
			++line_n;
			++line_it;
		}
		(*line_it).first = _key;
	}
	if (buffer_mode_ == Flush) {
		write_to_line(n);
		return true;
	} else {
		file_complete = false;
		return false;
	}
}
bool file_handler::value(int n, std::string _value) {
	if (purpose() == ReadFromFile) {
		throw Errors::FileHandlerException("Trying to write to file opened for read.");
	}
	if (n < 1)
		n = entries.size()+1;
	if (n <= furthest_line) {
		throw Errors::FileHandlerException("Trying to rewrite lines already stored in file.");
	}
	if (n > entries.size()) {
		if (n == entries.size()+1) {
			throw Errors::FileHandlerLineUnavailableException("Trying to write new line with only a value.");
		} else {
			throw Errors::FileHandlerLineUnavailableException("Trying to write beyond one new line.");
		}
	} else {
		if (n < line_n) {
			line_n = 1;
			line_it = entries.begin();
		}
		while (line_n < n) {
			++line_n;
			++line_it;
		}
		(*line_it).second = _value;
	}
	if (buffer_mode_ == Flush) {
		write_to_line(n);
		return true;
	} else {
		file_complete = false;
		return false;
	}
}
void file_handler::read() {
	read_to_end();
}
void file_handler::write() {
	write_to_end();
}
