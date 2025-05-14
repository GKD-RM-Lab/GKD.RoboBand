import std;
import robo;
using namespace std::chrono_literals;

constexpr auto c_context = robo::task_context::continuous::info::make();

constexpr robo::input::Gamepad::Info gamepad_info {
    .name = "/dev/input/js0",
    .offline_timeout = std::chrono::nanoseconds::max(),
    .context = c_context,
};

int main() {
    robo::input::Gamepad gamepad { gamepad_info };
    gamepad.on_update([&] {
        std::println("{:%T} {}", std::chrono::system_clock::now(), gamepad.stick()[0]);
    });

    robo::task_context::start();
    robo::task_context::wait_until_all_finished();
}

