#include "http_handler.h"

#pragma warning(disable:4996)

void handle_mime(wsh_server* server, const http_request& request, http_response& response)
{
    std::chrono::system_clock::time_point tp;
    std::string path = server->getInitConfig().wwwroot_dir + q_split_string(urlDecode(request.path));
    //std::string extension = getExtensionFromPath(path);
    response.http_proto = "HTTP/1.1";
    try {
        if (request.method != "GET" && request.method != "POST" && request.method != "HEAD") {
            auto sio = response.headers.find(H_RESP_SERVER);
            server->getLogger().log(LogLevel::WARN, u8"检测到无效的HTTP谓词.");
            get_error_page(response, 405, "HTTP \"" + request.method + "\"",
                sio != response.headers.end() ? sio->second : "wsh-webserver");
            return;
        }
        long long bytes_count = getFileBytesCount(path, tp);
        std::string etag = calculateFileETag(path, bytes_count, tp);
        long long start = -1, end = -1;
        auto range = request.headers.find(H_REQ_RANGE);
        auto h_etag = request.headers.find(H_REQ_IF_RANGE);
        auto h_none_match = request.headers.find(H_REQ_IF_NONE_MATCH);
        if (h_none_match != request.headers.end() && h_none_match->second == etag) {
            response.headers.insert(std::make_pair(H_RESP_CONTENT_LENGTH, std::to_string(bytes_count)));
            response.headers.insert(std::make_pair(H_RESP_ACCEPT_RANGES, "bytes"));
            response.headers.insert(std::make_pair(H_RESP_ETAG, etag));
            //response.headers.insert(std::make_pair(H_RESP_CONTENT_DISPOSITION, "attachment; filename=1.png"));
            response.status_code = 304;
            response.phrase = HTTP_304;
            return;
        }
        //部分下载
        else if (range != request.headers.end()) {
            parse_range(range->second, start, end);
            //cout << start << "\t" << end << endl;
            if (start == 0 || (h_etag != request.headers.end() && h_etag->second != etag)) {
                if (end == -1) end = bytes_count - 1;
                readPartialFile(path, start, end, response.body, tp);
                response.headers.insert(std::make_pair(H_RESP_CONTENT_LENGTH, std::to_string(bytes_count)));
                response.headers.insert(std::make_pair(H_RESP_CONTENT_RANGE,
                    make_content_range(start, 0, bytes_count)));
                response.headers.insert(std::make_pair(H_RESP_ACCEPT_RANGES, "bytes"));
                response.headers.insert(std::make_pair(H_RESP_ETAG, etag));
                //response.headers.insert(std::make_pair(H_RESP_CONTENT_DISPOSITION, "attachment; filename=1.png"));
                response.status_code = 200;
                response.phrase = HTTP_200;
            }
            else {
                if (start == -1 || end == -1) {
                    if (start == -1) start = 0;
                    if (end == -1) end = bytes_count - 1;
                }
                readPartialFile(path, start, end, response.body, tp);
                response.headers.insert(std::make_pair(H_RESP_CONTENT_LENGTH, std::to_string(bytes_count)));
                response.headers.insert(std::make_pair(H_RESP_CONTENT_RANGE,
                    make_content_range(start, end, bytes_count)));
                response.headers.insert(std::make_pair(H_RESP_ACCEPT_RANGES, "bytes"));
                response.headers.insert(std::make_pair(H_RESP_ETAG, etag));
                //response.headers.insert(std::make_pair(H_RESP_CONTENT_DISPOSITION, "attachment; filename=1.png"));
                response.status_code = 206;
                response.phrase = HTTP_206;
            }
        }
        else {
            try {
                readFileAndTimestamp(path, response.body, tp);
                response.headers.insert(std::make_pair(H_RESP_CONTENT_LENGTH, std::to_string(bytes_count)));
                response.headers.insert(std::make_pair(H_RESP_ACCEPT_RANGES, "bytes"));
                response.headers.insert(std::make_pair(H_RESP_ETAG, etag));
                response.status_code = 200;
                response.phrase = HTTP_200;
            }
            catch (runtime_error err) {
                auto sio = response.headers.find(H_RESP_SERVER);
                if (err.what() == "File Not Found.") {
                    get_error_page(response, 404, u8"你要的文件我这里没有.",
                        sio != response.headers.end() ? sio->second : "wsh-webserver");
                }
                else {
                    get_error_page(response, 500, u8"内部服务器错误.",
                        sio != response.headers.end() ? sio->second : "wsh-webserver");
                }
            }
        }
    }
    catch (runtime_error err) {
        get_error_page(response, 404, u8"你要的文件我这里没有.",
            server->getInitConfig().server_name);
    }
    if (request.method == "HEAD") {
        response.body = "";
    }
}

