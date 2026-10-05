#pragma once

#include <vector>
#include <functional>
#include <typeindex>
#include <any>
#include <print>

/**
 * @brief A simple generic event bus
 * @details This namespace contains a custom built event bus and its relevant objects.
 *
 * ### Basic Usage Example
 * @code
 *  
 * event_bus::EventBus eventBus{};
 *
 * event_bus::SubscriptionHandle sh{eventBus.subscribe<EventType>([](const EventType& eventdata) {std::print("eventd ata: {}", eventData.data); })};
 * ...
 * EventType event{.data = 1};
 * eventBus.publsh(event);
 *
 * @endcode 
 *
 * @warning Nothing in this namespace is thread safe
 * 
 */
namespace event_bus {

    /**
     * @brief Represents public accessable metadata about a subscribed handle
     */
    struct SubscriptionHandle {
        std::type_index eventType;
        unsigned int    eventId;
    };

    /**
     * @internal
     * @brief A handle, and its metadata that has been subscribed to the event bus
     */
    struct HandlerEntry {
        std::any     handler;
        unsigned int eventId;
    };

    /**
     * @brief A simple generic event bus
     * @details It is a simple event bus that can subscribe, unsubscribe and public events of any event type
     * guarenteeing to not cross call event types when publishing events.
     *
     * @warning This event bus is not thread safe
     * 
     */
    class EventBus {
    public:
        /**
         * @brief Subscribes a handler to the eventbus, organised by event type
         * @details Takes a handle, assigns it an ID, and places it in @ref handlers whose
         * bucket is determined by type @p T
         *
         * @warning This function is not thread safe
         * 
         * @tparam T The event type.
         * @param handler The callback function which receieves the event data object of type @p T.
         * @return SubscriptionHandle A handle to manage or unsubscribe the subscription.
         */
        template <typename T>
        SubscriptionHandle subscribe(std::function<void(const T& eventData)> callback) {
            unsigned int eventID{eventIDCounter++};
            std::type_index eventType = std::type_index(typeid(T));

            HandlerEntry handle{.handler = callback,
                                .eventId = eventID
            };

            handlers[eventType].push_back(handle);

            return SubscriptionHandle{.eventType = eventType,
                                      .eventId = eventID
            };
        }

        /**
         * @brief Publishes an event and calls its handles
         * @details Publishes an event and calls its bucket determined by type @p T
         *
         * @warning This function is not thread safe
         * 
         * @tparam T The event type.
         * @param eventData The event data.
         */
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
        
        /**
         * @brief Unsubscribes a subscription from the event bus
         *
         * @warning This function is not thread safe
         * 
         * @param handle The call
         */
        void Unsubscribe(const SubscriptionHandle& subHandle);
    private:
        /**
         * @internal
         * @brief A hashmap that holds containers of events seperated by type.
         * 
         */
        std::unordered_map<std::type_index, std::vector<HandlerEntry>> handlers;

        /**
         * @internal
         * @brief A global counter for @ref HandlerEntry
         */
        unsigned int eventIDCounter{0};
    };
}