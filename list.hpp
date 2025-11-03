#ifndef SJTU_LIST_HPP
#define SJTU_LIST_HPP

#include "exceptions.hpp"
#include "algorithm.hpp"

#include <climits>
#include <cstddef>

namespace sjtu {
/**
 * a data container like std::list
 * allocate random memory addresses for data and they are doubly-linked in a list.
 */
template<typename T>
class list {
protected:
    class node {
    public:
        T *data;
        node *prev;
        node *next;

        // Constructor for sentinel node (no data)
        node() : data(nullptr), prev(nullptr), next(nullptr) {}

        // Constructor with data
        node(const T &value) : data(new T(value)), prev(nullptr), next(nullptr) {}

        // Destructor
        ~node() {
            if (data != nullptr) {
                delete data;
            }
        }
    };

protected:
    node *head;  // sentinel node
    node *tail;  // sentinel node
    size_t listSize;

    /**
     * insert node cur before node pos
     * return the inserted node cur
     */
    node *insert(node *pos, node *cur) {
        cur->next = pos;
        cur->prev = pos->prev;
        pos->prev->next = cur;
        pos->prev = cur;
        return cur;
    }

    /**
     * remove node pos from list (no need to delete the node)
     * return the removed node pos
     */
    node *erase(node *pos) {
        pos->prev->next = pos->next;
        pos->next->prev = pos->prev;
        return pos;
    }

public:
    class const_iterator;
    class iterator {
    private:
        node *ptr;
        const list *listPtr;

        friend class list<T>;
        friend class const_iterator;

    public:
        iterator(node *p = nullptr, const list *l = nullptr) : ptr(p), listPtr(l) {}

        /**
         * iter++
         */
        iterator operator++(int) {
            iterator temp = *this;
            ptr = ptr->next;
            return temp;
        }

        /**
         * ++iter
         */
        iterator & operator++() {
            ptr = ptr->next;
            return *this;
        }

        /**
         * iter--
         */
        iterator operator--(int) {
            iterator temp = *this;
            ptr = ptr->prev;
            return temp;
        }

        /**
         * --iter
         */
        iterator & operator--() {
            ptr = ptr->prev;
            return *this;
        }

        /**
         * *it
         * remember to throw if iterator is invalid
         */
        T & operator *() const {
            if (ptr == nullptr || ptr->data == nullptr) {
                throw invalid_iterator();
            }
            return *(ptr->data);
        }

        /**
         * it->field
         * remember to throw if iterator is invalid
         */
        T * operator ->() const {
            if (ptr == nullptr || ptr->data == nullptr) {
                throw invalid_iterator();
            }
            return ptr->data;
        }

        /**
         * a operator to check whether two iterators are same (pointing to the same memory).
         */
        bool operator==(const iterator &rhs) const {
            return ptr == rhs.ptr;
        }

        bool operator==(const const_iterator &rhs) const {
            return ptr == rhs.ptr;
        }

        /**
         * some other operator for iterator.
         */
        bool operator!=(const iterator &rhs) const {
            return ptr != rhs.ptr;
        }

        bool operator!=(const const_iterator &rhs) const {
            return ptr != rhs.ptr;
        }
    };

    /**
     * has same function as iterator, just for a const object.
     * should be able to construct from an iterator.
     */
    class const_iterator {
    private:
        node *ptr;
        const list *listPtr;

        friend class list<T>;

    public:
        const_iterator(node *p = nullptr, const list *l = nullptr) : ptr(p), listPtr(l) {}

        const_iterator(const iterator &other) : ptr(other.ptr), listPtr(other.listPtr) {}

        /**
         * iter++
         */
        const_iterator operator++(int) {
            const_iterator temp = *this;
            ptr = ptr->next;
            return temp;
        }

        /**
         * ++iter
         */
        const_iterator & operator++() {
            ptr = ptr->next;
            return *this;
        }

