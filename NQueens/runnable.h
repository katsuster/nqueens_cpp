#ifndef RUNNABLE_H__
#define RUNNABLE_H__

namespace katsuster {
	class Runnable {
	public:
		virtual ~Runnable() {
			//do nothing
		}

		virtual void run() = 0;
	};
}

#endif //RUNNABLE_H__
