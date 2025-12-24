#include "DebugInfos.h"

#include "Scene.h"

#include <GLFW/glfw3.h>
#include <tuple>

#include "../deps/imgui/imgui.h"

#include "../deps/imgui/backends/imgui_impl_glfw.h"
#include "../deps/imgui/backends/imgui_impl_opengl3.h"
#include "../deps/implot/implot.h"
#include <spdlog/spdlog.h>

DebugInfos::DebugInfos(GLFWwindow *window)
    : lastFrameTime((float) glfwGetTime()),
      framerates_times(NB_VALUES_FRAMERATE, lastFrameTime),
      framerates_values(NB_VALUES_FRAMERATE, 0), show_fr_plot(false) {

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
  ImPlot::CreateContext();
}

DebugInfos::~DebugInfos() {}

void DebugInfos::initFrame() {
  float time = (float) glfwGetTime();
  framerate = 1.f / (time - lastFrameTime);
  if (time - time_last_framerate > FRAMERATE_TIME_INTERVAL && show_fr_plot) {

    framerates_values.push_back(framerate);
    framerates_times.push_back(time);

    framerates_times.erase(framerates_times.begin());
    framerates_values.erase(framerates_values.begin());
    time_last_framerate = time;
  }
  lastFrameTime = time;
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

void DebugInfos::drawFrame(Scene &scene) {
  ImGuiStyle &style = ImGui::GetStyle();

  constexpr float title_size = 1.5f;

  ImGui::PushFont(NULL, style.FontSizeBase * title_size);
  ImGui::Text("General");
  ImGui::PopFont();
  ImGui::Text("FPS : %lf", framerate);

  spdlog::debug("start : {}, end : {}", framerates_times[0],
                framerates_times[framerates_times.size() - 1]);
  if (ImPlot::BeginPlot("Line Plots")) {
    ImPlot::SetupAxes("x", "y", ImPlotAxisFlags_AutoFit,
                      ImPlotAxisFlags_AutoFit);
    spdlog::debug("start : {}, end : {}", framerates_times[0],
                  framerates_times[framerates_times.size() - 1]);
    ImPlot::SetupAxesLimits(framerates_times[0],
                            framerates_times[framerates_times.size() - 1], 0,
                            1000, 0);
    ImPlot::PlotLine("f(x)", framerates_times.data(), framerates_values.data(),
                     NB_VALUES_FRAMERATE);
    ImPlot::EndPlot();
  }

  ImGui::Separator();
  ImGui::PushFont(NULL, style.FontSizeBase * title_size);
  ImGui::Text("Camera");
  ImGui::PopFont();
  ImGui::Checkbox("Show fps plot", &show_fr_plot);
  if (show_fr_plot) {
    if (ImPlot::BeginPlot("Line Plots")) {
      ImPlot::SetupAxes("x", "y", ImPlotAxisFlags_AutoFit,
                        ImPlotAxisFlags_AutoFit);
      spdlog::debug("start : {}, end : {}", framerates_times[0],
                    framerates_times[framerates_times.size() - 1]);
      ImPlot::SetupAxesLimits(framerates_times[0],
                              framerates_times[framerates_times.size() - 1], 0,
                              1000, 0);
      ImPlot::PlotLine("f(x)", framerates_times.data(),
                       framerates_values.data(), NB_VALUES_FRAMERATE);
      ImPlot::EndPlot();
    }
  }

  ImGui::Separator();
  ImGui::PushFont(NULL, style.FontSizeBase * title_size);
  ImGui::Text("Camera");
  ImGui::PopFont();

  ImGui::Text(scene.getCamera().debug_pos().c_str());
  ImGui::Text(scene.getCamera().debug_direction().c_str());

  ImGui::Separator();

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void DebugInfos::terminate() {
  ImPlot::DestroyContext();
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}
