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
template <auto N>
std::array key { 0x55_b, 0xaa_b, 0x01_b, std::byte{N} };

int main() {
    std::array<std::size_t, 3> receiver {};
    auto callback = [&](std::size_t id, auto) { receiver[id]++; };

    robo::io::on_error(serial, [&](auto&& io, auto err) {
        if (err == robo::io::error::invalid_data) {
            auto view = std::views::transform(robo::io::get(serial).get_rx_bytes(), 
                                              [](auto e) { return std::to_integer<unsigned>(e); });
            std::print("[{}] {:: 8x}\nreceived invalid data: {::02x}\n\n",
                         std::chrono::system_clock::now(), receiver, view);
        } else {
            robo::io::allow_no_callback{}(io, err); 
        }
    });

    using IoParser = robo::io::with_parser<
        robo::io::prefix<4>, 
        robo::io::head<0x55, 0xaa, 0x01>,
        robo::io::tail<0x0a>, 
        robo::io::length<19>, 
        robo::io::crc<robo::spt::crc16::dm_imu, 0, 1>
    >;
    IoParser::on_data(serial, key<1>, [&](auto bytes) { callback(0, bytes); });
    IoParser::on_data(serial, key<2>, [&](auto bytes) { callback(1, bytes); });
    IoParser::on_data(serial, key<3>, [&](auto bytes) { callback(2, bytes); });

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

