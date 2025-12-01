//
// Created by DELL on 10/22/2025.
//

#ifndef STACK_STACK_H
#define STACK_STACK_H
#include <iostream>
using namespace std;


template <class T>
class Node
{
public :
    T data;
    Node* next;
};

template <class T>
class Stack
{
private :
    Node<T>* top;
    int length;

public :
    Stack();
    void push(T key);
    T    peek();
    T pop();
    bool empty();
    bool full();
    T    peek(int pos);
    int  size();
    bool is_equal(Stack<T> st);
    ~Stack();
};

template <class T>
Stack<T>::Stack() {
    length = 0;
    top = nullptr;
}

template <class T>
bool Stack<T>::is_equal(Stack<T> st){
    Stack<T> temp1 = *this;
    while(!temp1.empty() && !st.empty()){
        if(temp1.peek() != st.peek())
            return false;
        temp1.pop();
        st.pop();
    }
    return temp1.empty() && st.empty();
}

template <class T>
int Stack<T>::size() {
    return length;
}

template <class T>
void Stack<T>::push(T key) {

    if (length == 0) {
        top = new Node<T>;
        top->data = key;
        top->next = nullptr;
    }
    else {
        Node<T>* temp = new Node<T>;
        if(temp == NULL)
        {
            cout << "Stack Overflow\n";
            return;
        }
        temp->data = key;
        temp->next = top;
        top = temp;
    }
    length++;
}

template <class T>
T Stack<T>::peek() {
    if (top == nullptr) {
        return T();
    }
    else {
        return top->data;
    }
}

template <class T>
T Stack<T>::pop() {
    if (top == nullptr) {
        return T();
    }
    else {
        T temp = top->data;
        Node<T>* temp1 = top;
        top = top->next;
        delete temp1;
        length--;
        return temp;
    }
}

template <class T>
bool Stack<T>::empty() {
    return (length == 0);
}

template <class T>
bool Stack<T>::full() {
    Node<T>* temp = new Node<T>;

    if(temp == nullptr)
        return true;
    else
    {
        delete temp;
        return false;
    }
}

template <class T>
T Stack<T>::peek(int pos) {
    Node<T>* temp = top;
    while(pos-- != 1 && temp!= nullptr)
    {
        temp = temp->next;
    }
    if(temp == nullptr)
    {
        cout << "Invalid position\n";
        return T();
    }
    return temp->data;
}

template <class T>
Stack<T>::~Stack() {
}

#endif //STACK_STACK_H