#ifndef THREAD_H__
#define THREAD_H__

#include <stdexcept>

#if defined(_WIN32)
#	include <Windows.h>
#elif defined(__unix)
#	include <signal.h>
#	include <pthread.h>
#endif

#include "runnable.h"

using namespace std;

namespace katsuster {
	class Thread {
	public:
		Thread() : r(NULL) {
			//do nothing
		}

		Thread(Runnable *r) : r(r) {
#if defined(_WIN32)
			hthr = NULL;
			tid = 0;
#elif defined(__unix)
			thr_exist = 0;
#endif
		}

		virtual ~Thread() {
#if defined(_WIN32)
			if (hthr) {
				CloseHandle(hthr);
				hthr = NULL;
				tid = 0;
			}
#elif defined(__unix)
			if (thr_exist) {
				pthread_detach(thr);
				pthread_kill(thr, SIGKILL);
				thr_exist = 0;
			}
#endif
		}

		void start() {
#if defined(_WIN32)
			hthr = CreateThread(NULL, 0, 
				ThreadProc, this, 0, &tid);
			if (hthr == NULL) {
				throw runtime_error(
					"CreateThread() failed.");
			}
#elif defined(__unix)
			int result;

			result = pthread_create(&thr, NULL, 
				start_routine, this);
			if (result != 0) {
				throw runtime_error(
					"pthread_create() failed.");
			}
#endif
		}

		void run() {
			r->run();
		}

		void join() {
			if (r == NULL) {
				return;
			}
			
#if defined(_WIN32)
			DWORD result;

			if (hthr == NULL) {
				return;
			}

			result = WaitForSingleObjectEx(hthr, INFINITE, FALSE);
			if (result != WAIT_OBJECT_0) {
				throw runtime_error(
					"WaitForSingleObjectEx() failed.");
			}
#elif defined(__unix)
			int result;

			result = pthread_join(thr, NULL);
			if (result != 0) {
				throw runtime_error("pthread_join() failed.");
			}
#endif
		}

	protected:
#if defined(_WIN32)
		static DWORD WINAPI ThreadProc(LPVOID lpParameter) {
			Thread *t = (Thread *)lpParameter;

			t->run();

			return 0;
		}
#elif defined(__unix)
		static void *start_routine(void *arg) {
			Thread *t = (Thread *)arg;

			t->run();

			return NULL;
		}
#endif

	private:
		Runnable *r;

#if defined(_WIN32)
		HANDLE hthr;
		DWORD tid;
#elif defined(__unix)
		int thr_exist;
		pthread_t thr;
#endif
	};
}

#endif //THREAD_H__
