#include "Application.h"
#include "Logger.h"
#include "imgui/imgui.h"

#include <iostream>

namespace ClassGame {
        // #########################
        //          CHANGES
        // #########################
        // 
        // Changes I made to this namespace ClassGame:
        // - added the RenderConsole() and changed RenderGame() to RenderLogDemo(), used RenderGame() to contain game controls
        // - added global variables buffer and counter
        // - added two new windows: Debug Log and Game Window.
        // - Debug Log implements the console window with user input, help button, filtering, autoscroll, and test logs.
        // - Game Window implements a game that can be used to interface with Debug Log to log game properties and events.
        //
        // Changes I made to Application.h:
        // - added RenderConsole() and RenderLogDemo() to reflect the above changes.
        //
        // Changes I made to CMakeLists.txt:
        // - changed executable name from demo to loggingtool.
        //
        // Changes I made to main_win32.cpp:
        // - inserted ClassGame::RenderConsole() and ClassGame::RenderLogDemo().
        //
        // Changes I made to README.md:
        // - edited to fit the requirements in the Canvas assignment

        // our global variables
        char buffer[128] = "";

        // game stuff
        static int counter = 0;

        //
        // game starting point
        // this is called by the main render loop in main.cpp
        //
        void GameStartUp() 
        {
            // Initialize logging system

            LoggingTool& logger = LoggingTool::GetInstance();
            logger.LogNormal("Game started successfully");
            logger.LogGameNormal("Application initialized");
        }

        void RenderLogDemo() 
        {
            ImGui::DockSpaceOverViewport();
            ImGui::ShowDemoWindow();

            ImGui::Begin("ImGui Log Demo");
            ImGui::LogButtons();

            if (ImGui::Button("Copy \"Hello, world!\" to clipboard"))
            {
                ImGui::LogToClipboard();
                ImGui::LogText("Hello, world!");
                ImGui::LogFinish();
            }
            ImGui::End();
        }

        //
        // game render loop
        // this is called by the main render loop in main.cpp
        //

        void RenderGame() {
            LoggingTool& logger = LoggingTool::GetInstance();

            ImGui::Begin("Game Window");
            ImGui::Text("Control game with this window");

            // controls child window
            ImGui::BeginChild("controls", ImVec2(0, 140), true);

            ImGui::Columns(2, "controlcolumns", true);
            // left column: controls
            if (ImGui::Button("+", ImVec2(50, 50))) {
                counter++;
            }
            ImGui::SameLine();
            if (ImGui::Button("-", ImVec2(50, 50))) {
                counter--;
            }
            ImGui::SameLine();
            if (ImGui::Button("0", ImVec2(50, 50))) {
                counter = 0;
            }
            if (ImGui::Button("+10", ImVec2(50, 50))) {
                counter += 10;
            }
            ImGui::SameLine();
            if (ImGui::Button("-10", ImVec2(50, 50))) {
                counter -= 10;
            }
            ImGui::SameLine();
            if (ImGui::Button("x2", ImVec2(50, 50))) {
                counter *= 2;
            }

            ImGui::NextColumn();

            // right column: logging requests
            if (ImGui::Button("log count")) {
                std::string log_msg = "Current value: " + std::to_string(counter);
                logger.LogGameNormal(log_msg.c_str());
                if (counter == 0) {
                    logger.LogGameWarning("Try the above buttons to change the value!");
                }
            }
            if (ImGui::Button("log decimal")) {
                logger.LogGameError("Decimal functionality not implemented");
            }

            ImGui::Columns(1);
            ImGui::EndChild();

            ImGui::Separator();

            std::string count_string = "CURRENT COUNTER: " + std::to_string(counter);
            ImGui::Text(count_string.c_str()); 

            ImGui::End();
        }

        // this renders the debug console
        void RenderConsole() {
            LoggingTool& logger = LoggingTool::GetInstance();

            static bool auto_scroll = true;
            static bool show_info = true;
            static bool show_warns = true;
            static bool show_errs = true;
            static bool show_non_logs = true;

            ImGui::Begin("Debug Log");

            // header
            ImGui::BeginChild("Header", ImVec2(0, 0), ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize);

            if (ImGui::Button("test normal")) {
                logger.LogNormal("Normal log!");
            }
            ImGui::SameLine();
            ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(128, 0, 0, 255));
            if (ImGui::Button("test error")) {
                logger.LogError("You pressed the error button!");
            }
            ImGui::PopStyleColor();
            ImGui::SameLine();
            ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(80, 70, 0, 255));
            if (ImGui::Button("test warning")) {
                logger.LogWarning("Warning! Beep beep!");
            }
            ImGui::PopStyleColor();
            ImGui::SameLine();
            if (ImGui::Button("clear log")) {
                logger.ClearLog();
            }
            ImGui::Separator();
            if (ImGui::Checkbox("autoscroll", &auto_scroll)) {};
            ImGui::Separator();
            ImGui::Text("Filtering options");
            ImGui::SameLine();
            if (ImGui::Checkbox("info logs", &show_info)) {};
            ImGui::SameLine();
            if (ImGui::Checkbox("warning logs", &show_warns)) {};
            ImGui::SameLine();
            if (ImGui::Checkbox("error logs", &show_errs)) {};
            ImGui::SameLine();
            if (ImGui::Checkbox("non-logs", &show_non_logs)) {};
            ImGui::EndChild();
            
            // console output
            ImGui::BeginChild("LogOutput", ImVec2(0, -70), true);

            // log each message with appropriate color coding
            LoggingTool::Log log = logger.GetLog();
            for (int i = 0; i < logger.GetLog().size(); i++) {
                switch (log[i].first) {
                    case LoggingTool::Type::ERROR:
                        if (!show_errs) break;
                        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 70, 70, 255));
                        ImGui::TextWrapped(log[i].second.c_str());
                        ImGui::PopStyleColor();
                        break;
                    case LoggingTool::Type::WARNING:
                        if (!show_warns) break;
                        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(220, 150, 0, 255));
                        ImGui::TextWrapped(log[i].second.c_str());
                        ImGui::PopStyleColor();
                        break;
                    case LoggingTool::Type::NORMAL:
                        if (!show_info) break;
                        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(150, 150, 150, 255));
                        ImGui::TextWrapped(log[i].second.c_str());
                        ImGui::PopStyleColor();
                        break;
                    default: // non-log message (for example: help)
                        if (!show_non_logs) break;
                        ImGui::TextWrapped(log[i].second.c_str());
                        break;
                }
            }

            if (auto_scroll) {
                ImGui::SetScrollHereY(1.0f);
            }
                
            ImGui::EndChild();

            // console footer
            ImGui::BeginChild("ConsoleInput", ImVec2(0, 62), true);
            if (ImGui::InputText("Console", buffer, IM_ARRAYSIZE(buffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
                std::string input = buffer;
                logger.SendNonlog("User Typed: " + input);
                buffer[0] = '\0';
                ImGui::SetKeyboardFocusHere(-1);
            };
            ImGui::SameLine();
            if (ImGui::Button("Help")) {
                logger.DisplayHelpMessage();
            }
            ImGui::EndChild();
            ImGui::End();
        }

        //
        // end turn is called by the game code at the end of each turn
        // this is where we check for a winner
        //
        void EndOfTurn() 
        {
        }
}
