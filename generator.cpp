#include <iostream>
#include <sstream>
#include <iterator>
#include <vector>
#include <random>
#include <time.h>
#include <vector>

using namespace std;

struct node
{
	int val; // 퍼즐 블록의 값
	unsigned char neighbors;  // 퍼즐블록의 이웃이 있는지 저장 8자리 2진수 ex) 11000001 0부터 1번째 수가 1이면 dx[i],dy[i]이웃값이 있다는뜻
};   // 퍼즐의 한블록 구조체

int wid, hei, dx[8], dy[8], len, maxi; //wid : x, hei : y, max : 최대값, dx,dy이웃위치 8방향저장

node* arr; //퍼즐을 만들 배열


vector<int> startpoint;
int start = 1;


void findStart(int& x, int& y);
unsigned char getNeighbors(int x, int y);




bool search(int x, int y, int w) // w찾을값, x,y=(w-1)의 좌표값
{
	//cout << w << endl;

	node* n = &arr[x + y * wid];
	n->neighbors = getNeighbors(x, y);

	for (int d = 0; d < 8; d++)
	{
		if (n->neighbors & (1 << d))
		{
			int a = x + dx[d], b = y + dy[d];
			if (arr[a + b * wid].val == 0)
			{
				arr[a + b * wid].val = w; // w-1의 이웃중 하나에 w를 넣고
				if (w == maxi) return true;
				if (search(a, b, w + 1)) return true; // 다음찾을 값을 recursive하게부름
				arr[a + b * wid].val = 0; // 답이 아닐경우 backtracking
			}

		}
	}
	return false;
}

unsigned char getNeighbors(int x, int y) // node에대한 이웃값을 위의 neighborhood형식으로 만들어 return
{
	unsigned char c = 0; int m = -1, a, b;
	for (int yy = -1; yy < 2; yy++)
		for (int xx = -1; xx < 2; xx++)
		{
			if (!yy && !xx) continue;
			m++; a = x + xx, b = y + yy;
			if (a < 0 || b < 0 || a >= wid || b >= hei) continue;
			if (arr[a + b * wid].val > -1) c |= (1 << m);
		}
	return c;
}

bool solveIt()
{
	int x, y; 
	
	findStart(x, y);
	if (x < 0) { cout << "\nCan't find start point!\n"; return false; }
	cout << x << " " << y << endl;
	return search(x, y, 2);
}

void findStart(int& x, int& y) // 퍼즐에서 1이들어 있는 시작점을 찾아줌
{
	while (true) {
		int random = rand() % maxi;


		int start = startpoint[random];
		if (start != 0) {
			arr[start].val = 1;
			x = start % wid;
			y = start / wid;


			return;
		}

	}

	
	x = y = -1;
}

bool solve(vector<string>& puzz, int max_wid)
{
	if (puzz.size() < 1) return true;
	wid = max_wid; hei = static_cast<int>(puzz.size()) / wid; // puzzle width, height 설정
	len = wid * hei;
	int	c = 0; 
	 // len : 퍼즐의 크기
	arr = new node[len]; memset(arr, 0, len * sizeof(node)); // arr배열 크기설정, 0초기화
	


	for (vector<string>::iterator i = puzz.begin(); i != puzz.end(); i++) // string으로부터 퍼즐을 만듬 (. = 빈칸 = 0), (* = 칸없음 = -1)
	{
		if ((*i) == "*") { arr[c++].val = -1; continue; }
		if ((*i) == ".") { 
			startpoint.push_back(c);
			arr[c++].val = 0;
			maxi++; 
		}
		
	}

	bool ret = solveIt();
	c = 0;
	for (vector<string>::iterator i = puzz.begin(); i != puzz.end(); i++) // 만들어낸 퍼즐을 출력 string형식으로 저장
	{
		if ((*i) == ".")
		{
			ostringstream o; o << arr[c].val;
			(*i) = o.str();
		}
		c++;
	}
	delete[] arr;
	

	return ret;
}

int main(int argc, char* argv[])
{
	dx[0] = -1; dx[1] = 0; dx[2] = 1; dx[3] = -1; dx[4] = 1; dx[5] = -1; dx[6] = 0; dx[7] = 1;
	dy[0] = -1; dy[1] = -1; dy[2] = -1; dy[3] = 0; dy[4] = 0; dy[5] = 1; dy[6] = 1; dy[7] = 1;

	srand(time(NULL));

	int wid;
	//테스트 퍼즐들
	//string p = ". . . . . * * * . . . . . * * * . . . . . . * * . . . . . . * * . . . . . . . * * * . . . . . * * * * * . . . . * * * * * * . ."; wid = 8;
	string p = ". . . . . . . . . . . . . . . . . . . . . . . * * * * . . . . * * . . * . . . * . . . . . . . . * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . ."; wid = 9;
	//string p = ". . . . . . . . . . . . . . . . . . . . . . . . . . . . . . * * * . . . . . . * * * . . . . . . * * * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . ."; wid = 9;
	//string p = ". . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . ."; wid = 7;
	//string p = "* . * * . . . * . * . . . * . . . . . * . * * *"; wid = 4;
	//string p = ". . * . . ."; wid = 3;
	//string p = " . * ."; wid = 3;

	istringstream iss(p); vector<string> puzz;
	copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter<vector<string> >(puzz));

	int c = 0;
	cout << "hidato puzzle" << endl << endl;
	for (vector<string>::iterator i = puzz.begin(); i != puzz.end(); i++)
	{
		if ((*i) == ".")
			cout << "00 ";
		else if ((*i) != "*" && (*i) != ".")
		{
			if (atoi((*i).c_str()) < 10) cout << "0";
			cout << (*i) << " ";
		}
		else cout << "   ";
		if (++c >= wid) { cout << endl; c = 0; }
	}
	cout << endl << endl;
	if (solve(puzz, wid)) {

		c = 0;
		cout << "solution" << endl << endl;
		for (vector<string>::iterator i = puzz.begin(); i != puzz.end(); i++)
		{
			if ((*i) != "*" && (*i) != ".")
			{
				if (atoi((*i).c_str()) < 10) cout << "0";
				cout << (*i) << " ";
			}
			else cout << "   ";
			if (++c >= wid) { cout << endl; c = 0; }
		}
		cout << endl << endl;
	}

	else {
		cout << "No solution" << endl << endl;
	}


	return system("pause");
}