void handle_cgi(wsh_server* server, std::string cgi_handler_path, const http_request& request, http_response& response)
{
    auto us = q_split_string(server->getInitConfig().wwwroot_dir + urlDecode(request.path));
    Xreplace(us, "/", "\\");
    //std::string extension = getExtensionFromPath();
    std::wstring cgi_script_path = string_to_wstring(us);
    std::wstring query_string = string_to_wstring(q_split_string(urlDecode(request.path), true));
    // 构建 CGI 命令行参数
    std::string allref = wstring_to_string(server->directory + L"\\" + cgi_script_path);
    Xreplace(allref, "/", "\\");
    std::wstring commandLine = string_to_wstring(allref);// +L" ";
    std::chrono::system_clock::time_point tp;
    if (request.method != "GET" && request.method != "POST" && request.method != "HEAD") {
        auto sio = response.headers.find(H_RESP_SERVER);
        get_error_page(response, 405, "HTTP \"" + request.method + "\"",
            sio != response.headers.end() ? sio->second : "wsh-webserver");
        return;
    }
    try {
        long long bytes_count = getFileBytesCount(wstring_to_string(commandLine), tp);
    }
    catch (runtime_error err) {
        get_error_page(response, 404, u8"你要的文件我这里没有.",
            server->getInitConfig().server_name);
        return;
    }

    // 创建匿名管道
    SECURITY_ATTRIBUTES saAttr;
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = NULL;

    HANDLE hReadPipe, hWritePipe;
    if (!CreatePipe(&hReadPipe, &hWritePipe, &saAttr, 0)) {
        //std::cerr << "Error creating pipe" << std::endl;
        return;
    }

    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.hStdInput = NULL;
    si.hStdOutput = hWritePipe;
    si.hStdError = NULL;
    si.dwFlags |= STARTF_USESTDHANDLES;

    ZeroMemory(&pi, sizeof(pi));
    SetEnvironmentVariableW(H_CGI_REDIRECT_STATUS, L"200");
    auto mt = string_to_wstring(request.method);
    SetEnvironmentVariableW(H_CGI_REQUEST_METHOD, mt.c_str());
    auto wsd = string_to_wstring(q_split_string(request.path));
    SetEnvironmentVariableW(H_CGI_PHP_SELF, wsd.c_str());
    auto v = wstring_to_string(server->directory) + "\\" + us;
    auto vv = string_to_wstring(v);
    SetEnvironmentVariableW(H_CGI_SCRIPT_FILENAME, vv.c_str());
    auto it = vv.find_last_of(L"\\");
    auto xcc = vv.substr(it + 1);
    SetEnvironmentVariableW(H_CGI_SCRIPT_NAME, xcc.c_str());
    if (request.headers.find(H_REQ_COOKIE) != request.headers.end()) {
        auto ws = string_to_wstring(request.headers.find(H_REQ_COOKIE)->second);
        SetEnvironmentVariableW(H_CGI_HTTP_COOKIE, ws.c_str());
    }
    if (request.headers.find(H_REQ_USER_AGENT) != request.headers.end()) {
        auto ws = string_to_wstring(request.headers.find(H_REQ_USER_AGENT)->second);
        SetEnvironmentVariableW(H_CGI_HTTP_USER_AGENT, ws.c_str());
    }
    if (request.headers.find(H_REQ_HOST) != request.headers.end()) {
        auto ws = string_to_wstring(request.headers.find(H_REQ_HOST)->second);
        SetEnvironmentVariableW(H_CGI_HTTP_HOST, ws.c_str());
    }
    if (request.headers.find(H_REQ_CONTENT_LENGTH) != request.headers.end()) {
        auto ws = string_to_wstring(request.headers.find(H_REQ_CONTENT_LENGTH)->second);
        SetEnvironmentVariableW(H_CGI_CONTENT_LENGTH, ws.c_str());
    }
    if (!query_string.empty()) {
        SetEnvironmentVariableW(H_CGI_QUERY_STRING, query_string.c_str());
    }
    auto wss = string_to_wstring(request.method);// wss.c_str()
    //SetEnvironmentVariableW(H_CGI_REQUEST_METHOD, L"");

    if (!CreateProcess(NULL,   // No module name (use command line)
        const_cast<LPWSTR>(commandLine.c_str()), // Command line
        NULL,        // Process handle not inheritable
        NULL,        // Thread handle not inheritable
        TRUE,        // Set handle inheritance to TRUE
        0,           // No creation flags
        NULL,        // Use parent's environment block
        NULL,        // Use parent's starting directory 
        &si,         // Pointer to STARTUPINFO structure
        &pi)) {
        server->getLogger().log(LogLevel::ERR, "创建CGI子进程失败-> GetLastError=" + std::to_string(GetLastError()));
        return;
    }

    CloseHandle(hWritePipe);

    char* buffer = new char[X_MAX_CONTENT_BYTES];
    DWORD bytesRead;
    while (ReadFile(hReadPipe, buffer, X_MAX_CONTENT_BYTES, &bytesRead, NULL) && bytesRead != 0)
        response.body.append(buffer, bytesRead);
    response.http_proto = "HTTP/1.1";
    response.status_code = 200;
    response.phrase = HTTP_200;
    auto parsed = http_cgi_resp_parse(response.body);
    response.body = parsed.second;
    for (auto it = parsed.first.begin(); it != parsed.first.end(); it++)
        response.headers.insert(*it);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    CloseHandle(hReadPipe);
    delete[]buffer;
    return;
}

