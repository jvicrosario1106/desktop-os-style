#include "background.h"

Background::Background() : offset(0.0f), current_texture(0), background_width(0), background_height(0) {

}

//"resources/MTGWallpaper.jpg"
void Background::LoadBackground(const char* file) {
    bool ret = LoadTextureFromFile(file, &this->current_texture, &this->background_width, &this->background_height);
    if (!ret) {
        //IM_ASSERT(ret);
        current_texture = 0;
    }
}

bool Background::LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height) {
    // Load from disk into RAM
    int image_width = 0;
    int image_height = 0;
    unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
    if (image_data == NULL)
        return false;

    // Create a OpenGL texture identifier
    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Upload pixels to graphics card
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    stbi_image_free(image_data);

    *out_texture = image_texture;
    *out_width = image_width;
    *out_height = image_height;

    return true;
}

void Background::Draw(ImGuiViewport* viewport)
{
    if (!show_background) {
        return;
    }

    //ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImVec2 wallpaper_size = ImVec2(viewport->WorkSize.x, viewport->WorkSize.y - this->offset);
    ImGui::SetNextWindowSize(wallpaper_size);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    ImGuiWindowFlags desktop_flags = ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoScrollWithMouse |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoNav |
        ImGuiWindowFlags_NoBringToFrontOnFocus;

    ImGui::Begin("Desktop", &this->show_background, desktop_flags);

    if (current_texture != 0) {
        // A. Image loaded successfully! Render the wallpaper texture
        ImTextureID tex_id = (ImTextureID)(intptr_t)current_texture;
        ImGui::Image(tex_id, wallpaper_size, ImVec2(0, 0), ImVec2(1, 1));
    }
    else {
        // B. Fallback: Draw a solid background color if the image wasn't found
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        ImVec2 min_pos = viewport->WorkPos;
        ImVec2 max_pos = ImVec2(min_pos.x + viewport->WorkSize.x, min_pos.y + viewport->WorkSize.y);
        draw_list->AddRectFilled(min_pos, max_pos, IM_COL32(34, 112, 147, 255));
    }

    ImGui::End();
    ImGui::PopStyleVar(3);
}