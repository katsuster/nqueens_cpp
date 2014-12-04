#ifndef SOLVER_H__
#define SOLVER_H__

#include "runnable.h"

namespace katsuster
{
	class Solver : public Runnable {
	public:
		Solver(int row, int left, int right, int n, int y, long answer) 
			: row(row), left(left), right(right), 
			n(n), y(y), answer(answer), new_answer(0) {
			//do nothing
		}

		virtual ~Solver() {
			//do nothing
		}

		virtual long get_new_answer() {
			return new_answer;
		}

		virtual void run() {
			new_answer = solve_inner(row, left, right, n, y, answer);
		}

	protected:
		static long solve_inner(int row, int left, int right, int n, int y, long answer) {
			int field, xb;
			int n_row, n_left, n_right;

			if (y == n) {
				//found the answer
				return answer + 1;
			}

			field = ((1 << n) - 1) & ~(left | row | right);
			while (field != 0) {
				xb = -field & field;
				field = field & ~xb;

				n_row = row | xb;
				n_left = (left | xb) << 1;
				n_right = (right | xb) >> 1;

				//find the next line
				answer = solve_inner(n_row, n_left, n_right, n, y + 1, answer);
			}

			return answer;
		}

	private:
		int row;
		int left;
		int right;
		int n;
		int y;
		long answer;
		long new_answer;
	};
}

#endif //SOLVER_H__
