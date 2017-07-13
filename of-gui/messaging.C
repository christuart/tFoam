#include "messaging.H"

extern MessageBuffer exceptions;
extern MessageBuffer warnings;
extern DebugMessageBuffer debugging;

IMessageListener::~IMessageListener() {}
void IMessageListener::add_buffer(IMessageBuffer* _new_listener) { _new_listener->add_listener(this); }

standard_cout_listener::standard_cout_listener() {}
standard_cout_listener::~standard_cout_listener() {}
bool standard_cout_listener::handle_message(std::string msg, IMessageBuffer* sending_buffer, MessageType M) {
	switch (M) {
	case MTMessage:
	case MTMultiline:
		msg = sending_buffer->tag() + "> " + msg;
		if (last_message_buffer_type != sending_buffer->type()) {
			msg = "\n" + msg;
		}
		std::cout << std::endl << msg << std::flush;
		break;
	case MTPhrase:
		if (sending_buffer->type() == MBUnknown || last_message_buffer_type != sending_buffer->type()) {
			msg = "\n" + sending_buffer->tag() + "> " + msg;
		}
		std::cout << msg << std::flush;
		break;
	default:
		throw Errors::MessagingException("Unknown message type for '" + msg + "'.");
	}
	set_last_message_buffer_type(sending_buffer->type());
	return true;
}
bool standard_cout_listener::handle_message(std::string msg, MessageContext, IMessageBuffer* sending_buffer, MessageType M) {
	handle_message(msg, sending_buffer, M);
}
void standard_cout_listener::set_last_message_buffer_type(MessageBufferType _mbt, bool first) {
	last_message_buffer_type = _mbt;
	if (first) error_listener_->set_last_message_buffer_type(_mbt,false);
}

standard_cerr_listener::standard_cerr_listener() {}
standard_cerr_listener::~standard_cerr_listener() {}
bool standard_cerr_listener::handle_message(std::string msg, IMessageBuffer* sending_buffer, MessageType M) {
	switch (M) {
	case MTMessage:
	case MTMultiline:
		msg = sending_buffer->tag() + "> " + msg;
		if (last_message_buffer_type != sending_buffer->type()) {
			msg = "\n" + msg;
		}
		std::cerr << std::endl << msg;
		break;
	case MTPhrase:
		if (sending_buffer->type() == MBUnknown || last_message_buffer_type != sending_buffer->type()) {
			msg = "\n" + sending_buffer->tag() + "> " + msg;
		}
		std::cerr << msg;
		break;
	default:
		throw Errors::MessagingException("Unknown message type for '" + msg + "'.");
	}
	set_last_message_buffer_type(sending_buffer->type());
	return true;
}
bool standard_cerr_listener::handle_message(std::string msg, MessageContext, IMessageBuffer* sending_buffer, MessageType M) {
	handle_message(msg, sending_buffer, M);
}
void standard_cerr_listener::set_last_message_buffer_type(MessageBufferType _mbt, bool first) {
	last_message_buffer_type = _mbt;
	if (first) out_listener_->set_last_message_buffer_type(_mbt,false);
}

error_log_listener::error_log_listener() {
	error_log_file.open("error.log", std::ios_base::app); // ofstream will add ios_base::out
	if (error_log_file.good()) {
		error_log_file << "###==========================================================" << std::endl
		               << "### Error recording starts " << get_timestamp() << std::endl << std::endl;
	} else {
		LOG(warnings,"Error log failed to make start entry.");
	}
}
error_log_listener::~error_log_listener() {
	if (error_log_file.is_open() && error_log_file.good()) {
		error_log_file << "### Error recording ends " << get_timestamp() << std::endl
		               << "###==========================================================" << std::endl << std::endl;
		error_log_file.close();
	} else {
		LOG(warnings,"Error log failed to make finish entry.");
	}
}
bool error_log_listener::handle_message(std::string msg, IMessageBuffer*, MessageType M) {
	if (error_log_file.is_open() && error_log_file.good()) {
		error_log_file << "=== " << get_timestamp() << " === " << std::endl
		               << std::endl << msg << std::endl << std::endl;
		return true;
	}
	LOG(warnings,"Error log failed to make an entry.");
	return false;
}
bool error_log_listener::handle_message(std::string msg, MessageContext context, IMessageBuffer* sending_buffer, MessageType M) {
	if (error_log_file.is_open() && error_log_file.good()) {
		error_log_file << "=== " << get_timestamp() << " === " << std::endl
		               << "=== " << context.function << "(...) in "
		               << context.file << ", L" << context.line << " === " << std::endl
		               << msg << std::endl << std::endl;
		return true;
	}
	LOG(warnings,"Error log failed to make an entry.");
	return false;
}

