#pragma once
#include "http_parse.h"
#include "http_proto.h"
//#include "fcgi_stdio.h"
#include "file_utils.h"
#include "string_utils.h"
#include "server.h"
#include <Windows.h>

extern class wsh_server;

constexpr auto X_MAX_CONTENT_BYTES = 2 * 1024 * 1024;

void handle_mime( wsh_server* server, const http_request& request, http_response& response);

void handle_cgi( wsh_server* server, std::string cgi_handler_path, const http_request& request, http_response& response);

void handle_fastcgi( wsh_server* server, std::string fastcgi_handler_path, const http_request& request, http_response& response);

void get_error_page(http_response &response, int status_code, std::string err_message, std::string server_name);
