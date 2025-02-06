
#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>
#include <stdexcept> // Do obsługi wyjątków

template <typename T>
class Queue {
private:
    std::vector<T> array;
    int capacity;    // Maksymalna pojemność kolejki
    int size, rear, front;

public:
    Queue();
    explicit Queue(int capacity);
    ~Queue() = default;

    void push(T value);
    void pop();
    bool empty() const;
    bool isFull() const;
    int getSize() const;
    int getCapacity() const;
    T getFront();
};

template <typename T>
Queue<T>::Queue() : array(), capacity(0), size(0), front(0), rear(0) {
}

template <typename T>
Queue<T>::Queue(int capacity) : capacity(capacity), size(0), front(0), rear(capacity - 1) {
    if (capacity < 0) {
        throw std::invalid_argument("Capacity must be greater than zero.");
    }
    array.resize(capacity);
}

template <typename T>
void Queue<T>::push(T value) {
    if (isFull()) {
      //  std::cout << "Queue is full." << std::endl;
        return;
    }
    rear = (rear + 1) % capacity;
    array[rear] = value;
    size++;
}

template <typename T>
void Queue<T>::pop() {
    if (this->empty()) {
        throw std::runtime_error("Queue is empty. Cannot dequeue.");
    }
    front = (front + 1) % capacity;
    size--;
}

template <typename T>
bool Queue<T>::empty() const {
    return size == 0;
}

template <typename T>
bool Queue<T>::isFull() const {
    return size == capacity;
}

template <typename T>
int Queue<T>::getSize() const {
    return size;
}

template <typename T>
int Queue<T>::getCapacity() const {
    return capacity;
}

template <typename T>
T Queue<T>::getFront() {
    return array[front];
}

#endif // QUEUE_H
