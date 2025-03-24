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
    auto context = exec::thread_context{};
    exec::scheduler auto sched = context.get_scheduler();
    exec::sender auto s = exec::schedule(sched)
      | exec::then([]() static { std::println("Hello World"); });

    exec::operation_state auto op = exec::connect(s, my_receiver{});
    exec::start(op);
}

