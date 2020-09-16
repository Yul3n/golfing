#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct node {
	int p;
	int i;
} Node;

static void node_push(Node);

Node *node;
long node_len = 0, max_node_len;

int
main(int argc, char **argv)
{
	FILE *in;
	long len;
	char *data;

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
	node = (Node *)malloc(sizeof(Node) * max_node_len);
}
