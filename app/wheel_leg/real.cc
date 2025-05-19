import std;
import robo;
using namespace std::chrono_literals;
using namespace robo::spt::byte_literals;

///////////////////////////////////////////
// context
///////////////////////////////////////////
constexpr auto c_context = robo::task_context::continuous::info::make();
constexpr auto p_context = robo::task_context::periodic::info::make();

///////////////////////////////////////////
// io
///////////////////////////////////////////
constexpr robo::io::Serial::Info serial_imu {
    .name = "/dev/serial/by-id/usb-DM-IMU_DM-IMU_USB_CDC_2025021200-if00",
    .context = c_context,
    .rx_idle_duration = 50us,
    .rx_buffer_size = robo::imu::Dm::rx_size,
    .baud_rate = 4000000,
};
constexpr robo::io::Serial::Info serial_motor_0_1 {
    .name = "/dev/serial/by-id/usb-1a86_USB_Single_Serial_58ED020672-if00",
    .context = c_context,
    .rx_idle_duration = 50us,
    .rx_buffer_size = robo::motor::Unitree::rx_size,
    .baud_rate = 4000000,
};
constexpr robo::io::Serial::Info serial_motor_2_3 {
    .name = "/dev/serial/by-id/usb-1a86_USB_Single_Serial_594D003385-if00",
    .context = c_context,
    .rx_idle_duration = 50us,
    .rx_buffer_size = robo::motor::Unitree::rx_size,
    .baud_rate = 4000000,
};
constexpr robo::io::Serial::Info serial_motor_4_5 {
    .name = "/dev/serial/by-id/usb-1a86_USB_Single_Serial_594D003319-if00",
    .context = c_context,
    .rx_idle_duration = 50us,
    .rx_buffer_size = robo::motor::Unitree::rx_size,
    .baud_rate = 4000000,
};

///////////////////////////////////////////
// device
///////////////////////////////////////////
constexpr robo::input::Gamepad::Info gamepad {
    .name = "/dev/input/js0",
    .context = c_context,
};

constexpr robo::imu::Dm::Info imu {
    .io = serial_imu,
    .offline_timeout = 10ms,
};

constexpr robo::motor::Unitree::Info wheel_l {{
    .io = serial_motor_0_1,
    .offline_timeout = 10ms,
    .id = 0_b,
    .type = robo::motor::Unitree::type::go_m8010_6,
    .dir = robo::motor::dir::forward,
}};
constexpr robo::motor::Unitree::Info wheel_r {{
    .io = serial_motor_0_1,
    .offline_timeout = 10ms,
    .id = 1_b,
    .type = robo::motor::Unitree::type::go_m8010_6,
    .dir = robo::motor::dir::reverse,
}};
constexpr robo::motor::Unitree::Info joint_l1 {{
    .io = serial_motor_2_3,
    .offline_timeout = 10ms,
    .id = 2_b,
    .type = robo::motor::Unitree::type::go_m8010_6,
    .dir = robo::motor::dir::forward,
}};
constexpr robo::motor::Unitree::Info joint_l2 {{
    .io = serial_motor_2_3,
    .offline_timeout = 10ms,
    .id = 3_b,
    .type = robo::motor::Unitree::type::go_m8010_6,
    .dir = robo::motor::dir::reverse,
}};
constexpr robo::motor::Unitree::Info joint_r1 {{
    .io = serial_motor_4_5,
    .offline_timeout = 10ms,
    .id = 4_b,
    .type = robo::motor::Unitree::type::go_m8010_6,
    .dir = robo::motor::dir::forward,
}};
constexpr robo::motor::Unitree::Info joint_r2 {{
    .io = serial_motor_4_5,
    .offline_timeout = 10ms,
    .id = 5_b,
    .type = robo::motor::Unitree::type::go_m8010_6,
    .dir = robo::motor::dir::reverse,
}};

