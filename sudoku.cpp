#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <thread>
#include <fstream>
#include <string>
#include <pthread.h>

using namespace std;

#define tokenDelim " "
#define numThreads 11

struct cubeData {
	int row;
	int col;
};

int sudoku[9][9];

void *checkCube(void* args) {
	struct cubeData *data = (struct cubeData*)args;
	cout << data->row << " " << data->col << endl;
	cout << "CHECKING CUBES" << endl;

	return NULL;
}

void *checkAllRows(void* args) {
	cout << "CHECKING ROWS" << endl;
	return NULL;
}

void *checkAllColumns(void* args) {
	cout << "CHECKING COLUMNS" << endl;
	return NULL;
}

void parseLine(char *line) {
	char *token = strtok(line, tokenDelim);
	pthread_t threads[numThreads];
	memset(sudoku, -1, sizeof(sudoku[0][0]) * 9 * 9);
	int row = 0;
	int col = 0;
	int position = 0;
	while (token) {
		for (row = 0; row < 9; row++) {
			for (col = 0; col < 9; col++) {
				sudoku[row][col] = atoi(token);
				position++;
				token = strtok(NULL, tokenDelim);
				cout << sudoku[row][col] << " ";
			}

			cout << endl;
		}
	}

	int rowThread = pthread_create(&threads[0], NULL, &checkAllRows, NULL);
	int colThread = pthread_create(&threads[1], NULL, &checkAllColumns, NULL);

	int i = 0;
	int j = 0;
	int threadCount = 2;
	for (i = 0; i < 7; i+=3) {
		for (j = 0; j < 7; j +=3) {
			cout << i << " " << j << endl;
			struct cubeData readParams;
			readParams.row = i;
			readParams.col = j;

			int cubeThreads = pthread_create(&threads[threadCount], NULL, &checkCube, &readParams);
			threadCount++;
		}
	}
}

int main() {
	string line;

	ifstream myfile("sudoku.txt");
	if (myfile.is_open()) {
		while (getline(myfile, line))
		{
			char *puzzle = new char[line.length() + 1];
			strcpy(puzzle, line.c_str());
			parseLine(puzzle);
		}

		myfile.close();
	} else {
		cout << "Unable to open file";
	}

	return 0;
}