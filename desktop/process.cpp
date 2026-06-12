#include "process.h"

Process::Process(const std::string& name, int pid, float cpu, float memory)
    : name(name), pid(pid), cpu(cpu), memory(memory) {
}
