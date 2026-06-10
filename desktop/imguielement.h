#pragma once

#include "imgui.h"
#include <imgui_impl_opengl3_loader.h>
#include <GLFW/glfw3.h> 

class IMGUIElement {
public:
	IMGUIElement() = default;
	~IMGUIElement() = default;

	void ToggleShow();
	virtual void Initialize() = 0;
	virtual void Draw(ImGuiViewport* viewport, GLFWwindow* window) = 0;

	bool GetShowElement() const;
protected:
	bool show_element;
};