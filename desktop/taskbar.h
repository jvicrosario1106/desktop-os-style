#pragma once

#include "imgui.h"
#include "imguielement.h"
#include "powermenu.h"
#include "windows.h"
#include <memory>

struct GLFWwindow;

class Taskbar : public IMGUIElement {
public:
	Taskbar();
	~Taskbar() = default;

	void Initialize() override;
	void Draw(ImGuiViewport* viewport, GLFWwindow* window) override;

	int GetHeight() const;
private:
	bool DrawIconButton(const char* label, WindowType windowType);

	float height;
	float button_paddingY = 6.0f;
	float button_height;

	std::shared_ptr<PowerMenu> power_menu;
	std::shared_ptr<Windows> windows;
};