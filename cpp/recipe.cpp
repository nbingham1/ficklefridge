#include "recipe.h"

recipe::recipe()
{
	offset = 0;
	title = 0;
	rating = 0.0;
	rate_count = 0;
	servings = 0;
	prep_time = 0;
	cook_time = 0;
}

recipe::~recipe()
{

}

recipe &recipe::operator=(recipe r)
{
	this->offset = r.offset;
	this->title = r.title;
	this->rating = r.rating;
	this->rate_count = r.rate_count;
	this->servings = r.servings;
	this->prep_time = r.prep_time;
	this->cook_time = r.cook_time;
	this->ingr = r.ingr;
	this->instr = r.instr;
	this->amount = r.amount;
	return *this;
}

void recipe::clear()
{
	offset = 0;
	title = 0;
	rating = 0.0;
	rate_count = 0;
	servings = 0;
	prep_time = 0;
	cook_time = 0;
	ingr.clear();
	instr.clear();
	amount.clear();
}

query_recipe::query_recipe()
{
	score = -9.0E99;
	offset = 0;
}

query_recipe::query_recipe(const query_recipe &r)
{
	score = r.score;
	offset = r.offset;
	green = r.green;
	black = r.black;
	red = r.red;
}

query_recipe::~query_recipe()
{

}

query_recipe &query_recipe::operator=(query_recipe r)
{
	score = r.score;
	offset = r.offset;
	green = r.green;
	black = r.black;
	red = r.red;

	return *this;
}

bool operator<(query_recipe r1, query_recipe r2)
{
	return r1.score < r2.score;
}

bool operator>(query_recipe r1, query_recipe r2)
{
	return r2.score > r1.score;
}

bool operator==(query_recipe r1, query_recipe r2)
{
	return r1.score == r2.score;
}

database::database()
{
	ftitle = NULL;
	finstr = NULL;
	fing = NULL;
	famt = NULL;
	frec = NULL;
	fcon = NULL;
}

database::~database()
{
	if (ftitle != NULL)
		fclose(ftitle);
	if (finstr != NULL)
		fclose(finstr);
	if (fing != NULL)
		fclose(fing);
	if (famt != NULL)
		fclose(famt);
	if (frec != NULL)
		fclose(frec);
	if (fcon != NULL)
		fclose(fcon);

	ftitle = NULL;
	finstr = NULL;
	fing = NULL;
	famt = NULL;
	frec = NULL;
	fcon = NULL;
}

void database::load()
{
	if (ftitle == NULL)
		ftitle = fopen("/var/data/recipes/titles", "rb");
	if (finstr == NULL)
		finstr = fopen("/var/data/recipes/instructions", "rb");
	if (fing == NULL)
		fing = fopen("/var/data/recipes/ingredients", "rb");
	if (famt == NULL)
		famt = fopen("/var/data/recipes/amounts", "rb");
	if (frec == NULL)
		frec = fopen("/var/data/recipes/recipes", "rb");
	if (fcon == NULL)
		fcon = fopen("/var/data/recipes/connections", "rb");
}

