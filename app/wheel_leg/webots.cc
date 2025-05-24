import std;
import robo;
using namespace std::chrono_literals;

///////////////////////////////////////////
// context
///////////////////////////////////////////
constexpr auto context = robo::task_context::continuous::info::make();

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

constexpr robo::input::Gamepad::Info gamepad {
    .name = "/dev/input/js0",
    .context = context,
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
    .wheel_fixed_side = 2,
    .forward_installation = true,
};
constexpr robo::ctrl::FiveBarInfo leg_r {
    .motor1 = joint_r1,
    .motor2 = joint_r2,
    .l_a = 0.06f,
    .l_b = 0.2f,
    .l_c = 0.3f,
    .wheel_fixed_side = 2,
    .forward_installation = true,
};

constexpr robo::chassis::WheelLegInfo wheel_leg {
    .ctrl_period = 1ms,

    .imu = imu,
    .leg_left = leg_l,
    .leg_right = leg_r,
    .wheel_left = wheel_l,
    .wheel_right = wheel_r,

    .special = {},

    .constant = {
        .f_gravity = 33.0f,
        .wheel_mass = 0.1f,
        .wheel_radius = 0.10f,
        .wheel_spacing = 0.38f,
        .leg_length_min = 0.15f,
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
            .max = 2.5f,
            .delta_max = 5.0f,
            .dead_zone = 0.3f,
            .stop = 0.5f,
            .recalculate_delta = 0.3f,
        },
        .yaw_speed_set = {
            .max = 3.0f,
            .delta_max = 8.0f,
            .dead_zone = 0.2f,
            .stop = 0.2f,
            .recalculate_delta = 0.3f,
        },
        .theta_l_ref_max = 0.7f,
        .theta_b_ref_max = 0.3f,
    },

    .param = {
        .K00 = {
            -4.5459f, -8.1529f, -11.11f, -1.3892f, -20.397f, -1.1472f, -2.2374f, -0.307f, -24.959f, -1.7654f,
            -4.5459f, -8.1529f, 11.11f, 1.3892f, -2.2374f, -0.307f, -20.397f, -1.1472f, -24.959f, -1.7654f,
            2.0838f, 3.3417f, -12.984f, -1.7501f, 13.89f, 0.7818f, -2.1459f, -0.0029742f, -45.236f, -1.8227f,
            2.0838f, 3.3417f, 12.984f, 1.7501f, -2.1459f, -0.0029742f, 13.89f, 0.7818f, -45.236f, -1.8227f,
        },
        .K10 = {
            -6.1655f, -2.6736f, -27.084f, -3.2568f, -6.4456f, -5.0237f, 3.9151f, 0.41902f, 74.839f, 3.7229f,
            3.8378f, 11.796f, -1.2697f, -0.51099f, -53.694f, -5.0333f, 18.755f, 2.1683f, 21.736f, 2.1749f,
            12.043f, 16.187f, 28.593f, 3.3066f, 6.1623f, 2.4107f, -8.0156f, -0.28519f, -70.319f, -3.2246f,
            -19.874f, -30.612f, -18.466f, -2.0395f, -50.012f, -3.498f, 8.6728f, -0.58179f, 42.028f, 1.4855f,
        },
        .K01 = {
            3.8378f, 11.796f, 1.2697f, 0.51099f, 18.755f, 2.1683f, -53.694f, -5.0333f, 21.736f, 2.1749f,
            -6.1655f, -2.6736f, 27.084f, 3.2568f, 3.9151f, 0.41902f, -6.4456f, -5.0237f, 74.839f, 3.7229f,
            -19.874f, -30.612f, 18.466f, 2.0395f, 8.6728f, -0.58179f, -50.012f, -3.498f, 42.028f, 1.4855f,
            12.043f, 16.187f, -28.593f, -3.3066f, -8.0156f, -0.28519f, 6.1623f, 2.4107f, -70.319f, -3.2246f,
        },
        .K20 = {
            17.737f, 22.767f, 30.593f, 3.3451f, 3.6249f, 0.6626f, -9.3284f, 0.83587f, -102.89f, -3.4401f,
            0.62311f, -1.5259f, -2.3608f, -0.035666f, 63.574f, 1.6272f, -31.715f, -2.6101f, -17.767f, -1.1908f,
            -13.597f, -19.423f, -34.116f, -3.522f, 24.587f, 7.0771f, 16.804f, 0.17959f, 94.481f, 3.346f,
            13.241f, 21.299f, 20.978f, 1.916f, 25.931f, -6.6236f, -20.065f, -0.074397f, -50.853f, -1.6233f,
        },
        .K11 = {
            -15.389f, -35.221f, 3.9332f, 0.39442f, 14.484f, 0.70558f, -7.3718f, -2.4371f, 4.961f, -2.2921f,
            -15.389f, -35.221f, -3.9332f, -0.39442f, -7.3718f, -2.4371f, 14.484f, 0.70558f, 4.961f, -2.2921f,
            9.6349f, 17.203f, 7.2943f, 1.0693f, 5.0705f, -1.7156f, -1.5425f, 2.9039f, -1.9731f, 0.76933f,
            9.6349f, 17.203f, -7.2943f, -1.0693f, -1.5425f, 2.9039f, 5.0705f, -1.7156f, -1.9731f, 0.76933f,
        },
        .K02 = {
            0.62311f, -1.5259f, 2.3608f, 0.035666f, -31.715f, -2.6101f, 63.574f, 1.6272f, -17.767f, -1.1908f,
            17.737f, 22.767f, -30.593f, -3.3451f, -9.3284f, 0.83587f, 3.6249f, 0.6626f, -102.89f, -3.4401f,
            13.241f, 21.299f, -20.978f, -1.916f, -20.065f, -0.074397f, 25.931f, -6.6236f, -50.853f, -1.6233f,
            -13.597f, -19.423f, 34.116f, 3.522f, 16.804f, 0.17959f, 24.587f, 7.0771f, 94.481f, 3.346f,
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
            .ki = 0.00f,
            .kd = 12500.0f,
            .max_iout = 10.0f,
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
        .off_ground_time = 150ms,
        .off_ground_torque = 5.0f,
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

