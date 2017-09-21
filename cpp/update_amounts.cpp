#include "recipe.h"

void update_amts(vector<uint16_t> group, uint32_t offset)
{
    database data;
    data.load();
    list<query_recipe> recipe_scores;

    recipe r = data.get_recipe(offset);
    query_recipe qr = data.get_query_recipe(group, r.offset);

    for (size_t j = 0; j < qr.green.size(); j++)
        printf("<li class=\"ingredient\"><textarea class=\"dyn-textarea amount\" id=\"%d\" name=\"%d\">%s</textarea><div class=\"green-name\" onclick=\"recipe.add(%d)\">%s</div></li>", qr.green[j].first, qr.green[j].first, data.get_string(data.famt, r.amount[qr.green[j].second]).c_str(), qr.green[j].first, data.get_string(data.fing, qr.green[j].first).c_str());

    for (size_t j = 0; j < qr.black.size(); j++)
        printf("<li class=\"ingredient\"><textarea class=\"dyn-textarea amount\" id=\"%d\" name=\"%d\">%s</textarea><div class=\"name\" onclick=\"recipe.remove(%d)\">%s</div></li>", qr.black[j].first, qr.black[j].first, data.get_string(data.famt, r.amount[qr.black[j].second]).c_str(), qr.black[j].first, data.get_string(data.fing, qr.black[j].first).c_str());

    for (size_t j = 0; j < qr.red.size(); j++)
        printf("<li class=\"ingredient\"><textarea class=\"dyn-textarea amount\" id=\"%d\" name=\"%d\">some</textarea><div class=\"red-name\" onclick=\"recipe.remove(%d)\">%s</div></li>", qr.red[j], qr.red[j], qr.red[j], data.get_string(data.fing, qr.red[j]).c_str());

    printf("<li><textarea class=\"dyn-textarea\" id=\"add-ingredient\" autocomplete=\"off\">add ingredient</textarea><ul id=\"suggestions\"></ul></li>");
}

int main(int argc, char **argv)
{
    vector<uint16_t> group;
    if (argc < 2)
        return 0;

    uint32_t offset = atoi(argv[1]);

    for (int i = 2; i < argc; i++)
        group.push_back(atoi(argv[i]));

    update_amts(group, offset);
}