void database::initialize_strings(string filename)
{
	cout << "initializing strings" << endl;
	string str;
	string amt;
	string ing;
	char c;
	size_t idx;
	int entry;
	int i;

	if (ftitle == NULL)
		ftitle = fopen("/var/data/recipes/titles", "wb+");
	if (finstr == NULL)
		finstr = fopen("/var/data/recipes/instructions", "wb+");
	if (fing == NULL)
		fing = fopen("/var/data/recipes/ingredients", "wb+");
	if (famt == NULL)
		famt = fopen("/var/data/recipes/amounts", "wb+");

	list<string> titles;
	list<string> ingredients;
	list<string> amounts;
	list<string> instructions;

	FILE *database = fopen(filename.c_str(), "r");

	size_t max_ingredient = 0;
	size_t max_instruction = 0;
	size_t max_title = 0;
	size_t max_amount = 0;

	entry = 0;
	uint32_t count = 0;
	fseek(database, 0, SEEK_SET);
	while (!feof(database))
	{
		c = fgetc(database);
		if (c == ',' || c == '\n')
		{
			replace(str.begin(), str.end(), '_', ',');
			str = rtrim(ltrim(str));
			idx = str.find("->");
			if (idx != str.npos)
			{
				amt = parse_amount(str.substr(0, idx));
				amt = rtrim(ltrim(amt));
				if (str.length() > idx+2)
				{
					ing = parse_ing(str.substr(idx+2, str.length()-idx-2));
					ing = rtrim(ltrim(ing));
					if (ing.size() > max_ingredient)
						max_ingredient = ing.size();

					if (amt.size() > max_amount)
						max_amount = amt.size();

					list<string>::iterator search_ingredient = lower_bound(ingredients.begin(), ingredients.end(), ing);
					list<string>::iterator search_amount = lower_bound(amounts.begin(), amounts.end(), amt);
					if (search_ingredient == ingredients.end() || *search_ingredient != ing)
						ingredients.insert(search_ingredient, ing);

					if (search_amount == amounts.end() || *search_amount != amt)
						amounts.insert(search_amount, amt);
				}
			}
			else
			{
				if (entry == 0)
				{
					if (str.size() > max_title)
						max_title = str.size();
					list<string>::iterator search_title = lower_bound(titles.begin(), titles.end(), str);
					if (search_title == titles.end() || *search_title != str)
						titles.insert(search_title, str);
				}
				else if (entry > 5 && str != "")
				{
					if (str.size() > max_instruction)
						max_instruction = str.size();
					list<string>::iterator search_instruction = lower_bound(instructions.begin(), instructions.end(), str);
					if (search_instruction == instructions.end() || *search_instruction != str)
						instructions.insert(search_instruction, str);
				}
			}

			if (c == '\n')
			{
				entry = 0;
				if (((size_t)(count/100))*100 == count)
				{
					printf("\rrecipe %d", count);
					fflush(stdout);
				}
				count++;
			}
			else
				entry++;

			str = "";
			ing = "";
			amt = "";
		}
		else
			str += c;
	}

	max_ingredient++;
	i = ingredients.size();
	fwrite(&i, 4, 1, fing);
	fwrite(&max_ingredient, 4, 1, fing);
	for (list<string>::iterator iter = ingredients.begin(); iter != ingredients.end(); iter++)
	{
		cout << "\"" << *iter << "\"" << endl;
		iter->resize(max_ingredient, '\0');
		fwrite(iter->c_str(), 1, max_ingredient, fing);
	}
	ingredients.clear();
	clearerr(fing);
	fseek(fing, 0, SEEK_SET);

	max_instruction++;
	i = instructions.size();
	fwrite(&i, 4, 1, finstr);
	fwrite(&max_instruction, 4, 1, finstr);
	for (list<string>::iterator iter = instructions.begin(); iter != instructions.end(); iter++)
	{
		cout << "\"" << *iter << "\"" << endl;
		iter->resize(max_instruction, '\0');
		fwrite(iter->c_str(), 1, max_instruction, finstr);
	}
	instructions.clear();
	clearerr(finstr);
	fseek(finstr, 0, SEEK_SET);

	max_amount++;
	i = amounts.size();
	fwrite(&i, 4, 1, famt);
	fwrite(&max_amount, 4, 1, famt);
	for (list<string>::iterator iter = amounts.begin(); iter != amounts.end(); iter++)
	{
		cout << "\"" << *iter << "\"" << endl;
		iter->resize(max_amount, '\0');
		fwrite(iter->c_str(), 1, max_amount, famt);
	}
	amounts.clear();
	clearerr(famt);
	fseek(famt, 0, SEEK_SET);

	max_title++;
	i = titles.size();
	fwrite(&i, 4, 1, ftitle);
	fwrite(&max_title, 4, 1, ftitle);
	for (list<string>::iterator iter = titles.begin(); iter != titles.end(); iter++)
	{
		cout << "\"" << *iter << "\"" << endl;
		iter->resize(max_title, '\0');
		fwrite(iter->c_str(), 1, max_title, ftitle);
	}
	titles.clear();
	clearerr(ftitle);
	fseek(ftitle, 0, SEEK_SET);

	fclose(database);
	cout << "done" << endl;
}

