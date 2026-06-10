#pragma once

#include "../external/imgui/imgui.h"
#include "../external/imgui/backends/imgui_impl_opengl3_loader.h"
#include "imguielement.h"

class Background : public IMGUIElement {
public:
	Background();
	~Background() = default;

	float offset;

	void LoadBackground(const char* filename);
	void Initialize() override;
	void Draw(ImGuiViewport* viewport, GLFWwindow* window) override;
private:

	GLuint current_texture;
	int background_width;
	int background_height;

	bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height);
};