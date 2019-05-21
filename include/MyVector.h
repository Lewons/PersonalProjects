//
// Created by Lewon Simonian on 2019-05-21.
//
// using C++11

#ifndef GITHUB_PERSONALPROJECTS_MYVECTOR_H
#define GITHUB_PERSONALPROJECTS_MYVECTOR_H

template <class T>
class MyVector {

public:

    using size_type = int32_t;
    using iterator = T*;

    MyVector(); //default constructor
    explicit MyVector(size_type size); //create vector with size default inserted instances of T
    MyVector(size_type size, const T& value); //Constructs the container with count copies of elements with value value
    MyVector(iterator first, iterator last); //copy elements within range into new vector
    MyVector(const MyVector& other); //copy constructor
    MyVector(MyVector&& other) noexcept ; //move constructor
    ~MyVector(); //destructor

    MyVector& operator= (const MyVector& other); //copy assignment operator
    MyVector& operator= (MyVector&& other) noexcept ; //move assignment operator
    T& operator[] (int index) const;

    void reserve(int n, bool copy = false);

    iterator insert(iterator pos, const T& value);
    template <typename... Args> iterator emplace(iterator pos, Args&& ...args);

    void push_back(const T& item);
    template <typename... Args> void emplace_back(Args&& ...args);

    void pop_back();
    iterator erase(iterator pos);
    iterator erase(iterator first, iterator last);

    bool empty() const { return m_size == 0; }

    iterator begin() const {return m_arr; }
    iterator end() const { return m_arr + m_size; }
    T& front() const { return m_arr[0]; }
    T& back() const { return m_arr[m_size - 1]; }

    size_type size() const { return m_size; }
    size_type capacity() const { return m_capacity; }
    iterator data() const { return m_arr; }
    T& at(int index) const;

    void clear();
    void shrink_to_fit();

    void swap(MyVector& other);

private:

    size_type m_size;
    size_type m_capacity;
    T* m_arr;

};


template <typename T>
MyVector<T>::MyVector() : m_size(0), m_capacity(0), m_arr(nullptr) { }

template <typename T>
MyVector<T>::MyVector(const size_type size)
        : m_size(size)
        , m_capacity(0)
        , m_arr(nullptr)
{
    reserve(size);
    for (int i = 0; i < size; ++i) {
        m_arr[i] = T();
    }
}

template <typename T>
MyVector<T>::MyVector(const size_type size, const T& value)
        : m_size(size)
        , m_capacity(0)
        , m_arr(nullptr)
{
    reserve(size);
    for (int i = 0; i < size; ++i) {
        m_arr[i] = value;
    }
}

template <typename T>
MyVector<T>::MyVector(const MyVector<T>::iterator first, const MyVector<T>::iterator last)
        : m_size(last - first)
        , m_capacity(0)
        , m_arr(nullptr)
{
    reserve(m_size);
    for (int i = 0; i < m_size; ++i) {
        m_arr[i] = *(first + i);
    }
}

template <typename T>
MyVector<T>::MyVector(const MyVector<T>& other)
        : m_size(other.m_size)
        , m_capacity(other.m_capacity)
        , m_arr(nullptr)
{
    reserve(m_size);
    for (int i = 0; i < m_size; ++i) {
        m_arr[i] = other.m_arr[i];
    }
}

template <typename T>
MyVector<T>::MyVector(MyVector<T>&& other) noexcept
        : m_size(other.m_size)
        , m_capacity(other.m_capacity)
        , m_arr(other.m_arr)
{
    other.m_arr = nullptr;
}

template <typename T>
MyVector<T>::~MyVector() {
    delete[] m_arr;
}

template <typename T>
MyVector<T>& MyVector<T>::operator=(const MyVector& other) {
    if(this == &other) {
        return *this;
    }

    reserve(other.m_size);

    m_size = other.m_size;
    m_capacity = other.m_capacity;
    for (int i = 0; i < m_size; ++i) {
        m_arr[i] = other.m_arr[i];
    }
    return *this;
}

