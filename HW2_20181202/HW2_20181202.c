#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define MAX(a, b, c) ((a > b && a > c) ? a : (b < c) ? c : b)
char bin_name[32]; int total_len = 0;
int s, f, p, m, n;
int left, leftup, up; // ����(1), ������(2), ��(3)
int x_gap = 0, y_gap = 0;
char new_x[10000], new_y[10000];
char reverse_new_x[10000], reverse_new_y[10000];

// Dynamic Programming table ����ü ����
typedef struct tableNode {
	int data;
	int direction;
} tableNode;

int main() {
	// input.txt ���� ���� �� ��������
	FILE *fp; char buffer;
	fp = fopen("input.txt", "r");
	if (fp == NULL) {
		fprintf(stderr, "file open error \n");
		exit(1); }
	fscanf(fp, "%s", &bin_name);
	fscanf(fp, "%d%d%d", &s, &f, &p);
	fclose(fp);

	// twostrings.bin ���� ���� �� ��������
	fp = fopen(bin_name, "rb");
	if (fp == NULL) {
		fprintf(stderr, "file open error \n");
		exit(1);}
	fread(&m, sizeof(m), 1, fp);
	fread(&n, sizeof(n), 1, fp);
	char *X = malloc(sizeof(char) * m + 1);
	char *Y = malloc(sizeof(char) * n + 1);
	for (int i = 1; i < m + 1; i++) {
		fread(&buffer, sizeof(buffer), 1, fp);
		X[i] = buffer; }
	for (int i = 1; i < n + 1; i++) {
		fread(&buffer, sizeof(buffer), 1, fp);
		Y[i] = buffer;}
	fclose(fp);

	// table �ۼ��� �ʿ��� 2���� �迭 ����
	tableNode **table;
	table = malloc(sizeof(tableNode*) * m + 1);
	for (int i = 0; i < m + 1; i++)
		table[i] = malloc(sizeof(tableNode) * n + 1);

	// table�� ù ���� �� �ʱ�ȭ
	for (int i = 0; i < m + 1; i++) {
		table[i][0].data = (p * i * (-1));
		table[i][0].direction = 3; }
	for (int i = 0; i < n + 1; i++) {
		table[0][i].data = (p * i * (-1));
		table[0][i].direction = 1; }
	table[0][0].direction = 0;
	
	// Dynamic programming table ����
	for (int i = 1; i < m + 1; i++) {
		for (int j = 1; j < n + 1; j++) {
			left = table[i][j - 1].data + (-1) * p; // ����
			if (X[i] == Y[j]) leftup = table[i - 1][j - 1].data + s; // ������ ��ġ
			else leftup = table[i - 1][j - 1].data + (-1) * f; // ������ ����ġ
			up = table[i - 1][j].data + (-1) * p; // ��

			if (MAX(left, leftup, up) == leftup) {
				table[i][j].data = leftup;
				table[i][j].direction = 2; }
			else if (MAX(left, leftup, up) == up) {
				table[i][j].data = up;
				table[i][j].direction = 3; }
			else if (MAX(left, leftup, up) == left) {
				table[i][j].data = left;
				table[i][j].direction = 1; }
		}
	}

	// score, total_len, x_gap����, y_gap���� ���
	fp = fopen("output.txt", "wt");
	if (fp == NULL) {
		fprintf(stderr, "file open error \n");
		exit(1); }
	fprintf(fp, "%d\n", table[m][n].data);
	int idx_1 = m, idx_2 = n;
	while (table[idx_1][idx_2].direction != 0) {
		if (table[idx_1][idx_2].direction == 1) {
			new_x[total_len] = '-';
			new_y[total_len] = Y[idx_2];
			idx_2--; total_len++; x_gap++; } //left
		else if (table[idx_1][idx_2].direction == 2) {
			new_x[total_len] = X[idx_1];
			new_y[total_len] = Y[idx_2];
			idx_1--; idx_2--; total_len++; } //left_up
		else if (table[idx_1][idx_2].direction == 3) {
			new_x[total_len] = X[idx_1];
			new_y[total_len] = '-';
			idx_1--; total_len++; y_gap++; } //up
	} fprintf(fp, "%d\n", total_len);
	for (int i = 0; i < total_len; i++)
		reverse_new_x[total_len - 1 - i] = new_x[i];
	for (int i = 0; i < total_len; i++)
		reverse_new_y[total_len - 1 - i] = new_y[i];
	fprintf(fp, "%d\n", x_gap);
	for (int i = 0; i < total_len; i++)
		if (reverse_new_x[i] == '-') fprintf(fp, "%d\n", i + 1);
	fprintf(fp, "%d\n", y_gap);
	for (int i = 0; i < total_len; i++)
		if (reverse_new_y[i] == '-') fprintf(fp, "%d\n", i + 1);
	fclose(fp);
	printf("ouput.txt ������ �����Ǿ����ϴ� . . .\n");

	free(X); free(Y); 
	system("pause");
	return  0;
}