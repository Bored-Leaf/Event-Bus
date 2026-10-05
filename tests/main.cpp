#include <print>

#include "event_bus.hpp"

struct TestEvent1 {
    int num;
};

struct TestEvent2 {
    int num;
};

// TODO: More robust tests with google tests
int main() {
    event_bus::EventBus eventBus{};

    std::println("-- Testing a single handle subscribe and publish --");
    event_bus::SubscriptionHandle subscriptionHandle1{eventBus.subscribe<TestEvent1>([](const TestEvent1& testEvent1) { std::println("Test event1: {}", testEvent1.num); })};
    TestEvent1 testEvent1{.num = 2};
    eventBus.publish(testEvent1);
    std::println("Test event1 id: {}", subscriptionHandle1.eventId);

    std::println("-- Testing a multiple handle subscribe and publish --");
    event_bus::SubscriptionHandle subscriptionHandle2{eventBus.subscribe<TestEvent1>([](const TestEvent1& testEvent1) { std::println("test event2: {}", testEvent1.num * 2); })};
    event_bus::SubscriptionHandle subscriptionHandle3{eventBus.subscribe<TestEvent1>([](const TestEvent1& testEvent1) { std::println("test event3: {}", testEvent1.num * 4); })};
    testEvent1.num = 5;
    eventBus.publish(testEvent1);
    std::println("Test event2 id: {}", subscriptionHandle2.eventId);
    std::println("Test event3 id: {}", subscriptionHandle3.eventId);

    std::println("-- Testing no crossover calls of event publishing --");
    event_bus::SubscriptionHandle subscriptionHandle4{eventBus.subscribe<TestEvent2>([](const TestEvent2& testEvent2) { std::println("test event4: {}", testEvent2.num); } )};
    TestEvent2 testEvent2{.num = 3};
    std::println(" >> Should only be testEvent1-3");
    eventBus.publish(testEvent1);
    std::println(" >> Should only be testEvent4 and its id");
    eventBus.publish(testEvent2);
    std::println("testEvent4 id: {}", subscriptionHandle4.eventId);

    std::println("-- Testing unsubscribing stops further calls --");
    eventBus.Unsubscribe(subscriptionHandle1);
    std::println(" >> testEvent1 shouldn't fire");
    eventBus.publish(testEvent1);
}