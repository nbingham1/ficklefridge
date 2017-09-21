#include "recipe.h"

void query(vector<uint16_t> group)
{
	database data;
	data.load();
	list<query_recipe> recipe_scores;

	data.query(group, recipe_scores);
	int count = 0;
	for (list<query_recipe>::reverse_iterator i = recipe_scores.rbegin(); i != recipe_scores.rend() && count < 10; i++)
	{
		recipe r = data.get_recipe(i->offset);
		cout << "<li class=\"match\">";
		cout << "<div class=\"match-title\" id=\"match-title" << i->offset << "\">" << data.get_string(data.ftitle, r.title) << "</div>";
		if (i->green.size() > 0)
		{
			cout << "<div class=\"match-suggestion-list\">Just Add: ";
			for (size_t k = 0; k < i->green.size(); k++)
			{
				if (k != 0)
				{
					cout << ", ";
					if (k == i->green.size() - 1)
						cout << "and ";
				}
				cout << "<div class=\"match-suggestion\" onclick=\"recipe.add(" << i->green[k].first << ")\">" << data.get_string(data.fing, i->green[k].first) << "</div>";
			}
			cout << "</div>";
		}
		cout << "</li>";
		count++;
	}
}

int main(int argc, char **argv)
{
	vector<uint16_t> group;
	for (int i = 1; i < argc; i++)
		group.push_back(atoi(argv[i]));

	query(group);
}
