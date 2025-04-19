import std;
import robo;

using namespace std::chrono_literals;
using namespace robo::spt::byte_literals;

template <robo::dev::imu Imu, robo::dev::motor Motor>
struct RobotInfo {
    Imu imu;
    Motor motor;
};
    
template <robo::dev::imu Imu, robo::dev::motor Motor>
struct RobotImpl {
    using RobotInfo = RobotInfo<Imu, Motor>;
    Imu::dev_type imu_;
    Motor::dev_type motor_;

    explicit RobotImpl(const RobotInfo& info) :
        imu_ { info.imu }, motor_ { info.motor } {}

    void start() {
        robo::spt::thread_context::start();
    }
};

template <robo::dev::imu Imu, robo::dev::motor Motor>
struct Robot : public RobotImpl<Imu, Motor> {
    using RobotImpl = RobotImpl<Imu, Motor>;
    using RobotImpl::RobotImpl;

    void init() {};
    void init() requires std::same_as<Motor, robo::motor::Unitree> {
        this->motor_.setAngle(0.0f);
    }
};
template <robo::dev::imu Imu, robo::dev::motor Motor>
Robot(RobotInfo<Imu, Motor>) -> Robot<Imu, Motor>;

/** context ******************************/
constexpr auto context = robo::spt::thread_context::make();

/** io ***********************************/
constexpr robo::io::Serial::Info serial_motor {
    .name = "/dev/serial/by-id/usb-DM-IMU_DM-IMU_USB_CDC_2025021200-if00",
    .context = context,
    .rx_idle_duration = 50us,
    .rx_buffer_size = robo::motor::Unitree::rx_size,
    .baud_rate = 4000000
};
constexpr robo::io::Serial::Info serial_imu {
    .name = "/dev/serial/by-id/usb-FTDI_USB__-__Serial_Converter_FTAK92PX-if00-port0",
    .context = context,
    .rx_idle_duration = 50us,
    .rx_buffer_size = robo::imu::Dm::rx_size,
    .baud_rate = 4000000
};

/** device *******************************/
constexpr robo::imu::Dm::Info imu { 
    .io = serial_imu
};
constexpr robo::motor::Unitree::Info motor {{
    .io = serial_motor,
    .id = 0_b,
    .type = robo::motor::Unitree::type::go_m8010_6,
    .dir = robo::motor::Unitree::dir::forward
}};

/** robot ********************************/
constexpr RobotInfo robot_info {
    imu,
    motor
};

int main() {
    Robot robot { robot_info };
    robot.init();
    robot.start();

    std::this_thread::sleep_for(std::chrono::hours::max());
}