void database::initialize_ingredients(string filename)
{
	cout << "initializing strings" << endl;
	string str;
	string amt;
	string ing;
	char c;
	size_t idx;
	int entry;
	int i;

	if (fing == NULL)
		fing = fopen("/var/data/recipes/ingredients", "wb+");

	list<string> ingredients;

	FILE *database = fopen(filename.c_str(), "r");

	size_t max_ingredient = 0;

	entry = 0;
	uint32_t count = 0;
	fseek(database, 0, SEEK_SET);
	while (!feof(database))
	{
		c = fgetc(database);
		if (c == ',' || c == '\n')
		{
			replace(str.begin(), str.end(), '_', ',');
			str = rtrim(ltrim(str));
			idx = str.find("->");
			if (idx != str.npos)
			{
				amt = parse_amount(str.substr(0, idx));
				amt = rtrim(ltrim(amt));
				if (str.length() > idx+2)
				{
					ing = parse_ing(str.substr(idx+2, str.length()-idx-2));
					ing = rtrim(ltrim(ing));
					if (ing.size() > max_ingredient)
						max_ingredient = ing.size();

					list<string>::iterator search_ingredient = lower_bound(ingredients.begin(), ingredients.end(), ing);
					if (search_ingredient == ingredients.end() || *search_ingredient != ing)
						ingredients.insert(search_ingredient, ing);
				}
			}

			if (c == '\n')
			{
				entry = 0;
				if (((size_t)(count/100))*100 == count)
				{
					printf("\rrecipe %d", count);
					fflush(stdout);
				}
				count++;
			}
			else
				entry++;

			str = "";
			ing = "";
			amt = "";
		}
		else
			str += c;
	}

	max_ingredient++;
	i = ingredients.size();
	fwrite(&i, 4, 1, fing);
	fwrite(&max_ingredient, 4, 1, fing);
	for (list<string>::iterator iter = ingredients.begin(); iter != ingredients.end(); iter++)
	{
		cout << "\"" << *iter << "\"" << endl;
		iter->resize(max_ingredient, '\0');
		fwrite(iter->c_str(), 1, max_ingredient, fing);
	}
	ingredients.clear();
	clearerr(fing);
	fseek(fing, 0, SEEK_SET);
}

void database::initialize_amounts(string filename)
{
	cout << "initializing amounts" << endl;
	string str;
	string amt;
	string ing;
	char c;
	size_t idx;
	int entry;
	int i;

	if (famt == NULL)
		famt = fopen("/var/data/recipes/amounts", "wb+");

	list<string> amounts;

	FILE *database = fopen(filename.c_str(), "r");

	size_t max_amount = 0;

	entry = 0;
	uint32_t count = 0;
	fseek(database, 0, SEEK_SET);
	while (!feof(database))
	{
		c = fgetc(database);
		if (c == ',' || c == '\n')
		{
			replace(str.begin(), str.end(), '_', ',');
			str = rtrim(ltrim(str));
			idx = str.find("->");
			if (idx != str.npos)
			{
				amt = parse_amount(str.substr(0, idx));
				amt = rtrim(ltrim(amt));
				if (str.length() > idx+2)
				{
					ing = parse_ing(str.substr(idx+2, str.length()-idx-2));
					if (amt.size() > max_amount)
						max_amount = amt.size();

					list<string>::iterator search_amount = lower_bound(amounts.begin(), amounts.end(), amt);
					if (search_amount == amounts.end() || *search_amount != amt)
						amounts.insert(search_amount, amt);
				}
			}

			if (c == '\n')
			{
				entry = 0;
				if (((size_t)(count/100))*100 == count)
				{
					printf("\rrecipe %d", count);
					fflush(stdout);
				}
				count++;
			}
			else
				entry++;

			str = "";
			ing = "";
			amt = "";
		}
		else
			str += c;
	}

	max_amount++;
	i = amounts.size();
	fwrite(&i, 4, 1, famt);
	fwrite(&max_amount, 4, 1, famt);
	for (list<string>::iterator iter = amounts.begin(); iter != amounts.end(); iter++)
	{
		cout << "\"" << *iter << "\"" << endl;
		iter->resize(max_amount, '\0');
		fwrite(iter->c_str(), 1, max_amount, famt);
	}
	amounts.clear();
	clearerr(famt);
	fseek(famt, 0, SEEK_SET);

	fclose(database);
	cout << "done" << endl;
}

