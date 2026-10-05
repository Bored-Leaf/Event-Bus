#include "event_bus.hpp"

namespace event_bus {
    void EventBus::Unsubscribe(const SubscriptionHandle& subHandle) {
        auto bucketIT = handlers.find(subHandle.eventType);

        if (bucketIT == handlers.end()) {
            std::println("A bucket was not found to unsubscribe from an event!");
            return;
        }

        auto& bucket = bucketIT->second;

        auto it = std::find_if(bucket.begin(), bucket.end(), 
            [subHandle](const HandlerEntry& handler) { return handler.eventId == subHandle.eventId; }
        );

        if (it != bucket.end()) {
            bucket.erase(it);
        }
    }
}