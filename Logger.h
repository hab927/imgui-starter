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
        enum Type {
            NORMAL,
            WARNING,
            ERROR
        };

        typedef std::vector<std::pair<Type, std::string>> Log;
        
        static LoggingTool& GetInstance() {
            static LoggingTool instance;
            return instance;
        }

        void LogNormal(std::string msg) {
            std::string log_line = "[" + GetTimestamp() + "]" + " " + msg;
            full_log.push_back(std::make_pair(NORMAL, log_line + "\n"));

            // add the log line to the log file
            WriteToLogFile(log_line);
        }

        void LogGameNormal(std::string msg) { // will extend later as necessary
            std::string log_line = "[" + GetTimestamp() + "]" + " [GAME] " + msg;
            full_log.push_back(std::make_pair(NORMAL, log_line + "\n"));

            WriteToLogFile(log_line);
        }

        void LogError(std::string msg) {
            std::string log_line = "[" + GetTimestamp() + "]" + " ERROR: " + msg;
            full_log.push_back(std::make_pair(ERROR, log_line + "\n"));

            WriteToLogFile(log_line);
        }

        void LogGameError(std::string msg) { // will extend later as necessary
            std::string log_line = "[" + GetTimestamp() + "]" + " [GAME] ERROR: " + msg;
            full_log.push_back(std::make_pair(ERROR, log_line + "\n"));

            WriteToLogFile(log_line);
        }

        void LogWarning(std::string msg) {
            std::string log_line = "[" + GetTimestamp() + "]" + " WARNING: " + msg;
            full_log.push_back(std::make_pair(WARNING, log_line + "\n"));

            WriteToLogFile(log_line);
        }

        void LogGameWarning(std::string msg) { // will extend later as necessary
            std::string log_line = "[" + GetTimestamp() + "]" + " [GAME] WARNING: " + msg;
            full_log.push_back(std::make_pair(WARNING, log_line + "\n"));

            WriteToLogFile(log_line);
        }

        void ClearLog() {
            full_log = {};
            WriteToLogFile("[" + GetTimestamp() + "]" + " " + "Log cleared.");
        }

        Log GetLog() {
            return full_log;
        }

        void DisplayHelpMessage() {
            full_log.push_back(std::make_pair(NORMAL, help_message + "\n"));
        }

    private:
        Log full_log;
        std::string log_file;

        // help message that's shown when you click the "Help" button
        std::string help_message = 
            "The buttons above test various logging tool features.\n"
            "This window displays output from the log.\n"
            "You can enter text in the field at the bottom and it will be echoed back in the console, and therefore reflected in the log.\n"
            "The help button displays this message.\n"
            "This message will not be logged.\nEnjoy!";

        std::string GetTimestamp() {
            // get current local time (https://stackoverflow.com/questions/24686846/get-current-time-in-milliseconds-or-hhmmssmmm-format)
            auto now = system_clock::now();
            auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;
            auto timer = system_clock::to_time_t(now);
            struct std::tm bt;
            bool result = localtime_s(&bt, &timer);

            if (result != 0) {
                std::cout << "localtime_s failed" << std::endl;
                return "XX:XX:XX.XXX";
            }

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

        LoggingTool() {
            full_log = {};

            // NOTE: on my machine, this starts in the \build\ directory, goes out, goes to imgui-starter, and outputs to log.txt.
            // It might be different for you!
            log_file = "../imgui-starter/log.txt";

            // clear contents of log
            std::ofstream log_stream(log_file, std::ios::trunc);
            log_stream.close();
        }

        ~LoggingTool() {
            // if i need to do something when it closes
        }
};