#pragma once
#ifndef __SIGNAL_HANDLER_H__
#define __SIGNAL_HANDLER_H__

#ifdef WIN32
#include <signal.h>
#else
#include <unistd.h>
#endif

#include <cstdint>
#include <map>
#include <functional>

namespace mkay
{
	enum class Signal : std::int8_t
	{
		NOT_AVAILABLE = 0,
		INT = SIGINT,
		TERM = SIGTERM,
#ifdef SIGQUIT
		QUIT = SIGQUIT
#else
		QUIT = 0
#endif
	};

	class Signal_handler
	{
	public:
		typedef std::function<void (mkay::Signal)> signal_handler_func;

		void set(Signal signal, signal_handler_func signal_handler);

		Signal_handler();

	private:
		std::map<Signal, signal_handler_func> m_functions;
	};

	std::ostream & operator<<(std::ostream &ostr, const Signal & signal);
} // namespace mkay

#endif // __SIGNAL_HANDLER_H__