void handle_fastcgi(wsh_server* server, std::string fastcgi_handler_path, const http_request& request, http_response& response)
{
    auto us = q_split_string(server->getInitConfig().wwwroot_dir + urlDecode(request.path));
    Xreplace(us, "/", "\\");
    //std::string extension = getExtensionFromPath();
    std::wstring cgi_script_path = string_to_wstring(us);
    std::wstring query_string = string_to_wstring(q_split_string(request.path, true));
    // 构建 CGI 命令行参数
    std::string allref = wstring_to_string(server->directory) + "\\" + fastcgi_handler_path
        + " " + wstring_to_string(server->directory) + "\\" + us;
    Xreplace(allref, "/", "\\");
    std::wstring commandLine = string_to_wstring(allref);// +L" ";
    std::chrono::system_clock::time_point tp;
    if (request.method != "GET" && request.method != "POST" && request.method != "HEAD") {
        auto sio = response.headers.find(H_RESP_SERVER);
        get_error_page(response, 405, u8"检测到无效的HTTP谓词 \"" + request.method + "\"",
            sio != response.headers.end() ? sio->second : "wsh-webserver");
        return;
    }
    try {
        long long bytes_count = getFileBytesCount(wstring_to_string(server->directory) + "\\" + us, tp);
    }
    catch (runtime_error err) {
        get_error_page(response, 404, u8"你要的文件我这里没有.",
            server->getInitConfig().server_name);
        return;
    }

    // 创建匿名管道
    SECURITY_ATTRIBUTES saAttr;
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = NULL;

    HANDLE hReadPipe, hWritePipe;
    if (!CreatePipe(&hReadPipe, &hWritePipe, &saAttr, 0)) {
        server->getLogger().log(LogLevel::ERR, "创建pipeline失败-> GetLastError=" + std::to_string(GetLastError()));
        //std::cerr << "Error creating pipe" << std::endl;
        return;
    }

    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    /*si.hStdInput = hReadPipe;
    si.hStdOutput = hWritePipe;*/
    si.hStdInput = hReadPipe;
    si.hStdOutput = hWritePipe;
    //if raised php stderr info, it will be ignored.
    si.hStdError = NULL;
    si.dwFlags |= STARTF_USESTDHANDLES;

    ZeroMemory(&pi, sizeof(pi));
    SetEnvironmentVariableW(H_CGI_REDIRECT_STATUS, L"200");
    auto mt = string_to_wstring(request.method);
    SetEnvironmentVariableW(H_CGI_REQUEST_METHOD, mt.c_str());
    if (request.headers.find(H_REQ_COOKIE) != request.headers.end()) {
        auto ws = string_to_wstring(request.headers.find(H_REQ_COOKIE)->second);
        SetEnvironmentVariableW(H_CGI_HTTP_COOKIE, ws.c_str());
    }
    auto wsd = string_to_wstring(q_split_string(request.path));
    SetEnvironmentVariableW(H_CGI_PHP_SELF, wsd.c_str());
    auto v = wstring_to_string(server->directory) + "\\" + us;
    auto vv = string_to_wstring(v);
    SetEnvironmentVariableW(H_CGI_SCRIPT_FILENAME, vv.c_str());
    auto it = vv.find_last_of(L"\\");
    auto xcc = vv.substr(it + 1);
    SetEnvironmentVariableW(H_CGI_SCRIPT_NAME, xcc.c_str());
    if (request.headers.find(H_REQ_USER_AGENT) != request.headers.end()) {
        auto ws = string_to_wstring(request.headers.find(H_REQ_USER_AGENT)->second);
        SetEnvironmentVariableW(H_CGI_HTTP_USER_AGENT, ws.c_str());
    }
    if (request.headers.find(H_REQ_HOST) != request.headers.end()) {
        auto ws = string_to_wstring(request.headers.find(H_REQ_HOST)->second);
        SetEnvironmentVariableW(H_CGI_HTTP_HOST, ws.c_str());
    }
    if (!query_string.empty()) {
        SetEnvironmentVariableW(H_CGI_QUERY_STRING, query_string.c_str());
    }
    auto wss = string_to_wstring(request.method);// wss.c_str()
    //SetEnvironmentVariableW(H_CGI_REQUEST_METHOD, L"");
    std::string req = request.body;
    trim(req);
    if (request.headers.find(H_REQ_CONTENT_TYPE) != request.headers.end()) {
        auto ws = string_to_wstring(request.headers.find(H_REQ_CONTENT_TYPE)->second);
        SetEnvironmentVariableW(H_CGI_CONTENT_TYPE, ws.c_str());
    }
    if (request.headers.find(H_REQ_CONTENT_LENGTH) != request.headers.end()) {
        auto ws = string_to_wstring(request.headers.find(H_REQ_CONTENT_LENGTH)->second);
        SetEnvironmentVariableW(H_CGI_CONTENT_LENGTH, ws.c_str());
    }
    else {
        std::wstring defcont = std::to_wstring(req.size());
        SetEnvironmentVariableW(H_CGI_CONTENT_LENGTH, defcont.c_str());
    }
    
    if (!CreateProcess(NULL,   // No module name (use command line)
        const_cast<LPWSTR>(commandLine.c_str()), // Command line
        NULL,        // Process handle not inheritable
        NULL,        // Thread handle not inheritable
        TRUE,        // Set handle inheritance to TRUE
        0,           // No creation flags
        NULL,        // Use parent's environment block
        NULL,        // Use parent's starting directory 
        &si,         // Pointer to STARTUPINFO structure
        &pi)) {
        server->getLogger().log(LogLevel::ERR, "创建CGI子进程失败-> GetLastError=" + std::to_string(GetLastError()));
        //std::cerr << "CreateProcess failed (" <<  << ")" << std::endl;
        return;
    }
    DWORD bytesWritten;
    if (!req.empty() && !WriteFile(hWritePipe, req.c_str(), req.size(), &bytesWritten, NULL)) {
        server->getLogger().log(LogLevel::ERR, "无法与CGI程序进行通信-> GetLastError=" + std::to_string(GetLastError()));
        return;
    }
    CloseHandle(hWritePipe);
    char *buffer = new char[X_MAX_CONTENT_BYTES];
    DWORD bytesRead;
    while (ReadFile(hReadPipe, buffer, X_MAX_CONTENT_BYTES, &bytesRead, NULL) && bytesRead != 0)
        response.body.append(buffer, bytesRead);
    response.http_proto = "HTTP/1.1";
    response.status_code = 200;
    response.phrase = HTTP_200;
    auto parsed = http_cgi_resp_parse(response.body);
    response.body = parsed.second;
    for (auto it = parsed.first.begin(); it != parsed.first.end(); it++) {
        if ((*it).first == "Status") {
            trim((*it).second);
            size_t splitIndex = (*it).second.find(" ");
            if (splitIndex != std::string::npos) {
                std::string statusCode = (*it).second.substr(0, splitIndex);
                std::string statusMessage = v.substr(splitIndex + 1);
                response.status_code = std::stoi(statusCode);
                response.phrase = statusMessage;
                get_error_page(response, std::stoi(statusCode), u8"来自FastCgi程序的错误 \"" + request.method + "\"",
                    server->getInitConfig().server_name);
            }
        }
        else response.headers.insert(*it);
    }
    CloseHandle(hReadPipe);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    delete []buffer;
    return;
}