        /**
         * iter--
         */
        const_iterator operator--(int) {
            const_iterator temp = *this;
            ptr = ptr->prev;
            return temp;
        }

        /**
         * --iter
         */
        const_iterator & operator--() {
            ptr = ptr->prev;
            return *this;
        }

        /**
         * *it
         */
        const T & operator *() const {
            if (ptr == nullptr || ptr->data == nullptr) {
                throw invalid_iterator();
            }
            return *(ptr->data);
        }

        /**
         * it->field
         */
        const T * operator ->() const {
            if (ptr == nullptr || ptr->data == nullptr) {
                throw invalid_iterator();
            }
            return ptr->data;
        }

        bool operator==(const iterator &rhs) const {
            return ptr == rhs.ptr;
        }

        bool operator==(const const_iterator &rhs) const {
            return ptr == rhs.ptr;
        }

        bool operator!=(const iterator &rhs) const {
            return ptr != rhs.ptr;
        }

        bool operator!=(const const_iterator &rhs) const {
            return ptr != rhs.ptr;
        }
    };

    /**
     * Constructs
     * Atleast two: default constructor, copy constructor
     */
    list() : listSize(0) {
        head = new node();
        tail = new node();
        head->next = tail;
        tail->prev = head;
    }

    list(const list &other) : listSize(0) {
        head = new node();
        tail = new node();
        head->next = tail;
        tail->prev = head;

        for (node *cur = other.head->next; cur != other.tail; cur = cur->next) {
            push_back(*(cur->data));
        }
    }

    /**
     * Destructor
     */
    virtual ~list() {
        clear();
        delete head;
        delete tail;
    }

    /**
     * Assignment operator
     */
    list &operator=(const list &other) {
        if (this == &other) return *this;

        clear();
        for (node *cur = other.head->next; cur != other.tail; cur = cur->next) {
            push_back(*(cur->data));
        }
        return *this;
    }

    /**
     * access the first / last element
     * throw container_is_empty when the container is empty.
     */
    T & front() {
        if (empty()) {
            throw container_is_empty();
        }
        return *(head->next->data);
    }

    T & back() {
        if (empty()) {
            throw container_is_empty();
        }
        return *(tail->prev->data);
    }

    const T & front() const {
        if (empty()) {
            throw container_is_empty();
        }
        return *(head->next->data);
    }

    const T & back() const {
        if (empty()) {
            throw container_is_empty();
        }
        return *(tail->prev->data);
    }

    /**
     * returns an iterator to the beginning.
     */
    iterator begin() {
        return iterator(head->next, this);
    }

    const_iterator cbegin() const {
        return const_iterator(head->next, this);
    }

    /**
     * returns an iterator to the end.
     */
    iterator end() {
        return iterator(tail, this);
    }

    const_iterator cend() const {
        return const_iterator(tail, this);
    }

    /**
     * checks whether the container is empty.
     */
    virtual bool empty() const {
        return listSize == 0;
    }

    /**
     * returns the number of elements
     */
    virtual size_t size() const {
        return listSize;
    }

    /**
     * clears the contents
     */
    virtual void clear() {
        node *cur = head->next;
        while (cur != tail) {
            node *next = cur->next;
            delete cur;
            cur = next;
        }
        head->next = tail;
        tail->prev = head;
        listSize = 0;
    }

    /**
     * insert value before pos (pos may be the end() iterator)
     * return an iterator pointing to the inserted value
     * throw if the iterator is invalid
     */
    virtual iterator insert(iterator pos, const T &value) {
        if (pos.listPtr != this) {
            throw invalid_iterator();
        }
        node *newNode = new node(value);
        insert(pos.ptr, newNode);
        listSize++;
        return iterator(newNode, this);
    }

