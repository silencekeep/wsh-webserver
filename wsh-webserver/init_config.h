#pragma once
#include <unordered_map>
#include <string>
#include <vector>

#define TCP_PROC_MAX_BUFFERSIZE 0x400000

using namespace std;

struct init_config {
	string ip_addr;
	int port;
	int threadpool_capacity;
	int keep_alive_timeout;
	int max_keep_alive_req;
	string charset;
	string wwwroot_dir;
	string default_document;
	string log_path;
	string server_name;
	string host_name;
	unordered_map<string, string> mime_map;
	unordered_map<string, string> cgi_map;
	unordered_map<string, string> fastcgi_map;
	init_config() {
		ip_addr = "0.0.0.0";
		port = 12345;
		keep_alive_timeout = 60000;
		max_keep_alive_req = 10;
		threadpool_capacity = 5;
		charset = "utf-8";
		wwwroot_dir = "wwwroot";
		default_document = "welcome.html";
		log_path = "logs";
		server_name = "programming-practice task ustb_scce_wsh";
		host_name = "127.0.0.1";
		mime_map.insert(std::make_pair(".html","text/html;charset=utf-8"));
		mime_map.insert(std::make_pair(".htm", "text/html;charset=utf-8"));
		mime_map.insert(std::make_pair(".jpg", "image/jpeg"));
		mime_map.insert(std::make_pair(".ico", "x-icon"));
		mime_map.insert(std::make_pair(".png", "image/png"));
		mime_map.insert(std::make_pair(".js", "application/javascript"));
		mime_map.insert(std::make_pair(".css", "text/css"));
		mime_map.insert(std::make_pair(".mp4", "video/mp4"));
		cgi_map.insert(std::make_pair(".cgi", "cgi-bin/simp.cgi"));
		fastcgi_map.insert(std::make_pair(".php", "php7/php-cgi.exe"));
	}
};
