#pragma once

#include <iostream>
#include <boost/asio.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/algorithm/string.hpp>

#include "string_utils.h"
#include "http_request.h"
#include "http_response.h"
#include "http_proto.h"

#include <sstream>

#pragma warning(disable:4996)

using namespace boost::asio;

void http_req_parse(std::string request, http_request& parsed);

std::pair<std::map<std::string, std::string>, std::string> http_cgi_resp_parse(const std::string& response);

std::string make_http_response(const http_response& response);

void parse_range(std::string header, long long& start, long long& end);

std::string make_content_range(long long start,long long end,long long size);
