#include "recipe.h"

int main(int argc, char **argv)
{
	database data;
	data.load();
	if (argc > 1 && strlen(argv[1]) > 0)
		data.suggest(argv[1]);
}
