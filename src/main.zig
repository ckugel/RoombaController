const std = @import("std");
const net = std.net;
const fs = std.fs;
const os = std.os;
const atomic = std.atomic;
const posix = std.posix;


var gpa: std.heap.GeneralPurposeAllocator(.{}) = undefined;
var ogpa = std.heap.GeneralPurposeAllocator(.{}){};
const g_allocator = ogpa.allocator();
const Mutex = std.Thread.Mutex;
const Allocator = std.mem.Allocator;

// var objectRadius = Atom([10]f32);
// var objectX = Atom([10]f32);
// var objectY = Atom([10]f32);

pub const ThreadSafeArrayList = struct {
    const Self = @This();
    const default_alignment = @alignOf(Self);

    pub fn init(comptime T: type, allocator: *Allocator) !ThreadSafeArrayList(T) {
        return ThreadSafeArrayList(T){
            .list = try std.ArrayList(T).init(allocator),
            .mutex = Mutex.init(),
        };
    }

    pub fn deinit(self: *Self) void {
        self.mutex.deinit();
        self.list.deinit();
    }

    pub fn add(self: *Self, item: @field(self.list, "T")) !void {
        self.mutex.lock();
        defer self.mutex.unlock();

        try self.list.append(item);
    }
};

pub fn createThreadSafeArrayList(comptime T: type) type {
    return struct {
        list: std.ArrayList(T),
        mutex: Mutex,

        pub fn init(allocator: *Allocator) !ThreadSafeArrayList(T) {
            return ThreadSafeArrayList(T){
                .list = try std.ArrayList(T).init(allocator),
                .mutex = Mutex.init(),
            };
        }

        pub fn deinit(self: *@This()) void {
            self.mutex.deinit();
            self.list.deinit();
        }

        pub fn add(self: *@This(), item: T) !void {
            self.mutex.lock();
            defer self.mutex.unlock();

            try self.list.append(item);
        }
    };
}


var objectsRadi = ThreadSafeArrayList(f32);
var objectsX = ThreadSafeArrayList(f32);
var objectsY = ThreadSafeArrayList(f32);

const stdout = std.io.getStdOut().writer();

var message = atomic.Value(u8).init(0);

var angleSpin = atomic.Value(f32).init(45);
var angleSpining = atomic.Value(i16).init(45);

var logText = ThreadSafeArrayList(f32);
var logRawText = atomic.Value([]const u8).init("");

var quitAll = atomic.Value(bool).init(false);

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

        // std.debug.print("HUHHHHHHHHHHH\n\n\n\n", .{});
        var buffer: [256]u8 = undefined;
        const len = try stream.read(&buffer);
        const data = buffer[0..len];
        
        // Parse the data stream
        var ticker: u64 = 0;
        var nextLen: u64 = data.len;
        while (ticker < data.len) {
            if (data[ticker] == 'o') {
                // start reading routine
                // find upper bound of this, should occur when 3 spaces have been read
                var counter: u64 = 0;
                var temp: u64 = 0;
                while (counter < 3 and temp < data.len) {
                    if (data[temp] == ' ') {
                        counter += 1;
                    }
                    temp += 1;
                }
                nextLen = counter;
                
                var angle: f32 = 0;
                var x_coord: f32 = 0;
                var y_coord: f32 = 0;
                try parse_stream_data(data[ticker..nextLen], &angle, &x_coord, &y_coord);

                // Store the parsed data in a shared atom for UI rendering
                try update_object_position(angle, x_coord, y_coord);
                
                ticker = nextLen - 1;
            }
            // else if // handle other data types


            ticker += 1;
        }
        
        if (data.len > 0) {
            const msg = logRawText.get();

            _ = try stdout.print("\n\nMessage: {s}\n\n", .{data[0..len]});
            const result = try concat(msg, buffer[0..len]);
            defer allocator.free(result);
            logRawText.set(result);
        }

    }
}


fn handle_read(stream: Stream) !*void {
    var buffer: [256]u8 = undefined;
        const len = try stream.read(&buffer);
        const data = buffer[0..len];

        // Parse the data stream
        var ticker: u64 = 0;
        var nextLen: u64 = data.len;
        while (ticker < data.len) {
            if (data[ticker] == 'o') {
                // start reading routine
                // find upper bound of this, should occur when 3 spaces have been read
                var counter: u64 = 0;
                var temp: u64 = 0;
                while (counter < 3 and temp < data.len) {
                    if (data[temp] == ' ') {
                        counter += 1;
                    }
                    temp += 1;
                }
                nextLen = counter;
                
                var angle: f32 = 0;
                var x_coord: f32 = 0;
                var y_coord: f32 = 0;
                try parse_stream_data(data[ticker..nextLen], &angle, &x_coord, &y_coord);

                // Store the parsed data in a shared atom for UI rendering
                try update_object_position(angle, x_coord, y_coord);
                
                ticker = nextLen - 1;
            }
            // else if // handle other data types


            ticker += 1;
        }
        
        if (data.len > 0) {
            const msg = logRawText.get();

            _ = try stdout.print("\n\nMessage: {s}\n\n", .{data[0..len]});
            const result = try concat(msg, buffer[0..len]);
            defer allocator.free(result);
            logRawText.set(result);
        }
}

fn concat(a: []const u8, b: []const u8) ![]u8 {
   const result = try allocator.alloc(u8, a.len + b.len);
    @memcpy(result[0..a.len], a);
    @memcpy(result[a.len..], b);
    return result;
}

fn parse_stream_data(data: []const u8, angle: *f32, x: *f32, y: *f32) !void {
    std.debug.print("\nTried to parse: {s}\n", .{data});
    var tokenizer = std.mem.tokenize(u8, data, " "); // Tokenize data by space
    const x_str = tokenizer.next();
    const y_str = tokenizer.next(); 
    const angle_str = tokenizer.next();
    
    // Convert the string segments to floating-point values
    angle.* = try std.fmt.parseFloat(f32, angle_str orelse "10");
    x.* = try std.fmt.parseFloat(f32, x_str orelse "10");
    y.* = try std.fmt.parseFloat(f32, y_str orelse "10");
}

fn update_object_position(angle: f32, x: f32, y: f32) !void {
    // Update the object position (using Atom to share between threads)
    _ = try objectsRadi.append(angle);
    _ = try objectsX.append(x);
    _ = try objectsY.append(y);
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
            
            if (oldMessage == 'm'){
                // if we sent an m we need to handle updating the graph
                handle_read(stream);
            }

        }

        if (angleSpining.get() != oldAngle) {
            oldAngle = angleSpining.get();
            _ = try stream.write("t");
            var angleWrite: i16 = oldAngle;
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
