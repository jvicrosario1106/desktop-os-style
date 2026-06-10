#pragma once

#include "../external/imgui/imgui.h"
#include "background.h"
#include "taskbar.h"
#include <memory>

struct GLFWwindow;

class Desktop {
public:
	Desktop();
	~Desktop() = default;

	void Initialize();
	void Draw(ImGuiViewport* viewport, GLFWwindow* window);

	bool GetShowDesktop() const;
private:
	bool show_desktop = true;
	std::shared_ptr<Background> background;
	std::shared_ptr<Taskbar> taskbar;
};