#include "powermenu.h"

void PowerMenu::ToggleShow() {
    this->show_power_menu = !this->show_power_menu;
}

void PowerMenu::Draw(ImGuiViewport* viewport, GLFWwindow* window) {
    if (!this->show_power_menu) {
        return;
    }

    // Define the size of your small popout menu
    ImVec2 menu_size = ImVec2(150.0f, 100.0f);

    // Position it directly above the Start button
    ImVec2 menu_pos = ImVec2(this->start_pos.x, this->start_pos.y - menu_size.y - 2.0f); // 2px gap

    ImGui::SetNextWindowPos(menu_pos);
    ImGui::SetNextWindowSize(menu_size);

    // Add flags to make it look like a clean context menu panel
    ImGuiWindowFlags menu_flags = ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoScrollbar;

    ImGui::Begin("PowerMenu", &this->show_power_menu, menu_flags);

    ImGui::Text("System Options");
    ImGui::Separator();

    // Add a bit of spacing
    ImGui::Spacing();

    // THE POWER BUTTON: Closes the GLFW window application gracefully
    if (ImGui::Button("Shut Down", ImVec2(-FLT_MIN, 0))) {
        // 'window' is the GLFWwindow pointer initialized at the top of main.cpp
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    if (ImGui::Button("Log Out", ImVec2(-FLT_MIN, 0))) {
        show_power_menu = false; // Just closes the menu for now
    }

    // Close the menu if the user clicks anywhere outside of this window
    if (!ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows) && ImGui::IsMouseClicked(0)) {
        show_power_menu = false;
    }

    ImGui::End();
}