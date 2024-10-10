const capy = @import("capy");
const std = @import("std");

pub const LineGraph = struct {
    pub usingnamespace capy.internal.All(LineGraph);

    peer: ?capy.backend.Canvas = null,
    widget_data: LineGraph.WidgetData = .{},
    dataFn: capy.Atom(*const fn (x: f32) f32),

    pub fn init(config: LineGraph.Config) LineGraph {
        var line_graph = LineGraph.init_events(LineGraph{ .dataFn = capy.Atom(*const fn (x: f32) f32).of(config.dataFn) });
        line_graph.addDrawHandler(&LineGraph.draw) catch unreachable;
        return line_graph;
    }

    pub fn draw(self: *LineGraph, ctx: *capy.DrawContext) !void {
        const width = self.getWidth();
        const height = self.getHeight();
        ctx.setColor(1, 1, 1);
        ctx.rectangle(0, 0, width, height);
        ctx.fill();

        var x: f32 = 0;
        var minValue: f32 = 0;
        var maxValue: f32 = 0;
        while (x < 10) : (x += 0.1) {
            const value = self.dataFn.get()(x);
            maxValue = @max(maxValue, value);
            minValue = @min(minValue, value);
        }
        maxValue += maxValue / 10;
        minValue += minValue / 10;

        var legendValue: f32 = minValue;
        var legendBuf: [100]u8 = undefined; // the program can't handle a number that is 100 digits long so it's enough
        var legendLayout = capy.DrawContext.TextLayout.init();
        defer legendLayout.deinit();
        legendLayout.setFont(.{ .face = "Arial", .size = 12.0 });

        while (legendValue < maxValue) : (legendValue += (maxValue - minValue) / 10) {
            const y = @as(i32, @intCast(height)) - @as(i32, @intFromFloat(@floor((legendValue - minValue) * (@as(f32, @floatFromInt(height)) / (maxValue - minValue)))));
            const text = try std.fmt.bufPrint(&legendBuf, "{d:.1}", .{legendValue});

            ctx.setColor(0, 0, 0);
            ctx.text(0, y, legendLayout, text);
            ctx.line(0, y, @as(i32, @intCast(width)), y);
            ctx.stroke();
        }

        x = 0;
        var oldX: i32 = 0;
        var oldY: i32 = 0;
        while (x < 10) : (x += 0.1) {
            const y = self.dataFn.get()(x);
            var dy = @as(i32, @intCast(height)) - @as(i32, @intFromFloat(@floor((y - minValue) * (@as(f32, @floatFromInt(height)) / (maxValue - minValue)))));
            var dx = @as(i32, @intFromFloat(@floor(x * 100))) + 50;
            if (dy < 0) dy = 0;
            if (dx < 0) dx = 0;
            if (oldY == 0) oldY = dy;

            ctx.setColor(0, 0, 0);
            ctx.line(oldX, oldY, dx, dy);
            ctx.stroke();
            ctx.ellipse(oldX - 3, oldY - 3, 6, 6);
            ctx.fill();
            oldX = dx;
            oldY = dy;
        }
    }
}

