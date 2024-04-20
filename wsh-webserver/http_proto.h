#pragma once
#include <unordered_map>

/*常用的HTTP请求头：
Host：指定被请求资源的主机名和端口号。
User-Agent：发送请求的用户代理字符串，通常包含浏览器或客户端的相关信息。
Accept-Encoding：指定客户端可接受的内容编码。
Content-Type：指定请求主体的MIME类型。
Content-Length：指定请求主体的长度（字节数）。
Referer：指示请求的来源页面的URL。
Cookie：发送包含HTTP cookies的请求。*/

//仅支持HTTP/1.0标准下的三个METHOD
#define HTTP_GET "GET"
#define HTTP_POST "POST"
#define HTTP_HEAD "HEAD"

#define H_REQ_HOST_L "Host"
#define H_REQ_USER_AGENT_L "User-Agent"
#define H_REQ_CONNECTION_L "Connection"
#define H_REQ_ACCEPT_ENCODING_L "Accept-Encoding"
#define H_REQ_CONTENT_TYPE_L "Content-Type"
#define H_REQ_CONTENT_LENGTH_L "Content-Length"
#define H_REQ_REFERER_L "Referer"
#define H_REQ_COOKIE_L "Cookie"
#define H_REQ_IF_RANGE_L "If-Range"
#define H_REQ_RANGE_L "Range"
#define H_REQ_IF_MODIFIED_SINCE_L "If-Modified-Since"
#define H_REQ_IF_NONE_MATCH_L "If-None-Match"

#define H_REQ_HOST "host"
#define H_REQ_USER_AGENT "user-agent"
#define H_REQ_CONNECTION "connection"
#define H_REQ_ACCEPT_ENCODING "accept-encoding"
#define H_REQ_CONTENT_TYPE "content-type"
#define H_REQ_CONTENT_LENGTH "content-length"
#define H_REQ_REFERER "referer"
#define H_REQ_COOKIE "cookie"
#define H_REQ_IF_RANGE "if-range"
#define H_REQ_RANGE "range"
#define H_REQ_IF_MODIFIED_SINCE "if-modified-since"
#define H_REQ_IF_NONE_MATCH "if-none-match"

/*
常用的HTTP响应头：
Content-Type：指定响应主体的MIME类型。
Content-Length：指定响应主体的长度（字节数）。
Server：指示服务器的软件信息。
Set-Cookie：设置HTTP cookie。
Location：用于重定向响应的目标URL。
Cache-Control：指定缓存策略。
Expires：指定响应的过期时间。
Last-Modified：指示资源的最后修改时间。
*/

#define H_RESP_HOST "HOST"
#define H_RESP_CONTENT_TYPE "Content-Type"
#define H_RESP_CONTENT_LENGTH "Content-Length"
#define H_RESP_CONTENT_RANGE "Content-Range"
#define H_RESP_ACCEPT_RANGES "Accept-Ranges"
#define H_RESP_SERVER "Server"
#define H_RESP_SET_COOKIE "Set-Cookie"
#define H_RESP_LOCATION "Location"
#define H_RESP_CACHE_CONTROL "Cache-Control"
#define H_RESP_EXPIRES "Expires"
#define H_RESP_LAST_MODIFIED "Last-Modified"
#define H_RESP_LOCATION "Location"
#define H_RESP_ETAG "ETag"
#define H_RESP_CONTENT_DISPOSITION "Content-Disposition"

/*
1xx（信息性状态码）：
100 Continue：表示客户端应继续其请求。
101 Switching Protocols：表示服务器已经理解客户端的请求，并将切换到协议更适合的方式进行通信。
2xx（成功状态码）：

200 OK：请求成功。常用于GET和POST请求的响应。
201 Created：请求已经被成功处理，并且创建了新的资源。
204 No Content：服务器成功处理了请求，但不需要返回任何实体内容。
3xx（重定向状态码）：

301 Moved Permanently：请求的资源已被永久移动到新位置。
302 Found：请求的资源暂时被移动到了新的位置。
304 Not Modified：客户端缓存的资源未被修改，可以直接使用缓存的版本。
4xx（客户端错误状态码）：

400 Bad Request：客户端发送的请求有错误，服务器无法处理。
401 Unauthorized：请求要求用户身份验证。
403 Forbidden：服务器理解请求客户端的请求，但是拒绝执行此请求。
404 Not Found：服务器未能找到请求的资源。
5xx（服务器错误状态码）：

500 Internal Server Error：服务器遇到了一个未知的错误，无法完成请求。
502 Bad Gateway：服务器作为网关或代理，从上游服务器接收到无效响应。
503 Service Unavailable：服务器当前无法处理请求，通常是由于维护或过载导致的。*/

