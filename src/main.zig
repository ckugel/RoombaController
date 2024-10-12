const capy = @import("capy");
const std = @import("std");
const net = std.net;
const fs = std.fs;
const os = std.os;
pub usingnamespace capy.cross_platform;
const atomic = std.atomic;
const Atom = capy.Atom;

var gpa: std.heap.GeneralPurposeAllocator(.{}) = undefined;
pub const capy_allocator = gpa.allocator();

var corner_1 = Atom(f32).of(5);
var corner_2 = Atom(f32).of(5);
var corner_3 = Atom(f32).of(5);
var corner_4 = Atom(f32).of(5);

var objectAngle = Atom(f32).of(0);
var objectX = Atom(f32).of(0);
var objectY = Atom(f32).of(0);

var message = Atom(u8).of(0);

var angleSpin = Atom(f32).of(45);
var angleSpining = Atom(i16).of(45);

var logText = Atom([]const u8).of("");

var quitAll = Atom(bool).of(false);

const PORT = 288;

pub fn main() !void {
    gpa = .{};
    defer _ = gpa.deinit();

    defer corner_1.deinit();
    defer corner_2.deinit();
    defer corner_3.deinit();
    defer corner_4.deinit();

    try capy.init();
    defer capy.deinit();

    var window = try capy.Window.init();
    defer window.deinit();

    var somesliderValue = capy.Atom(f32).of(0);
    defer somesliderValue.deinit();

    const somesliderText = try capy.FormattedAtom(capy.internal.lasting_allocator, "{d:.1}", .{&somesliderValue});
    defer somesliderText.deinit();

    try window.set(capy.row(.{ .spacing = 0 }, .{
        capy.navigationSidebar(.{}),
        capy.tabs(.{
            capy.tab(.{ .label = "Basic Controls" }, mainPage()),
            capy.tab(.{ .label = "Graph" }, graph_page()),
            capy.tab(.{ .label = "Recieved data" }, raw_read_page()),
            // capy.tab(.{ .label = "Border Layout" }, BorderLayoutExample()),
            //           capy.tab(.{ .label = "Buttons" }, capy.column(.{}, .{
            //                // alignX = 0 means buttons should be aligned to the left
            //                // TODO: use constraint layout (when it's added) to make all buttons same width
            //                capy.alignment(.{ .x = 0 }, capy.button(.{ .label = "Button", .onclick = moveButton })),
            //                capy.button(.{ .label = "Button (disabled)", .enabled = false }),
            //                capy.checkBox(.{ .label = "Checked", .checked = true }), // TODO: dynamic label based on checked
            //                capy.checkBox(.{ .label = "Disabled", .enabled = false }),
            //                capy.row(.{}, .{
            //                    capy.expanded(capy.slider(.{ .min = -10, .max = 10, .step = 0.1 })
            //                        .bind("value", &somesliderValue)),
            //                    capy.label(.{})
            //                        .bind("text", somesliderText),
            //                }),
            //            })),
            //            capy.tab(.{ .label = "Rounded Rectangle" }, capy.column(.{}, .{
            //                capy.alignment(
            //                    .{},
            //                    capy.canvas(.{ .preferredSize = capy.Size.init(100, 100), .ondraw = drawRounded }),
            //                ),
            //                capy.row(.{}, .{
            //                    capy.expanded(capy.slider(.{ .min = 0, .max = 100, .step = 0.1 })
            //                        .bind("value", &corner_1)),
            //                    capy.expanded(capy.slider(.{ .min = 0, .max = 100, .step = 0.1 })
            //                        .bind("value", &corner_2)),
            //                }),
            //                capy.row(.{}, .{
            //                    capy.expanded(capy.slider(.{ .min = 0, .max = 100, .step = 0.1 })
            //                        .bind("value", &corner_3)),
            //                    capy.expanded(capy.slider(.{ .min = 0, .max = 100, .step = 0.1 })
            //                        .bind("value", &corner_4)),
            //                }),
            //            })),
            // capy.tab(.{ .label = "Drawing" }, capy.expanded(drawer(.{}))),
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
    return capy.column(.{}, .{
        capy.alignment(.{ .x = 0 }, capy.button(.{ .label = "Send angle", .onclick = sendAngleData })),
        capy.slider(.{ .min = -180, .max = 180, .step = 1 }).bind("value", &angleSpin),
        capy.label(.{}).bind("text", somesliderText),
        capy.alignment(.{ .x = 0 }, capy.button(.{ .label = "Forward", .onclick = sendW })),
        capy.alignment(.{ .x = 0 }, capy.button(.{ .label = "Backwards", .onclick = sendS })),
        capy.alignment(.{ .x = 0 }, capy.button(.{ .label = "Spin left", .onclick = sendA })),
        capy.alignment(.{ .x = 0 }, capy.button(.{ .label = "Spin right", .onclick = sendD })),
        capy.alignment(.{ .x = 0 }, capy.button(.{ .label = "Measure", .onclick = sendM })),
        capy.alignment(.{ .x = 0 }, capy.button(.{ .label = "Stop", .onclick = sendSpace })),
        capy.alignment(.{ .x = 0 }, capy.button(.{ .label = "Quit program", .onclick = sendQuit })),
    });
}

fn graph_page() anyerror!*capy.Container {
    return capy.column(.{}, .{
<<<<<<< HEAD
        capy.canvas(.{
            .preferredSize = capy.Size.init(800, 600),
            .ondraw = draw_objects_on_canvas,
        }),
=======
        // capy.alignment(.{}, capy.row(.{ .spacing = 10}, . {
        //    capy.button(. { .label = ""})
        // }))

>>>>>>> ce367c528e563a1898e3211b44f4a105f9626906
    });
}

fn draw_objects_on_canvas(cnv: *anyopaque, ctx: *capy.DrawContext) !void {
    _ = @as(*capy.Canvas, @ptrCast(@alignCast(cnv)));

    ctx.setColor(0.0, 1.0, 0.0); // Set color to green for the object

    const obj_x = objectX.get();
    const obj_y = objectY.get();
    const obj_radius = 30;  // Example radius, adjust according to data or input

    // Draw the object (circle) at x, y coordinates with radius
    ctx.drawCircle(obj_x, obj_y, obj_radius);
    ctx.fill();

    // You can also display other elements like angle or labels here
    ctx.setColor(1.0, 1.0, 1.0); // Set color to white for the text
    ctx.text(obj_x + obj_radius, obj_y, "Pillar", .{});
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
        //      capy.label(.{ .text = "TODO: cursor info" })
        //           .bind("text", label_text),
        // TODO: move into menu
    });

    // const resultText = try capy.FormattedAtom(capy.internal.lasting_allocator, "{d}", .{});
    // return capy.column(.{}, .{
    //    capy.textArea(.{.name = "" }).bind("text", &resultText),
    // });
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
    objectAngle.set(angle);
    objectX.set(x);
    objectY.set(y);
}

fn connect_tcp_writer() !void {
    // var server = net.Stream.init(.{});

    const Address = try net.Address.parseIp("192.168.1.1", PORT);
    var oldAngle: i16 = 45;
    var oldMessage: u8 = 0;

    const stream = try net.tcpConnectToAddress(Address);
    defer stream.close();

    //var listener = try Address.listen(.{
    //    .reuse_address = true,
    //    .kernel_backlog = 2048,
    //});
    //defer listener.deinit();

    // _ = try stream.write("w");

    while (true) {
        // if (listener.accept()) |conn| {
        // const client_addr = client.address;
        // const stream = client.stream;

        // var buffer: [256]u8 = undefined;

        // _ = try stream.read(&buffer);

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
        // } else |err| {
        //    std.log.err("failed to accept connection {}", .{err});
        //}
        //}
    }
}

fn drawRounded(cnv: *anyopaque, ctx: *capy.DrawContext) !void {
    const canvas = @as(*capy.Canvas, @ptrCast(@alignCast(cnv)));

    ctx.setColor(0.7, 0.9, 0.3);
    ctx.setLinearGradient(.{ .x0 = 80, .y0 = 0, .x1 = 100, .y1 = 100, .stops = &.{
        .{ .offset = 0.1, .color = capy.Color.yellow },
        .{ .offset = 0.8, .color = capy.Color.red },
    } });
    ctx.roundedRectangleEx(
        0,
        0,
        canvas.getWidth(),
        canvas.getHeight(),
        .{ corner_1.get(), corner_2.get(), corner_3.get(), corner_4.get() },
    );
    ctx.fill();
}

pub const Drawer = struct {
    pub usingnamespace capy.internal.All(Drawer);

    peer: ?capy.backend.Canvas = null,
    handlers: Drawer.Handlers = undefined,
    dataWrappers: Drawer.Atoms = .{},
    image: capy.ImageData,

    pub fn init() !Drawer {
        return Drawer.init_events(Drawer{
            .image = try capy.ImageData.new(1, 1, .RGB), // start with a 1x1 image
        });
    }

    pub fn onDraw(self: *Drawer, ctx: *capy.DrawContext) !void {
        const width = self.getWidth();
        const height = self.getHeight();
        ctx.image(0, 0, width, height, self.image);
    }

    pub fn onResize(self: *Drawer, size: capy.Size) !void {
        if (size.width > self.image.width or size.height > self.image.height) {
            self.image.deinit(); // delete old image
            self.image = try capy.ImageData.new(size.width, size.height, .RGB);
            @import("std").log.info("new image of size {}", .{size});
        }
    }

    pub fn show(self: *Drawer) !void {
        if (self.peer == null) {
            self.peer = try capy.backend.canvas.create();
            try self.show_events();
        }
    }

    pub fn getPreferredSize(self: *Drawer, _: capy.Size) capy.Size {
        return .{ .width = self.image.width, .height = self.image.height };
    }
};

pub fn drawer(config: Drawer.Config) !Drawer {
    _ = config;
    var lineGraph = try Drawer.init();
    try lineGraph.addDrawHandler(&Drawer.onDraw);
    try lineGraph.addResizeHandler(&Drawer.onResize);
    return lineGraph;
}

// You can simulate a border layout using only column, row and expanded
fn BorderLayoutExample() anyerror!*capy.Container {
    return capy.column(.{}, .{
        capy.label(.{ .text = "Top", .alignment = .Center }),
        capy.expanded(
            capy.row(.{}, .{
                capy.label(.{ .text = "Left", .alignment = .Center }),
                capy.expanded(
                    capy.label(.{ .text = "Center", .alignment = .Center }),
                ),
                capy.label(.{ .text = "Right", .alignment = .Center }),
            }),
        ),
        capy.label(.{ .text = "Bottom", .alignment = .Center }),
    });
}

fn moveButton(button_: *anyopaque) !void {
    const button = @as(*capy.Button, @ptrCast(@alignCast(button_)));
    const parent = button.getParent().?.as(capy.Alignment);

    const alignX = &parent.x;
    // Ensure the current animation is done before starting another
    if (!alignX.hasAnimation()) {
        if (alignX.get() == 0) { // if on the left
            alignX.animate(capy.Easings.InOut, 1, 1000);
        } else {
            alignX.animate(capy.Easings.InOut, 0, 1000);
        }
    }
}
