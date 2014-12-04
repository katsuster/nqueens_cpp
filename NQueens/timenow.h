#ifndef TIMENOW_H__
#define TIMENOW_H__

#include <stdexcept>

#if defined(_WIN32)
#	include <Windows.h>
#elif defined(__unix)
#	include <sys/time.h>
#endif

namespace katsuster {
	class TimeNow {
	public:
		static long long microtime() {
			long long ull;

#if defined(_WIN32)
			FILETIME ft;
			ULARGE_INTEGER u;

			//resolution: 100[ns]
			GetSystemTimeAsFileTime(&ft);
			u.HighPart = ft.dwHighDateTime;
			u.LowPart = ft.dwLowDateTime;

			ull = u.QuadPart / 10;
#elif defined(__unix)
			struct timeval tv;
			int result;

			result = gettimeofday(&tv, NULL);
			if (result != 0) {
				throw runtime_error("gettimeofday() failed.");
			}

			ull = (long long)tv.tv_sec * 1000000
				+ (long long)tv.tv_usec;
#endif

			return ull;
		}
	};
}

#endif //TIMENOW_H__
