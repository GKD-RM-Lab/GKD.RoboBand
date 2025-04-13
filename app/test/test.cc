import std;
import robo;


int main() {
    using namespace std::chrono_literals;

    robo::spt::thread_context context;
    context.add_task([] {});

    robo::io::Serial serial { context, "/dev/serial/by-id/usb-DM-IMU_DM-IMU_USB_CDC_2025021200-if00", 50us, 57, 4000000 };
    serial.register_callback([&serial] { 
        auto rx_bytes = serial.get_rx_bytes();
        if (rx_bytes.size() != 57) {
        // if (true) {
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

    context.start();
    std::this_thread::sleep_for(std::chrono::hours::max());
}