void get_error_page(http_response& response, int status_code, const std::string err_message, const std::string server_name)
{
    const char* htmlTemplate = R"(<!DOCTYPE html>
        <html lang="en">
        <head>
            <meta charset="UTF-8">
            <meta name="viewport" content="width=device-width, initial-scale=1.0">
            <title>Error Page</title>
            <style>
                body {
                    font-family: Arial, sans-serif;
                    margin: 0;
                    padding: 50px;
                    background-color: #f8f9fa;
                    text-align: center;
                }
                .container {
                    max-width: 600px;
                    margin: 0 auto;
                    background-color: #fff;
                    border-radius: 8px;
                    box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
                    padding: 20px;
                }
                h1 {
                    color: #dc3545;
                }
                p {
                    color: #6c757d;
                }
                .server-info {
                    position: absolute;
                    bottom: 10px;
                    right: 10px;
                    font-size: 12px;
                    color: #6c757d;
                }
            </style>
        </head>
        <body>
            <div class="container">
                <h1>%d - %s</h1>
                <p>%s</p>
                <div class="server-info">Server: %s</div>
            </div>
        </body>
        </html>)";

    char buffer[8192];
    auto find = HTTP_STATUS_CODE.find(status_code);
    std::string phrase = find != HTTP_STATUS_CODE.end() ? find->second : HTTP_STATUS_CODE.find(0)->second;
    int ret = std::snprintf(buffer, sizeof(buffer), htmlTemplate, status_code, phrase.c_str(), err_message.c_str(), server_name.c_str());
    if (ret >= 0 && ret < sizeof(buffer)) {
        response.http_proto = "HTTP/1.1";
        response.phrase = phrase;
        response.status_code = status_code;
        response.body = std::string(buffer);
        response.headers.insert(std::make_pair(H_RESP_CONTENT_TYPE, "text/html; charset=utf-8"));
        response.headers.insert(std::make_pair(H_RESP_CONTENT_LENGTH, std::to_string(response.body.length())));
        response.headers.insert(std::make_pair(H_RESP_SERVER, server_name));
    }
}