void database::initialize_recipes(string filename)
{
	cout << "initializing recipes" << endl;
	recipe r;
	string str;
	string amt;
	string ing;
	char c;
	size_t idx;
	int entry;

	if (ftitle == NULL)
		ftitle = fopen("/var/data/recipes/titles", "rb");
	if (finstr == NULL)
		finstr = fopen("/var/data/recipes/instructions", "rb");
	if (fing == NULL)
		fing = fopen("/var/data/recipes/ingredients", "rb");
	if (famt == NULL)
		famt = fopen("/var/data/recipes/amounts", "rb");
	if (frec == NULL)
		frec = fopen("/var/data/recipes/recipes", "wb+");
	if (fcon == NULL)
		fcon = fopen("/var/data/recipes/connections", "wb+");

	FILE *database = fopen(filename.c_str(), "r");

	vector<recipe> recipes;

	entry = 0;
	uint32_t count = 0;
	fseek(database, 0, SEEK_SET);
	while (!feof(database))
	{
		c = fgetc(database);
		if (c == ',' || c == '\n')
		{
			replace(str.begin(), str.end(), '_', ',');
			str = rtrim(ltrim(str));
			idx = str.find("->");
			if (idx != str.npos)
			{
				amt = parse_amount(str.substr(0, idx));
				amt = rtrim(ltrim(amt));
				if (str.length() > idx+2)
				{
					ing = parse_ing(str.substr(idx+2, str.length()-idx-2));
					ing = rtrim(ltrim(ing));

					r.ingr.push_back(get_index(fing, ing));
					r.amount.push_back(get_index(famt, amt));
				}
			}
			else if (entry == 0)
				r.title = get_index(ftitle, str);
			else if (entry == 1)
				r.rating = atof(str.c_str());
			else if (entry == 2)
				r.rate_count = atoi(str.c_str());
			else if (entry == 3)
				r.servings = atoi(str.c_str());
			else if (entry == 4)
				r.prep_time = parse_time(str);
			else if (entry == 5)
				r.cook_time = parse_time(str);
			else if (str != "")
				r.instr.push_back(get_index(finstr, str));

			if (c == '\n')
			{
				entry = 0;
				if (r.ingr.size() > 0)
				{
					recipes.push_back(r);
					if (((size_t)(count/100))*100 == count)
					{
						printf("\rrecipe %d", count);
						fflush(stdout);
					}
					count++;
				}
				r.clear();
			}
			else if (c == ',')
				entry++;

			str = "";
			ing = "";
			amt = "";
		}
		else
			str += c;
	}

	fclose(database);

	uint32_t cap = 0xFFFFFFFF;
	uint32_t con_offset = 0;
	uint32_t rec_offset = 0;
	for (size_t i = 0; i < recipes.size(); i++)
	{
		uint32_t old_rec_offset = rec_offset;
		// RECIPE DATABASE
		// Title Index
		fwrite(&recipes[i].title, 4, 1, frec);
		rec_offset += 4;
		// an offset into the connections database
		fwrite(&con_offset, 4, 1, frec);
		rec_offset += 4;

		fwrite(&recipes[i].rating, 4, 1, frec);
		rec_offset += 4;
		fwrite(&recipes[i].rate_count, 4, 1, frec);
		rec_offset += 4;
		fwrite(&recipes[i].servings, 4, 1, frec);
		rec_offset += 4;
		fwrite(&recipes[i].prep_time, 4, 1, frec);
		rec_offset += 4;
		fwrite(&recipes[i].cook_time, 4, 1, frec);
		rec_offset += 4;

		// List of amount indices followed by 0xFFFFFFFF
		for (size_t j = 0; j < recipes[i].amount.size(); j++)
		{
			fwrite(&recipes[i].amount[j], 4, 1, frec);
			rec_offset += 4;
		}
		fwrite(&cap, 4, 1, frec);
		rec_offset += 4;

		// List of instruction indices followed by 0xFFFFFFFF
		for (size_t j = 0; j < recipes[i].instr.size(); j++)
		{
			fwrite(&recipes[i].instr[j], 4, 1, frec);
			rec_offset += 4;
		}
		fwrite(&cap, 4, 1, frec);
		rec_offset += 4;

		// CONNECTIONS DATABASE
		fwrite(&recipes[i].rating, 4, 1, fcon);
		con_offset += 4;
		// List of ingredient indices followed by 0xFFFF
		for (size_t j = 0; j < recipes[i].ingr.size(); j++)
		{
			fwrite(&recipes[i].ingr[j], 2, 1, fcon);
			con_offset += 2;
		}
		fwrite(&cap, 2, 1, fcon);
		con_offset += 2;
		// an offset into the recipes database
		fwrite(&old_rec_offset, 4, 1, fcon);
		con_offset += 4;
	}

	clearerr(frec);
	fseek(frec, 0, SEEK_SET);
}

