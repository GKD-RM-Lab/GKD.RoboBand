import std;
import exec;

struct my_receiver {
    template <typename... Ts>
    friend void tag_invoke(exec::set_value_tag, my_receiver&&, Ts&&... values) noexcept {
        std::println("my_receiver set value in thread {}", std::this_thread::get_id());
        ((std::cout << values << " "), ...);
        std::println();
    }
    template <typename E>
    friend void tag_invoke(exec::set_error_tag, my_receiver&&, E&&) noexcept {
        std::println("my_receiver set error");
    }
    friend void tag_invoke(exec::set_done_tag, my_receiver&&) noexcept {
        std::println("my_receiver set done");
    }
};

int main() {
    // exec::sender auto s = exec::just(114, 514);
    // exec::sender auto s = exec::then(exec::just(114, 514), [](auto val1, auto val2) { return val1 * val2; });
    exec::sender auto s = exec::just(114, 514)
      | exec::then([](auto val1, auto val2) { return val1 * val2; })
      | exec::then([](auto val) { return val + 1919810; });
    auto op = exec::connect(s, my_receiver{});
    exec::start(op);
}

