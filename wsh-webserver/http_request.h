#pragma once
#include <string>
#include <map>
#include <vector>

using namespace std;

struct http_request {
	string method;
	string path;
	string http_proto;
	map<string, string> headers;
	string body;
};