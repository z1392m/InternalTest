# InternalTest

Standalone Android Dear ImGui 1.91.6 rendering and touch test for an application under your control. This clean harness excludes game hooks, ESP and memory patching.

## Native-only CI

GitHub Actions builds the release ARM64 JNI shared library directly with CMake, Ninja and Android NDK r27c (API 24, static C++ runtime). The sole artifact is InternalTest-ARM64, containing libinternaltest.so. No Gradle or APK compilation is performed by CI.

The pinned upstream ImGui source is fetched by CMake. The native library requires the matching MainActivity JNI interface (init, resize, frame, touch) and an active OpenGL ES 3 context. Loading the library alone does not create an overlay. The existing standalone app sources remain available for local testing.
