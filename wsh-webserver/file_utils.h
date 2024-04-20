#pragma once
#include <fstream>
#include <chrono>
#include <string>
#include <stdexcept>

std::string calculateFileETag(const std::string& filename, long fileLength,
	std::chrono::system_clock::time_point& endTime);

void readFileAndTimestamp(const std::string& filename,
	std::string& content, std::chrono::system_clock::time_point& endTime);

void readPartialFile(const std::string& filename, long start, long end,
	std::string& content, std::chrono::system_clock::time_point& endTime);

long long getFileBytesCount(const std::string& filename, std::chrono::system_clock::time_point& endTime);

std::string calculateFileETag(const std::string& filename, long fileLength,
	std::chrono::system_clock::time_point& endTime);