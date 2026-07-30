/*
 * pubsub_manager.h
 *
 *  Created on: Mar 29, 2025
 *      Author: tanmay
 */

#pragma once
#include "topic.h"
#include "topic_definition.h"

class PubSubManager {
public:
    static PubSubManager& getInstance() {
        static PubSubManager instance;
        return instance;
    }

    template<typename T>
    void advertise(TopicID topic_id) {
        const size_t index = static_cast<size_t>(topic_id);
        if (index >= kTopicCount) return;
        if (topics[index] == nullptr) {
            topics[index] = new Topic<T>{T{}, 0, xSemaphoreCreateMutex()};
        }
    }

    template<typename T>
    void publish(TopicID topic_id, const T& data) {
        const size_t index = static_cast<size_t>(topic_id);
        if (index >= kTopicCount) return;
        auto* topic = static_cast<Topic<T>*>(topics[index]);
        if (topic == nullptr) return;

        // Block until the topic is free: a sample must never be dropped
        // silently. The wait is bounded to microseconds — subscribers hold
        // the mutex only for a struct copy, and priority inheritance boosts
        // a lower-priority holder to this task's priority.
        if (xSemaphoreTake(topic->mutex, portMAX_DELAY)) {
            topic->data = data;
            topic->data.timestamp_ms = getCurrentTimeMs();
            topic->updated_mask = UINT32_MAX; // Set all bits to 1 to mark all as updated
            xSemaphoreGive(topic->mutex);
        }
    }

    template<typename T>
    bool copy(TopicID topic_id, T& data, int subscriber_id) {
        const size_t index = static_cast<size_t>(topic_id);
        if (index >= kTopicCount) return false;
        auto* topic = static_cast<Topic<T>*>(topics[index]);
        if (topic == nullptr || subscriber_id < 0 ||
            subscriber_id >= kMaxSubscribersPerTopic) return false;

        if (xSemaphoreTake(topic->mutex, 0)) { // Non-blocking attempt
            if (topic->updated_mask & (1 << subscriber_id)) { // Check if bit is set
                data = topic->data;
                topic->updated_mask &= ~(1 << subscriber_id); // Clear the bit for this subscriber
                xSemaphoreGive(topic->mutex);
                return true;
            }
            xSemaphoreGive(topic->mutex);
        }
        return false;
    }

    uint8_t registerSubscriber(TopicID topic_id) {
        const size_t index = static_cast<size_t>(topic_id);
        if ((index >= kTopicCount) ||
            (next_subscriber_id_[index] >= kMaxSubscribersPerTopic)) {
            return UINT8_MAX;
        }
        return next_subscriber_id_[index]++;
    }

private:
    static constexpr size_t kTopicCount =
        static_cast<size_t>(TopicID::MAX_TOPICS);
    static constexpr uint8_t kMaxSubscribersPerTopic = 10;
    void* topics[kTopicCount] = {};
    uint8_t next_subscriber_id_[kTopicCount] = {};
    PubSubManager() = default;
};

inline PubSubManager& pubSubManager() {
    return PubSubManager::getInstance();
}