string database::get_string(FILE *fin, int index)
{
	size_t step_size = 0, num_entries = 0;
	char c;
	string found = "";

	clearerr(fin);
	fseek(fin, 0, SEEK_SET);
	fread(&num_entries, 4, 1, fin);
	fread(&step_size, 4, 1, fin);
	fseek(fin, index*step_size+8, SEEK_SET);

	while ((c = fgetc(fin)) != '\0' && found.size() < step_size)
		found += c;

	clearerr(fin);
	fseek(fin, 0, SEEK_SET);

	return found;
}

int database::get_index(FILE *fin, string str)
{
	int ubound = 0, lbound = 0;
	int step_size = 0, num_entries = 0;
	string found = "";
	int idx;
	char c;

	clearerr(fin);
	fseek(fin, 0, SEEK_SET);
	fread(&num_entries, 4, 1, fin);
	fread(&step_size, 4, 1, fin);
	ubound = num_entries+1;

	while (found != str)
	{
		found = "";
		if (lbound == ubound)
			idx = lbound;
		else
			idx = (rand()%(ubound - lbound)) + lbound;

		fseek(fin, idx*step_size + 8, SEEK_SET);
		while (!feof(fin) && (int)found.size() < step_size && (c = fgetc(fin)) != '\0')
			found += c;

		if (lbound == ubound && found != str)
		{
			//cout << "failure to find id for " << str << " should be at " << found << endl;
			exit(0);
		}

		//cout << idx << " " << lbound << " " << ubound << endl;

		if (str > found)
			lbound = idx+1;
		else if (str < found)
			ubound = idx;
		else if (str.length() > found.length())
			lbound = idx+1;
		else if (str.length() < found.length())
			ubound = idx;
	}
	clearerr(fin);
	fseek(fin, 0, SEEK_SET);

	if (found != str)
		cout << "error '" << str << "' '" << found << "'" << endl;

	return idx;
}

recipe database::get_recipe(uint32_t offset)
{
	recipe result;
	uint32_t idx;

	clearerr(frec);
	fseek(frec, offset, SEEK_SET);
	fread(&result.title, 4, 1, frec);
	fread(&result.offset, 4, 1, frec);

	fread(&result.rating, 4, 1, frec);
	fread(&result.rate_count, 4, 1, frec);
	fread(&result.servings, 4, 1, frec);
	fread(&result.prep_time, 4, 1, frec);
	fread(&result.cook_time, 4, 1, frec);

	// List of amount indices followed by 0xFFFFFFFF
	fread(&idx, 4, 1, frec);
	while (!feof(frec) && idx != 0xFFFFFFFF)
	{
		result.amount.push_back(idx);
		fread(&idx, 4, 1, frec);
	}

	// List of instruction indices followed by 0xFFFFFFFF
	fread(&idx, 4, 1, frec);
	while (!feof(frec) && idx != 0xFFFFFFFF)
	{
		result.instr.push_back(idx);
		fread(&idx, 4, 1, frec);
	}
	clearerr(frec);
	fseek(frec, 0, SEEK_SET);
	return result;
}

