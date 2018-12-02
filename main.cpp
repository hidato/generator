#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <vector>
#include <random>
#include <time.h>
#include "generator.h"

using namespace std;

int main(int argc, char* argv[]) {

	srand(time(NULL));
	puzzle board = puzzle();

	ifstream input;
	input.open("grid9.txt");
	ofstream output;
	output.open("puzzle.txt");

	input >> board;

	cout << board.maxi << endl;
	if (board.solve()) {

		cout << "Answer" << endl;
		cout << board << endl << endl;


		board.makepuzzle();

		output << board;
	}
	else {
		cout << "fail" << endl;
	}

	system("pause");
}