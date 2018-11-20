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
	int val; // ���� ����� ��
	unsigned char neighbors;  // �������� �̿��� �ִ��� ���� 8�ڸ� 2���� ex) 11000001 0���� 1��° ���� 1�̸� dx[i],dy[i]�̿����� �ִٴ¶�
};   // ������ �Ѻ�� ����ü

int wid, hei, dx[8], dy[8], len, maxi; //wid : x, hei : y, max : �ִ밪, dx,dy�̿���ġ 8��������

node* arr; //������ ���� �迭


vector<int> startpoint;
int start = 1;


void findStart(int& x, int& y);
unsigned char getNeighbors(int x, int y);




bool search(int x, int y, int w) // wã����, x,y=(w-1)�� ��ǥ��
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
				arr[a + b * wid].val = w; // w-1�� �̿��� �ϳ��� w�� �ְ�
				if (w == maxi) return true;
				if (search(a, b, w + 1)) return true; // ����ã�� ���� recursive�ϰԺθ�
				arr[a + b * wid].val = 0; // ���� �ƴҰ�� backtracking
			}

		}
	}
	return false;
}

unsigned char getNeighbors(int x, int y) // node������ �̿����� ���� neighborhood�������� ����� return
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

void findStart(int& x, int& y) // ���񿡼� 1�̵�� �ִ� �������� ã����
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
	wid = max_wid; hei = static_cast<int>(puzz.size()) / wid; // puzzle width, height ����
	len = wid * hei;
	int	c = 0; 
	 // len : ������ ũ��
	arr = new node[len]; memset(arr, 0, len * sizeof(node)); // arr�迭 ũ�⼳��, 0�ʱ�ȭ
	


	for (vector<string>::iterator i = puzz.begin(); i != puzz.end(); i++) // string���κ��� ������ ���� (. = ��ĭ = 0), (* = ĭ���� = -1)
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
	for (vector<string>::iterator i = puzz.begin(); i != puzz.end(); i++) // ���� ������ ��� string�������� ����
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
	//�׽�Ʈ �����
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