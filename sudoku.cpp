#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <pthread.h>
#include <cstring>

using namespace std;

#define tokenDelim " "
#define numThreads 11

struct cubeData {
	int row;
	int col;
};

int sudoku[9][9];
int valid = 1;

void *checkCube(void* args) {
	struct cubeData *data = (struct cubeData*)args;

	int num[9];
	int i;
	for (i = 0; i < 9; i++) {
		num[i] = 0;
	}

	int j;
	for (i = data->row; i < data->row + 3; i++) {
		for (j = data->col; j < data->col + 3; j++) {
			num[sudoku[i][j]-1]++;
		}
	}

	for (i = 0; i < 9; i++) {
		if (num[i] == 0) {
			cout << i+1 << " is missing from subsection: rows(" << data->row << "..." << data->row+3 << "), cols(" << data->col << "..." << data->col+3 << ")" << endl;
			valid = 0;
		}
	}

	return NULL;
}

void *checkAllRows(void* args) {
	int num[9];
	int count;
	for (count = 0; count < 9; count++) {
		num[count] = 0;
	}

	int i;
	int j;
	int k;
	for (i = 0; i < 9; i++) {
		for (j = 0; j < 9; j++) {
			num[sudoku[i][j]-1]++;
		}

		for(k = 0; k < 9; k++) {
			if (num[k] == 0) {
				cout << k+1 << " is missing from row " << i + 1 << endl;
				valid = 0;
			}
		}

		for (count = 0; count < 9; count++) {
			num[count] = 0;
		}
	}

	return NULL;
}

void *checkAllColumns(void* args) {
	int num[9];
	int count;
	for (count = 0; count < 9; count++) {
		num[count] = 0;
	}

	int i;
	int j;
	int k;
	for (i = 0; i < 9; i++) {
		for (j = 0; j < 9; j++) {
			num[sudoku[j][i]-1]++;
		}

		for(k = 0; k < 9; k++) {
			if (num[k] == 0) {
				cout << k+1 << " is missing from column " << i + 1 << endl;
				valid = 0;
			}
		}

		for (count = 0; count < 9; count++) {
			num[count] = 0;
		}
	}

	return NULL;
}

void checkValid(int nums[]) {

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

		int rowThread = pthread_create(&threads[0], NULL, &checkAllRows, NULL);
		int colThread = pthread_create(&threads[1], NULL, &checkAllColumns, NULL);

		pthread_join(threads[0], NULL);
		pthread_join(threads[1], NULL);

		int i = 0;
		int j = 0;
		int threadCount = 2;
		for (i = 0; i < 7; i+=3) {
			for (j = 0; j < 7; j +=3) {
				struct cubeData *readParams = (cubeData *) malloc(sizeof(struct cubeData));
				readParams->row = i;
				readParams->col = j;

				int cubeThreads = pthread_create(&threads[threadCount], NULL, &checkCube, readParams);
				pthread_join(threads[threadCount], NULL);
				threadCount++;
			}
		}

		if (valid == 0) {
			cout << "Sudoku is invalid" << endl;
		} else {
			cout << "Sudoku is valid" << endl;
		}

		pthread_exit(threads);
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