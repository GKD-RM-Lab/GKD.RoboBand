import std;
import robo;

using namespace std::chrono_literals;
using namespace robo::spt::byte_literals;

constexpr robo::imu::Webots::Info imu {
    .inertial_unit_name = "inertial unit",
    .gyro_name = "gyro",
    .accelerometer_name = "accelerometer"
};

int main() {

    robo::spt::task_context::start();
    std::this_thread::sleep_for(std::chrono::hours::max());
}