    /**
     * remove the element at pos (the end() iterator is invalid)
     * returns an iterator pointing to the following element, if pos pointing to the last element, end() will be returned.
     * throw if the container is empty, the iterator is invalid
     */
    virtual iterator erase(iterator pos) {
        if (empty()) {
            throw container_is_empty();
        }
        if (pos.listPtr != this || pos.ptr == nullptr || pos.ptr->data == nullptr) {
            throw invalid_iterator();
        }
        node *next = pos.ptr->next;
        erase(pos.ptr);
        delete pos.ptr;
        listSize--;
        return iterator(next, this);
    }

    /**
     * adds an element to the end
     */
    void push_back(const T &value) {
        node *newNode = new node(value);
        insert(tail, newNode);
        listSize++;
    }

    /**
     * removes the last element
     * throw when the container is empty.
     */
    void pop_back() {
        if (empty()) {
            throw container_is_empty();
        }
        node *last = tail->prev;
        erase(last);
        delete last;
        listSize--;
    }

    /**
     * inserts an element to the beginning.
     */
    void push_front(const T &value) {
        node *newNode = new node(value);
        insert(head->next, newNode);
        listSize++;
    }

    /**
     * removes the first element.
     * throw when the container is empty.
     */
    void pop_front() {
        if (empty()) {
            throw container_is_empty();
        }
        node *first = head->next;
        erase(first);
        delete first;
        listSize--;
    }

    /**
     * sort the values in ascending order with operator< of T
     */
    void sort() {
        if (listSize <= 1) return;

        // Allocate raw memory for array (no default constructor required)
        T **arr = new T*[listSize];
        size_t idx = 0;

        // Collect pointers to data
        for (node *cur = head->next; cur != tail; cur = cur->next) {
            arr[idx++] = cur->data;
        }

        // Sort array of pointers
        sjtu::sort<T*>(arr, arr + listSize, [](T* const &a, T* const &b) { return *a < *b; });

        // Relink nodes in sorted order
        node *cur = head->next;
        for (size_t i = 0; i < listSize; i++) {
            cur->data = arr[i];
            cur = cur->next;
        }

        delete[] arr;
    }

    /**
     * merge two sorted lists into one (both in ascending order)
     * compare with operator< of T
     * container other becomes empty after the operation
     * for equivalent elements in the two lists, the elements from *this shall always precede the elements from other
     * the order of equivalent elements of *this and other does not change.
     * no elements are copied or moved
     */
    void merge(list &other) {
        if (this == &other) return;

        node *cur1 = head->next;
        node *cur2 = other.head->next;

        while (cur1 != tail && cur2 != other.tail) {
            if (*(cur2->data) < *(cur1->data)) {
                node *next2 = cur2->next;
                // Remove from other
                other.erase(cur2);
                // Insert before cur1
                insert(cur1, cur2);
                cur2 = next2;
            } else {
                cur1 = cur1->next;
            }
        }

        // Append remaining elements from other
        while (cur2 != other.tail) {
            node *next2 = cur2->next;
            other.erase(cur2);
            insert(tail, cur2);
            cur2 = next2;
        }

        listSize += other.listSize;
        other.listSize = 0;
    }

    /**
     * reverse the order of the elements
     * no elements are copied or moved
     */
    void reverse() {
        if (listSize <= 1) return;

        node *cur = head->next;
        while (cur != tail) {
            node *temp = cur->next;
            cur->next = cur->prev;
            cur->prev = temp;
            cur = temp;
        }

        // Swap head and tail connections
        node *temp = head->next;
        head->next = tail->prev;
        tail->prev = temp;

        head->next->prev = head;
        tail->prev->next = tail;
    }

    /**
     * remove all consecutive duplicate elements from the container
     * only the first element in each group of equal elements is left
     * use operator== of T to compare the elements.
     */
    void unique() {
        if (listSize <= 1) return;

        node *cur = head->next;
        while (cur != tail && cur->next != tail) {
            if (*(cur->data) == *(cur->next->data)) {
                node *duplicate = cur->next;
                erase(duplicate);
                delete duplicate;
                listSize--;
            } else {
                cur = cur->next;
            }
        }
    }
};

}

#endif //SJTU_LIST_HPP
