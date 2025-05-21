/*
 * subscriber.h
 *
 *  Created on: Mar 29, 2025
 *      Author: tanmay
 */

#pragma once
#include "pubsub_manager.h"

template<typename T>
class Subscriber {
public:
	explicit Subscriber(TopicID topic_id);
    bool copy(T& data);

private:
    TopicID topic_id;
	int subscriber_id;
};

template<typename T>
Subscriber<T>::Subscriber(TopicID topic_id) :
topic_id(topic_id),
subscriber_id(pubSubManager().registerSubscriber(topic_id)){

}

template<typename T>
bool Subscriber<T>::copy(T& data) {
    if (subscriber_id < 0) return false;
    return pubSubManager().copy<T>(topic_id, data, subscriber_id);
}
