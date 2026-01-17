#pragma once

namespace ClassGame {
    void GameStartUp();
    void RenderLogDemo();
    void RenderGame();
    void RenderConsole();
    void EndOfTurn();
    
    // Logging window management
    void ShowLogWindow(bool* p_open = nullptr);
    void ToggleLogWindow();
    bool IsLogWindowVisible();
}
