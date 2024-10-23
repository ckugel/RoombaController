const std = @import("std");
const builder = std.build.Builder;

pub fn build(b: *builder) void {
    const target = b.standardTargetOptions(.{});
    const mode = b.standardReleaseOptions();

    const exe = b.addExecutable(.{
        .name = "RoombaController",
        .root_source_file = .{ .path = "src/main.zig" },
        .target = target,
        .optimize = mode,
    });

    // Link to cimgui
    exe.linkSystemLibrary("cimgui");
    exe.linkSystemLibrary("imgui"); // Ensure that the main ImGui library is also linked

    // Set the necessary include directories
    exe.addIncludeDir("cimgui");

    // Add any required flags, e.g., for graphics backend (GLFW, OpenGL, etc.)
    exe.addPackagePath("cimgui", "include/Zig-ImGui/cimgui.zig");

    exe.install();
}
