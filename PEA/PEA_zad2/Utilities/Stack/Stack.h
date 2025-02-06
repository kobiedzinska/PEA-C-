

#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H
#include <stdexcept>


template<typename T>
class Stack {
    T* array;
    size_t count;
    size_t capacity;


public:
    Stack():count(0), capacity(1) {
        array = new T[capacity];
    };
    ~Stack();

    void push(const T& value);
    T pop();
    bool empty();
    T top();


};


template<typename T>
Stack<T>::~Stack() {
    delete[] array;
}

template<typename T>
void Stack<T>::push(const T &value) {
    size_t previousCapacity = capacity;
    while(count < capacity) {
        capacity *= 2;
    }
    if(previousCapacity != capacity) {
        T* newArray = new T[capacity];
        for(size_t i = 0; i < previousCapacity; i++) {
            newArray[i] = array[i];
        }
        delete[] array;
        array = newArray;
    }
    array[count] = value;
    ++count;
}

template<typename T>
T Stack<T>::pop() {
    if(count == 0) {
        throw std::out_of_range("Stack is empty");
    }
    T value = array[count--];
    --count;
    return value;
}

template<typename T>
bool Stack<T>::empty() {
    if(count == 0) {
        return true;
    }
    return false;
}

template<typename T>
T Stack<T>::top() {
    return array[0];
}


#endif //PRIORITYQUEUE_H