template <typename T>
MyVector<T>& MyVector<T>::operator=(MyVector&& other) noexcept {
    if(this == &other) {
        return *this;
    }
    delete[] m_arr;
    m_arr = other.m_arr;

    m_size = other.m_size;
    m_capacity = other.m_capacity;

    other.m_arr = nullptr;
    return *this;
}

template <typename T>
T& MyVector<T>::operator[](int index) const {
    return m_arr[index];
}

template <typename T>
void MyVector<T>::reserve(int n, bool copy) {
    T* newArr = n ? new T[n] : nullptr;
    if(copy) {
        for(int i = 0; i < m_size; ++i) {
            newArr[i] = m_arr[i];
        }
    }
    if(m_arr != nullptr) {
        delete[] m_arr;
    }
    m_arr = newArr;
    m_capacity = n;
}

template <typename T>
void MyVector<T>::push_back(const T& item) {
    if(m_size == m_capacity) {
        if(m_capacity == 0) {
            reserve(1);
        }
        else {
            reserve(m_capacity * 2, true);
        }
    }
    m_arr[m_size] = item;
    m_size++;
}

template <typename T>
template <typename... Args>
void MyVector<T>::emplace_back(Args&& ...args) {
    if(m_size == m_capacity) {
        if(m_capacity == 0) {
            reserve(1);
        }
        else {
            reserve(m_capacity * 2, true);
        }
    }
    m_arr[m_size] = T(std::forward<Args>(args)...);
    m_size++;
}

template<class T>
void MyVector<T>::pop_back() {
    m_size--;
}

template<class T>
typename MyVector<T>::iterator MyVector<T>::erase(MyVector::iterator pos) {
    if(m_capacity == 0) {
        return m_arr;
    }
    for (int i = pos - m_arr; i < m_size - 1; ++i) {
        m_arr[i] = m_arr[i + 1];
    }
    m_size--;
    return pos;
}

template<class T>
typename MyVector<T>::iterator MyVector<T>::erase(MyVector::iterator first, MyVector::iterator last) {
    if(m_capacity == 0) {
        return m_arr;
    }
    uint32_t count = last - first;
    for (int i = first - m_arr; i < m_size - count; ++i) {
        m_arr[i] = m_arr[i + count];
    }
    m_size-=count;
    return first;
}

template <typename T>
typename MyVector<T>::iterator MyVector<T>::insert(MyVector::iterator pos, const T& value) {
    uint32_t index = pos - m_arr;
    if(m_size == m_capacity) {
        if(m_capacity == 0) {
            reserve(1);
        }
        else {
            reserve(m_capacity * 2, true);
        }
    }
    for (int i = m_size; i > index; --i) {
        m_arr[i + 1] = m_arr[i];
    }
    m_size++;
    return &(m_arr[index] = value);
}

template <typename T>
template <typename... Args>
typename MyVector<T>::iterator MyVector<T>::emplace(MyVector::iterator pos, Args&& ...args) {
    uint32_t index = pos - m_arr;
    if(m_size == m_capacity) {
        if(m_capacity == 0) {
            reserve(1);
        }
        else {
            reserve(m_capacity * 2, true);
        }
    }
    for (int i = m_size; i > index; --i) {
        m_arr[i + 1] = m_arr[i];
    }
    m_arr[index] = T(std::forward<Args>(args)...);
    m_size++;
    return &m_arr[index];
}

template <typename T>
T& MyVector<T>::at(int index) const {
    if(index < 0 || index > m_size) {
        //throw exception
    }
    else {
        return m_arr[index];
    }
}

template<class T>
void MyVector<T>::clear() {
    m_size = 0;
}

template <typename T>
void MyVector<T>::shrink_to_fit() {
    m_capacity = m_size;
}

template<class T>
void MyVector<T>::swap(MyVector<T>& other) {
    std::swap(m_arr,other.m_arr);
    std::swap(m_size,other.m_size);
    std::swap(m_capacity,other.m_capacity);
}

#endif //GITHUB_PERSONALPROJECTS_MYVECTOR_H
