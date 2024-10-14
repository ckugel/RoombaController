const capy = @import("capy");
const std = @import("std");
const net = std.net;
const fs = std.fs;
const os = std.os;
pub usingnamespace capy.cross_platform;
const atomic = std.atomic;
const Atom = capy.Atom;
const ListAtom = capy.ListAtom;

var gpa: std.heap.GeneralPurposeAllocator(.{}) = undefined;
pub const capy_allocator = gpa.allocator();

// var objectRadius = Atom([10]f32);
// var objectX = Atom([10]f32);
// var objectY = Atom([10]f32);

var objectsRadi = ListAtom(f32).init(capy_allocator);
var objectsX = ListAtom(f32).init(capy_allocator);
var objectsY = ListAtom(f32).init(capy_allocator);

var message = Atom(u8).of(0);

var angleSpin = Atom(f32).of(45);
var angleSpining = Atom(i16).of(45);

var logText = Atom([]const u8).of("");

var quitAll = Atom(bool).of(false);

const PORT = 288;

const PolarPoint = struct {
    angle: f32,
    magnitude: f32,
    radius: f32,
};

const CartesianPoint = struct {
    x: f32,
    y: f32,
    radius: f32,

    pub fn toPolar(self: *CartesianPoint) !PolarPoint {
        return PolarPoint{
            .angle = std.math.atan2(self.y, self.x),
            .magnitude = (std.math.pow(self.x, 2) + std.math.pow(self.y, 2)),
            .radius = self.radius,
        };
    }
};

const Graph = struct {
    data: std.ArrayList(CartesianPoint),
};

pub fn main() !void {
    gpa = .{};
    defer _ = gpa.deinit();

    try capy.init();
    defer capy.deinit();

    var window = try capy.Window.init();
    defer window.deinit();

    try window.set(capy.row(.{ .spacing = 0 }, .{
        capy.navigationSidebar(.{}),
        capy.tabs(.{
            capy.tab(.{ .label = "Basic Controls" }, mainPage()),
            capy.tab(.{ .label = "Graph" }, graph_page()),
            capy.tab(.{ .label = "Recieved data" }, raw_read_page()),
        }),
    }));

    window.show();

    // maybe spawn thread here for main event loop???
    // _ = try std.Thread.spawn(.{}, connect_tcp_writer, .{});
    // _ = try std.Thread.spawn(.{}, connect_tcp_reader, .{});

    // _ = try connectViaTCP();

    capy.runEventLoop();
    std.log.info("Goodbye!", .{});
    quitAll.set(true);
}

fn mainPage() anyerror!*capy.Container {
    const somesliderText = try capy.FormattedAtom(capy.internal.lasting_allocator, "{d:1}", .{&angleSpin});
    return capy.column(.{}, .{ capy.row(.{}, .{
        capy.expanded(capy.slider(.{ .min = -180, .max = 180, .step = 1 }).bind("value", &angleSpin)),
    }), capy.row(.{}, .{
        capy.label(.{}).bind("text", somesliderText),
    }), capy.row(.{}, .{
        capy.alignment(.{}, capy.button(.{ .label = "Send angle", .onclick = sendAngleData })),
        capy.alignment(.{}, capy.button(.{ .label = "Forward", .onclick = sendW })),
        capy.alignment(.{}, capy.button(.{ .label = "Backwards", .onclick = sendS })),
        capy.alignment(.{}, capy.button(.{ .label = "Spin left", .onclick = sendA })),
        capy.alignment(.{}, capy.button(.{ .label = "Spin right", .onclick = sendD })),
        capy.alignment(.{}, capy.button(.{ .label = "Scan", .onclick = sendM })),
        capy.alignment(.{}, capy.button(.{ .label = "Stop", .onclick = sendSpace })),
        capy.alignment(.{}, capy.button(.{ .label = "Quit program", .onclick = sendQuit })),
    }) });
}

fn graph_page() anyerror!*capy.Container {
    return capy.column(.{}, .{
        capy.canvas(.{
            .preferredSize = capy.Size.init(800, 600),
            .ondraw = draw_objects_on_canvas,
        }),
    });
}

fn draw_objects_on_canvas(cnv: *anyopaque, ctx: *capy.DrawContext) !void {
    const canvas = @as(*capy.Canvas, @ptrCast(@alignCast(cnv)));

    ctx.setColor(0.0, 1.0, 0.0);

    const length = objectsX.getLength(); // assumes that objects X and Y have the same length (they should)

    for (0..length) |i| {
        const obj_x: i32 = @intFromFloat(objectsX.get(i));
        const obj_y: i32 = @intFromFloat(objectsY.get(i));
        const obj_radius: u32 = @intFromFloat(objectsRadi.get(i));

        // Draw the object (circle) at x, y coordinates with radius
        ctx.ellipse(
            obj_x,
            obj_y,
            obj_radius,
            obj_radius,
        );
    }

    _ = canvas;

    ctx.fill();
}

