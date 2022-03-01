#include <stdio.h>
#include <stdlib.h>
#define max_size 100
#define MAX3(a, b, c) (((a)>(b)) ? ((a)>(c) ? (a):(c)):((b)>(c) ? (b):(c)))
int startIndex, finIndex, maxsSum = 0;
int aux[10000][10000];

//�迭�� �ִ��� return���ִ� �Լ�
int FindMax1D(const int A[], int N) {
	int i, max = A[0];
	for (i = 0; i < N; i++) {
		if (A[i] > max) {
			max = A[i];
			startIndex = i;
			finIndex = i; }}
	return max;
}

/*Algorithm1 : MSS Problem(1D) - Divide & Conquer*/
static int MaxSubSum(const int A[], int Left, int Right) {
	int MaxLeftSum, MaxRightSum;
	int MaxLeftBorderSum, MaxRightBorderSum;
	int LeftBorderSum, RightBorderSum;
	int Center, i, max;
	int inLeft = 0, inRight = 0, tempL = 0;

	//�迭�� ������Ұ� 1���� ���
	if (Left == Right) return A[Left];

	//�迭�� ������Ұ� 2�� �̻��� ���
	Center = (Left + Right) / 2;
	MaxLeftSum = MaxSubSum(A, Left, Center);
	MaxRightSum = MaxSubSum(A, Center + 1, Right);

	//���� �κ����� �ִ�
	MaxLeftBorderSum = LeftBorderSum = 0;
	for (i = Center; i >= Left; i--) {
		LeftBorderSum += A[i];
		if (LeftBorderSum >= MaxLeftBorderSum) {
			MaxLeftBorderSum = LeftBorderSum;
			inLeft = i;
		}
	}

	//������ �κ����� �ִ�
	MaxRightBorderSum = RightBorderSum = 0;
	for (i = Center + 1; i <= Right; i++) {
		RightBorderSum += A[i];
		if (RightBorderSum >= MaxRightBorderSum) {
			MaxRightBorderSum = RightBorderSum;
			inRight = i;
		}
	}

	//�ִ� ��ȯ
	max = MAX3(MaxLeftSum, MaxRightSum, MaxLeftBorderSum + MaxRightBorderSum);
	if (maxsSum < max) {
		maxsSum = max;
		if (maxsSum == MaxLeftBorderSum + MaxRightBorderSum) {
			startIndex = inLeft; finIndex = inRight;
			//MSS�� ������Ұ� 1���� ���
			if (startIndex != 0 && finIndex == 0) startIndex = finIndex = inLeft;
			else if (startIndex == 0 && finIndex != 0) { if (maxsSum == A[finIndex]) startIndex = finIndex = inRight; }
			inLeft = 0; inRight = 0; }
	}

	return max;
}

int MaxSubsequenceSum(const int A[], int N) {
	int max = MaxSubSum(A, 0, N - 1);
	if (max > 0) return max;
	else { //�迭�� ������Ұ� ��� ������ ���
		max = FindMax1D(A, N);
		return max; }
}

/*Algorithm2 : MSS Problem(1D) - Kadane*/
int kadane(int* arr, int* start, int* finish, int n) {
	int sum = 0, maxSum = INT_MIN;
	*finish = -1;

	int local_start = 0;
	for (int i = 0; i < n; ++i) {
		sum += arr[i];
		if (sum < 0) { sum = 0; local_start = i + 1; }
		else if (sum > maxSum) {
			maxSum = sum;
			*start = local_start;
			*finish = i;
		}
	}

	if (*finish != 1) return maxSum;
	// �迭�� ��� ���Ұ� ������ ��
	maxSum = arr[0]; *start = *finish = 0;
	for (int i = 1; i < n; i++) {
		if (arr[i] > maxSum) {
			maxSum = arr[i];
			*start = *finish = i;
		}
	}

	return maxSum;
}

