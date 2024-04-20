#pragma once
#include <string>
#include <map>
#include <vector>

using namespace std;

struct http_response {
	string phrase;
	int status_code;
	string http_proto;
	map<string, string> headers;
	string body;
};