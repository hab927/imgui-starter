#pragma once

#include "imgui/imgui.h"

#include <vector>
#include <string>

#include <iostream>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <ctime>
#include <fstream>

using namespace std::chrono;

class LoggingTool {

    public: 
        LoggingTool() {
            full_log = "";
            log_file = "../imgui-starter/log.txt";

            // clear contents of log
            std::ofstream log_stream(log_file, std::ios::trunc);
            log_stream.close();
        }

        ~LoggingTool() {
            // if i need to do something when it closes
        }
        
        static LoggingTool& GetInstance() {
            static LoggingTool instance;
            return instance;
        }

        void LogInfo(std::string msg) {
            std::string log_line = ""; 

            // write timestamp to log line
            log_line += "[" + GetTimestamp() + "]" + " " + msg;
            full_log += log_line + "\n";

            // add the log line to the log file
            WriteToLogFile(log_line);
        }

        void ClearLog() {
            full_log = "";
            WriteToLogFile("[" + GetTimestamp() + "]" + " " + "Log cleared.");
        }

        std::string GetLog() {
            return full_log;
        }

    private:
        enum Type {
            NORMAL,
            WARNING,
            ERROR
        };

        std::string full_log;
        std::string log_file;

        std::string GetTimestamp() {
            // get current local time (https://stackoverflow.com/questions/24686846/get-current-time-in-milliseconds-or-hhmmssmmm-format)
            auto now = system_clock::now();
            auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;
            auto timer = system_clock::to_time_t(now);
            std::tm bt = *std::localtime(&timer);

            std::ostringstream oss;

            oss << std::put_time(&bt, "%H:%M:%S");
            oss << '.' << std::setfill('0') << std::setw(3) << ms.count();

            return oss.str();
        }

        void WriteToLogFile(std::string msg) {
            std::ofstream log_stream(log_file, std::ios_base::app);
            log_stream << msg;
            log_stream << "\n";
            log_stream.flush();
            log_stream.close();
        }
};