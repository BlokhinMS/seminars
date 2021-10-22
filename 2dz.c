#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int main(void) {

	char c;													//variable for filling the array
	int str = 1, col = 0;									//to count columns and rows
	int between;											//to record the number of columns in a counting cycle
	int** arr;												//two-dimensional array
	int rows = 0, colum = 0;								//columns and rows to fill a two-dimensional array with no spaces
	int chack = 0;											//variable for additional checking when filling the .gv file
	int mult;												//intermediate variable for the case if the edge is a loop
	int* con;												//array for checking the connectivity of the graph
	int com = 0;											//variable for checking connected vertices

	FILE* fin = NULL;

	fin = fopen("2dz.txt", "r");							//opening a file in ride mode
	if (!fin) {												//check: whether the file is open
		printf("Error!\n");
		return 0;
	}
	else {
		while ((c = fgetc(fin)) != EOF) {					//counting rows and columns
			if (c != ' ' && c != '\n') {
				col++;
			}
			if (c == '\n') {
				str++;
				between = col - 1;
				col = 0;
			}
		}

		rewind(fin);										//read the file first

		arr = (int**)calloc(str, sizeof(int*));				//dynamic memory allocation with array initialization

		if (arr == NULL) {									//check: enough memory allocated
			exit(1);
		}

		for (int i = 0; i < str; i++) {
			arr[i] = (int*)calloc(between, sizeof(int));	//dynamic memory allocation with array initialization
			if (arr[i] == NULL) {
				exit(1);
			}
		}

		while ((c = fgetc(fin)) != EOF) {					//filling a two-dimensional array excluding spaces
			if (!isspace(c)) {
				arr[rows][colum] = c - '0';
				colum++;
			}
			if (c == '\n') {
				rows++;
				colum = 0;
			}
		}

		for (int i = 0; i < str; i++) {						//outputting a two-dimensional array to the user
			for (int j = 0; j < between; j++) {
				printf("%d\t", arr[i][j]);
			}
			printf("\n");
		}
	}

	fclose(fin);											//closing the file

	fin = fopen("2dz_answer.gv", "w+");						//opening a file in write mode

	if (!fin) {
		printf("Error!\n");
		return 0;
	}

	fprintf(fin, "%s", "graph gr {\n");						//filling in the file

	for (int j = 0; j < between; j++) {
		for (int i = 0; i < str; i++) {
			if (i == 0) {
				chack = 0;
			}
			if (arr[i][j] == 1 && chack == 1) {
				fprintf(fin, "%d\n", i + 1);
				chack = 2;
				printf("pin 1 & %d\n", i + 1);
			}
			if (arr[i][j] == 1 && chack == 0) {
				fprintf(fin, "%d--", i + 1);
				mult = i + 1;
				chack = 1;
				printf("pin 0 & %d\n", i + 1);
			}
			if (i == (str - 1) && chack == 1) {
				fprintf(fin, "%d\n", mult);
			}
		}
	}

	fprintf(fin, "%s", "}");

	fclose(fin);

	fin = fopen("2dz_answer.gv", "r");						//opening a file in ride mode

	if (!fin) {
		printf("Error!\n");
		return 0;
	}

	con = (int*)calloc(str, sizeof(int));					//dynamic memory allocation with array initialization
	if (con == NULL) {
		exit(1);
	}

	con[0] = 1;												//checking the graph for connectivity

	for (int i = 0; i < str; i++) {
		if (con[i] == 1) {
			for (int j = 0; j < str; j++) {
				if (con[j] != 1) {
					for (int m = 0; m < between; m++) {
						if ((arr[i][m] == 1) && (arr[j][m] == 1)) {
							con[j] = 1;
							com++;
							break;
						}
					}
				}
			}
		}
	}
	if ((com + 1) == str) {
		printf("\ngraph converges!\n");
	}
	else {
		printf("\ngraph not converges!\n");
	}

	fclose(fin);

	system("dot -Tpng 2dz_answer.gv -o 'answer.png'");		//passing a command to the command line to render the graph in the grathviz environment
	system("wslview answer.png");

	for (int i = 0; i < str; i++) {							//freeing previously allocated memory
		free(arr[i]);
	}
	free(arr);
	free(con);

	return 0;
}
