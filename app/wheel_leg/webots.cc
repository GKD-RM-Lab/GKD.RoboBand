import std;
import robo;

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
constexpr robo::motor::Webots::Info joint_l1 {
    .name = "motor1",
    .dir = robo::motor::dir::forward,
};
constexpr robo::motor::Webots::Info joint_l2 {
    .name = "motor3",
    .dir = robo::motor::dir::forward,
};
constexpr robo::motor::Webots::Info joint_r1 {
    .name = "motor2",
    .dir = robo::motor::dir::forward,
};
constexpr robo::motor::Webots::Info joint_r2 {
    .name = "motor4",
    .dir = robo::motor::dir::forward,
};

///////////////////////////////////////////
// ctrl
///////////////////////////////////////////
constexpr robo::ctrl::FiveBarInfo leg_l {
    .motor1 = joint_l1,
    .motor2 = joint_l2,
    .l_a = 1.0f,
    .l_b = 1.0f,
    .l_c = 1.0f,
};
constexpr robo::ctrl::FiveBarInfo leg_r {
    .motor1 = joint_r1,
    .motor2 = joint_r2,
    .l_a = 1.0f,
    .l_b = 1.0f,
    .l_c = 1.0f,
};

constexpr robo::chassis::WheelLegInfo wheel_leg {
    .context = robo::spt::task_context::periodic::none(),
    .imu = imu,
    .leg_left = leg_l,
    .leg_right = leg_r,
    .wheel_left = wheel_l,
    .wheel_right = wheel_r,
    .f_gravity = 0.0f,
    .pid_l = {
        .kp = 0.0f,
        .ki = 0.0f,
        .kd = 0.0f,
        .max_iout = 0.0f,
        .max_out = 0.0f,
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

