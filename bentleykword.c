#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct node {
	int p;
	int i;
} Node;

static char *word_for(int);
static int create_child(int, char);
static int find_child(int, char);

Node *node;
long  node_len = 0, max_node_len;

static char *
word_for(int p)
{
	int   size, i, c;
	char *s;

	i    = 0;
	size = 256;
	s    = malloc(size);
	while (p != 0) {
		c = p % 27;
		if (++i >= size) {
			size += 256;
			s     = realloc(s, size);
		}
		*(s + i) = c;
		p = (p - c) ? node[p - c].p : 0;
	}
	return s;
}

static int
create_child(int p, char c)
{
	int len;

	len           = max_node_len;
	max_node_len += 27;
	node          = realloc((void *)node, max_node_len);
	node[len]     = (Node){p, -1};
	node[p].p     = len;
	return len + c;
}

static int
find_child(int p, char c)
{
	if (p == 0)
		return c;
	else if (node[p].p == 0)
		return create_child(p, c);
	else
		return node[p].p + c;
}

int
main(int argc, char **argv)
{
	FILE *in;
	long  len;
	char *data;
	int   p;

	/* Read the input file. Get the size of the file. */
	in = fopen(argv[1], "rb");
	fseek(in, 0, SEEK_END);
	len = ftell(in);
	/* Get the content. */
	rewind(in);
	data = (char *)malloc((size_t)len);
	fread(data, 1, len, in);
	(void)fclose(in);

	max_node_len = 27;
	node         = (Node *)malloc(sizeof(Node) * max_node_len);
	for (char i = 0; i <= 26; ++i)
		*(node + i) = ((Node){0, 0});

	p = 0;
	for (int i = 0; i < len; ++i) {
		char c = (data[i] | 32) - 'a' + 1; /* Get the alphabet index. */
		if (1 <= c && 1 <= 26)
			p = find_child(p, c);
		else {
			++node[p].i;
			p = 0;
		}
	}
	++node[p].i;
	node[0].i = 0;

	for (int i = 1; i < max_node_len; ++i)
		int count = node[i].i;
}
