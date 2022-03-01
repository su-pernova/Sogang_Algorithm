#include <stdlib.h>
#include <stdio.h>
#include <time.h>
char input_file[128]; char output_file[128];
int n_vertices, n_edges, MAX_WEIGHT;
long long total_weight = 0, count = 0;
int v_count = 0; int cntCmp_arr[1000];
clock_t start1, start2, end1, end2;

// min HEAP ������� ����ü
struct Element {
	int key;
	int index;
};

// min HEAP insert �Լ�
void insert_min_heap(struct Element *element, int new_element, int *n, int index) {
	int i = ++(*n);
	while ((i != 1) && (new_element) < element[i / 2].key) {
		element[i].key = element[i / 2].key;
		element[i].index = element[i / 2].index;
		i /= 2; }
	element[i].key = new_element;
	element[i].index = index;
}

// min HEAP delete �Լ�
int delete_min_heap(struct Element *element, int *n) {
	int parent, child;
	struct Element item, temp;
	item = element[1];
	temp = element[(*n)--];
	parent = 1; child = 2;
	while (child <= *n) {
		if ((child < *n) && (element[child].key > element[child + 1].key)) child++;
		if (temp.key <= element[child].key) break;
		element[parent] = element[child];
		parent = child;
		child *= 2;
	}
	element[parent] = temp;
	return item.index;
}

// weighted edge�� ��Ÿ���� ���� ����ü
struct Edge {
	int src, dest;
	int weight;
};

// �� Connected Components���� �ϳ��� �׷����� ����
struct Graph {
	int V, E;
	struct Edge* edge;
};

// �׷����� �����ϴ� �Լ�
struct Graph* createGraph(int V, int E) {
	struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));
	graph->V = V;
	graph->E = E;
	graph->edge = (struct Edge*)malloc(graph->E * sizeof(struct Edge));
	return graph;
}

// Union-Find�� subset ǥ���� ���� ����ü
struct subset {
	int parent;
	int rank;
};

// Find ������ �����ϴ� �Լ�
int find(struct subset subsets[], int i) {
	if (subsets[i].parent != i)
		subsets[i].parent = find(subsets, subsets[i].parent);
	return subsets[i].parent;
}

// Union ������ �����ϴ� �Լ�
void Union(struct subset subsets[], int x, int y) {
	int xroot = find(subsets, x);
	int yroot = find(subsets, y);
	if (subsets[xroot].rank < subsets[yroot].rank)
		subsets[xroot].parent = yroot;
	else if (subsets[xroot].rank > subsets[yroot].rank)
		subsets[yroot].parent = xroot;
	else { subsets[yroot].parent = xroot;
		subsets[xroot].rank++; }
}

// Disjoint Set�� ����� �Լ�
int dsjSet(struct Graph* graph, struct Element *element, int *n) {
	int V = graph->V, E = graph->E;
	int e = 0;

	struct subset *subsets = (struct subset*)malloc(V * sizeof(struct subset));
	for (int v = 0; v < V; ++v) {
		subsets[v].parent = v;
		subsets[v].rank = 0; }

	while (e < E) {
		struct Edge next_edge = graph->edge[e++];
		int x = find(subsets, next_edge.src);
		int y = find(subsets, next_edge.dest);
		if (x != y) Union(subsets, x, y); }

	int cntCmp = 0, flag = 0;
	int root[1000] = { -1 }; // �� vertex�� root vertex�� ����

	for (int v = 0; v < V; v++) {
		flag = 0; 
		for (int i = 0; i < cntCmp + 1; i++)
			if (root[i] == find(subsets, v)) flag = 1;
		if (flag == 0) { root[cntCmp] = find(subsets, v); cntCmp++; }
	}

	return cntCmp;
}

