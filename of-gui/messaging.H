#ifndef MESSAGE_HH
#define MESSAGE_HH

// remove surrounding underscores in order to silence debug messages
// best to do this as a compiler option "-D SILENT_DEBUG"
#define _SILENT_DEBUG_

#include <set>
#include <string>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_Browser.H>
#include "utilities.H"
#include "exceptions.H"

class IMessageBuffer;
class IMessageListener;

struct MessageContext {
	MessageContext(const char* _fn, const char* _fl, int _l):function(_fn),
	                                                                 file(_fl),
	                                                                 line(_l) {}
	const char* function;
	const char* file;
	int line;
};
enum MessageType {
	MTMessage,
	MTPhrase,    // doesn't add a new line at end
	MTMultiline  // expected to contain multiple lines
};
enum MessageBufferType {
	MBUnknown,
	MBGuiStatus,
	MBGuiAlerts,
	MBGuiActions,
	MBTdsLog,
	MBUnhandledException,
	MBRunFileProcessing,
	MBUnitsProcessing,
	MBWarnings,
	MBDebug,
	MBSimulationOutput
};
// This piece of cleverness borrowed from Matthieu M. at
// http://stackoverflow.com/a/8338089/2569197
#define DEBUG(Logger_, Message_)              \
Logger_.message(                              \
  static_cast<std::ostringstream&>(           \
    std::ostringstream().flush() << Message_  \
  ).str(),                                    \
  MessageContext(__FUNCTION__,                \
                 __FILE__,                    \
                 __LINE__)                    \
);
#define LOG(Logger_, Message_)                \
Logger_.message(                              \
  static_cast<std::ostringstream&>(           \
    std::ostringstream().flush() << Message_  \
  ).str()                                     \
);
#define LOGPHRASE(Logger_, Message_)          \
Logger_.message(                              \
  static_cast<std::ostringstream&>(           \
    std::ostringstream().flush() << Message_  \
  ).str(),                                    \
  MTPhrase                                    \
);
#define LOGMULTI(Logger_, Message_)           \
Logger_.message(                              \
  static_cast<std::ostringstream&>(           \
    std::ostringstream().flush() << Message_  \
  ).str(),                                    \
  MTMultiline                                 \
);


class IMessageBuffer {
protected:
	MessageBufferType message_buffer_type_;
	std::string message_buffer_tag_;
	// We could define the collection of IMessageListener pointers here as
	// all buffers will need one, but that will inadvertently push a
	// specific implementation, i.e. using std::vector allowing multiple
	// of the same vs. using std::set preventing multiple of the same
public:
	IMessageBuffer(MessageBufferType _type=MBUnknown, std::string _tag="UNKNOWN     ");
	virtual ~IMessageBuffer()=0;
	virtual void add_listener(IMessageListener* _new_listener)=0;
	virtual bool remove_listener(IMessageListener* _new_listener)=0;
	virtual void message(std::string msg, MessageType M=MTMessage)=0;
	inline MessageBufferType type() { return message_buffer_type_; }
	inline std::string tag() { return message_buffer_tag_; }
};

class MessageBuffer : public IMessageBuffer {
protected:
	std::set<IMessageListener*> listeners_;
	
public:
	MessageBuffer(MessageBufferType _type=MBUnknown, std::string _tag="UNKNOWN     ");
	virtual ~MessageBuffer();
	void add_listener(IMessageListener* _new_listener);
	bool remove_listener(IMessageListener* _new_listener);
	virtual void message(std::string msg, MessageType M=MTMessage);
};

class DebugMessageBuffer : public MessageBuffer {
public:
	DebugMessageBuffer(MessageBufferType _type=MBUnknown, std::string _tag="UNKNOWN     ");
	virtual ~DebugMessageBuffer();
	void message(std::string msg, MessageType M=MTMessage);
	void message(std::string msg, MessageContext context, MessageType M=MTMessage);
};

class IMessageListener {
public:
	virtual ~IMessageListener();
	virtual void add_buffer(IMessageBuffer* _new_listener);
	virtual bool handle_message(std::string msg, IMessageBuffer* _originator, MessageType M=MTMessage)=0;
	virtual bool handle_message(std::string msg, MessageContext context, IMessageBuffer* _originator, MessageType M=MTMessage)=0;
};
class standard_cerr_listener;
class standard_cout_listener : public IMessageListener {
private:
	MessageBufferType last_message_buffer_type;
	standard_cerr_listener* error_listener_;
public:
	standard_cout_listener();
	~standard_cout_listener();
	bool handle_message(std::string, IMessageBuffer*, MessageType M=MTMessage);
	bool handle_message(std::string, MessageContext, IMessageBuffer*, MessageType M=MTMessage);
	void set_last_message_buffer_type(MessageBufferType _mbt, bool first = true);
	inline void error_listener(standard_cerr_listener* _error_listener) { error_listener_ = _error_listener; }
};
class standard_cerr_listener : public IMessageListener {
private:
	MessageBufferType last_message_buffer_type;
	standard_cout_listener* out_listener_;
public:
	standard_cerr_listener();
	~standard_cerr_listener();
	bool handle_message(std::string, IMessageBuffer*, MessageType M=MTMessage);
	bool handle_message(std::string, MessageContext, IMessageBuffer*, MessageType M=MTMessage);
	void set_last_message_buffer_type(MessageBufferType _mbt, bool first = true);
	inline void out_listener(standard_cout_listener* _out_listener) { out_listener_ = _out_listener; }
};
class error_log_listener : public IMessageListener {
private:
	std::ofstream error_log_file;
public:
	error_log_listener();
	~error_log_listener();
	bool handle_message(std::string, IMessageBuffer*, MessageType M=MTMessage);
	bool handle_message(std::string, MessageContext, IMessageBuffer*, MessageType M=MTMessage);
};

class gui_status_bar_messages : public IMessageListener, public Fl_Text_Buffer {
public:
	gui_status_bar_messages();
	~gui_status_bar_messages();
	bool handle_message(std::string, IMessageBuffer*, MessageType M=MTMessage);
	bool handle_message(std::string, MessageContext, IMessageBuffer*, MessageType M=MTMessage);
	inline Fl_Text_Buffer* buffer() { return static_cast<Fl_Text_Buffer*>(this); }
};

class gui_console_messages : public IMessageListener {
private:
	MessageBufferType last_message_buffer_type;
	Fl_Browser* console_browser_;
private:
	inline bool browser_is_assigned() { return (console_browser_ != NULL); }
public:
	gui_console_messages();
	~gui_console_messages();
	bool handle_message(std::string, IMessageBuffer*, MessageType M=MTMessage);
	bool handle_message(std::string, MessageContext, IMessageBuffer*, MessageType M=MTMessage);
	// Setters
	inline void browser(Fl_Browser* _browser) { console_browser_ = _browser; }
	// Getters
	inline Fl_Browser& browser() { return *(console_browser_); }
};

#endif
