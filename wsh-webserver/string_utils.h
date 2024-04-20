#pragma once
#include <algorithm>
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <locale>
#include <codecvt>

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/locale.hpp>

namespace beast = boost::beast;

void toUpper(std::string& str);

void toLower(std::string& str);

std::string getExtensionFromPath(const std::string& path);

void Xreplace(std::string& str, std::string from, std::string to);

std::string urlEncode(const std::string& value);

std::string urlDecode(const std::string& value);

std::string q_split_string(const std::string& str, bool right_side = false);

std::wstring string_to_wstring(const std::string& str);

std::string wstring_to_string(const std::wstring& wstr);

void trim(std::string& str);