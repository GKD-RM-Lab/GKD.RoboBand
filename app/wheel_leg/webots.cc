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
//     .K = {
// -15.48f,  -11.902f,  -0.67339f,  -0.70543f,  -13.968f,  -2.6026f,  -12.434f,  -2.2599f,  -11.071f,  -0.98692f,
//      -15.48f,  -11.902f,  0.67339f,  0.70543f,  -12.434f,  -2.2599f,  -13.968f,  -2.6026f,  -11.071f,  -0.98692f, 
//      5.8756f,  4.2326f,  -0.39392f,  -0.44086f,  12.826f,  2.0722f,  -4.9121f,  -0.50649f,  -89.413f,  -2.8331f, 
//      5.8756f,  4.2326f,  0.39392f,  0.44086f,  -4.9121f,  -0.50649f,  12.826f,  2.0722f,  -89.413f,  -2.8331f, 
//     },
    // .K = { -15.228f,  -11.399f,  -0.65213f,  -0.68245f,  -11.544f,  -2.0419f,  -10.091f,  -1.6499f,  -14.448f,  -1.193f, 
    //  -15.228f,  -11.399f,  0.65213f,  0.68245f,  -10.091f,  -1.6499f,  -11.544f,  -2.0419f,  -14.448f,  -1.193f, 
    //  7.766f,  5.4695f,  -0.49907f,  -0.5544f,  10.797f,  1.8633f,  -2.145f,  -0.25145f,  -87.828f,  -2.7193f, 
    //  7.766f,  5.4695f,  0.49907f,  0.5544f,  -2.145f,  -0.25145f,  10.797f,  1.8633f,  -87.828f,  -2.7193f, },
    // .K = { -8.3267f,  -9.5854f,  -0.64029f,  -0.6749f,  -21.513f,  -2.0815f,  -10.669f,  -1.4066f,  -18.869f,  -1.183f, 
    //  -8.3267f,  -9.5854f,  0.64029f,  0.6749f,  -10.669f,  -1.4066f,  -21.513f,  -2.0815f,  -18.869f,  -1.183f, 
    //  4.346f,  4.8652f,  -0.5478f,  -0.58475f,  32.293f,  2.3824f,  -16.571f,  -0.81421f,  -110.62f,  -2.9378f, 
    //  4.346f,  4.8652f,  0.5478f,  0.58475f,  -16.571f,  -0.81421f,  32.293f,  2.3824f,  -110.62f,  -2.9378f },
    // .K = { -8.9933f,  -13.902f,  -0.63466f,  -0.66916f,  -27.017f,  -9.676f,  -21.215f,  -4.1301f,  -16.052f,  -2.9812f, 
    //        -8.9933f,  -13.902f,  0.63466f,  0.66916f,  -21.215f,  -4.1301f,  -27.017f,  -9.676f,  -16.052f,  -2.9812f, 
    //        7.9835f,  12.289f,  -0.56925f,  -0.60637f,  34.753f,  17.021f,  6.578f,  -3.8238f,  -50.333f,  -7.8617f, 
    //        7.9835f,  12.289f,  0.56925f,  0.60637f,  6.578f,  -3.8238f,  34.753f,  17.021f,  -50.333f,  -7.8617f, },
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