fn raw_read_page() anyerror!*capy.Container {
    //   const text_length = try capy.Atom(usize).derived(.{&text}, &struct {
    //       fn callback(txt: []const u8) usize {
    //           return txt.len;
    //       }
    //   }.callback);

    //    var label_text = try capy.FormattedAtom(capy.internal.lasting_allocator, "Text length: {d}", .{text_length});
    //    defer label_text.deinit();

    return capy.column(.{ .spacing = 0 }, .{
        capy.expanded(capy.textArea(.{})
            .bind("text", &logText)),
    });
}

fn sendW(_: *anyopaque) !void {
    message.set('w');
}

fn sendA(_: *anyopaque) !void {
    message.set('a');
}

fn sendS(_: *anyopaque) !void {
    message.set('s');
}

fn sendD(_: *anyopaque) !void {
    message.set('d');
}

fn sendSpace(_: *anyopaque) !void {
    message.set(' ');
}

fn sendM(_: *anyopaque) !void {
    message.set('m');
}

fn sendQuit(_: *anyopaque) !void {
    message.set('q');
}

fn sendAngleData(_: *anyopaque) !void {
    angleSpining.set(@intFromFloat(angleSpin.get()));
}

fn connect_tcp_reader() !void {
    const Address = try net.Address.parseIp("192.168.1.1", PORT);
    const stream = try net.tcpConnectToAddress(Address);
    defer stream.close();

    while (true) {
        if (quitAll.get()) {
            break;
        }
        var buffer: [256]u8 = undefined;
        const len = try stream.read(&buffer);
        const data = buffer[0..len];

        // Parse the data stream
        if (data.len > 0 and data[0] == 'o') {
            var angle: f32 = 0;
            var x_coord: f32 = 0;
            var y_coord: f32 = 0;
            try parse_stream_data(data, &angle, &x_coord, &y_coord);

            // Store the parsed data in a shared atom for UI rendering
            try update_object_position(angle, x_coord, y_coord);
        }

        logText.set(logText.get() ++ .{buffer[0..len]});
    }
}

fn parse_stream_data(data: []const u8, angle: *f32, x: *f32, y: *f32) !void {
    var tokenizer = std.mem.tokenize(data, " "); // Tokenize data by space
    const angle_str = try tokenizer.nextExpected() catch return error.InvalidData;
    const x_str = try tokenizer.nextExpected() catch return error.InvalidData;
    const y_str = try tokenizer.nextExpected() catch return error.InvalidData;

    // Convert the string segments to floating-point values
    (*angle) = try std.fmt.parseFloat(f32, angle_str, 10);
    (*x) = try std.fmt.parseFloat(f32, x_str, 10);
    (*y) = try std.fmt.parseFloat(f32, y_str, 10);
}

fn update_object_position(angle: f32, x: f32, y: f32) !void {
    // Update the object position (using Atom to share between threads)
    objectsRadi.append(angle);
    objectsX.append(x);
    objectsY.append(y);
}

fn connect_tcp_writer() !void {
    // var server = net.Stream.init(.{});

    const Address = try net.Address.parseIp("192.168.1.1", PORT);
    var oldAngle: i16 = 45;
    var oldMessage: u8 = 0;

    const stream = try net.tcpConnectToAddress(Address);
    defer stream.close();

    while (true) {
        if (quitAll.get()) {
            _ = try stream.write("q");
            break;
        }

        if (oldMessage != message.get()) {
            oldMessage = message.get();
            const my_array: []const u8 = &[_]u8{oldMessage}; // Create a constant array with one element

            _ = try stream.write(my_array);
        }

        if (angleSpining.get() != oldAngle) {
            oldAngle = angleSpining.get();
            _ = try stream.write("t");
            var angleWrite: f32 = oldAngle;
            if (oldAngle < 0) {
                _ = try stream.write("-");
                angleWrite = oldAngle * -1;
            }
            for (0..2) |i| {
                const value: u8 = @intCast(@rem(@divTrunc(angleWrite, (std.math.pow(i16, 10, @intCast(i)))), 10));
                std.debug.print("Huh value: {}", .{value});
                const my_message: []const u8 = &[_]u8{value};
                _ = try stream.write(my_message);
            }
            _ = try stream.write("\n");
        }
    }
}
