#pragma once

#include "../external/imgui/imgui.h"
#include "imguielement.h"
#include "powermenu.h"
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
	enum class ActiveScreen {
		None,
		Files,
		Messages,
		TaskManager
	};

	bool DrawIconButton(const char* label, ActiveScreen screen);
	void DrawActiveScreen();
	void DrawPlaceholderScreen(const char* title, const char* description, ActiveScreen screen);

	float height;
	float button_paddingY = 6.0f;
	float button_height;
	ActiveScreen active_screen = ActiveScreen::None;

	std::shared_ptr<PowerMenu> power_menu;
};