query_recipe database::get_query_recipe(vector<uint16_t> selected, uint32_t offset)
{
	query_recipe result;
	result.red = selected;

	clearerr(fcon);
	fseek(fcon, offset, SEEK_SET);

	float rating = 0.0;
	fread(&rating, 4, 1, fcon);

	uint16_t idx = 0;
	uint16_t count = 0;
	fread(&idx, 2, 1, fcon);
	while (!feof(fcon) && idx != 0xFFFF)
	{
		vector<uint16_t>::iterator red_search = find(result.red.begin(), result.red.end(), idx);
		if (red_search != result.red.end())
		{
			result.red.erase(red_search);
			result.black.push_back(pair<uint16_t, uint16_t>(idx, count));
		}
		else if (find(selected.begin(), selected.end(), idx) == selected.end())
			result.green.push_back(pair<uint16_t, uint16_t>(idx, count));

		count++;
		fread(&idx, 2, 1, fcon);
	}
	fread(&(result.offset), 4, 1, fcon);

	result.score = 100.0*((float)result.black.size() - (float)result.green.size())/((float)result.green.size() + (float)result.red.size() + (float)result.black.size());

	return result;
}

void database::suggest(string search)
{
        uint32_t step_size = 0;
        uint32_t num_entries = 0;
        char c;
        int step = 0;
        int count = 0;

        fseek(fing, 0, SEEK_SET);
        fread(&num_entries, 4, 1, fing);
        fread(&step_size, 4, 1, fing);

        for (size_t i = 0; i < num_entries && step < 2; i++)
        {
                string found = "";
                fseek(fing, i*step_size+8, SEEK_SET);
                while (found.length() < step_size && !feof(fing) && (c = fgetc(fing)) != '\0')
                        found += c;

                if (strncmp(found.c_str(), search.c_str(), search.length()) == 0)
                {
                        if (step == 0)
                                step++;
                        printf("<tr><td onclick=\"recipe.add(%d);$(\'#suggestions\').html(\'\');$(\'#suggestions\').css(\'border-style\', \'none\');$(\'#add\').val(\'\');\" style=\"cursor: pointer;\" class=\"sug-search\" id=\"suggest%d\">%s</td><tr>", i, count, found.c_str());
                        count++;
                        if (count >= 10)
                                return;
                }
                else if (step > 1)
                        step++;
        }
}

void database::query(vector<uint16_t> selected, list<query_recipe> &recipe_scores)
{
	clearerr(fcon);
	fseek(fcon, 0, SEEK_SET);
	while (!feof(fcon))
	{
		recipe_scores.push_back(query_recipe());
		recipe_scores.back().red = selected;

		float rating = 0.0;
		fread(&rating, 4, 1, fcon);

		uint16_t idx = 0;
		uint16_t count = 0;
		fread(&idx, 2, 1, fcon);
		while (!feof(fcon) && idx != 0xFFFF)
		{
			vector<uint16_t>::iterator red_search = find(recipe_scores.back().red.begin(), recipe_scores.back().red.end(), idx);
			if (red_search != recipe_scores.back().red.end())
			{
				recipe_scores.back().red.erase(red_search);
				recipe_scores.back().black.push_back(pair<uint16_t, uint16_t>(idx, count));
			}
			else if (find(selected.begin(), selected.end(), idx) == selected.end())
				recipe_scores.back().green.push_back(pair<uint16_t, uint16_t>(idx, count));

			count++;
			fread(&idx, 2, 1, fcon);
		}
		fread(&(recipe_scores.back().offset), 4, 1, fcon);

		recipe_scores.back().score = ((float)recipe_scores.back().black.size()/(0.01+(float)recipe_scores.back().green.size()));
	}

	recipe_scores.sort();
}

void database::query(vector<uint16_t> selected, vector<vector<float> > &ingredient_scores, vector<float> &ingredient_max_scores)
{
	clearerr(fcon);
	fseek(fcon, 0, SEEK_SET);
	while (!feof(fcon))
	{
		vector<uint16_t> green;
		vector<uint16_t> red = selected;
		uint32_t offset;

		float score = 0.0;
		float rating = 0.0;
		fread(&rating, 4, 1, fcon);

		uint16_t idx = 0;
		uint16_t count = 0;
		uint16_t match = 0;
		fread(&idx, 2, 1, fcon);
		while (!feof(fcon) && idx != 0xFFFF)
		{
			vector<uint16_t>::iterator red_search = find(red.begin(), red.end(), idx);
			if (red_search != red.end())
			{
				red.erase(red_search);
				match++;
				score += rating;
			}
			else if (find(selected.begin(), selected.end(), idx) == selected.end())
				green.push_back(idx);

			count++;
			fread(&idx, 2, 1, fcon);
		}
		fread(&(offset), 4, 1, fcon);

		for (size_t i = 0; i < green.size() && score > 0.0f; i++)
		{
			if (ingredient_scores.size() <= green[i])
				ingredient_scores.resize(green[i]+1, vector<float>());
			if (ingredient_scores[green[i]].size() <= match)
				ingredient_scores[green[i]].resize(match+1, 0.0f);

			ingredient_scores[green[i]][match] += score;
		}
	}

	for (size_t i = 0; i < ingredient_scores.size(); i++)
	{
		if (ingredient_max_scores.size() < ingredient_scores[i].size())
			ingredient_max_scores.resize(ingredient_scores[i].size(), 0.0);

		for (size_t j = 0; j < ingredient_scores[i].size(); j++)
			if (ingredient_scores[i][j] > ingredient_max_scores[j])
				ingredient_max_scores[j] = ingredient_scores[i][j];
	}
}

