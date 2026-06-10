#include "windows.h"

void Windows::Initialize() {
    this->show_element = true;
}

void Windows::Open(WindowType windowType) {
    bool* windowState = GetWindowState(windowType);
    if (windowState != nullptr) {
        *windowState = true;
    }
}

bool Windows::IsActive(WindowType windowType) const {
    const bool* windowState = GetWindowState(windowType);
    return windowState != nullptr && *windowState;
}

void Windows::DrawPlaceholderWindow(const char* title, const char* description, WindowType windowType) {
    bool* isOpen = GetWindowState(windowType);
    if (isOpen == nullptr || !*isOpen) {
        return;
    }

    ImGui::SetNextWindowSize(ImVec2(420.0f, 220.0f), ImGuiCond_FirstUseEver);
    if (ImGui::Begin(title, isOpen)) {
        ImGui::TextWrapped("%s", description);
        ImGui::Separator();
        ImGui::TextDisabled("Placeholder information for this window.");
    }
    ImGui::End();
}

void Windows::Draw(ImGuiViewport* viewport, GLFWwindow* window) {
    (void)viewport;
    (void)window;

    if (!this->show_element) {
        return;
    }

    DrawPlaceholderWindow("Files", "Browse local folders and recent documents here.", WindowType::Files);
    DrawPlaceholderWindow("Messages", "View system notifications and sample messages here.", WindowType::Messages);
    DrawPlaceholderWindow("Task Manager", "Task Manager will show process, CPU, and memory data in Component 3.", WindowType::TaskManager);
}

bool* Windows::GetWindowState(WindowType windowType) {
    switch (windowType) {
    case WindowType::Files:
        return &this->showFiles;
    case WindowType::Messages:
        return &this->showMessages;
    case WindowType::TaskManager:
        return &this->showTaskManager;
    case WindowType::None:
        return nullptr;
    }

    return nullptr;
}

const bool* Windows::GetWindowState(WindowType windowType) const {
    return const_cast<Windows*>(this)->GetWindowState(windowType);
}
