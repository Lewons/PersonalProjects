//
// Created by Lewon Simonian on 2019-06-05.
//

#ifndef EXERCISES_MYDOUBLYLL_H
#define EXERCISES_MYDOUBLYLL_H

template <class T> class MyDoublyLL;

template <class T>
class MyListIterator : public std::iterator<std::bidirectional_iterator_tag, typename MyDoublyLL<T>::Node> {

public:

    explicit MyListIterator(std::shared_ptr<typename MyDoublyLL<T>::Node> ptr = nullptr) { m_ptr = ptr; }
    MyListIterator(const MyListIterator& iter) { m_ptr = iter.m_ptr; }
    ~MyListIterator() = default;

    MyListIterator& operator= (std::shared_ptr<typename MyDoublyLL<T>::Node> ptr) { m_ptr = ptr; return *this; };
    MyListIterator& operator= (const MyListIterator& iter) { m_ptr = iter.m_ptr; return *this; }

    explicit operator bool () const noexcept {
        if(m_ptr) {
            return true;
        }
        else {
            return false;
        }
    };

    bool operator== (const MyListIterator& iter) const noexcept { return m_ptr == iter.m_ptr; }
    bool operator!= (const MyListIterator& iter) const noexcept { return m_ptr != iter.m_ptr; }

    MyListIterator& operator++ () noexcept {
        if(m_ptr->next) {
            m_ptr = m_ptr->next;
        }
        return *this;
    }

    MyListIterator operator++ (int) {
        MyListIterator copy(*this);
        if(m_ptr->next) {
            ++*this;
        }
        return copy;
    }

    MyListIterator& operator-- () noexcept {
        if(m_ptr->prev) {
            m_ptr = m_ptr->prev;
        }
        return *this;
    }

    MyListIterator operator-- (int) {
        MyListIterator copy(*this);
        if(m_ptr->prev) {
            --*this;
        }
        return copy;
    }

    MyListIterator& operator+= (int diff) noexcept {
        for (int i = 0; i < std::abs(diff); ++i) {
            if(diff < 0) {
                --*this;
            }
            else {
                ++*this;
            }
        }
        return *this;
    }

    MyListIterator& operator-= (int diff) noexcept {
        for (int i = 0; i < std::abs(diff); ++i) {
            if(diff < 0) {
                ++*this;
            }
            else {
                --*this;
            }
        }
        return *this;
    }

    MyListIterator operator+ (int diff) {
        MyListIterator copy(*this);
        copy += diff;
        return copy;
    }

    MyListIterator operator- (int diff) {
        MyListIterator copy(*this);
        copy -= diff;
        return copy;
    }

    T& operator* () { return m_ptr->m_data; }
    const T& operator* () const { return m_ptr->m_data; }

    std::shared_ptr<typename MyDoublyLL<T>::Node> get_ptr() { return m_ptr; }
    const std::shared_ptr<typename MyDoublyLL<T>::Node> get_const_ptr() { return m_ptr; }

protected:

    std::shared_ptr<typename MyDoublyLL<T>::Node> m_ptr;

};



template <class T>
class MyDoublyLL {

public:

    using size_type = int32_t;
    using iterator = MyListIterator<T>;
    using const_iterator = const MyListIterator<T>;

    explicit MyDoublyLL();
    explicit MyDoublyLL(size_type count, const T& value = T());
    MyDoublyLL(iterator first, iterator last);

    MyDoublyLL(const MyDoublyLL& other);
    MyDoublyLL(MyDoublyLL&& other) noexcept;
    MyDoublyLL(std::initializer_list<T> init);

    T& front() noexcept { return m_head->m_data; }
    T& back() noexcept { return m_tail->m_data; }

    iterator begin() const noexcept { return iterator(m_head); }
    iterator end() const noexcept { return iterator(m_end);}

    const_iterator cbegin() const noexcept { return const_iterator(m_head); }
    const_iterator cend() const noexcept { return const_iterator(m_end); }

    size_type size() const noexcept { return m_size; }
    bool empty() const noexcept { return m_size == 0; }

    iterator erase(iterator pos) noexcept;
    void clear() noexcept;

    void push_back(const T& value);
    void push_front(const T& value);


private:

    struct Node {

