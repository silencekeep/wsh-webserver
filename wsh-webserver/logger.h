#pragma once
#include <iostream>
#include <fstream>
#include <ctime>
#include <iomanip>

enum class LogLevel { INFO, WARN, ERR };

class Logger {
public:
    Logger(std::string filename) {
        std::cout << "\033[0m";
        filename_ = filename;
        logFile_.open(filename_, std::ios::out | std::ios_base::app);
        if (!logFile_.is_open()) {
            std::cerr << "创建日志文件失败: " << filename_ << std::endl;
        }
        else logFile_.close();
    }
    Logger() : Logger(getTimestampedFilename()) {
        std::time_t now = std::time(nullptr);
        std::tm* localTime = std::localtime(&now); 
    }
    ~Logger() {
        if (logFile_.is_open()) {
            logFile_.close();
        }
    }

    void log(LogLevel level, std::string message) {
        std::time_t now = std::time(nullptr);
        std::tm* localTime = std::localtime(&now);

        // 格式化时间
        char timeStr[20];
        strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", localTime);

        switch (level) {
        case LogLevel::INFO:
            std::cout << "[\033[1;32mINFO\033[0m] ";
            break;
        case LogLevel::WARN:
            std::cout << "[\033[1;33mWARNING\033[0m] ";
            break;
        case LogLevel::ERR:
            std::cout << "[\033[1;31mERROR\033[0m] ";
            break;
        }
        // 打印到控制台
        std::cout << "[" << timeStr << "] ";
        printf("%s\n", message.c_str());

        logFile_.open(filename_, std::ios::out | std::ios_base::app);
        if (logFile_.is_open()) {
            logFile_ << "[" << timeStr << "] ";
            switch (level) {
            case LogLevel::INFO:
                logFile_ << "[\033[1;32mINFO\033[0m] ";
                break;
            case LogLevel::WARN:
                logFile_ << "[\033[1;33mWARNING\033[0m] ";
                break;
            case LogLevel::ERR:
                logFile_ << "[\033[1;31mERROR\033[0m] ";
                break;
            }
            logFile_ << message << std::endl;
            logFile_.close();
        }
    }

private:
    std::string filename_;
    std::ofstream logFile_;

    static std::string getTimestampedFilename() {
        std::time_t now = std::time(nullptr);
        std::tm* localTime = std::localtime(&now);
        char timeStr[28];
        strftime(timeStr, sizeof(timeStr), "%Y%m%d%H%M%S.txt", localTime);
        return std::string(timeStr);
    }
};