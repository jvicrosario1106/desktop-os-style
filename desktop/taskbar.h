#pragma once

#include "datetimecontroller.h"
#include "imgui.h"
#include "powermenu.h"
#include <string>
#include <imgui_impl_opengl3_loader.h>
#include <GLFW/glfw3.h> 

class Taskbar {
public:
	Taskbar();
	~Taskbar() = default;

	void Draw(ImGuiViewport* viewport, GLFWwindow* window);

	int GetHeight() const;
private:
	bool show_taskbar = true;
	//bool show_power_menu = false;
	float height;
	float button_paddingY = 6.0f;
	float button_height;

	std::shared_ptr<PowerMenu> power_menu;
};