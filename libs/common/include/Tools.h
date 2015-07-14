#pragma once
#ifndef __MKAY_TOOLS_H__
#define __MKAY_TOOLS_H__

#include <type_traits>

#if (_MSC_VER < 1900)
	#define to_integral(x) (static_cast<unsigned int>(x))
#else
namespace mkay
{
	template<typename E>
	constexpr auto to_integral(E e) -> typename std::underlying_type<E>::type
	{
		return static_cast<typename std::underlying_type<E>::type>(e);
	}
} // namespace mkay
#endif

#endif // __MKAY_TOOLS_H__