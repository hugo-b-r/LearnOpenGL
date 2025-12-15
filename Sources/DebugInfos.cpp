#include "DebugInfos.h"

#include "Scene.h"


#include <GLFW/glfw3.h>

#include "../deps/imgui/imgui.h"

#include "../deps/imgui/backends/imgui_impl_glfw.h"
#include "../deps/imgui/backends/imgui_impl_opengl3.h"

DebugInfos::DebugInfos(GLFWwindow *window) : lastFrameTime (glfwGetTime()) {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  // Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  // Enable Gamepad Controls
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
  io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(
      window, true); // Second param install_callback=true will install GLFW
                     // callbacks and chain to existing ones.
  ImGui_ImplOpenGL3_Init();
}

DebugInfos::~DebugInfos() {

}

void DebugInfos::initFrame() {
    framerate = 1./(glfwGetTime() -lastFrameTime);
    lastFrameTime = glfwGetTime();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void DebugInfos::drawFrame(Scene& scene) {
    ImGuiStyle& style = ImGui::GetStyle();

    constexpr float title_size = 1.5f;

    ImGui::PushFont(NULL, style.FontSizeBase * title_size);
    ImGui::Text("General");
    ImGui::PopFont();
    ImGui::Text("FPS : %lf", framerate);

    ImGui::Separator();
    ImGui::PushFont(NULL, style.FontSizeBase * title_size);
    ImGui::Text("Camera");
    ImGui::PopFont();

    ImGui::Text(scene.getCamera().debug_pos().c_str());

    ImGui::Separator();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void DebugInfos::terminate() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
