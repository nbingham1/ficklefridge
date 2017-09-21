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
	list<query_recipe> recipe_scores;

	data.query(group, ingredient_scores, ingredient_max_scores, recipe_scores);
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

	cout << "====";

	int count = 0;
	for (list<query_recipe>::reverse_iterator i = recipe_scores.rbegin(); i != recipe_scores.rend() && count < 10; i++)
	{
		recipe r = data.get_recipe(i->offset);
		cout << "<div class=\"match-rec\">";
		cout << "<div class=\"match-rec-title\" id=\"match-rec-title" << i->offset << "\"><a class=\"match-rec-link\" href=\"#\">" << data.get_string(data.ftitle, r.title) << "</a></div>";
		if (i->green.size() > 0)
		{
				cout << "<div class=\"match-rec-ing\">Just Add: ";
				for (size_t k = 0; k < i->green.size(); k++)
				{
						if (k != 0)
						{
								cout << ", ";
								if (k == i->green.size() - 1)
										cout << "and ";
						}
						cout << "<div class=\"match-rec-sug-ing\" onclick=\"recipe.add(" << i->green[k].first << ")\">" << data.get_string(data.fing, i->green[k].first) << "</div>";
				}
				cout << "</div>";
		}
		cout << "<table class=\"match-rec-amt\" id=\"match-rec-amt" << i->offset << "\">";
		for (size_t j = 0; j < i->green.size(); j++)
			printf("<tr class=\"rec-ing\"><td class=\"rec-ing-amt\"><input class=\"rec-ing-amount\" id=\"%d\" name=\"%d\" value=\"%s\" /></td><td class=\"rec-ing-name-add\" onclick=\"recipe.add(%d)\">%s</td></tr>", i->green[j].first, i->green[j].first, data.get_string(data.famt, r.amount[i->green[j].second]).c_str(), i->green[j].first, data.get_string(data.fing, i->green[j].first).c_str());

		for (size_t j = 0; j < i->black.size(); j++)
			printf("<tr class=\"rec-ing\"><td class=\"rec-ing-amt\"><input class=\"rec-ing-amount\" id=\"%d\" name=\"%d\" value=\"%s\" /></td><td class=\"rec-ing-name\" onclick=\"recipe.remove(%d)\">%s</td></tr>", i->black[j].first, i->black[j].first, data.get_string(data.famt, r.amount[i->black[j].second]).c_str(), i->black[j].first, data.get_string(data.fing, i->black[j].first).c_str());

		for (size_t j = 0; j < i->red.size(); j++)
			printf("<tr class=\"rec-ing\"><td class=\"rec-ing-amt\"><input class=\"rec-ing-amount\" id=\"%d\" name=\"%d\" value=\"some\" /></td><td class=\"rec-ing-name-sub\" onclick=\"recipe.remove(%d)\">%s</td></tr>", i->red[j], i->red[j], i->red[j], data.get_string(data.fing, i->red[j]).c_str());

		cout << "</table>";

		cout << "<div class=\"match-rec-content\" id=\"match-rec-content" << i->offset << "\">";
		cout << "<div class=\"match-rec-meta\"></div>";
		for (size_t k = 0; k < r.instr.size(); k++)
				cout << "<div class=\"match-rec-step\">" << data.get_string(data.finstr, r.instr[k]) << "</div>";
		cout << "</div>";

		cout << "</div>";
		count++;
	}
}

void query1(vector<uint16_t> group)
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

