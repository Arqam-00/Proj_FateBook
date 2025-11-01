#pragma once
#include<iostream>
using namespace std;

template <typename T>
class DoublyList {
private:
    struct Node {
        T data;
        Node* prev;
        Node* next;
        Node(const T& value, Node* p = nullptr, Node* n = nullptr)
            : data(value), prev(p), next(n) {
        }
    };
    Node* head = nullptr;
    Node* tail = nullptr;
    int Size = 0;
public:
    class Iterator {
    private:
        Node* current = nullptr;
        Iterator(Node* ptr) :current(ptr) {}
        friend class DoublyList<T>;
    public:

        T& operator*() { return current->data; }

        Iterator& operator++() { return current = current->next; }
        Iterator operator++(int) {
            auto temp = current;
            current = current->next;
            return temp;
        }

        Iterator& operator--() { return current = current->prev; }
        Iterator operator--(int) {
            auto temp = current;
            current = current->prev;
            return temp;
        }

        bool operator==(const Iterator& other) const { return current == other.current; }
        bool operator!=(const Iterator& other) const { return current != other.current; }
    };

    DoublyList() {}
    ~DoublyList() {
        while (head) {
            auto temp = head;
            head = head->next;
            delete temp;
        }
    }
    DoublyList(const DoublyList& other) {
        for (auto i = other.begin(); i != other.end(); i++) {
            this->push_back(*i);
        }
    }
    DoublyList& operator=(const DoublyList& other) {
        if (this == &other) return *this;
        this->~DoublyList();
        for (auto i = other.begin(); i != other.end(); i++) {
            this->push_back(*i);
        }
        return *this;
    }

    bool empty() const { return Size == 0; }
    int size() const { return Size; }

    T& front() {
        if (!head)throw exception("The List is empty!");
        return head->data;
    }
    const T& front() const {
        if (!head)throw exception("The List is empty!");
        return head->data;
    }

    T& back() {
        if (!head)throw exception("The List is empty!");
        return tail->data;
    }
    const T& back() const {
        if (!head)throw exception("The List is empty!");
        return tail->data;
    }

    T& operator[](int i) {
        if (i < 0 || i >= Size) throw exception("Index out of range");
        if (i < Size / 2) {
            auto temp = head;
            for (int j = 0; j < i; j++) {
                temp = temp->next;
            }
            return temp->data;
        }
        else {
            auto temp = tail;
            for (int j = Size - 1; j > i; j--) {
                temp = temp->prev;
            }
            return temp->data;
        }
    }

    const T& operator[](int i) const {
        if (i < 0 || i >= Size) throw exception("Index out of range");
        if (i < Size / 2) {
            auto temp = head;
            for (int j = 0; j < i; j++) {
                temp = temp->next;
            }
            return temp->data;
        }
        else {
            auto temp = tail;
            for (int j = Size - 1; j > i; j--) {
                temp = temp->prev;
            }
            return temp->data;
        }
    }

    void push_front(const T& value) {
        Size++;
        if (!head) {
            head = tail = new Node(value);
            return;
        }
        head = head->prev = new Node(value, nullptr, head);
    }
    void push_back(const T& value) {
        Size++;
        if (!head) {
            head = tail = new Node(value);
            return;
        }
        tail = tail->next = new Node(value, tail, nullptr);
    }

    void pop_front() {
        if (!head)throw exception("The List is empty!");
        Size--;
        if (head == tail) {
            delete head;
            head = tail = nullptr;
            return;
        }
        auto temp = head;
        head = head->next;
        head->prev = nullptr;
        delete temp;
    }
    void pop_back() {
        if (!tail)throw exception("The List is empty!");
        Size--;
        if (head == tail) {
            delete head;
            head = tail = nullptr;
            return;
        }
        auto temp = tail;
        tail->prev->next = nullptr;
        tail = tail->prev;
        delete temp;
    }

    void insert_at(Iterator pos, const T& value) {
        if (!pos.current) {
            push_back(value);
            return;
        }
        if (!pos.current->prev) {
            push_front(value);
            return;
        }
        Size++;
        Node* Temp = new Node(value, pos.current->prev, pos.current);
        pos.current->prev->next = Temp;
        pos.current->prev = Temp;
    }
    void erase_at(Iterator pos) {
        if (!pos.current)return;
        if (!pos.current->next) {
            pop_back();
            return;
        }
        if (!pos.current->prev) {
            pop_front();
            return;
        }
        Size--;
        auto temp = pos.current;
        pos.current->prev->next = pos.current->next;
        pos.current->next->prev = pos.current->prev;
        delete temp;
    }

    Iterator begin() const { return Iterator(head); }
    Iterator end() const { return Iterator(nullptr); }
    Iterator rbegin() const { return Iterator(tail); }
    Iterator rend() const { return Iterator(nullptr); }

    friend ostream& operator<<(ostream& os, const DoublyList<T>& DLL) {
        os << "{ ";
        for (auto i = DLL.begin(); i != DLL.end(); i++) {
            os << *i << " ";
        }
        return os << "}";
    }


};

