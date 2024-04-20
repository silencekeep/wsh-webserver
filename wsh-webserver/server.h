#pragma once
#include "boost/asio.hpp"
#include "init_config.h"
#include "threadpool.h"
#include "http_parse.h"
#include "timer.h"
#include "http_proto.h"
#include "http_handler.h"
#include "logger.h"

using namespace std;
using namespace boost::asio;
using namespace boost::asio::ip;

static const init_config default_config;

class wsh_server;

struct listener_wrapper {
	wsh_server* derived;
	std::shared_ptr<tcp::socket> socket_ptr;
};

class wsh_server {
public:
	std::wstring directory;
	wsh_server(init_config conf);
	wsh_server() : wsh_server(default_config) {}
	void run();
	init_config getInitConfig() const{
		return config;
	}
	Logger &getLogger(){
		return my_logger;
	}
private:
	Logger my_logger;
	threadpool thread_pool;
	init_config config;
	io_context ioc;
	tcp::acceptor acceptor;
	friend void handle_listener(void* parent);
};