///////////////////////////////////////////
// ctrl
///////////////////////////////////////////
constexpr robo::ctrl::FiveBarInfo leg_l {
    .motor1 = joint_l1,
    .motor2 = joint_l2,
    .l_a = 0.07f,
    .l_b = 0.17f,
    .l_c = 0.285f,
    .wheel_fixed_side = 2,
    .forward_installation = true,
};
constexpr robo::ctrl::FiveBarInfo leg_r {
    .motor1 = joint_r1,
    .motor2 = joint_r2,
    .l_a = 0.07f,
    .l_b = 0.17f,
    .l_c = 0.285f,
    .wheel_fixed_side = 2,
    .forward_installation = false,
};

constexpr robo::chassis::WheelLegInfo wheel_leg {
    .ctrl_period = 1ms,

    .imu = imu,
    .leg_left = leg_l,
    .leg_right = leg_r,
    .wheel_left = wheel_l,
    .wheel_right = wheel_r,

    .special = {
        .context = p_context,
        .motor_ctrl_interval = 360us,
        .pid_retract = {
            .kp = 1.0f,
            .ki = 0.005f,
            .kd = 0.0f,
            .max_iout = 8.0f,
            .max_out = 15.0f,
        },
        .retract_time_min = 500ms,
        .retract_speed = 6.0f,
        .retract_stop_speed = 0.02f,
        .joint_limit_angle = 3.49f,
    },

    .constant = {
        .f_gravity = 40.0f,
        .wheel_radius = 0.10f,
        .wheel_spacing = 0.38f,
    },

    .limit = {
        .height_set = { 
            .min = 0.22f,
            .max = 0.32f,
            .mid = 0.27f,
        },
        .roll_set = {
            .max = 0.2f,
            .dead_zone = 0.02f,
        },
        .speed_set = {
            .max = 1.0f,
            .delta_max = 3.0f,
            .dead_zone = 0.3f,
            .stop = 0.2f,
            .recalculate_delta = 0.3f,
        },
        .yaw_speed_set = {
            .max = 1.5f,
            .delta_max = 6.0f,
            .dead_zone = 0.2f,
            .stop = 0.2f,
            .recalculate_delta = 0.3f,
        },
        .theta_l_ref_max = 0.7f,
        .theta_b_ref_max = 0.3f,
    },

    .param = {
        .K = {
            -4.9529f,  -6.0926f,  -10.151f,  -0.66441f,  -24.1f,  -2.0052f,  -4.887f,  -0.9113f,  -7.8273f,  -0.60803f, 
            -4.9529f,  -6.0926f,  10.151f,  0.66441f,  -4.887f,  -0.9113f,  -24.1f,  -2.0052f,  -7.8273f,  -0.60803f, 
            0.6846f,  0.82061f,  -12.123f,  -1.4456f,  15.743f,  1.0686f,  -11.658f,  -0.71293f,  -49.781f,  -2.1164f, 
            0.6846f,  0.82061f,  12.123f,  1.4456f,  -11.658f,  -0.71293f,  15.743f,  1.0686f,  -49.781f,  -2.1164f,
        },
        .pid_height = {
            .kp = 200.0f,
            .ki = 0.15f,
            .kd = 20000.0f,
            .max_iout = 5.0f,
            .max_out = 20.0f,
        },
        .pid_psi = {
            .kp = 200.0f,
            .ki = 0.05f,
            .kd = 5000.0f,
            .max_iout = 30.0f,
            .max_out = 60.0f,
        },
        .speed_observer = {
            .A = { 1.0f, 0.001f, 0.0f, 1.0f },
            .H = { 1.0f, 0.0f, 0.0f, 1.0f },
            .Q_c = { 20.0f * 1e-3f, 0.0f, 0.0f, 0.01f * 1e-3f },
            .R_c = { 1000.0f, 0.0f, 0.0f, 0.01f },
        }
    },
};

///////////////////////////////////////////
// robot
///////////////////////////////////////////
constexpr robo::bot::WheelLegInfo info {
    .input = gamepad,
    .wheel_leg = wheel_leg,
};

///////////////////////////////////////////
// main
///////////////////////////////////////////
int main() {
    robo::bot::WheelLeg robot { info };
    robot.run();
}

