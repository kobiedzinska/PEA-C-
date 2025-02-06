#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include "Queue.h"
#include <vector>
#include <algorithm>

template <typename T>
class PriorityQueue {
private:
    std::vector<T> elements;

public:
    PriorityQueue() = default;
    ~PriorityQueue() = default;

    void push(T value);
    void pop();
    bool empty() const;
    int getSize() const;
    T getFront() const;
};

template <typename T>
void PriorityQueue<T>::push(T value) {
    elements.push_back(value);
    std::sort(elements.begin(), elements.end(), std::greater<T>());
}

template <typename T>
void PriorityQueue<T>::pop() {
    if (this->empty()) {
        throw std::runtime_error("PriorityQueue is empty. Cannot dequeue.");
    }
    elements.erase(elements.begin());
}

template <typename T>
bool PriorityQueue<T>::empty() const {
    return elements.empty();
}

template <typename T>
int PriorityQueue<T>::getSize() const {
    return elements.size();
}

template <typename T>
T PriorityQueue<T>::getFront() const {
    if (this->empty()) {
        throw std::runtime_error("PriorityQueue is empty.");
    }
    return elements.front();
}

#endif // PRIORITY_QUEUE_H
