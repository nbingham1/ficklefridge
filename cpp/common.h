/*
 * common.h
 *
 *  Created on: Nov 23, 2013
 *      Author: nbingham
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <string>
#include <iostream>
#include <math.h>
#include <vector>
#include <map>
#include <list>
#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>
#include <time.h>
#include <streambuf>
#include <sstream>

using namespace std;

#ifndef common_h
#define common_h

string &ltrim(string &s);
string &rtrim(std::string &s);

float factorial(float n, float s);
float choose(int n, int k);

#endif
