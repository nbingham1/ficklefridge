#include "recipe.h"

void update_content(vector<uint16_t> group, uint32_t offset)
{
        database data;
        data.load();
        list<query_recipe> recipe_scores;

        recipe r = data.get_recipe(offset);
        query_recipe qr = data.get_query_recipe(group, r.offset);

	/*cout << "<div id=\"servings\">Servings: " << r.servings << "</div>";
	cout << "<div id=\"rating\">";
        for (float i = 0.0f; i < r.rating; i++)
                cout << "<img width=50px height=50px src=\"http://www.larastheme.net/wp-content/uploads/2014/03/goldstar.jpg\">";
        cout << "</div>";*/
        for (size_t k = 0; k < r.instr.size(); k++)
        {
            string s = data.get_string(data.finstr, r.instr[k]) + "\n\n";
            string result;
            for (size_t i = 0; i < s.size(); i++)
            {
                if (s[i] == '\n')
                    result += "<br>";
                else if (s[i] != '\r')
                    result += s[i];
            }
            cout << result;
        }
}

int main(int argc, char **argv)
{
	vector<uint16_t> group;
	if (argc < 2)
		return 0;

	uint32_t offset = atoi(argv[1]);

	for (int i = 2; i < argc; i++)
        	group.push_back(atoi(argv[i]));

	update_content(group, offset);
}
