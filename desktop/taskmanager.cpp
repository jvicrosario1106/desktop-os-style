#include "taskmanager.h"
#include <imgui.h>
#include <algorithm>
#include <cstdlib>
#include <cmath>
#include <fstream>
#include <sstream>

namespace {

float RandomFloat(float minValue, float maxValue) {
    const float normalized = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX); // get a value between 0.0 and 1.0
    return std::clamp(minValue + normalized * (maxValue - minValue), minValue, maxValue);
}
}

TaskManager::TaskManager() {
    this->Initialize();
}

void TaskManager::SetOpenState(bool* isOpen) {
    this->openState = isOpen;
}

void TaskManager::Initialize() {
    if (this->initialized) {
        return;
    }

    this->show_element = true;
    this->LoadConfiguration();
    this->InitializeDummyProcesses();
    this->lastUpdateTime = std::chrono::high_resolution_clock::now();
    this->initialized = true;
}

void TaskManager::LoadConfiguration() {
    // Try to load config from resources/config.txt
    std::ifstream configFile("resources/config.txt");
    
    // Default to 8192 MB if file not found
    this->totalAvailableMemory = 8192.0f;
    
    if (configFile.is_open()) {
        std::string line;
        while (std::getline(configFile, line)) {
            // Skip comments and empty lines
            if (line.empty() || line[0] == '#') {
                continue;
            }
            
            // Parse TOTAL_MEMORY_MB=value
            size_t pos = line.find("TOTAL_MEMORY_MB=");
            if (pos != std::string::npos) {
                try {
                    std::string valueStr = line.substr(pos + 16);  // 16 is length of "TOTAL_MEMORY_MB="
                    this->totalAvailableMemory = std::stof(valueStr);
                } catch (...) {
                    // retain default data
                }
                break;
            }
        }
        configFile.close();
    }
}

float TaskManager::CalculateTotalCPU() const {
    float total = 0.0f;
    for (const auto& process : this->processes) {
        total += process->GetCPU();
    }
    // Cap at 100%
    return std::min(total, 100.0f);
}

float TaskManager::CalculateTotalMemory() const {
    float total = 0.0f;
    for (const auto& process : this->processes) {
        total += process->GetMemory();
    }
    return total;
}

void TaskManager::InitializeDummyProcesses() {
    this->processes.push_back(std::make_shared<Process>("explorer.exe", 1, 2.5f, 128.4f));
    this->processes.push_back(std::make_shared<Process>("chrome.exe", 2, 15.8f, 512.3f));
    this->processes.push_back(std::make_shared<Process>("notepad.exe", 3, 0.3f, 45.2f));
    this->processes.push_back(std::make_shared<Process>("VSCode.exe", 4, 8.2f, 356.7f));
    this->processes.push_back(std::make_shared<Process>("discord.exe", 5, 5.1f, 289.5f));
    this->processes.push_back(std::make_shared<Process>("spotify.exe", 6, 3.4f, 178.9f));
    this->processes.push_back(std::make_shared<Process>("steam.exe", 7, 1.2f, 95.6f));
    this->processes.push_back(std::make_shared<Process>("MTGArena.exe", 8, 0.1f, 12.3f));
}

void TaskManager::UpdateProcessData() {
    float remainingCPU = 100.0f;
    float remainingMemory = std::max(0.0f, this->totalAvailableMemory);

    for (auto& process : this->processes) {
        // Random CPU between 0 to 20 percent, capped by remaining allocation.
        float requestedCPU = RandomFloat(0.0f, 20.0f);
        float cpu = std::clamp(requestedCPU, 0.0f, remainingCPU);
        process->SetCPU(cpu);
        remainingCPU = std::max(0.0f, remainingCPU - cpu);

        float memoryVariation = RandomFloat(-1.0f, 1.0f);
        float currentMemory = process->GetMemory();
        float requestedMemory = std::max(5.0f, currentMemory + memoryVariation);
        float newMemory = std::clamp(requestedMemory, 0.0f, remainingMemory);
        process->SetMemory(newMemory);
        remainingMemory = std::max(0.0f, remainingMemory - newMemory);
    }
}

void TaskManager::DrawProcessTable() {
    if (ImGui::BeginTable("ProcessTable", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
        ImGui::TableSetupColumn("Process Name", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("PID", ImGuiTableColumnFlags_WidthFixed, 80.0f);
        ImGui::TableSetupColumn("CPU %", ImGuiTableColumnFlags_WidthFixed, 80.0f);
        ImGui::TableSetupColumn("Memory (MB)", ImGuiTableColumnFlags_WidthFixed, 100.0f);
        ImGui::TableHeadersRow();

        for (const auto& process : this->processes) {
            ImGui::TableNextRow();

            // Process Name
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%s", process->GetName().c_str());

            // PID
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%d", process->GetPID());

            // CPU %
            ImGui::TableSetColumnIndex(2);
            ImGui::Text("%.1f%%", process->GetCPU());

            // Memory (MB)
            ImGui::TableSetColumnIndex(3);
            ImGui::Text("%.1f", process->GetMemory());
        }

        ImGui::EndTable();
    }
}

void TaskManager::Draw(ImGuiViewport* viewport, GLFWwindow* window) {
    (void)viewport;
    (void)window;

    if (this->openState != nullptr && !*this->openState) {
        return;
    }

    if (!this->show_element) {
        return;
    }

    // Update dummy data only once per second to avoid too-rapid changes
    auto currentTime = std::chrono::high_resolution_clock::now();
    auto elapsedSeconds = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - this->lastUpdateTime).count() / 1000.0;
    
    if (elapsedSeconds >= 1.0) {
        this->UpdateProcessData();
        this->lastUpdateTime = currentTime;
    }

    ImGui::SetNextWindowSize(ImVec2(600, 400), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Task Manager", this->openState)) {
        // Display total CPU and memory usage
        float totalCPU = this->CalculateTotalCPU();
        float totalMemory = this->CalculateTotalMemory();
        float memoryPercent = (totalMemory / this->totalAvailableMemory) * 100.0f;
        
        ImGui::Text("System Usage:");
        ImGui::Text("  CPU: %.1f%%", totalCPU);
        ImGui::Text("  Memory: %.1f MB / %.0f MB (%.1f%%)", totalMemory, this->totalAvailableMemory, memoryPercent);
        
        ImGui::Spacing();
        ImGui::Text("Running Processes:");
        ImGui::Separator();

        this->DrawProcessTable();

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Text("Total Processes: %zu", this->processes.size());
    }
    ImGui::End();
}
