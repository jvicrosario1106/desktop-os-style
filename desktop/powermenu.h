#pragma once

#include "imgui.h"
#include <imgui_impl_opengl3_loader.h>
#include <GLFW/glfw3.h> 

class PowerMenu {
public:
	PowerMenu() = default;
	~PowerMenu() = default;

	ImVec2 start_pos;
	
	void ToggleShow();
	void Draw(ImGuiViewport* viewport, GLFWwindow* window);
private:
	bool show_power_menu = false;
};