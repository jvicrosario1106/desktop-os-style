#pragma once

#include <string>

class Process {
public:
    Process(const std::string& name, int pid, float cpu, float memory);
    ~Process() = default;

    const std::string& GetName() const { return this->name; }
    int GetPID() const { return this->pid; }
    float GetCPU() const { return this->cpu; }
    float GetMemory() const { return this->memory; }

    void SetCPU(float cpu) { this->cpu = cpu; }
    void SetMemory(float memory) { this->memory = memory; }

private:
    std::string name;
    int pid;
    float cpu;           // CPU percentage (0-100)
    float memory;        // Memory in MB
};
