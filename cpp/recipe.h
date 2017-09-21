/*
 * recipe.h
 *
 *  Created on: Nov 23, 2013
 *      Author: nbingham
 */

#include "common.h"

#ifndef recipe_h
#define recipe_h

struct recipe
{
	recipe();
	~recipe();

	uint32_t offset;
	uint32_t title;
	float rating;
	int rate_count;
	int servings;
	int prep_time;
	int cook_time;

	vector<uint16_t> ingr;
	vector<uint32_t> amount;
	vector<uint32_t> instr;

	recipe &operator=(recipe r);

	void clear();
};

struct query_recipe
{
	query_recipe();
	query_recipe(const query_recipe &r);
	~query_recipe();

	float score;
	uint32_t offset;
	vector<pair<uint16_t, uint16_t> > green;
	vector<pair<uint16_t, uint16_t> > black;
	vector<uint16_t> red;

	query_recipe &operator=(query_recipe r);
};

bool operator<(query_recipe r1, query_recipe r2);
bool operator>(query_recipe r1, query_recipe r2);
bool operator==(query_recipe r1, query_recipe r2);

struct database
{
	database();
	~database();

	FILE *finstr;
	FILE *ftitle;
	FILE *fing;
	FILE *famt;
	FILE *fcon;
	FILE *frec;

	void load();
	void initialize_strings(string filename);
	void initialize_amounts(string filename);
	void initialize_recipes(string filename);
	void initialize_ingredients(string filename);
	string get_string(FILE *fin, int index);
	int get_index(FILE *fin, string str);
	recipe get_recipe(uint32_t offset);
	query_recipe get_query_recipe(vector<uint16_t> selected, uint32_t offset);

	void suggest(string search);
	void query(vector<uint16_t> selected, list<query_recipe> &recipe_scores);
	void query(vector<uint16_t> selected, vector<vector<float> > &ingredient_scores, vector<float> &ingredient_max_scores);
	void query(vector<uint16_t> selected, vector<vector<float> > &ingredient_scores, vector<float> &ingredient_max_scores, list<query_recipe> &recipe_scores);
};

string parse_amount(string amt);
string parse_ing(string ing);
int parse_time(string t);

#endif
