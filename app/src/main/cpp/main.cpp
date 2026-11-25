#include <jni.h>
#include <GLES3/gl3.h>
#include <chrono>
#include <algorithm>
#include "imgui.h"
#include "imgui_impl_opengl3.h"

namespace {
int width = 0, height = 0, taps = 0;
bool ready = false, checked = false, show_demo = false;
float value = 0.5f;
auto previous = std::chrono::steady_clock::now();
}

extern "C" JNIEXPORT void JNICALL
Java_com_internaltest_demo_MainActivity_init(JNIEnv*, jclass) {
    if (ready) {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui::DestroyContext();
        ready = false;
    }
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    auto& io = ImGui::GetIO();
    io.IniFilename = nullptr;
    io.Fonts->AddFontDefault();
    io.FontGlobalScale = 1.6f;
    ImGui::GetStyle().ScaleAllSizes(1.6f);
    ready = ImGui_ImplOpenGL3_Init("#version 300 es");
    if (!ready) ImGui::DestroyContext();
    previous = std::chrono::steady_clock::now();
}

extern "C" JNIEXPORT void JNICALL
Java_com_internaltest_demo_MainActivity_resize(JNIEnv*, jclass, jint w, jint h) {
    width = w; height = h;
}

extern "C" JNIEXPORT void JNICALL
Java_com_internaltest_demo_MainActivity_touch(JNIEnv*, jclass, jfloat x, jfloat y, jint action) {
    if (!ready) return;
    auto& io = ImGui::GetIO();
    io.AddMouseSourceEvent(ImGuiMouseSource_TouchScreen);
    if (action != 3) io.AddMousePosEvent(x, y);
    if (action == 0) { ++taps; io.AddMouseButtonEvent(0, true); }
    if (action == 1 || action == 3) io.AddMouseButtonEvent(0, false);
}

extern "C" JNIEXPORT void JNICALL
Java_com_internaltest_demo_MainActivity_frame(JNIEnv*, jclass) {
    if (!ready || width <= 0 || height <= 0) return;
    auto now = std::chrono::steady_clock::now();
    auto& io = ImGui::GetIO();
    io.DeltaTime = std::clamp(std::chrono::duration<float>(now - previous).count(), 0.001f, 0.1f);
    previous = now;
    io.DisplaySize = ImVec2(float(width), float(height));
    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();
    ImGui::SetNextWindowPos(ImVec2(20, 40), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(std::min(600.f, width - 40.f), 370), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("InternalTest - Touch & rendering")) {
        ImGui::Text("Dear ImGui %s | %.1f FPS", IMGUI_VERSION, io.Framerate);
        ImGui::Text("Touch downs: %d", taps);
        ImGui::Text("Touch position: %.1f, %.1f", io.MousePos.x, io.MousePos.y);
        ImGui::Checkbox("Test checkbox", &checked);
        ImGui::SliderFloat("Test slider", &value, 0.0f, 1.0f);
        if (ImGui::Button("Reset")) { checked = false; value = 0.5f; taps = 0; }
        ImGui::Checkbox("Show demo window", &show_demo);
        ImGui::TextWrapped("Drag the title bar, adjust the slider, and tap controls.");
    }
    ImGui::End();
    if (show_demo) ImGui::ShowDemoWindow(&show_demo);
    ImGui::Render();
    glViewport(0, 0, width, height);
    glClearColor(0.08f, 0.10f, 0.13f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
