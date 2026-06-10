#pragma once

#include "imgui.h"
#include "imguielement.h"
#include "background.h"
#include "taskbar.h"
#include <memory>

struct GLFWwindow;

class Desktop : public IMGUIElement {
public:
	Desktop();
	~Desktop() = default;

	void Initialize() override;
	void Draw(ImGuiViewport* viewport, GLFWwindow* window) override;
private:
	std::shared_ptr<Background> background;
	std::shared_ptr<Taskbar> taskbar;
};