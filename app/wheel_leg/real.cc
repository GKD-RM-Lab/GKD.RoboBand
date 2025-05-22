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
            .delta_max = 5.0f,
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
        .K00 = {
            -4.5079f, -7.0517f, -4.7422f, -0.69481f, -22.157f, -1.0247f, -0.19968f, -0.065429f, -28.38f, -1.6811f,
            -4.5079f, -7.0517f, 4.7422f, 0.69481f, -0.19968f, -0.065429f, -22.157f, -1.0247f, -28.38f, -1.6811f,
            2.4563f, 3.5165f, -15.823f, -1.9586f, 10.633f, 0.52651f, 2.986f, 0.28028f, -43.759f, -1.7816f,
            2.4563f, 3.5165f, 15.823f, 1.9586f, 2.986f, 0.28028f, 10.633f, 0.52651f, -43.759f, -1.7816f,
        },
        .K10 = {
            -13.665f, -13.693f, -23.117f, -1.4894f, -21.895f, -4.571f, 9.5778f, -0.24032f, 102.49f, 3.7562f,
            11.314f, 19.067f, 10.68f, 1.2904f, -36.686f, -3.1965f, 15.523f, 1.7665f, 8.9315f, 1.9811f,
            4.6742f, 5.2282f, 22.446f, 1.8291f, 1.6032f, 0.056104f, -11.83f, -0.8853f, -84.005f, -4.0063f,
            -14.413f, -20.566f, 2.2674f, 0.64388f, -55.882f, -1.6041f, 14.38f, 0.27032f, 46.749f, 1.9814f,
        },
        .K01 = {
            11.314f, 19.067f, -10.68f, -1.2904f, 15.523f, 1.7665f, -36.686f, -3.1965f, 8.9315f, 1.9811f,
            -13.665f, -13.693f, 23.117f, 1.4894f, 9.5778f, -0.24032f, -21.895f, -4.571f, 102.49f, 3.7562f,
            -14.413f, -20.566f, -2.2674f, -0.64388f, 14.38f, 0.27032f, -55.882f, -1.6041f, 46.749f, 1.9814f,
            4.6742f, 5.2282f, -22.446f, -1.8291f, -11.83f, -0.8853f, 1.6032f, 0.056104f, -84.005f, -4.0063f,
        },
        .K20 = {
            24.71f, 31.676f, 9.7651f, -0.0023538f, 28.298f, -1.6608f, -25.264f, 1.1568f, -136.81f, -3.1639f,
            -5.3923f, -8.327f, -21.004f, -2.4468f, 53.805f, 2.999f, -26.25f, -2.106f, -5.9526f, -1.2978f,
            -2.4218f, -2.8006f, -19.902f, -1.6944f, 32.288f, 9.3559f, 29.643f, 2.1008f, 111.93f, 4.2275f,
            0.79461f, 2.3996f, -10.987f, -1.2217f, 22.763f, -8.936f, -36.056f, -2.6074f, -48.606f, -2.0151f,
        },
        .K11 = {
            -16.469f, -32.292f, 1.2487f, 0.025543f, 7.175f, 0.90598f, -8.387f, -4.3134f, 9.9995f, -2.2404f,
            -16.469f, -32.292f, -1.2487f, -0.025543f, -8.387f, -4.3134f, 7.175f, 0.90598f, 9.9995f, -2.2404f,
            13.604f, 20.75f, 2.8681f, -0.15069f, 30.302f, 0.46963f, -17.012f, 1.7111f, -8.5763f, 0.6922f,
            13.604f, 20.75f, -2.8681f, 0.15069f, -17.012f, 1.7111f, 30.302f, 0.46963f, -8.5763f, 0.6922f,
        },
        .K02 = {
            -5.3923f, -8.327f, 21.004f, 2.4468f, -26.25f, -2.106f, 53.805f, 2.999f, -5.9526f, -1.2978f,
            24.71f, 31.676f, -9.7651f, 0.0023538f, -25.264f, 1.1568f, 28.298f, -1.6608f, -136.81f, -3.1639f,
            0.79461f, 2.3996f, 10.987f, 1.2217f, -36.056f, -2.6074f, 22.763f, -8.936f, -48.606f, -2.0151f,
            -2.4218f, -2.8006f, 19.902f, 1.6944f, 29.643f, 2.1008f, 32.288f, 9.3559f, 111.93f, 4.2275f,
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
