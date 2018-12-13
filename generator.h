#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <time.h>

using namespace std;


class puzzle {
private:
	vector<int> getNext(int x, int y);

	bool search(int x, int y, int w);

	void findStart(int& x, int& y);

	bool solveIt();

	int getDegrees(int x, int y, int base);

	

public:
	int wid, hei, dx[8], dy[8], len, maxi;
	int* board;
	int way[201] = { 0, };
	int known = -1;
	vector<int> startPoint;
	puzzle() : wid(0), hei(0) {

	}
	puzzle(int wid, int hei) :wid(wid), hei(hei) {
		len = wid * hei;
		maxi = 0;
		board = new int[len]; memset(board, 0, len * sizeof(int));
		dx[0] = -1; dx[1] = 0; dx[2] = 1; dx[3] = -1; dx[4] = 1; dx[5] = -1; dx[6] = 0; dx[7] = 1;
		dy[0] = -1; dy[1] = -1; dy[2] = -1; dy[3] = 0; dy[4] = 0; dy[5] = 1; dy[6] = 1; dy[7] = 1;
	}

	bool solve();



	void makepuzzle() {
		int block = rand() % 7 + 1;

		for (int i = 1; i < maxi - 1; i++) {

			if (block != 0) {
				board[way[i]] = 0;
				block--;
			}

			else
				block = rand() % 7 + 1;
		}
	}

	int* operator[](int i) {
		return board + i;
	}

	friend ostream& operator<<(ostream &out, puzzle &board) {
		int c = 0;
		for (int i = 0; i < board.len; i++)
		{
			//cout << *board[i] << " ";
			if (*board[i] > 0)
			{
				if (*board[i] < 10) out << "0";
				out << *board[i] << " ";
			}
			else out << "   ";
			if (++c >= board.wid) { out << endl; c = 0; }
		}
		out << endl << endl;

		return out;
	}

	friend ifstream& operator>> (ifstream &in, puzzle &board) {
		int m, n, input;

		in >> n >> m;

		board = puzzle(m, n);

		for (int i = 0; i < board.len; i++) {
			in >> input;
			if (input == 0) { *board[i] = -1; continue; }
			if (input == 1) {
				

				board.maxi = board.maxi + 1;
			}
		}

		

		return in;
	}

	friend ofstream& operator<<(ofstream &out, puzzle &board) {
		out << board.hei << " " << board.wid << endl;

		int c = 0;
		for (int i = 0; i < board.len; i++) {
			//cout << *board[i] << " ";
			if (*board[i] > 0)
			{
				if (*board[i] < 10) out << "0";
				out << (*board[i]) << " ";
			}
			else if (*board[i] == -1)
			{
				out << "00 ";
			}
			else out << "-1 ";
			if (++c >= board.wid) { out << endl; c = 0; }
		}

		return out;
	}
};

bool puzzle::search(int x, int y, int w) // w찾을값, x,y=(w-1)의 좌표값
{
	/*cout << " w : " << w << " x : " << x << " y : " << y << endl;
	
	
	
	cout << w << "degree : ";

	for (int d = 0; d < nei.size(); d++)
		cout << nei[d] << " ";

	cout << endl;*/

	vector<int> nei = getNext(x, y);

	
		for (int d = 0; d < nei.size(); d++)
		{
			int a = x + dx[nei[d]], b = y + dy[nei[d]];

			if (board[a + b * wid] == 0)
			{
				board[a + b * wid] = w; // w-1의 이웃중 하나에 w를 넣고
										//cout << "val : " << board[a + b * wid] << " ";
				way[w - 1] = a + b * wid;
				if (w == maxi) return true;
				if (search(a, b, w + 1)) return true; // 다음찾을 값을 recursive하게부름
				board[a + b * wid] = 0; // 답이 아닐경우 backtracking
			}
		}
	
	return false;
}

void puzzle::findStart(int& x, int& y) // 퍼즐에서 랜덤한곳을 시작점 1로잡음
{
	bool solution = true;
	int degrees = 0;
	
	for (int i = 0; i < hei; i++) {
		for (int j = 0; j < wid; j++) {
			if (board[i*wid + j] == 0) {
				degrees = getDegrees(j, i, 0);
				if (degrees == 0)
					solution = false;
				else if (degrees == 1)
					startPoint.push_back(i*wid + j);
			}
		}
	}

	if (solution == false || startPoint.size() > 2) {
		x = y = -1;
		return;
	}

	if (startPoint.size() == 1) {
		board[startPoint[0]] = 1;
		x = startPoint[0] % wid;
		y = startPoint[0] / wid;
		way[0] = startPoint[0];
		return;
	}
	else if (startPoint.size() == 2) {
		if (startPoint[0] % wid < startPoint[1] % wid) {
			board[startPoint[0]] = 1;
			x = startPoint[0] % wid;
			y = startPoint[0] / wid;
			way[0] = startPoint[0];
			return;
		}
		else {
			board[startPoint[1]] = 1;
			x = startPoint[1] % wid;
			y = startPoint[1] / wid;
			way[0] = startPoint[1];
			return;
		}
	}

	x = 0;
	y = hei - 1;

	while (true) {
		

		if (board[x+y*wid] == 0) {
			board[x+y*wid] = 1;
			way[0] = x + y * wid;
			break;
		}

		else {
			if (x + 1 < wid)
				x++;
			else {
				x = 0;
				y--;
			}
		}
	}

	return;
}

bool puzzle::solveIt()
{
	int x, y;

	findStart(x, y);
	if (x < 0) { cout << "\nCan't find start point!\n"; return false; }
	cout << x << " " << y << endl;
	return search(x, y, 2);
}

bool puzzle::solve()
{
	bool ret = solveIt();

	return ret;
}

int puzzle::getDegrees(int x, int y, int base) // node에대한 이웃값을 위의 neighborhood형식으로 만들어 return
{
	if (x < 0 || y < 0 || x >= wid || y >= hei || board[x+y*wid] != base)
		return 9;

	int ret = 0; int a, b;
	for (int yy = -1; yy < 2; yy++)
		for (int xx = -1; xx < 2; xx++)
		{
			if (!yy && !xx) continue;
			a = x + xx, b = y + yy;
			if (a < 0 || b < 0 || a >= wid || b >= hei) continue;
			if (board[a + b * wid] == base) ret++;
		}
	return ret;
}

vector<int> puzzle::getNext(int x, int y)
{
	int deg[8] = { 9, 9, 9, 9, 9, 9, 9, 9};
	vector<int> ret;

	for (int i = 0; i < 8; i++) {
		int d = getDegrees(x + dx[i], y + dy[i], 0);

		if (d != 9)
			deg[i] = d;
	}

	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 8; j++) {
			if (deg[j] == i)
				ret.push_back(j);
		}
	}



	return ret;
}

