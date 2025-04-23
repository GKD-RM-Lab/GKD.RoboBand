import std;
import robo;

int main() {
    robo::spt::task_context::start();
    robo::spt::task_context::wait_until_all_finished();
}

