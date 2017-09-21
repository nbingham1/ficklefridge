#include "recipe.h"

int main(int argc, char **argv)
{
	database data;
	data.load();
	printf("%d", data.get_index(data.fing, string(argv[1])));
}
