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

constexpr robo::motor::Unitree::Info wheel_l { {
    .io = serial_motor_0_1,
    .offline_timeout = 10ms,
    .id = 0_b,
    .type = robo::motor::Unitree::type::go_m8010_6,
    .dir = robo::motor::dir::forward,
} };
constexpr robo::motor::Unitree::Info wheel_r { {
    .io = serial_motor_0_1,
    .offline_timeout = 10ms,
    .id = 1_b,
    .type = robo::motor::Unitree::type::go_m8010_6,
    .dir = robo::motor::dir::reverse,
} };
constexpr robo::motor::Unitree::Info joint_l1 { {
    .io = serial_motor_2_3,
    .offline_timeout = 10ms,
    .id = 2_b,
    .type = robo::motor::Unitree::type::go_m8010_6,
    .dir = robo::motor::dir::forward,
} };
constexpr robo::motor::Unitree::Info joint_l2 { {
    .io = serial_motor_2_3,
    .offline_timeout = 10ms,
    .id = 3_b,
    .type = robo::motor::Unitree::type::go_m8010_6,
    .dir = robo::motor::dir::reverse,
} };
constexpr robo::motor::Unitree::Info joint_r1 { {
    .io = serial_motor_4_5,
    .offline_timeout = 10ms,
    .id = 4_b,
    .type = robo::motor::Unitree::type::go_m8010_6,
    .dir = robo::motor::dir::forward,
} };
constexpr robo::motor::Unitree::Info joint_r2 { {
    .io = serial_motor_4_5,
    .offline_timeout = 10ms,
    .id = 5_b,
    .type = robo::motor::Unitree::type::go_m8010_6,
    .dir = robo::motor::dir::reverse,
} };

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
        .wheel_mass = 0.1f,
        .wheel_radius = 0.10f,
        .wheel_spacing = 0.38f,
        .leg_length_min = 0.12f,
    },

    .limit = {
        .height_set = { 
            .min = 0.22f,
            .max = 0.32f,
            .mid = 0.27f,
        },
        .roll_set = {
            .max = 0.12f,
            .delta_max = 2.0f,
            .dead_zone = 0.02f,
        },
        .speed_set = {
            .max = 1.6f,
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
        .K00 = {
            -4.5162f, -6.976f, -4.7381f, -0.69436f, -22.175f, -1.0265f, -0.21235f, -0.068317f, -28.349f, -1.6715f,
            -4.5162f, -6.976f, 4.7381f, 0.69436f, -0.21235f, -0.068317f, -22.175f, -1.0265f, -28.349f, -1.6715f,
            2.4544f, 3.4595f, -15.831f, -1.9595f, 10.632f, 0.52632f, 2.9742f, 0.27976f, -43.786f, -1.7862f,
            2.4544f, 3.4595f, 15.831f, 1.9595f, 2.9742f, 0.27976f, 10.632f, 0.52632f, -43.786f, -1.7862f,
        },
        .K10 = {
            -13.679f, -13.259f, -23.214f, -1.4969f, -21.621f, -4.4802f, 9.6743f, -0.1789f, 102.74f, 3.7779f,
            11.387f, 18.981f, 10.63f, 1.2878f, -36.433f, -3.1711f, 15.654f, 1.7554f, 8.9893f, 1.9623f,
            4.6309f, 5.0482f, 22.49f, 1.8353f, 1.2285f, -0.0053629f, -11.818f, -0.90935f, -83.987f, -4.0134f,
            -14.414f, -20.263f, 2.2405f, 0.64255f, -56.024f, -1.6132f, 14.541f, 0.29583f, 46.769f, 2.0034f,
        },
        .K01 = {
            11.387f, 18.981f, -10.63f, -1.2878f, 15.654f, 1.7554f, -36.433f, -3.1711f, 8.9893f, 1.9623f,
            -13.679f, -13.259f, 23.214f, 1.4969f, 9.6743f, -0.1789f, -21.621f, -4.4802f, 102.74f, 3.7779f,
            -14.414f, -20.263f, -2.2405f, -0.64255f, 14.541f, 0.29583f, -56.024f, -1.6132f, 46.769f, 2.0034f,
            4.6309f, 5.0482f, -22.49f, -1.8353f, -11.818f, -0.90935f, 1.2285f, -0.0053629f, -83.987f, -4.0134f,
        },
        .K20 = {
            24.894f, 31.353f, 9.2613f, -0.074824f, 29.923f, -1.4069f, -26.697f, 0.99062f, -137.87f, -3.2196f,
            -5.024f, -7.7426f, -20.809f, -2.4438f, 55.266f, 3.2494f, -26.427f, -2.047f, -6.7346f, -1.2905f,
            -2.1002f, -2.4195f, -19.516f, -1.6535f, 32.87f, 9.353f, 30.34f, 2.2011f, 112.08f, 4.2505f,
            0.40289f, 2.0428f, -11.438f, -1.27f, 22.536f, -8.8925f, -36.899f, -2.682f, -48.553f, -2.033f,
        },
        .K11 = {
            -17.128f, -32.913f, 2.0526f, 0.11176f, 5.9918f, 0.81271f, -8.0343f, -4.187f, 11.805f, -2.1934f,
            -17.128f, -32.913f, -2.0526f, -0.11176f, -8.0343f, -4.187f, 5.9918f, 0.81271f, 11.805f, -2.1934f,
            13.716f, 20.593f, 1.8962f, -0.2537f, 30.794f, 0.57816f, -17.372f, 1.593f, -8.7806f, 0.67089f,
            13.716f, 20.593f, -1.8962f, 0.2537f, -17.372f, 1.593f, 30.794f, 0.57816f, -8.7806f, 0.67089f,
        },
        .K02 = {
            -5.024f, -7.7426f, 20.809f, 2.4438f, -26.427f, -2.047f, 55.266f, 3.2494f, -6.7346f, -1.2905f,
            24.894f, 31.353f, -9.2613f, 0.074824f, -26.697f, 0.99062f, 29.923f, -1.4069f, -137.87f, -3.2196f,
            0.40289f, 2.0428f, 11.438f, 1.27f, -36.899f, -2.682f, 22.536f, -8.8925f, -48.553f, -2.033f,
            -2.1002f, -2.4195f, 19.516f, 1.6535f, 30.34f, 2.2011f, 32.87f, 9.353f, 112.08f, 4.2505f,
        },

        .pid_height = {
            .kp = 250.0f,
            .ki = 0.05f,
            .kd = 12000.0f,
            .max_iout = 5.0f,
            .max_out = 20.0f,
        },
        .pid_psi = {
            .kp = 175.0f,
            .ki = 0.0f,
            .kd = 12500.0f,
            .max_iout = 30.0f,
            .max_out = 60.0f,
        },
        .speed_observer = {
            .A = { 1.0f, 0.001f, 0.0f, 1.0f },
            .H = { 1.0f, 0.0f, 0.0f, 1.0f },
            .Q_c = { 20.0f * 1e-3f, 0.0f, 0.0f, 0.01f * 1e-3f },
            .R_c = { 1000.0f, 0.0f, 0.0f, 0.01f },
        },

        .cent_acc_filter_t = 100ms,
        .wheel_height_acc_filter_t = 50ms,

        .roll_compensation_ratio = 0.2f,
        .off_ground_time = 100ms,
        .off_ground_torque = 6.0f,
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
