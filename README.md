# RoboBand.Exp
## About

这是基于 C++23 的适用于 RoboMaster 比赛的通用机器人上位机控制程序. 它是一个用于验证想法的实验性程序, 并且尚未完成, 它并不是一个完善的比赛用程序, 但希望能对你有所启发. 

本项目的特色: 
- 使用更为现代的 C++ 风格编写
- 高代码复用率
- 分层模块化设计
- 修改配置文件中的 `info` 即可在不同机器人平台或设备组合间复用同一套代码
- 支持 Webots 仿真并可以做到仿真与实机代码的无缝切换
- 较为完备的异常处理
- 良好的兼容性
- 几乎无依赖

## Getting Started

该项目使用 xmake 作为构建工具, 利用 Eigen 库进行矩阵运算 (可由 xmake 自动安装). 除此以外无其他依赖. 此外, xmake 构建工具会自动检测当前环境是否安装了 Webots, 如果未安装, 与 Webots 相关的功能将不会被构建. 

使用该项目只需要克隆到本地, 然后使用 `xmake` 命令构建即可. 

## Usage

本项目的目录结构如下: 

```
.
├── xmake.lua
├── app
└── src
    ├── robo.mpp
    ├── support
    ├── io
    ├── device
    ├── ctrl
    └── robot
```

代码分为 `src` 和 `app` 两部分. `src` 是通用部分, 将会编译为静态链接库供 `app` 中的各个机器人实例使用. 

### `src`

`src` 部分使用了 C++20 modules, 每个子文件夹对应一个子接口模块. 

#### `support`

```
.
└── support
    ├── spt.mpp
    ├── spt.crc.mpp
    ├── spt.math.mpp
    ├── spt.misc.mpp
    ├── spt.multiton.mpp
    ├── spt.task_context.mpp
    └── spt.webots.mpp
```

首先是 `support` 文件夹, 对应 `spt` 子接口模块. `multiton` 为多例模式的基础设施, 供 `task_context` 和 `io` 等使用. `task_context` 是任务的运行线程. 下面是 `task_context` 的用法举例: 

```cpp
///////////////////////////////////////////
// task_context
//     type:
//         continuous
//         periodic
//     make:
//         task_context::<type>::info::make()    -> context_info
//         task_context::<type>::info::make<N>() -> std::array<context_info, N>
//         task_context::<type>::info::webots()  -> context_info_for_webots
//     usage:
//         task_context::add_task(info, time, func)
//         task_context::start()
//         task_context::finish()
//         task_context::finished() -> bool
//         task_context::wait_until_all_finished()
///////////////////////////////////////////
constexpr auto context = robo::task_context::periodic::info::make();

int main() {
    robo::task_context::add_task(context, 1s, [] {
        std::println("{:%T}: Link Start!", std::chrono::system_clock::now());
    });
    robo::task_context::start();
    robo::task_context::wait_until_all_finished();
}
```

在机器人配置阶段创建 `task_context`, 可安排各个 IO 轮询和控制任务的线程. 此外, 使用 Webots 的机器人将会自动配置为使用 Webots 的 `task_context`. 

#### `io`

```
.
└── io
    ├── io.mpp
    ├── io.base.mpp
    ├── io.parser.mpp
    └── io.serial.mpp
```

`io` 文件夹, 对应 `io` 子接口模块. `io` 分为 `bare_io` (如 serial) 和 `keyed_io` (如 CAN), 它们的区别是是否天然带有 key. 下面是其具体 concept 的区别: 

```cpp
///////////////////////////////////////////
// concepts
//     io_base  : info
//                io::get(info) -> impl
//                io::error_handler(io, const io::error)
//                io::on_error(io, error_handler)
//     bare_io  : io_base
//                io::on_data(io, callback)
//     keyed_io : io_base
//                io::on_data(io, key, callback)
//     io       : bare_io or keyed_io
//     /* `callback(std::span<const std::byte>)` */
//     /* `error_handler(io, const io::error)` */
///////////////////////////////////////////
```

