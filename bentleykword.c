#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int p;
	int i;
} Node;

typedef struct {
	char *s;
	int   i;
} WordCount;

int wordsort(const void *, const void *);
static void parent_push(int);
static char *word_for(int);
static int create_child(int, char);
static int find_child(int, char);

Node *node;
int  *parents;
int  node_len = 0, max_node_len;

int
wordsort(const void *p1, const void *p2)
{
	WordCount *w1, *w2;

	w1 = (WordCount *)p1;
	w2 = (WordCount *)p2;
	return w1->i - w2->i;
}

static void
parent_push(int n)
{
	parents[node_len++] = n;
}

static char *
word_for(int p)
{
	int   size, i, c;
	char *s;

	i    = 0;
	size = 256;
	s    = malloc(size);
	while (p != -1) {
		c = p % 27;
		if (++i >= size) {
			size += 256;
			s     = realloc(s, size);
		}
		*(s + i) = 'a' + c;
		p = (p - c) ? node[p - c].p : -1;
	}
	s[i] = '\0';
	return s;
}

static int
create_child(int p, char c)
{
	int len;

	len           = max_node_len;
	max_node_len += 27;
	printf("%d, %d\n", p, max_node_len);
	node          = realloc((void *)node, sizeof(Node) *
	                        (size_t)max_node_len);
	parents       = realloc((void *)node, sizeof(int) *
	                        (size_t)max_node_len);
	node[p].p     = len;
	for (int i = len; i < len + 27; ++i)
		node[i] = (Node){.p = -1, .i = 0};
	parent_push(p);
	return len + c;
}

static int
find_child(int p, char c)
{
	if (node[p].p == -1)
		return create_child(p, c);
	else {
		return node[p].p + c;
	}
}

int
main(int argc, char **argv)
{
	FILE *in;
	long  len, wc;
	char *data;
	int   p;

	/* Read the input file. Get the size of the file. */
	in = fopen(argv[1], "rb");
	(void)fseek(in, 0, SEEK_END);
	len = ftell(in);
	/* Get the content. */
	rewind(in);
	data = (char *)malloc((size_t)len);
	(void)fread(data, 1, len, in);
	(void)fclose(in);

	max_node_len = 27;
	node         = malloc(sizeof(Node) * max_node_len);
	parents      = malloc(sizeof(int) * max_node_len);
	parent_push(-1);
	for (char i = 0; i <= 26; ++i)
		*(node + i) = ((Node){-1, 0});

	p = 0;
	for (int i = 0; i < len; ++i) {
		int c = (data[i] | 32) - 'a'; /* Get the alphabet index. */
		if (c < 26) {
			p = c;
			for (; i < len; ++i) {
				c = (data[i] | 32) - 'a';
				puts("ee");
				if (c < 26)
					p = find_child(p, c);
				else
					break;
			}
			++node[p].i;
		}
	}
	++node[p].i;
	node[0].i = 0;

	WordCount words[max_node_len];
	wc = 0;
	for (int i = 1; i < max_node_len; ++i) {
		int count = node[i].i;
		if (count == 0 || i % 27 == 0) continue;
		words[wc++] = (WordCount){.s = word_for(i), .i = count};
	}
	qsort(&words[0], max_node_len, sizeof(WordCount), wordsort);
	printf("%d, %s\n", words[0].i, words[0].s);
}
