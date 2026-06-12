#pragma once

#include "imguielement.h"
#include "taskmanager.h"
#include <memory>

enum class WindowType {
    None,
    Files,
    Messages,
    TaskManager
};

class DesktopWindows : public IMGUIElement {
public:
    DesktopWindows() = default;
    ~DesktopWindows() = default;

    void Initialize() override;
    void Draw(ImGuiViewport* viewport, GLFWwindow* window) override;

    void Open(WindowType windowType);
    bool IsActive(WindowType windowType) const;

private:
    void DrawPlaceholderWindow(const char* title, const char* description, WindowType windowType);
    bool* GetWindowState(WindowType windowType);
    const bool* GetWindowState(WindowType windowType) const;

    bool showFiles = false;
    bool showMessages = false;
    bool showTaskManager = false;

    std::shared_ptr<TaskManager> taskManager;
};
