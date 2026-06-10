#include "desktop.h"

Desktop::Desktop() {
    this->background = std::make_shared<Background>();
    this->taskbar = std::make_shared<Taskbar>();
}

void Desktop::Initialize() {
    this->show_element = true;

    this->background->offset = this->taskbar->GetHeight();
    this->background->Initialize();

    this->taskbar->Initialize();
}

void Desktop::Draw(ImGuiViewport* viewport, GLFWwindow* window) {
    if (!show_element) {
        return;
    }

    this->background->Draw(viewport, window);
    this->taskbar->Draw(viewport, window);
}