/*Algorithm3 : MSS Problem(2D) / Summed Area Table*/
int preProcess(int mat[][10000], int aux[][10000], int row, int col) {
	for (int i = 0; i < col; i++)
		aux[0][i] = mat[0][i];

	for (int i = 1; i < row; i++)
		for (int j = 0; j < col; j++)
			aux[i][j] = mat[i][j] + aux[i - 1][j];

	for (int i = 0; i < row; i++)
		for (int j = 1; j < col; j++)
			aux[i][j] += aux[i][j - 1];
}

int sumQuery(int aux[][10000], int tli, int tlj, int rbi, int rbj) {
	int res = aux[rbi][rbj];
	if (tli > 0) res = res - aux[tli - 1][rbj];
	if (tlj > 0) res = res - aux[rbi][tlj - 1];
	if (tli > 0 && tlj > 0) res = res + aux[tli - 1][tlj - 1];
	return res;
}

void SAT(int mat[][10000], int aux[][10000], int row, int col, char output[]) {
	int result, maxSum = mat[0][0];
	int finalTop = 0, finalLeft = 0, finalBottom = 0, finalRight = 0;

	preProcess(mat, aux, row, col);

	for (int i = 0; i < row; i++)
		for (int j = 0; j < col; j++)
			for (int k = 0; k < row; k++) {
				if (k < i) continue;
				for (int m = 0; m < col; m++) {
					if (m < j) continue;
					result = sumQuery(aux, i, j, k, m);
					if (result > maxSum) {
						maxSum = result;
						finalTop = i, finalLeft = j, finalBottom = k, finalRight = m; }}}

	printf(">> Maximum Subsequence Sum : %d\n", maxSum);
	printf(">> StartIndex : (%d, %d), FinIndex : (%d, %d)\n", finalTop, finalLeft, finalBottom, finalRight);
	printf(">> Answer : %d %d %d %d %d\n\n", maxSum, finalTop, finalLeft, finalBottom, finalRight);

	result = ("%d %d %d %d %d", maxSum, finalTop, finalLeft, finalBottom, finalRight);


	FILE *out = fopen(output, "wb");
	if (out == NULL) {
		printf("������ �����ϴ�.\n");
		return -1; }
	else {
		fwrite(&maxSum, sizeof(maxSum), 1, out);
		fwrite(&finalTop, sizeof(finalTop), 1, out);
		fwrite(&finalLeft, sizeof(finalLeft), 1, out);
		fwrite(&finalBottom, sizeof(finalBottom), 1, out);
		fwrite(&finalRight, sizeof(finalRight), 1, out);
	}

	fclose(out);

}

/*Algorithm4 : MSS Problem(2D) / Divide & Conquer*/
void findMaxSum2(int M[][10000], int row, int col, char output[]) {
	int maxSum = INT_MIN, finalLeft = 0, finalRight = 0, finalTop = 0, finalBottom = 0;
	int left, right, i;
	int temp[10000], sum, start, finish;

	for (left = 0; left < col; ++left) {
		memset(temp, 0, sizeof(temp));
		for (right = left; right < col; ++right) {
			//printf("temp : ");
			for (i = 0; i < row; ++i) {
				temp[i] += M[i][right];
				//printf("%d ", temp[i]);
			} //printf("\n");
			sum = MaxSubsequenceSum(temp, row); //kadane ��� Divde & Conquer �Լ��� �Է�

			if (startIndex != 0 && finIndex == 0) finIndex = startIndex;
			else if (startIndex == 0 && finIndex != 0) { if (maxsSum == temp[finIndex]) startIndex = finIndex; }

			start = startIndex; finish = finIndex;
			startIndex = 0; finIndex = 0; maxsSum = 0; //�� �ʱ�ȭ

			if (sum > maxSum) {
				maxSum = sum;
				finalLeft = left; finalRight = right;
				finalTop = start; finalBottom = finish;
			}
		}
	}

	//MSS ���� ��� ���
	printf(">> Maximum Subsequence Sum : %d\n", maxSum);
	printf(">> StartIndex : (%d, %d), FinIndex : (%d, %d)\n", finalTop, finalLeft, finalBottom, finalRight);
	//out.bin ���Ͽ� ��µǾ�� �� ����
	printf(">> Answer : %d %d %d %d %d\n\n", maxSum, finalTop, finalLeft, finalBottom, finalRight);

	FILE *out = fopen(output, "wb");
	if (out == NULL) {
		printf("������ �����ϴ�.\n");
		return -1; }
	else {
		fwrite(&maxSum, sizeof(maxSum), 1, out);
		fwrite(&finalTop, sizeof(finalTop), 1, out);
		fwrite(&finalLeft, sizeof(finalLeft), 1, out);
		fwrite(&finalBottom, sizeof(finalBottom), 1, out);
		fwrite(&finalRight, sizeof(finalRight), 1, out);
	}

	fclose(out);
}

