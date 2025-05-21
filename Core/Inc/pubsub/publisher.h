/*
 * publisher.h
 *
 *  Created on: Mar 29, 2025
 *      Author: tanmay
 */
#pragma once
#include "pubsub_manager.h"

template<typename T>
class Publisher {
public:
    explicit Publisher(TopicID topic_id);
    void publish(const T& data);

private:
    TopicID topic_id;
};

template<typename T>
Publisher<T>::Publisher(TopicID topic_id) : topic_id(topic_id) {
    pubSubManager().advertise<T>(topic_id);
}

template<typename T>
void Publisher<T>::publish(const T& data) {
    pubSubManager().publish<T>(topic_id, data);
}
