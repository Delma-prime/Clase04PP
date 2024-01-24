#include <thread>
#include <chrono>
#include <iostream>
#include <mutex>

bool chopping = true;
std::mutex count_mutex;
unsigned int vegetable_count = 0;

void vegetable_chopper(const char* name) {
    while (true) {
        std::unique_lock<std::mutex> lock(count_mutex);
        if (!chopping) {
            lock.unlock();
            break;
        }
        vegetable_count++;
        lock.unlock();
    }
    printf("%s chopped %u vegetables.\n", name, vegetable_count);
}

int main() {
    std::thread olivia(vegetable_chopper, "Olivia");
    std::thread barron(vegetable_chopper, "Barron");

    printf("Barron and Olivia are chopping vegetables...\n");
    std::this_thread::sleep_for(std::chrono::seconds(1));

    {
        std::lock_guard<std::mutex> lock(count_mutex);
        chopping = false;
    }

    barron.join();
    olivia.join();

    return 0;
}
