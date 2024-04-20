#pragma once
#include <unordered_map>

/*���õ�HTTP����ͷ��
Host��ָ����������Դ���������Ͷ˿ںš�
User-Agent������������û������ַ�����ͨ�������������ͻ��˵������Ϣ��
Accept-Encoding��ָ���ͻ��˿ɽ��ܵ����ݱ��롣
Content-Type��ָ�����������MIME���͡�
Content-Length��ָ����������ĳ��ȣ��ֽ�������
Referer��ָʾ�������Դҳ���URL��
Cookie�����Ͱ���HTTP cookies������*/

//��֧��HTTP/1.0��׼�µ�����METHOD
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
���õ�HTTP��Ӧͷ��
Content-Type��ָ����Ӧ�����MIME���͡�
Content-Length��ָ����Ӧ����ĳ��ȣ��ֽ�������
Server��ָʾ�������������Ϣ��
Set-Cookie������HTTP cookie��
Location�������ض�����Ӧ��Ŀ��URL��
Cache-Control��ָ��������ԡ�
Expires��ָ����Ӧ�Ĺ���ʱ�䡣
Last-Modified��ָʾ��Դ������޸�ʱ�䡣
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
1xx����Ϣ��״̬�룩��
100 Continue����ʾ�ͻ���Ӧ����������
101 Switching Protocols����ʾ�������Ѿ����ͻ��˵����󣬲����л���Э����ʺϵķ�ʽ����ͨ�š�
2xx���ɹ�״̬�룩��

200 OK������ɹ���������GET��POST�������Ӧ��
201 Created�������Ѿ����ɹ��������Ҵ������µ���Դ��
204 No Content���������ɹ����������󣬵�����Ҫ�����κ�ʵ�����ݡ�
3xx���ض���״̬�룩��

301 Moved Permanently���������Դ�ѱ������ƶ�����λ�á�
302 Found���������Դ��ʱ���ƶ������µ�λ�á�
304 Not Modified���ͻ��˻������Դδ���޸ģ�����ֱ��ʹ�û���İ汾��
4xx���ͻ��˴���״̬�룩��

400 Bad Request���ͻ��˷��͵������д��󣬷������޷�����
401 Unauthorized������Ҫ���û������֤��
403 Forbidden���������������ͻ��˵����󣬵��Ǿܾ�ִ�д�����
404 Not Found��������δ���ҵ��������Դ��
5xx������������״̬�룩��

500 Internal Server Error��������������һ��δ֪�Ĵ����޷��������
502 Bad Gateway����������Ϊ���ػ���������η��������յ���Ч��Ӧ��
503 Service Unavailable����������ǰ�޷���������ͨ��������ά������ص��µġ�*/

//��֧��websocketЭ���л��ȣ��᲻��ɾ
//#define HTTP_100 "Continue"
//#define HTTP_101 "Switching Protocols"
#define HTTP_200 "OK"
#define HTTP_204 "No Content"
#define HTTP_206 "Partial Content"
#define HTTP_301 "Moved Permanently"
#define HTTP_302 "Found"
#define HTTP_304 "Not Modified"
#define HTTP_400 "Bad Request"
//û��Auth��֤���᲻��ɾ
//#define HTTP_401 "Unauthorized"
#define HTTP_403 "Forbidden"
#define HTTP_404 "Not Found"
#define HTTP_405 "Method Not Allowed"
#define HTTP_500 "Internal Server Error"
//��֧�ַ���������᲻��ɾ
//#define HTTP_502 "Bad Gateway"
#define HTTP_503 "Service Unavailable"
#define HTTP_505 "HTTP Version Not Supported"
#define HTTP_UNKNOWN "Unknown HTTP Phrase"

/*SERVER_NAME������CGI��Ϊ��������IP��ַ��
SERVER_INTERFACE��WWW�����������ͣ��磺CERN�ͻ�NCSA�͡�
SERVER_PROTOCOL��ͨ��Э�飬Ӧ����HTTP/1.0��
SERVER_PORT��TCP�˿ڣ�һ��˵��web�˿���80��
HTTP_ACCEPT��HTTP�����������ܹ����ܵ��������͡�
HTTP_REFERER�����ͱ����ļ�URL�����������е��������������һ������
HTTP_USER-AGENT�����ͱ���������й���Ϣ��
GETWAY_INTERFACE��CGI����İ汾����UNIX��Ϊ CGI/1.1��
PATH_TRANSLATED��PATH_INFO�а�����ʵ��·������
PATH_INFO���������GET��ʽ��������ʱ�ĸ���·����
SCRIPT_NAME��CGI�����·������
QUERY_STRING������������ݣ�URL���ʺź�����ݡ�
REMOTE_HOST�����ͳ����������������ȷ����ֵ��
REMOTE_ADDR�����ͳ���Ļ�����IP��ַ��
REMOTE_USER�����ͳ����������
CONTENT_TYPE��POST���ͣ�һ��Ϊapplication/xwww-form-urlencoded��
CONTENT_LENGTH��POST������������ݵ��ֽ�����
HTTP_COOKIE�������ͻ��˷��͵�����cookie��
HTTP_REFERER�������ͻ��˴������ӵ���ǰҳ���URL��
HTTP_USER_AGENT�������ͻ�������������ƺͰ汾�š�
PATH_INFO��������CGI�ű������ĸ���·����Ϣ��
QUERY_STRING��������ѯ�ַ�������URL�е����в�����
REMOTE_ADDR�������ͻ��˵�IP��ַ��
REMOTE_HOST�������ͻ��˵���������������ã���
REQUEST_METHOD�������ͻ���ʹ�õ�HTTP��������GET��POST��
SCRIPT_FILENAME�������������е�CGI�ű�������·����
SCRIPT_NAME�������������е�CGI�ű������ơ�
SERVER_NAME������Web����������������
SERVER_PORT������Web�������Ķ˿ںš�
SERVER_PROTOCOL������Web������ʹ�õ�Э�����ƺͰ汾�ţ���HTTP/1.1��
SERVER_SOFTWARE������Web�����������ƺͰ汾�š�
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