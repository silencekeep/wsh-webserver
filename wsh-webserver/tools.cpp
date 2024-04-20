#include "string_utils.h"

void toUpper(std::string& str) {
    std::transform(str.begin(), str.end(), str.begin(),
        [](unsigned char c) { return std::toupper(c); });
}

void toLower(std::string& str) {
    std::transform(str.begin(), str.end(), str.begin(),
        [](unsigned char c) { return std::tolower(c); });
}

std::string getExtensionFromPath(const std::string& path) {
    size_t dotPos = path.find_last_of('.');
    if (dotPos != std::string::npos && dotPos < path.size() - 1) {
        return path.substr(dotPos);
    }
    return "";
}

void Xreplace(std::string& str, std::string from, std::string to) {
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', advance start_pos to avoid infinite loop
    }
}
void Gb2312ToUnicode(WCHAR* pOut, char* gbBuffer)
{
    ::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, gbBuffer, 2, pOut, 1);
    return;
}
void UTF_8ToUnicode(WCHAR* pOut, char* pText)
{
    char* uchar = (char*)pOut;

    uchar[1] = ((pText[0] & 0x0F) << 4) + ((pText[1] >> 2) & 0x0F);
    uchar[0] = ((pText[1] & 0x03) << 6) + (pText[2] & 0x3F);

    return;
}
void UnicodeToUTF_8(char* pOut, WCHAR* pText)
{
    // 注意 WCHAR高低字的顺序,低字节在前，高字节在后
    char* pchar = (char*)pText;

    pOut[0] = (0xE0 | ((pchar[1] & 0xF0) >> 4));
    pOut[1] = (0x80 | ((pchar[1] & 0x0F) << 2)) + ((pchar[0] & 0xC0) >> 6);
    pOut[2] = (0x80 | (pchar[0] & 0x3F));

    return;
}
void GB2312ToUTF_8(std::string& pOut, char* pText, int pLen)
{
    char buf[4];
    memset(buf, 0, 4);

    pOut.clear();

    int i = 0;
    while (i < pLen)
    {
        //如果是英文直接复制就可以
        if (pText[i] >= 0)
        {
            char asciistr[2] = { 0 };
            asciistr[0] = (pText[i++]);
            pOut.append(asciistr);
        }
        else
        {
            WCHAR pbuffer;
            Gb2312ToUnicode(&pbuffer, pText + i);
            UnicodeToUTF_8(buf, &pbuffer);
            pOut.append(buf);
            i += 2;
        }
    }
    return;
}
unsigned char ToHex(unsigned char x)
{
    return  x > 9 ? x + 55 : x + 48;
}

unsigned char FromHex(unsigned char x)
{
    unsigned char y;
    if (x >= 'A' && x <= 'Z') y = x - 'A' + 10;
    else if (x >= 'a' && x <= 'z') y = x - 'a' + 10;
    else if (x >= '0' && x <= '9') y = x - '0';
    else assert(0);
    return y;
}

std::string gb2312_to_utf8(const std::string& gb2312) {
    if (gb2312.empty()) return "";
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::wstring wide = boost::locale::conv::to_utf<wchar_t>(gb2312, "gb2312");
    return converter.to_bytes(wide);
}
std::string utf8_to_gb2312(const std::string& utf8) {
    if (utf8.empty()) return "";
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::wstring wide = converter.from_bytes(utf8);
    return boost::locale::conv::from_utf<wchar_t>(wide, "gb2312");
}

std::string urlEncode(const std::string& s)
{
    std::string strTemp = "";
    std::string str = gb2312_to_utf8(s);
    size_t length = str.length();
    for (size_t i = 0; i < length; i++)
    {
        if (isalnum((unsigned char)str[i]) ||
            (str[i] == '-') ||
            (str[i] == '_') ||
            (str[i] == '.') ||
            (str[i] == '~'))
            strTemp += str[i];
        else if (str[i] == ' ')
            strTemp += "+";
        else
        {
            strTemp += '%';
            strTemp += ToHex((unsigned char)str[i] >> 4);
            strTemp += ToHex((unsigned char)str[i] % 16);
        }
    }
    return strTemp;
}

std::string urlDecode(const std::string& str)
{
    std::string strTemp = "";
    size_t length = str.length();
    for (size_t i = 0; i < length; i++)
    {
        if (str[i] == '+') strTemp += ' ';
        else if (str[i] == '%')
        {
            assert(i + 2 < length);
            unsigned char high = FromHex((unsigned char)str[++i]);
            unsigned char low = FromHex((unsigned char)str[++i]);
            strTemp += (high << 4) | low;
        }
        else strTemp += str[i];
    }
    std::string s = utf8_to_gb2312(strTemp);
    return s;
}
std::wstring string_to_wstring(const std::string& str) {
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    return converter.from_bytes(str);
}

std::string wstring_to_string(const std::wstring& wstr)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    return converter.to_bytes(wstr);
}


//std::string urlEncode(const std::string& value) {
//    std::ostringstream escaped;
//    escaped.fill('0');
//    escaped << std::hex;
//
//    for (char c : value) {
//        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~' || c > 127) {
//            escaped << c;
//            continue;
//        }
//        escaped << std::uppercase;
//        escaped << '%' << std::setw(2) << int((unsigned char)c);
//        escaped << std::nouppercase;
//    }
//    return escaped.str();
//}
//
//std::string urlDecode(const std::string& value) {
//    std::ostringstream decoded;
//
//    for (size_t i = 0; i < value.length(); ++i) {
//        if (value[i] == '%') {
//            int hexValue = 0;
//            std::istringstream hexChars(value.substr(i + 1, 2));
//            hexChars >> std::hex >> hexValue;
//            decoded << static_cast<char>(hexValue);
//            i += 2;
//        }
//        else if (value[i] == '+') {
//            decoded << ' ';
//        }
//        else {
//            decoded << value[i];
//        }
//    }
//
//    return decoded.str();
//}

std::string q_split_string(const std::string& str, bool right_side) {
    size_t pos = str.find('?');
    if (pos == std::string::npos) {
        if (right_side) {
            return "";
        }
        else {
            return str;
        }
    }
    else {
        if (right_side) {
            return str.substr(pos + 1);
        }
        else {
            return str.substr(0, pos);
        }
    }
}

void trim(std::string& str) {
    if (str.find_first_not_of(" \t\n\v\f\r") == std::string::npos) {
        str.clear();
        return;
    }
    auto first = std::find_if_not(str.begin(), str.end(), [](char c) {
        return std::isspace(static_cast<unsigned char>(c));
        });
    auto last = std::find_if_not(str.rbegin(), str.rend(), [](char c) {
        return std::isspace(static_cast<unsigned char>(c));
        }).base();
    str = std::string(first, last);
}