对于用户, io 有如下用法:  

```cpp
enum class error {
    offline,
    invalid_data,
    callback_unregistered,
    read_failed,
    write_failed,
};

decltype(auto) get(auto&& io);
auto get_state(auto&& io);
void send(auto&& io, auto&&... args);
void on_data(auto&& io, auto&&... args);
void on_error(auto&& io, auto&&... args);
void error_handler(auto&& io, const io::error err);
```

其中, `on_data()` 用于注册回调函数, `on_error()` 用于注册错误处理函数. 

```cpp
///////////////////////////////////////////
// with_parser
//     io::with_parser<bare_parser...>::on_data(...)
//     io::with_parser<keyed_parser>::on_data(...)
//     io::with_parser<keyed_parser, bare_parser...>::on_data(...)
///////////////////////////////////////////
```

此外, `io` 还可以组合 `parser` 实现数据的筛选. `parser` 也分为 `bare_parser` 和 `keyed_parser`, `bare_parser` 实质上是在用户的回调函数上套了一层判断, 而 `keyed_parser` 则通过处理接收数据得到 key 并且调用对应的回调函数. 

以串口为例, 它是一个 `bare_io`, 可以有如下用法: 

```cpp
constexpr auto context = robo::task_context::continuous::info::make();
constexpr robo::io::Serial::Info serial {
    .name = "/dev/serial/by-id/usb-DM-IMU_DM-IMU_USB_CDC_2025021200-if00",
    .context = context,
    .rx_idle_duration = 50us,
    .rx_buffer_size = 19,
    .baud_rate = 4000000
};
template <auto N>
std::array key { 0x55_b, 0xaa_b, 0x01_b, std::byte{N} };

int main() {
    std::array<std::size_t, 3> receiver {};
    auto callback = [&](std::size_t id, auto) { receiver[id]++; };

    robo::io::on_error(serial, [&](auto&& io, auto err) {
        if (err == robo::io::error::invalid_data) {
            auto view = std::views::transform(robo::io::get(serial).get_rx_bytes(), 
                                              [](auto e) { return std::to_integer<unsigned>(e); });
            std::print("[{}] {:: 8x}\nreceived invalid data: {::02x}\n\n",
                         std::chrono::system_clock::now(), receiver, view);
        } else {
            robo::io::allow_no_callback(io, err); 
        }
    });

    using IoParser = robo::io::with_parser<
        robo::io::prefix<4>, 
        robo::io::head<0x55, 0xaa, 0x01>,
        robo::io::tail<0x0a>, 
        robo::io::length<19>, 
        robo::io::crc<robo::crc16::dm_imu, 0, 1>
    >;
    IoParser::on_data(serial, key<1>, [&](auto bytes) { callback(0, bytes); });
    IoParser::on_data(serial, key<2>, [&](auto bytes) { callback(1, bytes); });
    IoParser::on_data(serial, key<3>, [&](auto bytes) { callback(2, bytes); });

    std::jthread send_thread[3] { std::jthread{[] {
        while (true) {
            std::array<std::byte, 100> arr {};
            robo::io::send(serial, arr);
            std::this_thread::sleep_for(1ms);
        }
    }}};
    
    robo::task_context::start();
    robo::task_context::wait_until_all_finished();
}
```

#### `device`

```
.
└── device
    ├── dev.mpp
    ├── dev.base.mpp
    ├── imu
    │   ├── dev.imu.mpp
    │   ├── dev.imu.base.mpp
    │   ├── dev.imu.dm.mpp
    │   └── dev.imu.webots.mpp
    └── motor
        ├── dev.motor.mpp
        ├── dev.motor.base.mpp
        ├── dev.motor.unitree.mpp
        └── dev.motor.webots.mpp
```

