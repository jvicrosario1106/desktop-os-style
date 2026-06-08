// Dear ImGui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)

// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <string>
#include <sstream>
#include <iomanip>
#include <chrono>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <stdio.h>
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <imgui_impl_opengl3_loader.h>
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

// This example can also compile and run with Emscripten! See 'Makefile.emscripten' for details.
#ifdef __EMSCRIPTEN__
#include "../libs/emscripten/emscripten_mainloop_stub.h"
#endif


static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height)
{
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

std::string GetCurrentTimeString() {
    // Get current time point from system clock
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);

    // Convert to local time structure
    std::tm local_time;
#if defined(_MSC_VER)
    localtime_s(&local_time, &now_time); // Windows safe version
#else
    localtime_r(&now_time, &local_time); // POSIX/Linux safe version
#endif

    // Format string to HH:MM:SS (e.g., 14:30:05)
    std::stringstream ss;
    ss << std::setfill('0')
        << std::setw(2) << local_time.tm_hour << ":"
        << std::setw(2) << local_time.tm_min << ":"
        << std::setw(2) << local_time.tm_sec;

    return ss.str();
}

std::string GetCurrentDateString() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);

    std::tm local_time;
#if defined(_MSC_VER)
    localtime_s(&local_time, &now_time);
#else
    localtime_r(&now_time, &local_time);
#endif

    // Format string to MM/DD/YYYY (e.g., 06/08/2026)
    std::stringstream ss;
    ss << std::setfill('0')
        << std::setw(2) << (local_time.tm_mon + 1) << "/"
        << std::setw(2) << local_time.tm_mday << "/"
        << (local_time.tm_year + 1900);

    return ss.str();
}

// Main code
int main(int, char**)
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100 (WebGL 1.0)
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(IMGUI_IMPL_OPENGL_ES3)
    // GL ES 3.0 + GLSL 300 es (WebGL 2.0)
    const char* glsl_version = "#version 300 es";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    // Create window with graphics context
    float main_scale = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor()); // Valid on GLFW 3.3+ only
    GLFWwindow* window = glfwCreateWindow((int)(1280 * main_scale), (int)(800 * main_scale), "Dear ImGui GLFW+OpenGL3 example", nullptr, nullptr);
    if (window == nullptr)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup scaling
    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(main_scale);        // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
    style.FontScaleDpi = main_scale;        // Set initial font scale. (in docking branch: using io.ConfigDpiScaleFonts=true automatically overrides this for every window depending on the current monitor)

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
#ifdef __EMSCRIPTEN__
    ImGui_ImplGlfw_InstallEmscriptenCallbacks(window, "#canvas");
#endif
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Load Fonts
    // - If fonts are not explicitly loaded, Dear ImGui will select an embedded font: either AddFontDefaultVector() or AddFontDefaultBitmap().
    //   This selection is based on (style.FontSizeBase * style.FontScaleMain * style.FontScaleDpi) reaching a small threshold.
    // - You can load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - If a file cannot be loaded, AddFont functions will return a nullptr. Please handle those errors in your code (e.g. use an assertion, display an error and quit).
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use FreeType for higher quality font rendering.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    // - Our Emscripten build process allows embedding fonts to be accessible at runtime from the "fonts/" folder. See Makefile.emscripten for details.
    //style.FontSizeBase = 20.0f;
    //io.Fonts->AddFontDefaultVector();
    //io.Fonts->AddFontDefaultBitmap();
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf");
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf");
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf");
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf");
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf");
    //IM_ASSERT(font != nullptr);

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    bool show_desktop = true;
    bool show_taskbar = true;
    bool show_power_menu = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    GLuint desktop_texture = 0;
    int image_width = 0;
    int image_height = 0;

    // 1. Define the taskbar size
    float taskbar_height = 50.0f;
    float button_paddingY = 6.0f;
    float button_height = taskbar_height - button_paddingY;

    bool ret = LoadTextureFromFile("resources/MTGWallpaper.jpg", &desktop_texture, &image_width, &image_height);
    IM_ASSERT(ret);
    if (!ret) {
        // TODO: Put a default color here instead
        
    }

    // Main loop
