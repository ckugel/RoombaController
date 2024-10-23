const std = @import("std");

pub fn build(b: *std.build.Builder) void {
    const exe = b.addExecutable("zig-cimgui-example", "src/main.zig");
    exe.setTarget(b.standardTargetOptions(.{}));
    exe.setBuildMode(b.standardReleaseOptions());

    // Link to cimgui
    exe.linkSystemLibrary("cimgui");
    exe.linkSystemLibrary("imgui"); // Ensure that the main ImGui library is also linked

    // Set the necessary include directories
    exe.addIncludeDir("cimgui");

    // Add any required flags, e.g., for graphics backend (GLFW, OpenGL, etc.)
    exe.addPackagePath("cimgui", "cimgui/cimgui.zig");

    exe.install();
}