// MST�� ���ϴ� �Լ�1 (Connected Component�� 1��)
void KruskalMST1(struct Graph* graph, struct Element *element, int *n) {
	int V = graph->V; int e = 0;
	struct Edge *result;
	result = (struct Edge*)malloc((V - 1) * sizeof(struct Edge));

	struct subset *subsets = (struct subset*)malloc(V * sizeof(struct subset));
	for (int v = 0; v < V; ++v) {
		subsets[v].parent = v;
		subsets[v].rank = 0; }

	while (e < V - 1) {
		int i = delete_min_heap(element, n); count++;
		struct Edge next_edge = graph->edge[i];
		int x = find(subsets, next_edge.src);
		int y = find(subsets, next_edge.dest);
		if (x != y) { // ���� �ٸ� tree�� ���ϴ� ��� Union
			result[e++] = next_edge;
			Union(subsets, x, y); }
	}

	for (int i = 0; i < e; ++i)
		total_weight += result[i].weight;
}

// MST�� ���ϴ� �Լ�2-1 (Connected Component�� 2�� �̻�)
int* KruskalMST2(struct Graph* graph, struct Element *element, int *n) {
	int V = graph->V, E = graph->E;
	int *vertex = malloc(sizeof(int) * V);
	int e = 0;

	struct Edge *result;
	result = (struct Edge*)malloc((V - 1) * sizeof(struct Edge));

	struct subset *subsets = (struct subset*)malloc(V * sizeof(struct subset));
	for (int v = 0; v < V; ++v) {
		subsets[v].parent = v;
		subsets[v].rank = 0; }

	while (e < E) {
		struct Edge next_edge = graph->edge[e++];
		int x = find(subsets, next_edge.src);
		int y = find(subsets, next_edge.dest);
		if (x != y) Union(subsets, x, y); }

	int cntCmp = 0, flag = 0;
	int root[1000] = { -1 }; // �� vertex�� root vertex�� ����

	for (int v = 0; v < V; v++) {
		flag = 0;
		for (int i = 0; i < cntCmp + 1; i++)
			if (root[i] == find(subsets, v)) flag = 1;
		if (flag == 0) { root[cntCmp] = find(subsets, v); cntCmp++; }
	}

	e = 0; int vv_count = 0;
	for (int i = 0; i < cntCmp; i++) {
		for (int v = 0; v < V; v++) {
			if (find(subsets, v) == root[i]) {
				vertex[vv_count] = v;
				v_count++; vv_count++; }
		}
		cntCmp_arr[i] = v_count;
;		v_count = 0;
	}

	return vertex;
}

// MST�� ���ϴ� �Լ�2-2 (Connected Component�� 2�� �̻�)
void KruskalMST3(struct Graph* graph, struct Element *element, int *n, int *inside, int number) {
	int V = graph->V; int e = 0;
	struct Edge *result;
	result = (struct Edge*)malloc((V - 1) * sizeof(struct Edge));

	struct subset *subsets_2 = (struct subset*)malloc(V * sizeof(struct subset));
	for (int v = 0; v < V; ++v) {
		subsets_2[v].parent = v;
		subsets_2[v].rank = 0; }

	while (e < number - 1) {
		int i = delete_min_heap(element, n); count++;
		struct Edge next_edge = graph->edge[i];
		for (int i = 0; i < number; i++) {
			if (inside[i] == next_edge.src) {
				int x = find(subsets_2, next_edge.src);
				int y = find(subsets_2, next_edge.dest);
				if (x != y) { // ���� �ٸ� tree�� ���ϴ� ��� Union
					result[e++] = next_edge;
					Union(subsets_2, x, y); }
				break; }
		}
	}

	for (int i = 0; i < e; ++i)
		total_weight += result[i].weight;
}