void query2(vector<uint16_t> group)
{
	database data;
	data.load();
	list<query_recipe> recipe_scores;

	data.query(group, recipe_scores);
	int count = 0;
	for (list<query_recipe>::reverse_iterator i = recipe_scores.rbegin(); i != recipe_scores.rend() && count < 10; i++)
	{
		recipe r = data.get_recipe(i->offset);
		cout << "<div class=\"match-rec\">";
		cout << "<div class=\"match-rec-title\" id=\"match-rec-title" << i->offset << "\"><a class=\"match-rec-link\" href=\"#\">" << data.get_string(data.ftitle, r.title) << "</a></div>";
		if (i->green.size() > 0)
		{
				cout << "<div class=\"match-rec-ing\">Just Add: ";
				for (size_t k = 0; k < i->green.size(); k++)
				{
						if (k != 0)
						{
								cout << ", ";
								if (k == i->green.size() - 1)
										cout << "and ";
						}
						cout << "<div class=\"match-rec-sug-ing\" onclick=\"recipe.add(" << i->green[k].first << ")\">" << data.get_string(data.fing, i->green[k].first) << "</div>";
				}
				cout << "</div>";
		}
		cout << "<table class=\"match-rec-amt\" id=\"match-rec-amt" << i->offset << "\">";
		for (size_t j = 0; j < i->green.size(); j++)
			printf("<tr class=\"rec-ing\"><td class=\"rec-ing-amt\"><input class=\"rec-ing-amount\" id=\"%d\" name=\"%d\" value=\"%s\" /></td><td class=\"rec-ing-name-add\" onclick=\"recipe.add(%d)\">%s</td></tr>", i->green[j].first, i->green[j].first, data.get_string(data.famt, r.amount[i->green[j].second]).c_str(), i->green[j].first, data.get_string(data.fing, i->green[j].first).c_str());

		for (size_t j = 0; j < i->black.size(); j++)
			printf("<tr class=\"rec-ing\"><td class=\"rec-ing-amt\"><input class=\"rec-ing-amount\" id=\"%d\" name=\"%d\" value=\"%s\" /></td><td class=\"rec-ing-name\" onclick=\"recipe.remove(%d)\">%s</td></tr>", i->black[j].first, i->black[j].first, data.get_string(data.famt, r.amount[i->black[j].second]).c_str(), i->black[j].first, data.get_string(data.fing, i->black[j].first).c_str());

		for (size_t j = 0; j < i->red.size(); j++)
			printf("<tr class=\"rec-ing\"><td class=\"rec-ing-amt\"><input class=\"rec-ing-amount\" id=\"%d\" name=\"%d\" value=\"some\" /></td><td class=\"rec-ing-name-sub\" onclick=\"recipe.remove(%d)\">%s</td></tr>", i->red[j], i->red[j], i->red[j], data.get_string(data.fing, i->red[j]).c_str());

		cout << "</table>";

		cout << "<div class=\"match-rec-content\" id=\"match-rec-content" << i->offset << "\">";
		cout << "<div class=\"match-rec-meta\"></div>";
		for (size_t k = 0; k < r.instr.size(); k++)
				cout << "<div class=\"match-rec-step\">" << data.get_string(data.finstr, r.instr[k]) << "</div>";
		cout << "</div>";

		cout << "</div>";
		count++;
	}
}

void update_amts(vector<uint16_t> group, uint16_t offset)
{
	database data;
	data.load();
	list<query_recipe> recipe_scores;

	recipe r = data.get_recipe(offset);
	query_recipe qr = data.get_query_recipe(group, r.offset);

	for (size_t j = 0; j < qr.green.size(); j++)
		printf("<tr class=\"rec-ing\"><td class=\"rec-ing-amt\"><input class=\"rec-ing-amount\" id=\"%d\" name=\"%d\" value=\"%s\" /></td><td class=\"rec-ing-name-add\" onclick=\"recipe.add(%d)\">%s</td></tr>", qr.green[j].first, qr.green[j].first, data.get_string(data.famt, r.amount[qr.green[j].second]).c_str(), qr.green[j].first, data.get_string(data.fing, qr.green[j].first).c_str());

	for (size_t j = 0; j < qr.black.size(); j++)
		printf("<tr class=\"rec-ing\"><td class=\"rec-ing-amt\"><input class=\"rec-ing-amount\" id=\"%d\" name=\"%d\" value=\"%s\" /></td><td class=\"rec-ing-name\" onclick=\"recipe.remove(%d)\">%s</td></tr>", qr.black[j].first, qr.black[j].first, data.get_string(data.famt, r.amount[qr.black[j].second]).c_str(), qr.black[j].first, data.get_string(data.fing, qr.black[j].first).c_str());

	for (size_t j = 0; j < qr.red.size(); j++)
		printf("<tr class=\"rec-ing\"><td class=\"rec-ing-amt\"><input class=\"rec-ing-amount\" id=\"%d\" name=\"%d\" value=\"some\" /></td><td class=\"rec-ing-name-sub\" onclick=\"recipe.remove(%d)\">%s</td></tr>", qr.red[j], qr.red[j], qr.red[j], data.get_string(data.fing, qr.red[j]).c_str());
}

void update_content(vector<uint16_t> group, uint16_t offset)
{
	database data;
	data.load();
	list<query_recipe> recipe_scores;

	recipe r = data.get_recipe(offset);
	query_recipe qr = data.get_query_recipe(group, r.offset);

	cout << "<div class=\"match-rec-meta\"></div>";
	for (size_t k = 0; k < r.instr.size(); k++)
			cout << "<div class=\"match-rec-step\">" << data.get_string(data.finstr, r.instr[k]) << "</div>";
	cout << "</div>";
}

int main()
{
//	database data;
//	data.initialize_ingredients("database.csv");
//	data.initialize_recipes("database.csv");

//	vector<uint16_t> group;
//	group.push_back(3828);
//	group.push_back(23191);
//	query2(group);

	database data;
	data.load();

	data.suggest("gar");
}
