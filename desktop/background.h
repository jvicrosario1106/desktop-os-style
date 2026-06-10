#pragma once

#include "imgui.h"
#include "stb_image.h" 
#include <imgui_impl_opengl3_loader.h>
#include <GLFW/glfw3.h> 

class Background {
public:
	Background();
	~Background() = default;

	float offset;

	void LoadBackground(const char* filename);
	void Draw(ImGuiViewport* viewport);
private:
	bool show_background = true;

	GLuint current_texture;
	int background_width;
	int background_height;

	bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height);
};