gui_status_bar_messages::gui_status_bar_messages() {}
gui_status_bar_messages::~gui_status_bar_messages() {}
bool gui_status_bar_messages::handle_message(std::string msg, IMessageBuffer*, MessageType M) {
	buffer()->text(msg.c_str()); return true;
}
bool gui_status_bar_messages::handle_message(std::string msg, MessageContext, IMessageBuffer*, MessageType M) {
	buffer()->text(msg.c_str()); return true;
}

gui_console_messages::gui_console_messages():console_browser_(NULL) {}
gui_console_messages::~gui_console_messages() {}
bool gui_console_messages::handle_message(std::string msg, IMessageBuffer* sending_buffer, MessageType M) {
	if (browser_is_assigned()) {
		int n = browser().size();
		if (sending_buffer->type() == MBUnknown || last_message_buffer_type != sending_buffer->type()) {
			browser().add("");
			++n;
		}
		switch (M) {
		case MTMessage:
		case MTMultiline:
			msg = sending_buffer->tag() + "> " + msg;
			browser().add("");
			++n;
			browser().text(n,msg.c_str());
			break;
		case MTPhrase:
			if (browser().text(n) == "") {
				msg = sending_buffer->tag() + "> " + msg;
			} else {
				msg = browser().text(n) + msg;
			}
			browser().text(n,msg.c_str());
			break;
		default:
			throw Errors::MessagingException("Unknown message type for '" + msg + "'.");
		}
		last_message_buffer_type = sending_buffer->type();
		browser().bottomline(browser().size());
		return true;
	} else {
		return false;
	}
}
bool gui_console_messages::handle_message(std::string msg, MessageContext context, IMessageBuffer* sending_buffer, MessageType M) {
	return handle_message(msg, sending_buffer, M);
}

IMessageBuffer::IMessageBuffer(MessageBufferType _type, std::string _tag):message_buffer_type_(_type),message_buffer_tag_(_tag) {}
IMessageBuffer::~IMessageBuffer() {}

MessageBuffer::MessageBuffer(MessageBufferType _type, std::string _tag):IMessageBuffer(_type,_tag) {}
MessageBuffer::~MessageBuffer() {}
void MessageBuffer::add_listener(IMessageListener* _new_listener) {
	listeners_.insert(_new_listener);
}
bool MessageBuffer::remove_listener(IMessageListener* _new_listener) {
	return listeners_.erase(_new_listener);
}
void MessageBuffer::message(std::string msg, MessageType M) {
	typedef std::set<IMessageListener*>::iterator it_type;
	for (it_type it = listeners_.begin(); it != listeners_.end(); ++it)
		(*it)->handle_message(msg,this,M);
}

DebugMessageBuffer::DebugMessageBuffer(MessageBufferType _type, std::string _tag):MessageBuffer(_type,_tag) {}
DebugMessageBuffer::~DebugMessageBuffer() {}
void DebugMessageBuffer::message(std::string msg, MessageType M) {
	
#ifndef SILENT_DEBUG
	MessageBuffer::message(msg,M);
#endif
	
}
void DebugMessageBuffer::message(std::string msg, MessageContext context, MessageType M) {
	
#ifndef SILENT_DEBUG
	typedef std::set<IMessageListener*>::iterator it_type;
	for (it_type it = listeners_.begin(); it != listeners_.end(); ++it)
		(*it)->handle_message(msg,context,this,M);
#endif
	
}