int main() {
	// commands.txt ���� ����
	FILE *fp; FILE *out;
	fp = fopen("commands.txt", "r");
	if (fp == NULL) {
		fprintf(stderr, "file open error \n");
		exit(1); }
	fscanf(fp, "%s", &input_file);
	fscanf(fp, "%s", &output_file);
	fclose(fp);

	// input ���� ����
	fp = fopen(input_file, "r");
	if (fp == NULL) {
		fprintf(stderr, "file open error \n");
		exit(1); }
	fscanf(fp, "%d", &n_vertices);
	fscanf(fp, "%d", &n_edges);
	fscanf(fp, "%d", &MAX_WEIGHT);

	// ouput ���� ����
	out = fopen(output_file, "wt");
	if (out == NULL) {
		fprintf(stderr, "file open error \n");
		exit(1); }

	// Create Graph according to file
	int x, y, z, n = 0; start1 = clock();
	struct Graph* graph = createGraph(n_vertices, n_edges);
	struct Element* element = malloc(sizeof(struct Element) * n_edges);
	printf("����� . . .\n");
	for (int i = 0; i < n_edges; i++) {
		fscanf(fp, "%d%d%d", &x, &y, &z);
		graph->edge[i].src = x;
		graph->edge[i].dest = y;
		graph->edge[i].weight = z;
		insert_min_heap(element, z, &(n), i);
	} fclose(fp);

	// Make Disjoint Sets - count Connected Components ����
	int cntCmp = dsjSet(graph, element, &(n));
	printf("Connected Components : %d ��\n\n", cntCmp);
	fprintf(out, "%d\n", cntCmp);

	// Make MST1 (Connected Component�� 1��)
	if (cntCmp == 1) {
		graph = createGraph(n_vertices, n_edges); n = 0;
		element = malloc(sizeof(struct Element) * n_edges);
	
		fp = fopen(input_file, "r");
		fscanf(fp, "%d%d%d", &x, &y, &z);

		printf("����� . . .\n");
		start2 = clock();
		for (int i = 0; i < n_edges; i++) {
			fscanf(fp, "%d%d%d", &x, &y, &z);
			graph->edge[i].src = x;
			graph->edge[i].dest = y;
			graph->edge[i].weight = z;
			insert_min_heap(element, z, &(n), i);
		} fclose(fp); end2 = clock();


		KruskalMST1(graph, element, &(n));
		printf("vertex_num : %d\n", n_vertices);
		printf("total_weight : %lld\n", total_weight);
		printf("K scanned : %lld\n", count);
		fprintf(out, "%d %lld", n_vertices, total_weight);
	}

	// Make MST2 (Connected Component�� 2�� �̻�)
	else if (cntCmp > 1) {
		int* vertex; int start = 0;
		graph = createGraph(n_vertices, n_edges); n = 0;
		element = malloc(sizeof(struct Element) * n_edges);

		fp = fopen(input_file, "r");
		fscanf(fp, "%d%d%d", &x, &y, &z);

		printf("����� . . .\n");
		for (int i = 0; i < n_edges; i++) {
			fscanf(fp, "%d%d%d", &x, &y, &z);
			graph->edge[i].src = x;
			graph->edge[i].dest = y;
			graph->edge[i].weight = z;
			insert_min_heap(element, z, &(n), i);
		} fclose(fp);

		vertex = KruskalMST2(graph, element, &(n));

		for (int t = 0; t < cntCmp; t++) {
			graph = createGraph(n_vertices, n_edges); n = 0;
			element = malloc(sizeof(struct Element) * n_edges);

			fp = fopen(input_file, "r");
			fscanf(fp, "%d%d%d", &x, &y, &z);

			start2 = clock();
			for (int i = 0; i < n_edges; i++) {
				fscanf(fp, "%d%d%d", &x, &y, &z);
				graph->edge[i].src = x;
				graph->edge[i].dest = y;
				graph->edge[i].weight = z;
				insert_min_heap(element, z, &(n), i);
			} fclose(fp); end2 = clock();

			float time2 = (float)(end2 - start2) / CLOCKS_PER_SEC;
			printf(">> minHEAP ����ð� : %.1f ��\n", time2);

			int *inside = malloc(sizeof(int) * cntCmp_arr[t]);
			for (int u = 0; u < cntCmp_arr[t]; u++)
				inside[u] = vertex[start++];

			KruskalMST3(graph, element, &(n), inside, cntCmp_arr[t]);
			printf("vertex_num : %d\n", cntCmp_arr[t]);
			printf("total_weight : %lld\n", total_weight);
			printf("K scanned : %lld\n\n", count);
			fprintf(out, "%d %lld\n", cntCmp_arr[t], total_weight);
			total_weight = 0; count = 0;
		}
	}

	// �ð� ���� �� ���
	end1 = clock();
	float time1 = (float)(end1 - start1) / CLOCKS_PER_SEC;
	float time2 = (float)(end2 - start2) / CLOCKS_PER_SEC;
	printf("\n��ü ����ð� : %.1f ��\n", time1);
	printf("minHEAP ����ð� : %.1f ��\n\n", time2);

	fclose(out);
	system("pause");
	return 0;
}