#pragma once
#include "http_parse.h"

std::pair<std::map<std::string, std::string>, std::string> http_cgi_resp_parse(const std::string& response) {
    std::map<std::string, std::string> headers;
    std::string body;
    std::istringstream iss(response);
    std::string line;
    while (std::getline(iss, line)) {
        trim(line);
        if (line.empty()) {
            // 遇到空行，表示头部解析完毕，剩下的是响应体
            break;
        }
        else {
            // 解析头部信息
            size_t pos = line.find(": ");
            if (pos != std::string::npos) {
                std::string key = line.substr(0, pos);
                std::string value = line.substr(pos + 1);
                trim(key);
                trim(value);
                headers[key] = value;
            }
        }
    }

    // 读取响应体
    body = iss.str().substr(iss.tellg());

    return { headers, body };
}

void http_req_parse(std::string request, http_request& parsed) {
    std::istringstream request_stream(request);

    std::getline(request_stream, parsed.method, ' ');
    std::getline(request_stream, parsed.path, ' ');
    std::getline(request_stream, parsed.http_proto, '\n');

    std::string header_line;
    while (std::getline(request_stream, header_line, '\r') && header_line != "\n") {
        std::istringstream header_stream(header_line);
        std::string header_name;
        std::getline(header_stream, header_name, ':');
        std::string header_value;
        std::getline(header_stream, header_value);
        trim(header_name);
        trim(header_value);
        toLower(header_name);
        parsed.headers.insert(std::make_pair(header_name, header_value));
    }
    auto content_length_iter = parsed.headers.find(H_REQ_CONTENT_LENGTH);
    if (content_length_iter != parsed.headers.end()) {
        int content_length = std::stoi(content_length_iter->second) + 1;
        parsed.body.resize(content_length);
        request_stream.read(&parsed.body[0], content_length);
        trim(parsed.body);
    }
}

std::string make_http_response(const http_response& response) {
    std::stringstream response_stream;
    response_stream << response.http_proto << " " << response.status_code << " " << response.phrase << "\r\n";
    for (const auto& pair : response.headers)
        response_stream << pair.first << ": " << pair.second << "\r\n";
    response_stream << "\r\n";
    response_stream << response.body;
    response_stream << "\r\n";
    return response_stream.str();
}

void parse_range(std::string header, long long& start, long long& end)
{
    if (header.find("bytes=") == 0) {
        std::stringstream ss(header.substr(6));
        char delimiter;
        ss >> start >> delimiter;
        if (delimiter == '-') ss >> end;
        else end = -1;
    }
    else start = end = -1;
}

std::string make_content_range(long long start, long long end, long long size)
{
    return "bytes " + std::to_string(start) + "-" +
        std::to_string(end) + "/" + std::to_string(size);
}