//不支持websocket协议切换等，舍不得删
//#define HTTP_100 "Continue"
//#define HTTP_101 "Switching Protocols"
#define HTTP_200 "OK"
#define HTTP_204 "No Content"
#define HTTP_206 "Partial Content"
#define HTTP_301 "Moved Permanently"
#define HTTP_302 "Found"
#define HTTP_304 "Not Modified"
#define HTTP_400 "Bad Request"
//没有Auth认证，舍不得删
//#define HTTP_401 "Unauthorized"
#define HTTP_403 "Forbidden"
#define HTTP_404 "Not Found"
#define HTTP_405 "Method Not Allowed"
#define HTTP_500 "Internal Server Error"
//不支持反向代理，还舍不得删
//#define HTTP_502 "Bad Gateway"
#define HTTP_503 "Service Unavailable"
#define HTTP_505 "HTTP Version Not Supported"
#define HTTP_UNKNOWN "Unknown HTTP Phrase"

/*SERVER_NAME：运行CGI序为机器名或IP地址。
SERVER_INTERFACE：WWW服务器的类型，如：CERN型或NCSA型。
SERVER_PROTOCOL：通信协议，应当是HTTP/1.0。
SERVER_PORT：TCP端口，一般说来web端口是80。
HTTP_ACCEPT：HTTP定义的浏览器能够接受的数据类型。
HTTP_REFERER：发送表单的文件URL。（并非所有的浏览器都传送这一变量）
HTTP_USER-AGENT：发送表单的浏览的有关信息。
GETWAY_INTERFACE：CGI程序的版本，在UNIX下为 CGI/1.1。
PATH_TRANSLATED：PATH_INFO中包含的实际路径名。
PATH_INFO：浏览器用GET方式发送数据时的附加路径。
SCRIPT_NAME：CGI程序的路径名。
QUERY_STRING：表单输入的数据，URL中问号后的内容。
REMOTE_HOST：发送程序的主机名，不能确定该值。
REMOTE_ADDR：发送程序的机器的IP地址。
REMOTE_USER：发送程序的人名。
CONTENT_TYPE：POST发送，一般为application/xwww-form-urlencoded。
CONTENT_LENGTH：POST方法输入的数据的字节数。
HTTP_COOKIE：包含客户端发送的所有cookie。
HTTP_REFERER：包含客户端从中链接到当前页面的URL。
HTTP_USER_AGENT：包含客户端浏览器的名称和版本号。
PATH_INFO：包含与CGI脚本关联的附加路径信息。
QUERY_STRING：包含查询字符串，即URL中的所有参数。
REMOTE_ADDR：包含客户端的IP地址。
REMOTE_HOST：包含客户端的主机名（如果可用）。
REQUEST_METHOD：包含客户端使用的HTTP方法，如GET或POST。
SCRIPT_FILENAME：包含正在运行的CGI脚本的完整路径。
SCRIPT_NAME：包含正在运行的CGI脚本的名称。
SERVER_NAME：包含Web服务器的主机名。
SERVER_PORT：包含Web服务器的端口号。
SERVER_PROTOCOL：包含Web服务器使用的协议名称和版本号，如HTTP/1.1。
SERVER_SOFTWARE：包含Web服务器的名称和版本号。
*/

//REQUEST_METHOD HTTP_COOKIE
#define H_CGI_SCRIPT_FILENAME L"SCRIPT_FILENAME"

#define H_CGI_HTTP_HOST L"HTTP_HOST"
#define H_CGI_REQUEST_URI L"REQUEST_URI"
#define H_CGI_PHP_SELF L"PHP_SELF"
#define H_CGI_REDIRECT_STATUS L"REDIRECT_STATUS"
#define H_CGI_HTTP_COOKIE L"HTTP_COOKIE"
#define H_CGI_REQUEST_METHOD L"REQUEST_METHOD"
#define H_CGI_SERVER_NAME L"SERVER_NAME"
#define H_CGI_SERVER_INTERFACE L"SERVER_INTERFACE"
#define H_CGI_SERVER_PROTOCOL L"SERVER_PROTOCOL"
#define H_CGI_SERVER_PORT L"SERVER_PORT"
#define H_CGI_HTTP_ACCEPT L"HTTP_ACCEPT"
#define H_CGI_HTTP_REFERER L"HTTP_REFERER"
#define H_CGI_HTTP_USER_AGENT L"HTTP_USER_AGENT"
#define H_CGI_PATH_TRANSLATED L"PATH_TRANSLATED"
#define H_CGI_PATH_INFO L"PATH_INFO"
#define H_CGI_SCRIPT_NAME L"SCRIPT_NAME"
#define H_CGI_QUERY_STRING L"QUERY_STRING"
#define H_CGI_REMOTE_HOST L"REMOTE_HOST"
#define H_CGI_REMOTE_ADDR L"REMOTE_ADDR"
#define H_CGI_REMOTE_USER L"REMOTE_USER"
#define H_CGI_CONTENT_TYPE L"CONTENT_TYPE"
#define H_CGI_CONTENT_LENGTH L"CONTENT_LENGTH"


static const std::unordered_map<int, std::string> HTTP_STATUS_CODE = {
    {200, HTTP_200},{204, HTTP_204},{206, HTTP_206},{301, HTTP_301},
    {302, HTTP_302},{304, HTTP_304},{400, HTTP_400},{403, HTTP_403},
    {404, HTTP_404},{405, HTTP_405},{500, HTTP_500},{503, HTTP_503},
    {505, HTTP_505},{0, HTTP_UNKNOWN},
};