`device` 文件夹, 对应 `dev` 子接口模块. 这里的代码是对各种设备的实现与抽象. 以 `motor` 为例, `robo::dev::motor` 是一个 concept, 而在 `robo::motor` 这个 namespace 下的设备的 `info` 的类型都满足 `robo::dev::motor` 这个 concept, 这样下一层级的代码在使用 `motor` 时只需要使用满足 `robo::dev::motor` 的模板类型, 而不必关心 `motor` 的具体类型. 下面是一个使用案例: 

```cpp
constexpr auto context = robo::task_context::continuous::info::make();
constexpr robo::io::Serial::Info serial {
    .name = "/dev/serial/by-id/usb-FTDI_USB__-__Serial_Converter_FTAK92PX-if00-port0",
    .context = context,
    .rx_idle_duration = 50us,
    .rx_buffer_size = robo::motor::Unitree::rx_size,
    .baud_rate = 4000000
};
constexpr robo::motor::Unitree::Info motor {{
    .io = serial,
    .id = 0_b,
    .type = robo::motor::Unitree::type::go_m8010_6,
    .dir = robo::motor::dir::forward
}};

template <robo::dev::motor M>
struct Snail {
    M motor_info;
    float step;
    M::dev_type motor { motor_info };
    float torque {};
    void run() {
        if (motor.speed() == 0.0f) torque += step;
        motor.setTorque(torque);
    }
};

int main() {
    Snail s { motor, 0.0001f };
    robo::task_context::start();
    std::jthread th {[&] {
        s.run(); 
        std::this_thread::sleep_for(1ms);
    }};
    robo::task_context::wait_until_all_finished();
}
```

#### `ctrl`

```
.
└── ctrl
    ├── ctrl.mpp
    ├── ctrl.basic.mpp
    └── chassis
        ├── ctrl.chassis.wheel_leg.mpp
        └── leg
            ├── ctrl.leg.five_bar.mpp
            └── ctrl.leg.mpp
```

`ctrl` 文件夹, 对应 `ctrl` 子接口模块. 主体部分为对控制算法的实现, 与底层硬件无关. 这是 `ctrl` 对应的 `info` 借助 `struct` 的 deduction guide 实现的. 而针对特定硬件的代码则可以通过特化完成. 需要特化的主要是硬件初始化的部分. 

现有的代码实现了对平衡底盘的抽象, 其中 `ctrl::Leg` 是对腿的抽象, 后续可以方便地加入串联腿之类. 下面是 `robo::ctrl::WheelLegInfo` 的定义: 

```cpp
template <dev::imu Imu, ctrl::leg Leg, dev::motor M>
struct WheelLegInfo {
    using ctrl_type = WheelLeg<Imu, Leg, M>;

    spt::task_context::periodic::info context;
    std::chrono::nanoseconds ctrl_period;

    Imu imu;
    Leg leg_left;
    Leg leg_right;
    M wheel_left;
    M wheel_right;

    float f_gravity;
    float wheel_radius;

    float s_ref_max;
    float phi_ref_max;
    float theta_l_ref_max;
    float theta_b_ref_max;
    
    std::array<float, 40> K;
    ctrl::pid::param pid_l;
    ctrl::pid::param pid_psi;
};
```

#### `robot`

```
.
└── robot
    ├── bot.mpp
    └── bot.wheel_leg.mpp
```

`robot` 文件夹, 对应 `bot` 子接口模块. 这是机器人实例对应的类, 在这里将多个 `ctrl` 和其他组件一起组合起来组成一个可适配不同硬件实现的机器人模板类. 在这个模板类中执行 `task_context::run()` 等, 它本身只暴露出 `run()` 这一个接口. 下面是 `robo::bot::WheelLegInfo` 的定义.

```cpp
template <typename WL>
struct WheelLegInfo {
    WL wheel_leg;
};
```

### `app`

在这个文件夹中, 我们对机器人进行实例化, 即确定其所使用的硬件等. 其子目录与 `src/robot` 相对应. 每一个 `.cc` 文件都是一个机器人的实例, 它的结构是先按层级依次列举所有配置, 包括 `context`, `io`, `device`, `ctrl`, `robot`, 所有这些配置都必须加上 `constexpr` 限定. 之后在 `main()` 函数中实例化机器人并运行. 下面是一个 Webots 轮腿机器人的例子 (使用 Webots 时不需要手动配置 `context`): 