void database::query(vector<uint16_t> selected, vector<vector<float> > &ingredient_scores, vector<float> &ingredient_max_scores, list<query_recipe> &recipe_scores)
{
	clearerr(fcon);
	fseek(fcon, 0, SEEK_SET);
	while (!feof(fcon))
	{
		recipe_scores.push_back(query_recipe());
		recipe_scores.back().red = selected;

		float score = 0.0;
		float rating = 0.0;
		fread(&rating, 4, 1, fcon);

		uint16_t idx = 0;
		uint16_t count = 0;
		fread(&idx, 2, 1, fcon);
		while (!feof(fcon) && idx != 0xFFFF)
		{
			vector<uint16_t>::iterator red_search = find(recipe_scores.back().red.begin(), recipe_scores.back().red.end(), idx);
			if (red_search != recipe_scores.back().red.end())
			{
				recipe_scores.back().red.erase(red_search);
				recipe_scores.back().black.push_back(pair<uint16_t, uint16_t>(idx, count));
				score += rating;
			}
			else if (find(selected.begin(), selected.end(), idx) == selected.end())
				recipe_scores.back().green.push_back(pair<uint16_t, uint16_t>(idx, count));

			count++;
			fread(&idx, 2, 1, fcon);
		}
		fread(&(recipe_scores.back().offset), 4, 1, fcon);

		for (size_t i = 0; i < recipe_scores.back().green.size() && score > 0.0f; i++)
		{
			if (ingredient_scores.size() <= recipe_scores.back().green[i].first)
				ingredient_scores.resize(recipe_scores.back().green[i].first+1, vector<float>());
			if (ingredient_scores[recipe_scores.back().green[i].first].size() <= recipe_scores.back().black.size())
				ingredient_scores[recipe_scores.back().green[i].first].resize(recipe_scores.back().black.size()+1, 0.0f);

			ingredient_scores[recipe_scores.back().green[i].first][recipe_scores.back().black.size()] += score;
		}

		recipe_scores.back().score = ((float)recipe_scores.back().black.size()/(0.01*(float)recipe_scores.back().green.size()));
	}

	for (size_t i = 0; i < ingredient_scores.size(); i++)
	{
		if (ingredient_max_scores.size() < ingredient_scores[i].size())
			ingredient_max_scores.resize(ingredient_scores[i].size(), 0.0);

		for (size_t j = 0; j < ingredient_scores[i].size(); j++)
			if (ingredient_scores[i][j] > ingredient_max_scores[j])
				ingredient_max_scores[j] = ingredient_scores[i][j];
	}

	recipe_scores.sort();
}

string parse_amount(string amt)
{
	replace(amt.begin(), amt.end(), '\"', ' ');
	replace(amt.begin(), amt.end(), '\'', ' ');
	replace(amt.begin(), amt.end(), '_', ',');
	replace(amt.begin(), amt.end(), '-', ' ');
	replace(amt.begin(), amt.end(), '*', ' ');
	return amt;
}

