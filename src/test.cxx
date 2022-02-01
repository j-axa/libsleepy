#include "sleepy.hxx"
#include <chrono>
#include <iostream>

int main() {
    using namespace std::chrono;
    using clock = high_resolution_clock;
    const auto begin = clock::now();
    sleepy::sleep(seconds { 1 });
    std::cout << "sleep for " << duration_cast<duration<float>>(clock::now() - begin).count() << "s\n";
    return 0;
}