/*Algorithm5 : MSS Problem(2D) / Kadane*/
void findMaxSum1(int M[][10000], int row, int col, char output[]) {
	int maxSum = INT_MIN, finalLeft = 0, finalRight = 0, finalTop = 0, finalBottom = 0;
	int left, right, i;
	int temp[10000], sum, start = 0, finish = 0;

	for (left = 0; left < col; ++left) {
		memset(temp, 0, sizeof(temp));
		for (right = left; right < col; ++right) {
			for (i = 0; i < row; ++i)
				temp[i] += M[i][right];
			sum = kadane(temp, &start, &finish, row);

			if (sum > maxSum) {
				maxSum = sum;
				finalLeft = left; finalRight = right;
				finalTop = start; finalBottom = finish;
			}
		}
	}

	//MSS ���� ��� ���
	printf(">> Maximum Subsequence Sum : %d\n", maxSum);
	printf(">> StartIndex : (%d, %d), FinIndex : (%d, %d)\n", finalTop, finalLeft, finalBottom, finalRight);
	//out.bin ���Ͽ� ��µǾ�� �� ����
	printf(">> Answer : %d %d %d %d %d\n\n", maxSum, finalTop, finalLeft, finalBottom, finalRight);

	FILE *out = fopen(output, "wb");
	if (out == NULL) {
		printf("������ �����ϴ�.\n");
		return -1; }
	else {
		fwrite(&maxSum, sizeof(maxSum), 1, out);
		fwrite(&finalTop, sizeof(finalTop), 1, out);
		fwrite(&finalLeft, sizeof(finalLeft), 1, out);
		fwrite(&finalBottom, sizeof(finalBottom), 1, out);
		fwrite(&finalRight, sizeof(finalRight), 1, out);
	}

	fclose(out);
}