#ifdef __EMSCRIPTEN__
    // For an Emscripten build we are disabling file-system access, so let's not attempt to do a fopen() of the imgui.ini file.
    // You may manually call LoadIniSettingsFromMemory() to load settings from your own storage.
    io.IniFilename = nullptr;
    EMSCRIPTEN_MAINLOOP_BEGIN
#else
    while (!glfwWindowShouldClose(window))
#endif
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();
        if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0)
        {
            ImGui_ImplGlfw_Sleep(10);
            continue;
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // --- DESKTOP BACKGROUND CONTENT ---

        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImVec2 wallpaper_size = ImVec2(viewport->WorkSize.x, viewport->WorkSize.y - taskbar_height);
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

        ImGui::Begin("Desktop", &show_desktop, desktop_flags);

        if (desktop_texture != 0)
        {
            // A. Image loaded successfully! Render the wallpaper texture
            ImTextureID tex_id = (ImTextureID)(intptr_t)desktop_texture;
            ImGui::Image(tex_id, wallpaper_size, ImVec2(0, 0), ImVec2(1, 1));
        }
        else
        {
            // B. Fallback: Draw a solid background color if the image wasn't found
            ImDrawList* draw_list = ImGui::GetWindowDrawList();
            ImVec2 min_pos = viewport->WorkPos;
            ImVec2 max_pos = ImVec2(min_pos.x + viewport->WorkSize.x, min_pos.y + viewport->WorkSize.y);
            draw_list->AddRectFilled(min_pos, max_pos, IM_COL32(34, 112, 147, 255));
        }

        ImGui::End();
        ImGui::PopStyleVar(3);
      
        // --- TASKBAR ---

        ImVec2 taskbar_pos = ImVec2(viewport->Pos.x, viewport->Pos.y + viewport->Size.y - taskbar_height);
        ImVec2 taskbar_size = ImVec2(viewport->Size.x, taskbar_height);

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

        ImGui::Begin("Taskbar", &show_taskbar, taskbar_flags);

        // --- TASKBAR CONTENT ---

        ImGui::SetCursorPosY((taskbar_height - button_height) * 0.5f);

        // Center the button vertically inside the taskbar
        ImGui::SetCursorPosY((taskbar_height - button_height) * 0.5f);

        // Define the full button dimensions (Width: 70px, Height: calculated dynamically)
        ImVec2 start_button_size = ImVec2(70.0f, button_height);

        if (ImGui::Button("Start", start_button_size)) {
            // Pop up power menu
            show_power_menu = !show_power_menu;
        }

        ImVec2 start_button_pos = ImGui::GetItemRectMin();

        // Right-align the clock text
        std::string clock_str = GetCurrentTimeString();
        std::string date_str = GetCurrentDateString();

        float clock_width = ImGui::CalcTextSize(clock_str.c_str()).x;
        float date_width = ImGui::CalcTextSize(date_str.c_str()).x;
        float largest_width = (clock_width > date_width) ? clock_width : date_width;

        // Move the cursor to the right side of the taskbar, minus a small margin
        ImGui::SameLine(ImGui::GetWindowWidth() - largest_width - 15.0f);

        // Center the text vertically within the 40px bar
        float stack_height = ImGui::GetTextLineHeightWithSpacing() * 2.0f;
        ImGui::SetCursorPosY((taskbar_height - stack_height) * 0.5f);

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

        if (show_power_menu) {
            // Define the size of your small popout menu
            ImVec2 menu_size = ImVec2(150.0f, 100.0f);

            // Position it directly above the Start button
            ImVec2 menu_pos = ImVec2(start_button_pos.x, start_button_pos.y - menu_size.y - 2.0f); // 2px gap

            ImGui::SetNextWindowPos(menu_pos);
            ImGui::SetNextWindowSize(menu_size);

            // Add flags to make it look like a clean context menu panel
            ImGuiWindowFlags menu_flags = ImGuiWindowFlags_NoTitleBar |
                ImGuiWindowFlags_NoResize |
                ImGuiWindowFlags_NoMove |
                ImGuiWindowFlags_NoCollapse |
                ImGuiWindowFlags_NoScrollbar;

            ImGui::Begin("PowerMenu", &show_power_menu, menu_flags);

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

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }
#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_MAINLOOP_END;
#endif

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
