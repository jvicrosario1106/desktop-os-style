#pragma once

#include "background.h"
#include "taskbar.h"
#include <imgui_impl_opengl3_loader.h>
#include <GLFW/glfw3.h> 
#include <memory>

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