        Node() : m_data(T()) {}
        explicit Node(const T& data) : m_data(data) {}
        T m_data;
        std::shared_ptr<Node> prev;
        std::shared_ptr<Node> next;

    };

    std::shared_ptr<Node> m_head;
    std::shared_ptr<Node> m_tail;
    std::shared_ptr<Node> m_end;

    friend class MyListIterator<T>;

    size_type m_size;

};

template <class T>
MyDoublyLL<T>::MyDoublyLL() : m_end(std::make_shared<Node>()), m_size(0) {
    m_head = m_end;
    m_tail = m_end;
}

template <class T>
MyDoublyLL<T>::MyDoublyLL(size_type count, const T& value)
    : m_end(std::make_shared<Node>())
    , m_size(0)
{
    m_head = m_end;
    m_tail = m_end;
    for (int i = 0; i < count; ++i) {
        push_back(value);
    }
}

template <class T>
MyDoublyLL<T>::MyDoublyLL(MyDoublyLL::iterator first, MyDoublyLL::iterator last)
    : m_end(std::make_shared<Node>())
    , m_size(0)
{
    m_head = m_end;
    m_tail = m_end;
    while(first != last && first.get_const_ptr()->next) {
        push_back(*first++);
    }
}

template <class T>
MyDoublyLL<T>::MyDoublyLL(const MyDoublyLL& other)
    : m_end(std::make_shared<Node>())
    , m_size(0)
{
    m_head = m_end;
    m_tail = m_end;
    for(const T& el : other) {
        push_back(el);
    }
}

template <class T>
MyDoublyLL<T>::MyDoublyLL(MyDoublyLL&& other) noexcept
    : m_end(other.m_end)
    , m_head(other.m_head)
    , m_tail(other.m_tail)
    , m_size(other.m_size)
{
    other.m_end.reset();
    other.m_head.reset();
    other.m_tail.reset();
}

template <class T>
MyDoublyLL<T>::MyDoublyLL(std::initializer_list<T> init)
    : m_end(std::make_shared<Node>())
    , m_size(0)
{
    m_head = m_end;
    m_tail = m_end;
    for(const T& el : init) {
        push_back(el);
    }
}

template <class T>
typename MyDoublyLL<T>::iterator MyDoublyLL<T>::erase(MyDoublyLL::iterator pos) noexcept {
    if(pos.get_ptr() == m_end) { return end(); }
    auto before = pos.get_ptr()->prev;
    auto after = pos.get_ptr()->next;
    if(before) {
        before->next = after;
    }
    else {
        m_head = after;
    }
    if(after != m_end) {
        after->prev = before;
    }
    else {
        m_tail = before;
        m_end->prev = m_tail;
    }
    pos.get_ptr().reset();
    if(--m_size == 0) { m_tail = m_end; }
    return iterator(after);
}

template <class T>
void MyDoublyLL<T>::clear() noexcept {
    auto it = begin();
    while(it != end()) {
        it = erase(it);
    }
}

template <class T>
void MyDoublyLL<T>::push_back(const T& value) {
    //create Node and set its prev ptr to the current tail and the next ptr to the end Node
    std::shared_ptr<Node> node = std::make_shared<Node>(value);
    node->next = m_end;
    if(m_tail != m_end) {
        //if there is already a tail (list was not empty) set the new Node's prev to old tail and old tail's next ptr to new Node
        node->prev = m_tail;
        m_tail->next = node;
    }
    m_tail = node; //set new Node as tail Node
    m_end->prev = m_tail; //set end Node's prev ptr to new Node
                        // (so that you can decrement from end() and get to the last element of list)
    if(m_head == m_end) {
        //if that was list's first element, set head equal to tail
        m_head = m_tail;
    }
    ++m_size;
}

template <class T>
void MyDoublyLL<T>::push_front(const T& value) {
    //create Node and set its next ptr to the current head
    std::shared_ptr<Node> node = std::make_shared<Node>(value);
    node->next = m_head;
    if(m_head != m_end) {
        //if there was already a head (list was not empty) set the old head's prev ptr to new Node
        m_head->prev = node;
    }
    m_head = node; //set new Node as head Node
    if(m_tail == m_end) {
        //if that was list's first element, set tail equal to head
        m_tail = m_head;
    }
    ++m_size;
}

#endif //EXERCISES_MYDOUBLYLL_H