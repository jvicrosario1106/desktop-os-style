#pragma once

#include "imgui.h"

struct GLFWwindow;

class IMGUIElement {
public:
	IMGUIElement() = default;
	~IMGUIElement() = default;

	void ToggleShow();
	virtual void Initialize() = 0;
	virtual void Draw(ImGuiViewport* viewport, GLFWwindow* window) = 0;

	bool GetShowElement() const;
protected:
	bool show_element = false;
};