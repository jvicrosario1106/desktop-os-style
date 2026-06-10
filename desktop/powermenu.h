#pragma once

#include "../external/imgui/imgui.h"

struct GLFWwindow;

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