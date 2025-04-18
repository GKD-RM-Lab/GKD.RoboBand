import std;
import robo;

using namespace std::chrono_literals;
using namespace robo::spt::byte_literals;
namespace stdv = std::views;

constexpr auto context = robo::spt::thread_context::make();
constexpr robo::io::Serial::Info serial {
    .name = "/dev/serial/by-id/usb-DM-IMU_DM-IMU_USB_CDC_2025021200-if00",
    .context = context,
    .rx_idle_duration = 50us,
    .rx_buffer_size = 19,
    .baud_rate = 4000000
};
constexpr robo::imu::Dm::Info imu { serial };

int main() {
    robo::imu::Dm dev { imu };
    robo::io::on_error(serial, [&](auto&& io, auto err) {
        if (err == robo::io::error::invalid_data) {
            auto view = stdv::transform(robo::io::get(serial).get_rx_bytes(), 
                                        [](auto e) { return std::to_integer<unsigned>(e); });
            std::print("[{}] invalid data: {::02x}\n\n", std::chrono::system_clock::now(), view);
        } else {
            robo::io::allow_no_callback(io, err); 
        }
    });
    std::jthread send_thread[3] { std::jthread{[] {
        while (true) {
            std::array<std::byte, 100> arr {};
            robo::io::send(serial, arr);
            std::this_thread::sleep_for(1ms);
        }
    }}};

    robo::spt::thread_context::start();
    std::this_thread::sleep_for(std::chrono::hours::max());
}