```cpp
import std;
import robo;
using namespace std::chrono_literals;

///////////////////////////////////////////
// device
///////////////////////////////////////////
constexpr robo::imu::Webots::Info imu {
    .inertial_unit_name = "inertial unit",
    .gyro_name = "gyro",
    .accelerometer_name = "accelerometer",
};

constexpr robo::motor::Webots::Info wheel_l {
    .name = "motor5",
    .dir = robo::motor::dir::forward,
};
constexpr robo::motor::Webots::Info wheel_r {
    .name = "motor6",
    .dir = robo::motor::dir::forward,
};
constexpr robo::motor::Webots::Info joint_l2 {
    .name = "motor1",
    .dir = robo::motor::dir::reverse,
};
constexpr robo::motor::Webots::Info joint_l1 {
    .name = "motor3",
    .dir = robo::motor::dir::forward,
};
constexpr robo::motor::Webots::Info joint_r2 {
    .name = "motor2",
    .dir = robo::motor::dir::reverse,
};
constexpr robo::motor::Webots::Info joint_r1 {
    .name = "motor4",
    .dir = robo::motor::dir::forward,
};

///////////////////////////////////////////
// ctrl
///////////////////////////////////////////
constexpr robo::ctrl::FiveBarInfo leg_l {
    .motor1 = joint_l1,
    .motor2 = joint_l2,
    .l_a = 0.06f,
    .l_b = 0.2f,
    .l_c = 0.3f,
};
constexpr robo::ctrl::FiveBarInfo leg_r {
    .motor1 = joint_r1,
    .motor2 = joint_r2,
    .l_a = 0.06f,
    .l_b = 0.2f,
    .l_c = 0.3f,
};

constexpr robo::chassis::WheelLegInfo wheel_leg {
    .ctrl_period = 1ms,

    .imu = imu,
    .leg_left = leg_l,
    .leg_right = leg_r,
    .wheel_left = wheel_l,
    .wheel_right = wheel_r,

    .f_gravity = 33.0f,
    .wheel_radius = 0.10f,

    .s_ref_max = 0.3f,
    .phi_ref_max = 0.3f,
    .theta_l_ref_max = 0.7f,
    .theta_b_ref_max = 0.1f,

    .K = {
        -4.897f, -6.659f, -4.447f, -0.660f, -25.649f, -2.170f, -3.080f, -0.751f, -10.089f, -0.863f,
        -4.897f, -6.659f, 4.447f, 0.660f, -3.080f, -0.751f, -25.649f, -2.170f, -10.089f, -0.863f,
        1.009f, 1.329f, -15.173f, -2.479f, 9.472f, 0.498f, -4.430f, -0.018f, -48.971f, -2.084f,
        1.009f, 1.329f, 15.173f, 2.479f, -4.430f, -0.018f, 9.472f, 0.498f, -48.971f, -2.084f
    },
    .pid_l = {
        .kp = 400.0f,
        .ki = 0.15f,
        .kd = 100000.0f,
        .max_iout = 5.0f,
        .max_out = 20.0f,
    },
    .pid_psi = {
        .kp = 3000.0f,
        .ki = 0.05f,
        .kd = 1600.0f,
        .max_iout = 10.0f,
        .max_out = 20.0f,
    },
};

///////////////////////////////////////////
// robot
///////////////////////////////////////////
constexpr robo::bot::WheelLegInfo info {
    .wheel_leg = wheel_leg,
};

///////////////////////////////////////////
// main
///////////////////////////////////////////
int main() {
    robo::bot::WheelLeg robot { info };
    robot.run();
}
```

## Roadmap

- [ ] `dev` 实现为 `multiton` (达到多个 `ctrl` 使用一个 device 的目的)
- [ ] 添加 log 系统
- [ ] ...

