/*
 * common.cpp
 *
 *  Created on: Nov 23, 2013
 *      Author: nbingham
 */

#include "common.h"

// trim from start
string &ltrim(string &s) {
	s.erase(s.begin(), find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace))));
	return s;
}

// trim from end
string &rtrim(string &s) {
	s.erase(find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(), s.end());
	return s;
}

float factorial(float n, float s)
{
	return (n <= s) ? 1.0f : (factorial(n - 1.0f, s) * n);
}

float choose(int n, int k)
{
	return factorial(n, n-k)/factorial(k, 1.0f);
}
