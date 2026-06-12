#pragma once

#include "imguielement.h"
#include "process.h"
#include <vector>
#include <memory>
#include <chrono>
#include <string>

class TaskManager : public IMGUIElement {
public:
    TaskManager();
    ~TaskManager() = default;

    void Initialize() override;
    void Draw(ImGuiViewport* viewport, GLFWwindow* window) override;
    void SetOpenState(bool* isOpen);

    // Update process data
    void UpdateProcessData();

private:
    bool initialized = false;
    bool* openState = nullptr;
    std::vector<std::shared_ptr<Process>> processes;
    std::chrono::high_resolution_clock::time_point lastUpdateTime;
    float totalAvailableMemory = 0.0f;  // in MB, loaded from config.txt

    void InitializeDummyProcesses();

    void DrawProcessTable();

    // load total memory from resources/config.txt
    void LoadConfiguration();

    // Calculate total CPU usage (sum of all process CPU)
    float CalculateTotalCPU() const;

    // Calculate total memory usage (sum of all process memory)
    float CalculateTotalMemory() const;
};
