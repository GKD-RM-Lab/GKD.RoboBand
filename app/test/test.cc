import std;
import exec;

struct my_receiver {
    template <typename... Vs>
    friend void tag_invoke(exec::set_value_tag, my_receiver&&, Vs&&... values) noexcept {
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
    auto s = exec::then(exec::then(exec::just(42), [](auto val) { return val + 1; }), [](auto val) { return val + 1; });
    auto op = exec::connect(s, my_receiver{});
    exec::start(op);
}