string parse_ing(string ing)
{
	size_t sch, sch2;
	transform(ing.begin(), ing.end(), ing.begin(), ::tolower);

	replace(ing.begin(), ing.end(), '_', ',');
	replace(ing.begin(), ing.end(), '-', ' ');
	replace(ing.begin(), ing.end(), '*', ' ');

	sch = ing.find("(such as");
	sch2 = ing.find_first_of(")", sch);
	if (sch != ing.npos && sch2 != ing.npos)
		ing = ing.substr(0, sch) + ing.substr(sch2+1);

	sch = ing.find("(e.g.");
	sch2 = ing.find_first_of(")", sch);
	if (sch != ing.npos && sch2 != ing.npos)
		ing = ing.substr(0, sch) + ing.substr(sch2+1);

	replace(ing.begin(), ing.end(), '(', ' ');
	replace(ing.begin(), ing.end(), ')', ' ');

	while ((sch = ing.find("  ")) != ing.npos)
		ing = ing.substr(0, sch) + " " + ing.substr(sch+2);

	while ((sch = ing.find(" ,")) != ing.npos)
		ing = ing.substr(0, sch) + "," + ing.substr(sch+2);

	if (ing[0] == 'o' && ing[1] == 'f' && ing[2] == ' ')
		ing = ing.substr(3);

	ing = ltrim(rtrim(ing));

	if ((sch = ing.find(", or more")) != ing.npos || (sch = ing.find(" or more")) != ing.npos ||
		(sch = ing.find(", plus ")) != ing.npos || (sch = ing.find(" plus ")) != ing.npos ||
		(sch = ing.find(", or to more")) != ing.npos || (sch = ing.find(" or to more")) != ing.npos ||
		(sch = ing.find(", or to taste")) != ing.npos || (sch = ing.find(" or to taste")) != ing.npos ||
		(sch = ing.find(", or as desired")) != ing.npos || (sch = ing.find(" or as desired")) != ing.npos ||
		(sch = ing.find(", or as needed")) != ing.npos || (sch = ing.find(" or as needed")) != ing.npos ||
		(sch = ing.find(", more")) != ing.npos)
			ing = ing.substr(0, sch);

	// softened creamed trimmed crumbled melted grated cooled cubed julienned split toasted mashed squeezed cooked rinsed dried boiled skined deboned seeded halved chilled thawed divided cleaned sectioned peeled cored cut drained pitted diced chopped sliced
	if ((sch = ing.find(", and cut in")) != ing.npos || (sch = ing.find(" and cut in")) != ing.npos ||
		(sch = ing.find(", cut in")) != ing.npos || (sch = ing.find(" cut in")) != ing.npos)
			ing = ing.substr(0, sch);

	/*if (ing.find("salmon") != ing.npos)
	{
			if (ing.find("fillet") != ing.npos && ing.find("steak") != ing.npos)
					return "salmon";
			else if (ing.find("fillet") != ing.npos)
					return "salmon fillet";
			else if (ing.find("steak") != ing.npos)
					return "salmon steak";

			else if (ing.find("smoked") != ing.npos)
			{
	//              cout << ing << endl;
			}
	}*/

	//if ((sch = ing.find(", peeled ")) != ing.npos || (sch = ing.find(" peeled ")) != ing.npos)
	//      ing = ing.substr(0, sch);

	//cout << ing << endl;


	if (ing.find("chicken") != ing.npos && ing.find("turkey") == ing.npos && ing.find("beef") == ing.npos &&
		ing.find("vegetable") == ing.npos && ing.find("ham") == ing.npos && ing.find("lam") == ing.npos &&
		ing.find("pork") == ing.npos)
	{
			if (ing.find("deli") != ing.npos)
					return "chicken cold cuts";
			else if (ing.find("ground") != ing.npos)
					return "ground chicken";
			else if (((ing.find("breast") != ing.npos) + (ing.find("thigh") != ing.npos) +
					  (ing.find("drumstick") != ing.npos) + (ing.find("wing") != ing.npos) +
					  (ing.find("leg") != ing.npos)) > 1)
					return "chicken";
			else if (ing.find("breast") != ing.npos)
					return "chicken breast";
			else if (ing.find("thigh") != ing.npos)
					return "chicken thighs";
			else if (ing.find("drumstick") != ing.npos || ing.find("leg") != ing.npos)
					return "chicken legs";
			else if (ing.find("wing") != ing.npos)
					return "chicken wings";
			else if (ing.find("broth") != ing.npos || ing.find("stock") != ing.npos)
			{}      //cout << ing << endl;
	}

	return ing;
}

int parse_time(string t)
{
	return 0;
}
