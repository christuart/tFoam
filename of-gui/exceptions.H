#ifndef EXCEPTION_HH
#define EXCEPTION_HH

#include <exception>
#include <stdexcept>
namespace Errors {
	class FutureImplementationException : public std::logic_error {
	public:
		explicit FutureImplementationException(const std::string& what_arg):std::logic_error("Missing implementation: " + what_arg) {}
	};
	class AlgorithmFailedException : public std::logic_error {
	public:
		explicit AlgorithmFailedException(const std::string& what_arg):std::logic_error("Algorithm failed: " + what_arg) {}
	};
	class MessagingException : public std::logic_error {
	public:
		explicit MessagingException(const std::string& what_arg):std::logic_error("Messaging exception: " + what_arg) {}
	};
	class PluginException : public std::logic_error {
	public:
		explicit PluginException(const std::string& what_arg):std::logic_error("(plugin exception) " + what_arg) {}
	};
	class PluginNotInMapException : public PluginException {
	public:
		explicit PluginNotInMapException(const std::string& what_arg):PluginException("String does not map to known plug-in: " + what_arg) {}
	};
	class PluginNotInSwitchException : public PluginException {
	public:
		explicit PluginNotInSwitchException(const std::string& what_arg):PluginException("Mapped plug-in missing constructor call: " + what_arg) {}
	};
	class PluginIncompleteImplementationException : public PluginException {
	public:
		explicit PluginIncompleteImplementationException(const std::string& what_arg):PluginException("Call to missing implementation in plug-in: " + what_arg) {}
	};
	/*
	  class  : public std::logic_error {
	  public:
	  explicit (const std::string& what_arg):std::logic_error("Problem type: " + what_arg) {}
	  };
	*/

	class MissingInputDataException : public std::runtime_error {
	public:
		explicit MissingInputDataException(const std::string& what_arg):std::runtime_error("Missing input data: " + what_arg) {}
	};
	class BadInputDataException : public std::runtime_error {
	public:
		explicit BadInputDataException(const std::string& what_arg):std::runtime_error("Bad input data: " + what_arg) {}
	};
	class InvalidMaterialPropertyException : public BadInputDataException {
		// This exception will later take in the material name and
		// the invalid value
	public:
		explicit InvalidMaterialPropertyException(const std::string& what_arg):BadInputDataException(what_arg) {}
	};
	class ConvertingMissingUnitException : public BadInputDataException {
	public:
		// This exception will later take in the unknown unit name, the dimension
		// and a description of where (in the code) it was being converted
		explicit ConvertingMissingUnitException(const std::string& what_arg):BadInputDataException(what_arg) {}
	};
	class FileHandlerException : public std::runtime_error {
	public:
		explicit FileHandlerException(const std::string& what_arg):std::runtime_error("File handling error: " + what_arg) {}
	};
	class FileHandlerLineUnavailableException : public FileHandlerException {
	public:
		explicit FileHandlerLineUnavailableException(const std::string& what_arg):FileHandlerException(what_arg) {}
	};
	class BadRunFileException : public std::runtime_error {
	public:
		explicit BadRunFileException(const std::string& what_arg):std::runtime_error("Run file error: " + what_arg) {}
	};
	class EarlyRunFileUnitsException : public BadRunFileException {
	public:
		explicit EarlyRunFileUnitsException(const std::string& what_arg):BadRunFileException("Cannot interpret without units; need to set the config file before line:\n\t" + what_arg) {}
	};

	class AnalysisException : public std::runtime_error {
	public:
		explicit AnalysisException(const std::string& what_arg):std::runtime_error(what_arg) {}
	};
	class LowSimulationAccuracyException : public AnalysisException {
	public:
		explicit LowSimulationAccuracyException(const std::string& what_arg):AnalysisException("Unacceptable inaccuracies: " + what_arg) {};
	};
	class NegativeContaminationException : public AnalysisException {
	public:
		explicit NegativeContaminationException():AnalysisException("Trying to store a negative contamination. ") {}
	};
	class InvalidContaminationException : public AnalysisException {
	public:
		explicit InvalidContaminationException():AnalysisException("Trying to store a negative contamination. ") {}
	};

	class PluginRuntimeException : public std::runtime_error {
	public:
		explicit PluginRuntimeException(const std::string& what_arg):std::runtime_error("(plugin runtime exception) " + what_arg) {};
	};

	class UIException : public std::runtime_error {
	public:
		explicit UIException(const std::string& what_arg):std::runtime_error("UI Exception: " + what_arg) {}
	};

	class VectorOutOfBoundsException : public std::runtime_error {
	public:
		explicit VectorOutOfBoundsException(const std::string& what_arg):std::runtime_error("Vector index out of bounds: " + what_arg) {}
	  };
	
	class NotANeighbourException : public std::runtime_error {
	public:
		explicit NotANeighbourException():std::runtime_error("Requested the neighbour of an element not belonging to the link.") {}
	};
	
	/*
	  class  : public std::runtime_error {
	  public:
	  explicit (const std::string& what_arg):std::runtime_error("Problem type: " + what_arg) {}
	  };
	*/
}

#endif
