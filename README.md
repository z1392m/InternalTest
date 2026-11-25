# InternalTest

Standalone Android Dear ImGui 1.91.6 rendering and touch test for an application under your control. This is a clean test harness based on the ImGui version in the supplied archive; it is not a full source import. It excludes game hooks, ESP, memory patching, generated build files, and prebuilt binaries.

Build with Java 17, Gradle 8.9, Android SDK 35, NDK 27.2.12479018 and CMake 3.22.1. The pinned upstream ImGui source is fetched by CMake. Run `gradle :app:assembleDebug` or the GitHub Actions workflow.

Install the debug APK and test title bar dragging, sliders, checkboxes, rotation and pause/resume. The native library is designed to be loaded by this app's own JNI activity.
