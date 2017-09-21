#include "recipe.h"

int main(int argc, char **argv)
{
	database data;
	data.load();
	printf("%s", data.get_string(data.fing, atoi(argv[1])).c_str());
}
