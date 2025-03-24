import std;
import exec;

struct my_receiver {
    template <typename... Ts>
    void set_value(my_receiver&&, Ts&&... values) noexcept {
        std::println("my_receiver set value in thread {}", std::this_thread::get_id());
        ((std::cout << values << " "), ...);
        std::println();
    }
    template <typename E>
    void set_error(my_receiver&&, E&&) noexcept {
        std::println("my_receiver set error");
    }
    void set_done(my_receiver&&) noexcept {
        std::println("my_receiver set done");
    }
};

int main() {
    auto context1 = exec::thread_context{};
    auto context2 = exec::thread_context{};
    exec::scheduler auto sched1 = context1.get_scheduler();
    exec::scheduler auto sched2 = context2.get_scheduler();
    exec::sender auto s = 
        exec::schedule(sched1)
      | exec::then([]() static { std::println("Hello Thread: {}", std::this_thread::get_id()); })
      | exec::schedule_from(sched2)
      | exec::then([]() static { std::println("Hello Thread: {}", std::this_thread::get_id()); });

    exec::operation_state auto op = exec::connect(s, my_receiver{});
    exec::start(op);

    using namespace std::chrono_literals;
    std::this_thread::sleep_for(1s);
}

