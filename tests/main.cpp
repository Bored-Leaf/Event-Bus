#include <print>

#include "event_bus.hpp"

struct TestPrintEvent {
    int num;
};

int main() {
    std::println("Hello world!");
}