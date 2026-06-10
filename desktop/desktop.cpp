#include "desktop.h"

Desktop::Desktop()
{
    this->background = std::make_shared<Background>();
    this->taskbar = std::make_shared<Taskbar>();
}

void Desktop::Initialize() {
    this->background->LoadBackground("resources/MTGWallpaper.jpg");
    this->background->offset = this->taskbar->GetHeight();
}

void Desktop::Draw(ImGuiViewport* viewport, GLFWwindow* window)
{
    if (!show_desktop) {
        return;
    }

    this->background->Draw(viewport);
    this->taskbar->Draw(viewport, window);
}

bool Desktop::GetShowDesktop() const {
	return show_desktop;
}
