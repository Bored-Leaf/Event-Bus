#include "event_bus.hpp"

namespace event_bus {
    void printHello() {
        std::println("Hello from event bus!");
    }

    void EventBus::Unsubscribe(const SubscriptionHandle& handle) {
        auto bucketIT = handlers.find(handle.eventType);

        if (bucketIT == handlers.end()) {
            std::println("A bucket was not found to unsubscribe from an event!");
            return;
        }

        auto& bucket = bucketIT->second;

        auto it = std::find_if(bucket.begin(), bucket.end(), 
            [handle](const HandlerEntry& handler) { return handler.eventId == handle.eventId; }
        );

        if (it != bucket.end()) {
            bucket.erase(it);
        }
    }
}