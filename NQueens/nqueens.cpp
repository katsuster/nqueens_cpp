#include <iostream>

#include <cstdlib>
#include <cstring>

#include "solver.h"
#include "thread.h"
#include "timenow.h"

#if defined(_WIN32)
#define strcasecmp _strcmpi
#endif //_WIN32

using namespace std;

using namespace katsuster;

int safe_main(int argc, char *argv[])
{
	int n;
	int parallel;
	long answer;
	int center, x, row, left, right;
	long long start, elapse;
	Solver **solvers;
	Thread **solver_threads;
	Solver *solver_remain;
	Thread *solver_remain_thread;

	if (argc < 2) {
		cout << "usage: \n"
		    << "NQueens queens(1 ... 30) parallel(true or false)"
		    << endl;
		return -1;
	}

	//get the arguments
	n = strtol(argv[1], NULL, 0);
	if (n < 0 || 30 < n) {
		n = 0;
	}
	cout << "queens  : " << n << endl;

	parallel = false;
	if (argc >= 3) {
		if (strcasecmp(argv[2], "true") == 0) {
			parallel = 1;
		} else {
			parallel = 0;
		}
	}
	cout << "parallel: " << parallel << endl;

	//start
	start = TimeNow::microtime();

	//solve
	center = n >> 1;
	solvers = new Solver *[center];
	solver_threads = new Thread *[center];

	answer = 0;
	for (x = 0; x < center; x++) {
		//left half
		row = 1 << x;
		left = row << 1;
		right = row >> 1;

		solvers[x] = new Solver(row, left, right, n, 1, 0);
		solver_threads[x] = new Thread(solvers[x]);
		if (parallel) {
			solver_threads[x]->start();
		} else {
			solver_threads[x]->run();
		}
	}

	solver_remain = NULL;
	solver_remain_thread = NULL;
	if (n % 2 == 1) {
		//center(if N is odd)
		row = 1 << center;
		left = row << 1;
		right = row >> 1;

		solver_remain = new Solver(row, left, right, n, 1, 0);
		solver_remain_thread = new Thread(solver_remain);
		if (parallel) {
			solver_remain_thread->start();
		} else {
			solver_remain_thread->run();
		}
	}

	//join
	for (x = 0; x < center; x++) {
		if (parallel) {
			solver_threads[x]->join();
		}
		answer += solvers[x]->get_new_answer();
	}
	answer *= 2;

	if (solver_remain) {
		if (parallel) {
			solver_remain_thread->join();
		}
		answer += solver_remain->get_new_answer();
	}

	//delete
	for (x = 0; x < center; x++) {
		delete solvers[x];
		delete solver_threads[x];
	}
	delete[] solvers;
	delete[] solver_threads;
	delete solver_remain;
	delete solver_remain_thread;

	//finished
	elapse = TimeNow::microtime() - start;

	//solved
	cout << "answers : " << answer << endl;
	cout << "time    : " << (elapse / 1000) << "[ms]" << endl;

	return 0;
}

int main(int argc, char *argv[])
{
	try {
		return safe_main(argc, argv);
	} catch(const exception & ex) {
		cout << "caught the exception." << endl
		    << "reason: " << ex.what() << endl;
	}
}
