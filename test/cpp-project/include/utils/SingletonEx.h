// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)

// modify by wfh(wang007987@163.com)

#pragma once

#include "Noncopyable.h"

#include <assert.h>
#include <stdlib.h> // atexit
#include <mutex>

namespace util
{
	namespace detail
	{
		// This doesn't detect inherited member functions!
		// http://stackoverflow.com/questions/1966362/sfinae-to-check-for-inherited-member-functions
		template <typename T>
		struct has_no_destroy
		{
			template <typename C>
			static char test(decltype(&C::no_destroy));
			template <typename C>
			static int32_t test(...);
			const static bool value = sizeof(test<T>(0)) == 1;
		};
	} // namespace detail

	template <typename T>
	class SingletonEx : Noncopyable
	{
	public:
		SingletonEx() = delete;
		~SingletonEx() = delete;

		template <class... Args>
		static T& Instance(Args &&...args)
		{
			std::call_once(m_once, &SingletonEx::init<Args...>, std::forward<Args>(args)...);
			assert(value_ != NULL);
			return *value_;
		}

	private:
		template <class... Args>
		static void init(Args &&...args)
		{
			value_ = new T(std::forward<Args>(args)...);
			if (!detail::has_no_destroy<T>::value)
			{
				::atexit(destroy);
			}
		}

		static void destroy()
		{
			typedef char T_must_be_complete_type[sizeof(T) == 0 ? -1 : 1];
			T_must_be_complete_type dummy;
			(void)dummy;

			delete value_;
			value_ = NULL;
		}

	private:
		static std::once_flag m_once;
		static T *value_;
	};

	template <typename T>
	std::once_flag SingletonEx<T>::m_once;

	template <typename T>
	T *SingletonEx<T>::value_ = NULL;

}
