#include "taskbar.h"
#include "datetimecontroller.h"
#include <string>

namespace {
constexpr float AppButtonWidth = 118.0f;
constexpr float ButtonSpacing = 8.0f;
}

Taskbar::Taskbar() : height(50.0f) {
    this->button_height = this->height - this->button_paddingY;
    this->power_menu = std::make_shared<PowerMenu>();
    this->power_menu->Initialize();
    this->windows = std::make_shared<DesktopWindows>();
    this->windows->Initialize();
}

int Taskbar::GetHeight() const {
	return this->height;
}

bool Taskbar::DrawIconButton(const char* label, WindowType windowType) {
    const bool isActive = this->windows->IsActive(windowType);

    if (isActive) {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.18f, 0.36f, 0.68f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.24f, 0.44f, 0.78f, 1.0f));
    }

    const bool wasClicked = ImGui::Button(label, ImVec2(AppButtonWidth, this->button_height));

    if (isActive) {
        ImGui::PopStyleColor(2);
    }

    if (wasClicked) {
        this->windows->Open(windowType);
    }

    return wasClicked;
}

void Taskbar::Initialize() {
    this->show_element = true;
}

void Taskbar::Draw(ImGuiViewport* viewport, GLFWwindow* window) {
    if (!this->show_element) {
        return;
    }

    this->windows->Draw(viewport, window);

    ImVec2 taskbar_pos = ImVec2(viewport->Pos.x, viewport->Pos.y + viewport->Size.y - this->height);
    ImVec2 taskbar_size = ImVec2(viewport->Size.x, this->height);

    ImGui::SetNextWindowPos(taskbar_pos);
    ImGui::SetNextWindowSize(taskbar_size);

    // 2. Style variations for a flat, modern taskbar
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    // Optional: Pushes a dark background color specifically for the taskbar
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.10f, 0.10f, 0.12f, 1.0f));

    // 3. Flags to make it a sticky bar that stays on top
    ImGuiWindowFlags taskbar_flags = ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoSavedSettings;
    // Note: We DO NOT use NoBringToFrontOnFocus here, because we WANT the taskbar on top!

    ImGui::Begin("Taskbar", &this->show_element, taskbar_flags);

    // --- TASKBAR CONTENT ---

    ImGui::SetCursorPosY((this->height - button_height) * 0.5f);

    // Center the button vertically inside the taskbar
    ImGui::SetCursorPosY((this->height - button_height) * 0.5f);

    // Define the full button dimensions (Width: 70px, Height: calculated dynamically)
    ImVec2 start_button_size = ImVec2(70.0f, button_height);

    if (ImGui::Button("Start", start_button_size)) {
        // Pop up power menu
        this->power_menu->ToggleShow();
    }

    ImVec2 start_button_pos = ImGui::GetItemRectMin();

    ImGui::SameLine(0.0f, ButtonSpacing);
    this->DrawIconButton("[F] Files", WindowType::Files);

    ImGui::SameLine(0.0f, ButtonSpacing);
    this->DrawIconButton("[M] Messages", WindowType::Messages);

    ImGui::SameLine(0.0f, ButtonSpacing);
    this->DrawIconButton("[T] Task Manager", WindowType::TaskManager);

    // Right-align the clock text
    std::string clock_str = DateTimeController::GetCurrentTimeString();
    std::string date_str = DateTimeController::GetCurrentDateString();

    float clock_width = ImGui::CalcTextSize(clock_str.c_str()).x;
    float date_width = ImGui::CalcTextSize(date_str.c_str()).x;
    float largest_width = (clock_width > date_width) ? clock_width : date_width;

    // Move the cursor to the right side of the taskbar, minus a small margin
    ImGui::SameLine(ImGui::GetWindowWidth() - largest_width - 15.0f);

    // Center the text vertically within the 40px bar
    float stack_height = ImGui::GetTextLineHeightWithSpacing() * 2.0f;
    ImGui::SetCursorPosY((this->height - stack_height) * 0.5f);

    ImGui::BeginGroup();
    // Print the updating clock text
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (largest_width - clock_width) * 0.5f);
    ImGui::Text("%s", clock_str.c_str());

    // Print the date
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (largest_width - date_width) * 0.5f);
    ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "%s", date_str.c_str());
    ImGui::EndGroup();

    ImGui::End();
    ImGui::PopStyleColor(1);
    ImGui::PopStyleVar(2);

    // --- POWER MENU CONTENT ---

    this->power_menu->start_pos = start_button_pos;
    this->power_menu->Draw(viewport, window);
}