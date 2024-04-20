#include "server.h"

wsh_server::wsh_server(init_config conf) : config(conf), acceptor(ioc, 
	ip::tcp::endpoint(address::from_string(conf.ip_addr).to_v4(), conf.port)), 
	thread_pool(conf.threadpool_capacity){
	wchar_t buffer[260];
	GetModuleFileName(NULL, buffer, MAX_PATH);
	std::wstring fullPath(buffer);
	size_t pos = fullPath.find_last_of(L"\\/");
	directory = fullPath.substr(0, pos);
}
static bool timer(int millsecond) {
	auto start = std::chrono::steady_clock::now();
	std::this_thread::sleep_for(std::chrono::milliseconds(millsecond));
	auto end = std::chrono::steady_clock::now();
	auto diff = end - start;
	if (std::chrono::duration_cast<std::chrono::milliseconds>(diff).count() >= millsecond)
		return false;
	else
		return true;
}

static void handle_listener(void* parents)
{
	listener_wrapper* source = static_cast<listener_wrapper*>(parents);
	wsh_server* parent = source->derived;
	tcp::socket &socks = *(source->socket_ptr);
	int completed_session_counter = 0;
	char *buffer = new char[TCP_PROC_MAX_BUFFERSIZE];
	ka_timer ktimer(parent->config.keep_alive_timeout);
	
	while (true) {
		if (ktimer.expired() || completed_session_counter >= parent->config.max_keep_alive_req) {
			socks.close();
			parent->getLogger().log(LogLevel::WARN, "TCP ���ʱ�䳬ʱ�򳬹�������������Ͽ�Keep-Alive����");
			break;
		}
		boost::system::error_code ec;
		size_t bytes_received = socks.read_some(
			boost::asio::buffer(buffer, TCP_PROC_MAX_BUFFERSIZE), ec);
		if (ec == boost::asio::error::eof) {
			socks.close();
			break;
		}
		else if (ec) {
			// ��������
			parent->getLogger().log(LogLevel::ERR, "Socket�����쳣: " + ec.message());
			//std::cerr << "Error receiving message: " << ec.message() << std::endl;
			break;
		}
		std::string message(buffer, bytes_received);
		http_request request;
		http_response response;
		http_req_parse(message,request);
		request.body = urlDecode(request.body);
		//����Http����
		auto connection_value = request.headers.find(H_REQ_CONNECTION);
		if (connection_value != request.headers.end()) {
			toUpper(connection_value->second);
			if (connection_value->second == "CLOSE") ktimer.terminate();
		}
		std::string extension = getExtensionFromPath(q_split_string(request.path));
		
		auto fastcgi_kvp = parent->config.fastcgi_map.find(extension);
		auto cgi_kvp = parent->config.cgi_map.find(extension);
		auto mime_kvp = parent->config.mime_map.find(extension);
		//is FastCGI
		if (fastcgi_kvp != parent->config.fastcgi_map.end()) {
			handle_fastcgi(parent, fastcgi_kvp->second, request, response);
			response.headers.insert(std::make_pair(H_RESP_CONTENT_LENGTH, 
				std::to_string(response.body.size())));
		}
		//is CGI
		else if (cgi_kvp != parent->config.cgi_map.end()) {
			handle_cgi(parent, cgi_kvp->second, request, response);
			response.headers.insert(std::make_pair(H_RESP_CONTENT_LENGTH,
				std::to_string(response.body.size())));
		}
		//is MIME
		else if (mime_kvp != parent->config.mime_map.end()) {
			handle_mime(parent, request, response);
			response.headers.insert(std::make_pair(H_RESP_CONTENT_TYPE,
				parent->config.mime_map.find(extension)->second));
		}
		else {
			if (extension.empty()) {
				if (request.path == "/") {
					request.path = "/" + parent->config.default_document;
					handle_mime(parent, request, response);
				}
				else {
					get_error_page(response, 403, u8"Ϊ�˷�������ȫ����WebServer��֧��Ŀ¼���.", parent->config.server_name);
				}
			}
			else {
				get_error_page(response, 403, u8"��֧�ֵ�MIMEӳ������.", parent->config.server_name);
			}
		}	
		if (response.headers.find(H_RESP_LOCATION) != response.headers.end()) {
			response.status_code = 302;
			response.phrase = HTTP_302;
		}
		response.headers.insert(std::make_pair(H_RESP_SERVER, parent->config.server_name));
		std::string bytes_gen = make_http_response(response);
		std::string msg = "����:" + socks.remote_endpoint().address().to_string()
			+ ":" + std::to_string(socks.remote_endpoint().port())
			+ " " + request.method + " " + response.phrase + " " + request.path;
		parent->getLogger().log(LogLevel::INFO, msg);
		//cout << "ssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssss" << endl;


		socks.write_some(boost::asio::buffer(bytes_gen.c_str(), bytes_gen.length())
		);
		completed_session_counter++;
	}
	delete[] buffer;
	delete source;
	return;
}

void wsh_server::run()
{
	getLogger().log(LogLevel::INFO, "���������������� " + 
		getInitConfig().ip_addr + ":" + std::to_string(getInitConfig().port) + " �Ͻ��м�����");
	while (true) {
		tcp::socket sock(ioc);
		acceptor.accept(sock);
		listener_wrapper *l_wrap = new listener_wrapper;
		l_wrap->derived = this;
		l_wrap->socket_ptr = std::make_shared<tcp::socket>(std::move(sock));
		thread_pool.enqueue(std::bind(handle_listener, std::placeholders::_1), static_cast<void*>(l_wrap));
	}
}


