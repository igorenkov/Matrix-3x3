#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <string.h>

typedef struct Matrix_3x3 {
	float data[3][3];
} matrix;
//Функция инициирования матрицы
matrix InitMatrix(FILE *fp) {
	matrix resultmat;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			printf("%d row %d column:  ", i + 1, j + 1);
			fscanf(fp, "%f", &resultmat.data[i][j]);
			if (fp != stdin) {	//Если вывод из файла, то в поток вывода выписываются значения из файла
				printf("%.2f \n", resultmat.data[i][j]);
			}
		}
	}
	return resultmat;
}
//Функция вывода и записи в файл матрицы
void ReturnMatrix(matrix mat) {
	FILE *fp = fopen("Result.txt", "w");
	
	printf("Final matrix: \n");
	
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			printf("%.1f  ", mat.data[i][j]);	//Вывод в поток вывода
			fprintf(fp, "%.1f ", mat.data[i][j]);	//Запись в файл
		}
		printf("\n");
		putc('\n', fp);
	}

	fclose(fp);
}
//Функция суммирования матриц
matrix SummMatrix(matrix mat1, matrix mat2) {
	matrix resultmat;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			resultmat.data[i][j] = mat1.data[i][j] + mat2.data[i][j];
		}
	}
	return resultmat;
}
//Функция умножения матриц
matrix MultMatrix(matrix mat1, matrix mat2) {
	float temp;
	matrix resultmat;

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			temp = 0;
			for (int k = 0; k < 3; k++) {
				temp += mat1.data[i][k] * mat2.data[k][j];
			}

			resultmat.data[i][j] = temp;
		}
	}
	return resultmat;
}
//Функция вычисления определителя
float DetMatrix(matrix mat) {
	float det;
	//По-тупому
	det = mat.data[0][0] * mat.data[1][1] * mat.data[2][2] + mat.data[1][0] * mat.data[2][1] * mat.data[0][2] +
		mat.data[0][1] * mat.data[1][2] * mat.data[2][0] - mat.data[2][0] * mat.data[1][1] * mat.data[0][2] -
		mat.data[0][0] * mat.data[2][1] * mat.data[1][2] - mat.data[1][0] * mat.data[0][1] * mat.data[2][2];
	return det;
}
//Функция вычисления определителя дополнительного минора i-го j-го элемента матрицы
float addMatrix(matrix mat, int i, int j) {
	float temp, det;
	for (int l = i; l > 0; l--) {
		for (int k = 0; k < 3; k++) {
			temp = mat.data[l][k];
			mat.data[l][k] = mat.data[l - 1][k];
			mat.data[l - 1][k] = temp;
		}
	}
	for (int l = 0; l < 3; l++) {
		for (int k = j; k > 0; k--) {
			temp = mat.data[l][k];
			mat.data[l][k] = mat.data[l][k - 1];
			mat.data[l][k - 1] = temp;
		}
	}
	det = mat.data[1][1] * mat.data[2][2] - mat.data[1][2] * mat.data[2][1];
	return det;
}
//Функция транспонирования матрицы
matrix TransMatrix(matrix mat) {
	matrix resultmat;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			resultmat.data[i][j] = mat.data[j][i];
		}
	}
	return resultmat;
}
//Функция вычисления обратной матрицы
matrix InvMatrix(matrix mat) {
	matrix resultmat;

	float det = DetMatrix(mat);
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			resultmat.data[i][j] = pow(-1.0, (double)i + j) * addMatrix(mat, i, j) / det;
		}
	}

	resultmat = TransMatrix(resultmat);
	return resultmat;
}

int main() {
	char ch;
	do {
		FILE *fp;
		char iotype;
		printf("Input type of insertion(s - stdio/f - file): ");
		scanf_s("%c", &iotype);
		getchar();

		if (iotype == 'S' || iotype == 's') {
			fp = stdin;
		}
		else if (iotype == 'F' || iotype == 'f') {
			fp = fopen("Matrix.txt", "r");
		}
		else {
			printf("Mistake :)");
			fp = NULL;
		}
		
		if (fp == NULL) {
			printf("File opening error!\n");
		}
		else {
			
			char type;
			printf("Input type of operation (S - Summ/M - Multiplication/D - Determinant/I - Invertible): ");
			scanf_s("%c", &type);
			getchar();

			if (type == 'S' || type == 's') {
				matrix mat1, mat2, resultmat;
				printf("1-st matrix initialization: \n");
				mat1 = InitMatrix(fp);
				printf("2-nd matrix initialization: \n");
				mat2 = InitMatrix(fp);
				resultmat = SummMatrix(mat1, mat2);
				ReturnMatrix(resultmat, fp);
			}
			else if (type == 'M' || type == 'm') {
				matrix mat1, mat2, resultmat;
				printf("1-st matrix initialization: \n");
				mat1 = InitMatrix(fp);
				printf("2-nd matrix initialization: \n");
				mat2 = InitMatrix(fp);
				resultmat = MultMatrix(mat1, mat2);
				ReturnMatrix(resultmat, fp);
			}
			else if (type == 'D' || type == 'd') {
				matrix mat;
				float det;
				printf("Matrix initialization: \n");
				mat = InitMatrix(fp);
				det = DetMatrix(mat);
				
				printf("%.1f", det);

				FILE *fp = fopen("Result.txt", "w");
				fprintf(fp, "%.1f ", det);
				fclose(fp);
			}
			else if (type == 'I' || type == 'i') {
				matrix mat, resultmat;
				printf("Matrix initialization: \n");
				mat = InitMatrix(fp);
				resultmat = InvMatrix(mat);
				ReturnMatrix(resultmat, fp);
			}
			else {
				printf("Mistake, sweetheart :)");
			}
		}

		if (fp != stdin) {
			fclose(fp);
		}

		//Повтор программы
		printf("\nDo you wish to restart program? (y/n)\n");
		scanf("%c", &ch);
		getchar();
		
		
	} while (ch == 'y' || ch == 'Y');

	return 0;
}