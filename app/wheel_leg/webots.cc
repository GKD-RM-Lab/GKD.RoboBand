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
    .offline_timeout = std::chrono::nanoseconds::max(),
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

    .constant = {
        .f_gravity = 33.0f,
        .wheel_radius = 0.10f,
    },

    .limit = {
        .height_set = { 
            .min = 0.10f, // TODO
            .max = 0.50f, // TODO
        },
        .speed_set = {
            .max = 2.5f, // TODO
            .delta_max = 1.25f, // TODO
            .dead_zone = 0.05f, // TODO
        },
        .yaw_err_set = {
            .max = 0.0f, // TODO
            .delta_max = 10.0f, // TODO
            .dead_zone = 0.1f, // TODO
        },
        .s_ref_max = 0.3f,
        .phi_err_max = 0.3f,
        .theta_l_ref_max = 0.7f,
        .theta_b_ref_max = 0.1f,
    },

    .param = {
        .K = {
            -4.897f, -6.659f, -4.447f, -0.660f, -25.649f, -2.170f, -3.080f, -0.751f, -10.089f, -0.863f,
            -4.897f, -6.659f, 4.447f, 0.660f, -3.080f, -0.751f, -25.649f, -2.170f, -10.089f, -0.863f,
            1.009f, 1.329f, -15.173f, -2.479f, 9.472f, 0.498f, -4.430f, -0.018f, -48.971f, -2.084f,
            1.009f, 1.329f, 15.173f, 2.479f, -4.430f, -0.018f, 9.472f, 0.498f, -48.971f, -2.084f
        },
        .pid_height = {
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
    },
};

///////////////////////////////////////////
// robot
///////////////////////////////////////////
constexpr robo::bot::WheelLegInfo info {
    .input = gamepad,
    .wheel_leg = wheel_leg,
    .forward_ratio = wheel_leg.limit.speed_set.max, // TODO
    .turning_ratio = wheel_leg.limit.yaw_err_set.max, // TODO
};

///////////////////////////////////////////
// main
///////////////////////////////////////////
int main() {
    robo::bot::WheelLeg robot { info };
    robot.run();
}

