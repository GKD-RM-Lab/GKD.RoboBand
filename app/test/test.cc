import std;
import robo;

using namespace std::chrono_literals;
using namespace robo::spt::byte_literals;

constexpr auto context = robo::spt::thread_context::make();
constexpr robo::io::Serial::Info serial {
    .name = "/dev/serial/by-id/usb-DM-IMU_DM-IMU_USB_CDC_2025021200-if00",
    .context = context,
    .rx_idle_duration = 50us,
    .rx_buffer_size = 19,
    .baud_rate = 4000000
};

constexpr std::array key1 { 0x55_b, 0xaa_b, 0x01_b, 0x01_b };
constexpr std::array key2 { 0x55_b, 0xaa_b, 0x01_b, 0x02_b };
constexpr std::array key3 { 0x55_b, 0xaa_b, 0x01_b, 0x03_b };

int main() {
    auto callback = [](auto key, auto rx_bytes) {
        // if (rx_bytes.size() != 19) {
        if (true) {
            std::println("{}", std::chrono::system_clock::now());
            std::print("key 0x{:02x}: {:3} bytes received: ", 
                std::to_integer<int>(key[3]), rx_bytes.size()); 
            for (auto e : rx_bytes) {
                std::print("{:02x} ", std::to_integer<unsigned int>(e));
            }
            std::print("\n\n"); 
        }
    };

    robo::io::register_error_handler(serial, robo::io::allow_no_callback{});

    robo::io::register_callback<robo::io::prefix_key<4>>(
        serial, key1, [&](auto bytes) { callback(key1, bytes); });
    robo::io::register_callback<robo::io::prefix_key<4>>(
        serial, key2, [&](auto bytes) { callback(key2, bytes); });
    robo::io::register_callback<robo::io::prefix_key<4>>(
        serial, key3, [&](auto bytes) { callback(key3, bytes); });

    auto send_task = [] {
        while (true) {
            std::array<std::byte, 100> arr {};
            robo::io::send(serial, arr);
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

