#pragma once

#include <vector>
#include <functional>
#include <typeindex>
#include <any>
#include <print>

// TODO: Doxygen thread safety when job scheduler has threaded implementation
// as events could be published/subscribed from different threads.
// Job scheduler is synchronous for now but as a future concern.

// TODO: Doxygen async dispatch eventually

namespace event_bus {
    void printHello();

    struct SubscriptionHandle {
        std::type_index eventType;
        unsigned int    eventId;
    };

    struct HandlerEntry {
        std::any     handler;
        unsigned int eventId;
    };

    class EventBus {
    public:
        template <typename T>
        SubscriptionHandle subscribe(std::function<void(const T& eventData)> handler) {
            unsigned int eventID{eventIDCounter++};
            std::type_index eventType = std::type_index(typeid(T));

            HandlerEntry handle{.handler = handler,
                                .eventId = eventID
            };

            handlers[eventType].push_back(handle);

            return SubscriptionHandle{.eventType = eventType,
                                      .eventId = eventID
            };
        }

        template <typename T>
        void publish(const T& eventData) {
            std::type_index eventType = std::type_index(typeid(T));
            auto bucketIT = handlers.find(eventType);

            if (bucketIT == handlers.end()) {
                std::println("A bucket was not found for publishing an event!");
                return;
            }

            auto& bucket = bucketIT->second;

            for (auto& handleEntry : bucket) {
                std::function<void(const T&)> handle = std::any_cast<std::function<void(const T&)>>(handleEntry.handler);
                handle(eventData);
            }
        }

        void Unsubscribe(const SubscriptionHandle& handle);
    private:
        std::unordered_map<std::type_index, std::vector<HandlerEntry>> handlers;
        unsigned int eventIDCounter{0};
    };
}