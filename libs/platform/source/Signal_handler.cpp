#include <platform/include/Signal_handler.h>

#include <common/include/Logger.h>

using std::endl;

namespace mkay
{
	Signal_handler::Signal_handler()
	{
	}

	void Signal_handler::set(Signal signal, signal_handler_func signal_handler)
	{
		if (signal == Signal::NOT_AVAILABLE)
		{
			logdeb << "Signal will be ignored: " << signal << endl;
		}
		else
		{
			m_functions[signal] = signal_handler;
		}
	}

	std::ostream & operator<<(std::ostream &ostr, const Signal & signal)
	{
		switch (signal)
		{
		case Signal::INT: ostr << "SIGINT" << endl; break;
		case Signal::TERM: ostr << "SIGINT" << endl; break;
		case Signal::QUIT: ostr << "SIGQUIT" << endl; break;
		default: ostr << "<UNKNOWN>" << endl; break;
		}
		return ostr;
	}

}
