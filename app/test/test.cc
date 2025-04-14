import std;
import robo;

using namespace std::chrono_literals;

constexpr auto context = robo::spt::thread_context::make();
constexpr robo::io::Serial::Info serial_info {
    .name = "/dev/serial/by-id/usb-DM-IMU_DM-IMU_USB_CDC_2025021200-if00",
    .context = context[0],
    .rx_idle_duration = 50us,
    .rx_buffer_size = 57,
    .baud_rate = 4000000
};

int main() {
    auto& serial = robo::io::Serial::get(serial_info);
    serial.register_callback([&serial] {
        auto rx_bytes = serial.get_rx_bytes();
        // if (rx_bytes.size() != 57) {
        if (true) {
            std::println("{}", std::chrono::system_clock::now());
            std::print("{:3} bytes received: ", rx_bytes.size()); 
            for (auto e : rx_bytes) {
                std::print("{:02x} ", std::to_integer<unsigned int>(e));
            }
            std::print("\n\n"); 
        }
    });

    auto send_task = [&serial] {
        while (true) {
            std::array<std::byte, 100> arr {};
            serial.send(arr);
            std::this_thread::sleep_for(1ms);
        }
    };
    std::jthread send_thread1 { send_task };
    std::jthread send_thread2 { send_task };
    std::jthread send_thread3 { send_task };
    std::jthread send_thread4 { send_task };
    std::jthread send_thread5 { send_task };
    std::jthread send_thread6 { send_task };

    robo::spt::thread_context::start();
    std::this_thread::sleep_for(std::chrono::hours::max());
}