int main() {
	int MSS; int n;
	int TestCase; //TestCase�� ����
	int AlgorithmNum; //����� �˰��� ��ȣ
	char input[max_size]; //�Է� ������ ���� �̸�
	char output[max_size]; //��� ���� �̸�
	int Input2D[10000][10000];

	//HW1_config.txt ���� ����
	FILE* fp1 = fopen("HW1_config.txt", "r");
	if (fp1 == NULL) {
		fprintf(stderr, "file open error \n");
		exit(1); }

	while (!feof(fp1)) {
		fscanf(fp1, "%d", &TestCase);
		printf("TestCase : %d\n\n", TestCase);

		//TestCase�� ������ŭ �ݺ�
		for (int k = 0; k < TestCase; k++) {
			fscanf(fp1, "%d", &AlgorithmNum);
			fscanf(fp1, "%s", &input);
			fscanf(fp1, "%s", &output);

			printf("AlgorithmNum : %d\n", AlgorithmNum);
			printf("Input File Name : %s\n", input);
			printf("Output File Name : %s\n", output);

			//1,2�� �˰����� ����ϴ� ���
			if (AlgorithmNum == 1 || AlgorithmNum == 2) {
				FILE* fp2 = fopen(input, "rb");
				if (fp2 == NULL) {
					fprintf(stderr, "file open error \n");
					exit(1); }

				//�迭 InputArr�� ���� �Ҵ� �� �� ����
				int *InputArr;
				fread(&n, sizeof(n), 1, fp2); //n = ���� ����
				InputArr = (int*)malloc(sizeof(int)*n); int i = 0;
				while (!feof(fp2)) {
					fread(&InputArr[i], sizeof(InputArr[i]), 1, fp2);
					i++;
				} fclose(fp2);

				//�迭 ������� ���
				//printf(">> Input Array : ");
				//for (int i = 0; i < n - 1; i++) printf("%d, ", InputArr[i]);
				//printf("%d\n", InputArr[n - 1]);

				//MSS ���� ���(MSS, startIndex, finIndex) ���
				if (AlgorithmNum == 1) { //1�� �˰����� ����ϴ� ���
					MSS = MaxSubsequenceSum(InputArr, n);
					printf(">> Maximum Subsequence Sum : %d\n", MSS);
					if (startIndex != 0 && finIndex == 0) finIndex = startIndex;
					printf(">> StartIndex : %d, FinIndex : %d\n", startIndex, finIndex);

					//out.bin���Ͽ� ��µǾ�� �� �κ�
					printf(">> Answer : %d %d %d\n\n", MSS, startIndex, finIndex);

					FILE *out = fopen(output, "wb");
					if (out == NULL) {
						printf("������ �����ϴ�.\n");
						return -1;}
					else {
						fwrite(&MSS, sizeof(MSS), 1, out);
						fwrite(&startIndex, sizeof(startIndex), 1, out);
						fwrite(&finIndex, sizeof(finIndex), 1, out);
					}

					fclose(out);

					startIndex = 0; finIndex = 0; maxsSum = 0; //�������� �ʱ�ȭ
				}

				else if (AlgorithmNum == 2) { //2�� �˰����� ����ϴ� ���
					int start, finish;
					MSS = kadane(InputArr, &start, &finish, n);
					printf(">> Maximum Subsequence Sum : %d\n", MSS);
					printf(">> StartIndex : %d, FinIndex : %d\n", start, finish);

					//out.bin���Ͽ� ��µǾ�� �� �κ�
					printf(">> Answer : %d %d %d\n\n", MSS, start, finish);

					FILE *out = fopen(output, "wb");
					if (out == NULL) {
						printf("������ �����ϴ�.\n");
						return -1; }
					else {
						fwrite(&MSS, sizeof(MSS), 1, out);
						fwrite(&start, sizeof(start), 1, out);
						fwrite(&finish, sizeof(finish), 1, out);
					}

					fclose(out);
				}
				free(InputArr);
			}

			//3,4,5 �� �˰����� ����ϴ� ���
			else {
				FILE* fp2 = fopen(input, "rb");
				if (fp2 == NULL) {
					fprintf(stderr, "file open error \n");
					exit(1); }
				fread(&n, sizeof(n), 1, fp2); //n = �� ����

				//2���� �迭 Input2D�� �� ����
				int k = 0, j = 0; char comma;
				while (!feof(fp2)) {
					if (fread(&Input2D[k][j++], sizeof(Input2D[k][j - 1]), 1, fp2)) {
						if (j == n) { k++; j = 0; }
					};
				} int row, col; row = n; col = n;

				fclose(fp2);

				//MSS ���� ���(MSS, startIndex, finIndex) ���
				if (AlgorithmNum == 3) SAT(Input2D, aux, row, col, output); //3�� �˰����� ����ϴ� ���
				else if (AlgorithmNum == 4) findMaxSum2(Input2D, row, col, output); //4�� �˰����� ����ϴ� ���
				else if (AlgorithmNum == 5) findMaxSum1(Input2D, row, col, output); //5�� �˰����� ����ϴ� ���
			}
		}
	} fclose(fp1); //��� TestCase Ȯ�� �Ϸ�

	return(0);
}