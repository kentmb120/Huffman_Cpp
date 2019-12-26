#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#define MAXNODE 20
typedef struct {
	unsigned int weight;
	unsigned int parent, lchild, rchild;
}HTNode, HuffmanTree[MAXNODE + 1];/*数组存储赫夫曼树，下标从1开始*/
typedef struct {
	char ch;     /*结点的值（字符）*/
	char* code;   /*结点的huffman编码*/
}codenode, HuffmanCode[MAXNODE + 1];/*数组存储赫夫曼编码，下标从1开始*/

void select(HuffmanTree ht, int n, unsigned int* s1, unsigned int* s2)
{/*在ht[1..n]中选择parent为0且weight最小的两个结点,其序号分别在*s1,*s2中返回  */
	int min1, min2;
	int i = 1;
	while (ht[i].parent != 0 && i <= n)
	{
		i++;
	}
	min1 = ht[i].weight;
	*s1 = i;
	i++;
	while (ht[i].parent != 0 && i <= n)
	{
		i++;
	}
	if (ht[i].weight < min1) {
		min2 = min1;
		*s2 = *s1;
	}
	else
	{
		min2 = ht[i].weight;
		*s2 = i;
	}
	for (int j = i + 1; j <= n; j++) {
		if (ht[j].parent != 0) {
			continue;
		}
		if (ht[j].weight < min1) {
			min2 = min1;
			min1 = ht[j].weight;
			*s2 = *s1;
			*s1 = j;
		}
		else if (ht[j].weight >= min1 && ht[j].weight < min2)
		{
			min2 = ht[j].weight;
			*s2 = j;
		}
	}
	*s1 = min1;
	*s2 = min2;
}
void huffmancoding(HuffmanTree ht, HuffmanCode hc, int* w, char* c, unsigned int n)
{/*w存放n个叶子结点的权值, c存放n个叶子结点的值(字符),构造赫夫曼树ht,并求出赫夫曼编码hc*/
	unsigned int m, i, s1, s2, start, p, f;
	char* cd;
	if (n <= 1)return;
	m = 2 * n - 1;

	for (i = 1; i <= n; i++)
	{
		ht[i].weight = w[i - 1];
		ht[i].parent = 0;
		ht[i].lchild = 0;
		ht[i].rchild = 0;
		hc[i].ch = c[i - 1];
	}
	for (i = n + 1; i <= m; ++i) {/* 以下构造huffman树，生成n-1个非叶子结点*/
		select(ht, i - 1, &s1, &s2);
		ht[s1].parent = i; ht[s2].parent = i;
		ht[i].lchild = s1; ht[i].rchild = s2; ht[i].parent = 0;
		ht[i].weight = ht[s1].weight + ht[s2].weight;
	}
	for (i = 1; i <= n; i++)
	{
		hc[i].code = (char*)malloc(n * sizeof(char));
	}
	cd = (char*)malloc(n * sizeof(char));
	cd[n - 1] = '\0';
	for (i = 1; i <= n; i++) {/* 以下求n个叶子节点的huffman编码*/
		start = n - 1;
		for (p = i, f = ht[i].parent; f != 0; f = ht[f].parent) {
			if (ht[f].lchild == p)
				cd[--start] = '0';
			else cd[--start] = '1';
		}
		strcpy(hc[i].code, &cd[start]);
	}
	free(cd);
}
void printhuffmantree(HuffmanTree ht, int n)
{
	int k;
	printf("\n  weight   parent   lchild    rchild");
	for (k = 1; k <= n; k++)
		printf("\n   %d      %d         %d         %d", ht[k].weight, ht[k].parent, ht[k].lchild, ht[k].rchild);
}

void printhuffmancode(HuffmanCode hc, int n)
{
	int k;
	for (k = 1; k <= n; k++)
		printf("\n  %d   %c   %s", k, hc[k].ch, hc[k].code);
}
void coding(char* sourcecode, char* destcode, HuffmanCode hc, int n)
{/*将字符数组sourcecode 中的字符串转换为对应的二进制符号串，放在字符数组destcode中 */
	char* p, * q;
	int i, j, nofind;
	j = 0;
	p = sourcecode;
	while (*p != '\0')
	{
		nofind = 1;
		for (i = 1; i <= n && nofind; i++)
			if (hc[i].ch == *p) {

				q = hc[i].code;
				while (*q != '\0')destcode[j++] = *q++;
				nofind = 0;
			}

		p++;
	}
	destcode[j++] = '\0';
}

void decoding(char* sourcecode, char* destcode, HuffmanTree ht, HuffmanCode hc, int n)
{
	{/*将字符数组destcode中的二进制符号串转换为对应的字符串，放在字符数组sourcecode 中*/
		char* p, * q;
		int f;
		p = sourcecode;
		q = destcode;

		while (*q != '\0')
		{
			f = 2 * n - 1;
			while (ht[f].lchild != 0 && ht[f].rchild != 0 && *q != '\0')
			{
				if (*q == '0')f = ht[f].lchild; else f = ht[f].rchild;
				q++;
			}
			*p++ = hc[f].ch;
		}
		*p++ = '\0';
	}
}

int	main() {
	int n, k;
	HuffmanTree ht;
	HuffmanCode hc;
	int* w, x;
	char ch, * c, * str, * bin, * p;
	for (scanf("%d", &n); 2 * n > MAXNODE; printf("please input n again\n"), scanf("%d", &n));

	w = (int*)malloc(n * sizeof(int));
	c = (char*)malloc(n * sizeof(char));
	for (k = 0; k < n; k++) {
		scanf("%d", &x);
		*(w + k) = x;
	}
	getchar();
	for (k = 0; k < n; k++) {
		*(c + k) = getchar();
	}
	getchar();
	huffmancoding(ht, hc, w, c, n);
	printf("\n huffmantree:");
	printhuffmantree(ht, 2 * n - 1);
	printf("\nhuffmancode:");
	printhuffmancode(hc, n);
	str = (char*)malloc(100 * sizeof(char));
	bin = (char*)malloc(200 * sizeof(char));

	printf("\n请输入要传输的字符串，以#结束\n");
	ch = getchar();
	k = 0;
	while (ch != '#')
	{
		str[k++] = ch;
		ch = getchar();

	}
	str[k++] = '\0';
	coding(str, bin, hc, n);

	printf("\nbin=%s", bin); getchar();

	printf("\n请输入二进制符号串，以#结束\n");
	ch = getchar();
	k = 0;
	while (ch != '#')
	{
		bin[k++] = ch;
		ch = getchar();

	}
	bin[k++] = '\0';
	decoding(str, bin, ht, hc, n);
	printf("\nbin=%s", bin);
	printf("\nstr=%s", str);

}

