#include "file_utils.h"
#include "string_utils.h"

// FNV-1a¹þÏ£Ëã·¨
uint32_t fnv1a(const std::string& str)
{
    const uint32_t prime = 0x01000193; // 16777619
    uint32_t hash = 0x811c9dc5;         // 2166136261
    for (char c : str) {
        hash ^= static_cast<uint32_t>(c);
        hash *= prime;
    }
    return hash;
}

void readFileAndTimestamp(const std::string& fname, std::string& content, std::chrono::system_clock::time_point& endTime) {
    readPartialFile(fname, 0, getFileBytesCount(fname, endTime) - 1, content, endTime);
}

void readPartialFile(const std::string& fname, long start, long end, std::string& content, std::chrono::system_clock::time_point& endTime)
{
    std::string filename = fname;
    Xreplace(filename, std::string("/"), std::string("\\"));
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        //std::cerr << "Error: Unable to open file." << std::endl;
        throw std::runtime_error::runtime_error("File Not Found.");
        return;
    }
    file.seekg(start);
    std::streamsize size = end - start;
    content.resize(size);
    file.read(&content[0], size);
    file.close();

    struct stat fileStat;
    if (stat(filename.c_str(), &fileStat) != -1) {
        std::time_t lastModifiedTime = fileStat.st_mtime;
        endTime = std::chrono::system_clock::from_time_t(lastModifiedTime);
    }
    else {
        throw std::runtime_error::runtime_error("Get Last Modified Time Failed.");
    }

}

long long getFileBytesCount(const std::string& fname, std::chrono::system_clock::time_point& endTime)
{
    std::string filename = fname;
    Xreplace(filename, std::string("/"), std::string("\\"));

    struct stat fileStat;
    if (stat(filename.c_str(), &fileStat) != -1) {
        std::time_t lastModifiedTime = fileStat.st_mtime;
        endTime = std::chrono::system_clock::from_time_t(lastModifiedTime);
    }
    else {
        throw std::runtime_error::runtime_error("Get Last Modified Time Failed.");
    }
    return fileStat.st_size;
}

std::string calculateFileETag(const std::string& fname, long fileLength, std::chrono::system_clock::time_point& endTime)
{
    std::string filename = fname;
    Xreplace(filename, std::string("/"), std::string("\\"));
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(endTime.time_since_epoch()).count();;
    std::string sgm = filename + "$" + std::to_string(fileLength) + "$" + std::to_string(millis);
    return std::to_string(fnv1a(sgm));
}
