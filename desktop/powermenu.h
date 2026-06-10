#pragma once

#include "../external/imgui/imgui.h"
#include "imguielement.h"

struct GLFWwindow;

class PowerMenu : public IMGUIElement{
public:
	PowerMenu() = default;
	~PowerMenu() = default;

	ImVec2 start_pos;
	
	void Initialize() override;
	void Draw(ImGuiViewport* viewport, GLFWwindow* window) override;
};