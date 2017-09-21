#include "recipe.h"

#define RED0 49
#define RED1 175
#define GREEN0 175
#define GREEN1 175
#define BLUE0 245
#define BLUE1 175

void query(vector<uint16_t> group)
{
	database data;
	data.load();
	vector<vector<float> > ingredient_scores;
	vector<float> ingredient_max_scores;

	data.query(group, ingredient_scores, ingredient_max_scores);
	int mr = RED1-RED0, mg = GREEN1-GREEN0, mb = BLUE1-BLUE0;
	for (size_t i = 0; i < ingredient_scores.size(); i++)
	{
			float v = 0.0;
			for (size_t j = 0; j < ingredient_scores[i].size(); j++)
					if (ingredient_max_scores[j] > 0.0)
							v += 5000.0*((float)j/(float)ingredient_max_scores.size())*(ingredient_scores[i][j]/ingredient_max_scores[j]);

			v = sqrt(v);
			if (v >= 8.0)
			{
					int n = ((int)v)*3;
					int r = (n*mr)/255;
					int g = (n*mg)/255;
					int b = (n*mb)/255;
					r = RED1 - (r > mr ? mr : r);
					g = GREEN1 - (g > mg ? mg : g);
					b = BLUE1 - (b < mb ? mb : b);
					printf("<div class=\"sug-ing\" onclick=\"recipe.add(%d)\" style=\"font-size:%dpx;color:#%02x%02x%02x;\">%s</div>", i, (int)v, r, g, b, data.get_string(data.fing, i).c_str());
			}
	}
}

int main(int argc, char **argv)
{
	vector<uint16_t> group;
	for (int i = 1; i < argc; i++)
        	group.push_back(atoi(argv[i]));

	query(group);
}
