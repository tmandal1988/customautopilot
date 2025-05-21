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
        int index = static_cast<int>(topic_id);
        if (topics[index] == nullptr) {
            topics[index] = new Topic<T>{T{}, 0, xSemaphoreCreateMutex()};
        }
    }

    template<typename T>
    void publish(TopicID topic_id, const T& data) {
        int index = static_cast<int>(topic_id);
        auto* topic = static_cast<Topic<T>*>(topics[index]);
        if (topic == nullptr) return;

        if (xSemaphoreTake(topic->mutex, 0)) { // Non-blocking attempt
            topic->data = data;
            topic->data.timestamp_us = getCurrentTimeUs();
            topic->updated_mask = UINT32_MAX; // Set all bits to 1 to mark all as updated
            xSemaphoreGive(topic->mutex);
        }
    }

    template<typename T>
    bool copy(TopicID topic_id, T& data, int subscriber_id) {
        int index = static_cast<int>(topic_id);
        auto* topic = static_cast<Topic<T>*>(topics[index]);
        if (topic == nullptr || subscriber_id >= MAX_SUBSCRIBERS) return false;

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

    uint8_t registerSubscriber(TopicID topic_id){
    	uint8_t index = static_cast<uint8_t>(topic_id);
    	if (next_subscriber_id_[index] >= MAX_SUBSCRIBERS) {
    		return -1; // Or assert or handle error
    	}
    	return next_subscriber_id_[index]++;
    }

private:
    static constexpr uint8_t MAX_SUBSCRIBERS = 10;
    void* topics[static_cast<uint8_t>(TopicID::MAX_TOPICS)] = {};
    uint8_t next_subscriber_id_[MAX_SUBSCRIBERS] = {}; // <-- NEW
    PubSubManager() = default;
};

inline PubSubManager& pubSubManager() {
    return PubSubManager::getInstance();
}
