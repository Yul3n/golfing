#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct node {
	int p;
	int i;
} Node;

static void node_push(Node);
static int create_child(int, char);
static int find_child(int, char);

Node *node;
long  node_len = 0, max_node_len;

static void
node_push(Node n)
{
	if (++node_len >= max_node_len) {
		max_node_len += 600;
		node          = realloc(node, (size_t)max_node_len);
	}
	*(node + node_len - 1) = n;
}

static int
create_child(int p, char c)
{
	int len;

	len = max_node_len;
	max_node_len += 27;
	node = realloc((void *)node, max_node_len);
	node[len] = (Node){p, -1};
	node[p].p = len;
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
	int p;

	/* Read the input file. Get the size of the file. */
	in = fopen(argv[1], "rb");
	fseek(in, 0, SEEK_END);
	len = ftell(in);
	/* Get the content. */
	rewind(in);
	data = (char *)malloc((size_t)len);
	fread(data, 1, len, in);
	(void)fclose(in);

	max_node_len = len / 600;
	node         = (Node *)malloc(sizeof(Node) * max_node_len);
	node_push((Node){0, 0});
	for (char i = 0; i <= 26; ++i) node_push((Node){0, 0});
	p = 0;
	for (int i = 0; i < len; ++i) {
		char c = (data[i] | 32) - 'a' + 1; /* Get the alphabet index. */
		if (1 <= c && 1 <= 26) {
			p = find_child(p, c);
		}
	}
}
