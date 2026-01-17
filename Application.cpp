#include "Application.h"
#include "Logger.h"
#include "imgui/imgui.h"

#include <iostream>

namespace ClassGame {
        //
        // our global variables
        //
        void RenderConsole();

        //
        // game starting point
        // this is called by the main render loop in main.cpp
        //
        void GameStartUp() 
        {
            // Initialize logging system

            LoggingTool& logger = LoggingTool::GetInstance();
            logger.LogInfo("Game started successfully");
//            logger.LogGameEvent("Application initialized");
        }

        //
        // game render loop
        // this is called by the main render loop in main.cpp
        //
        void RenderGame() 
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

            RenderConsole();
        }

        void RenderConsole() {
            LoggingTool& logger = LoggingTool::GetInstance();

            ImGui::Begin("Debug Log");
            if (ImGui::Button("test log message")) {
                logger.LogInfo("The button has been pressed.");
            }
            ImGui::SameLine();
            if (ImGui::Button("test error")) {
                logger.ClearLog();
            }
            ImGui::SameLine();
            if (ImGui::Button("clear log")) {
                logger.ClearLog();
            }
            
            ImGui::BeginChild("LogOutput");
            ImGui::Text(logger.GetLog().c_str());
            ImGui::SetScrollHereY(1.0f);
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
