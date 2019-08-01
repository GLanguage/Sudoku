# include <iostream>
# include <memory.h>
using namespace std;

typedef bool bit;

struct Sudoku {
	static const int a = 9;

	bit map[a * a * 4]; // binary: map[a][a]
	bool placed[a * a * a]; // (x \in [1, a]) * a * a -> (Can't map[a][a] be x)?

	Sudoku() { memset(placed, 0, sizeof(placed)); }

	void readin() {
		char c;
		int t = 0;
		while (1) {
			c = getchar();
			if (c >= '0' && c <= '9') {
				int n = c ^ '0';
				do {
					*(map + t) = n & (1 << ((t & 3) ^ 3));
					t++;
				} while (t & 3);
			}
			if (t >= a * a * 4)
				break;
		}
	}

	void setup() {
		// to place the given numbers
		for (int t = 0; t < a * a * 4; t += 4) {
			bit* pos = map + t;
			int num = 0;
			for (int i = 0; i < 4; i++)
				num = (num << 1) | *(pos + i);
			if (num)
				place(pos, num);
		}
	}

	void write() {
		int t = 0;
		int x = 0;
		while (1) {
			if ((t | 0) && !(t & 3)) {
				putchar(x + '0');
				putchar(' ');
				x = 0;
				if (t % (4 * a) == 0) {
					puts("");
					if (t >= a * a * 4)
						break;
				}
			}
			x = (x << 1) + (*(map + t) & 1);
			t++;
		}
	}

	void place(bit* pos, int num) {
		for (int i = 0; i < 4; i++)
			*(pos + i) = (num >> (i ^ 3)) & 1;
		int x = (pos - map) / 4 / a;
		int y = (pos - map) / 4 % a;
		int blockx = x / 3;
		int blocky = y / 3;
		int b = a * a * (num - 1);
		int e = a * a * num;
		for (int t = b; t < e; t++) {
			int tx = (t - b) / a;
			int ty = (t - b) % a;
			if (tx == x || ty == y)
				*(placed + t) = 1;
			if (tx / 3 == blockx && ty / 3 == blocky)
				*(placed + t) = 1;
		}
	}

	void remove(bit* pos, int num, bool* origin) {
		memset(pos, 0, 4);
		int x = (pos - map) / 4 / a;
		int y = (pos - map) / 4 % a;
		int blockx = x / 3;
		int blocky = y / 3;
		int b = a * a * (num - 1);
		int e = a * a * num;
		for (int t = b; t < e; t++) {
			int tx = (t - b) / a;
			int ty = (t - b) % a;
			if (tx == x || ty == y)
				*(placed + t) = *(origin + (t - b));
			if (tx / 3 == blockx && ty / 3 == blocky)
				*(placed + t) = *(origin + (t - b));
		}
	}

	bool solve(bit* pos) { // DFS
		if (pos >= (map + a * a * 4))
			return true;
		if (*(pos) | *(pos + 1) | *(pos + 2) | *(pos + 3))
			return solve(pos + 4);
		for (int num = 1; num <= a; num++) {
			if (*(placed + (((num - 1) * a * a) + ((pos - map) / 4))))
				continue;
			bool origin[a * a];
			for (int t = 0; t < a * a; t++)
				*(origin + t) = *(placed + a * a * (num - 1) + t);
			place(pos, num);
			if (solve(pos + 4))
				return true;
			remove(pos, num, origin);
		}
		return false;
	}
};

Sudoku puzzle;
int main() {
	puzzle.readin();
	puzzle.setup();
	puzzle.solve(puzzle.map);
	puzzle.write();
	return 0;
}