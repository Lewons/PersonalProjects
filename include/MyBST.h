//
// Created by Lewon Simonian on 2019-06-03.
//

#ifndef EXERCISES_MYBST_H
#define EXERCISES_MYBST_H

template <class T>
class MyBST {

public:

    MyBST() : m_root(nullptr), m_nodeCounter(0) {};
    explicit MyBST(T firstElement);

    ~MyBST();

    void insert(const T& value);
    void insert(const std::vector<T>& values);

    bool exists(const T& value) const;

    T getSum() const;
    int getNoOfNodes() const;

    bool erase(const T& value);

private:

    struct Node {

        explicit Node(const T& data) : m_data(data) {}

        T m_data;
        std::unique_ptr<Node> m_leftNode;
        std::unique_ptr<Node> m_rightNode;

    };

    void insertNode(const std::unique_ptr<Node>& root, const T& value);
    bool existsNode(const std::unique_ptr<Node>& root, const T& value) const;

    T getSumSubtree(const std::unique_ptr<Node>& root) const;

    void deleteNode(std::unique_ptr<Node>& root);

    bool eraseNode(std::unique_ptr<Node>& node, const T& value);
    void swapAndEraseNode(std::unique_ptr<Node>& node);
    void appendSubtree(std::unique_ptr<Node>& node, std::unique_ptr<MyBST::Node>& parent);

    std::unique_ptr<Node> m_root;
    int m_nodeCounter;

};

template <class T>
MyBST<T>::MyBST(T firstElement) : m_root(nullptr), m_nodeCounter(0){
    std::unique_ptr<MyBST::Node> root(new MyBST::Node(firstElement));
    m_root = std::move(root);
    m_nodeCounter++;
}

template <class T>
MyBST<T>::~MyBST() {
    deleteNode(m_root);
}

template <class T>
void MyBST<T>::insert(const T& value) {
    insertNode(m_root, value);
}

template <class T>
void MyBST<T>::insert(const std::vector<T>& values) {
    for(const auto& value : values) {
        insertNode(m_root, value);
    }
}

template <class T>
void MyBST<T>::insertNode(const std::unique_ptr<MyBST::Node>& root, const T& value) {
    if(!root) {
        std::unique_ptr<MyBST::Node> node(new MyBST::Node(value));
        m_root = std::move(node);
        m_nodeCounter++;
        return;
    }
    if(value <= root->m_data) {
        if(root->m_leftNode == nullptr) {
            std::unique_ptr<MyBST::Node> node(new MyBST::Node(value));
            root->m_leftNode = std::move(node);
            m_nodeCounter++;
        }
        else {
            insertNode(root->m_leftNode,value);
        }
    }
    else {
        if(root->m_rightNode == nullptr) {
            std::unique_ptr<MyBST::Node> node(new MyBST::Node(value));
            root->m_rightNode = std::move(node);
            m_nodeCounter++;
        }
        else {
            insertNode(root->m_rightNode,value);
        }
    }
}

template <class T>
bool MyBST<T>::exists(const T& value) const {
    return existsNode(m_root, value);
}

template <class T>
bool MyBST<T>::existsNode(const std::unique_ptr<MyBST::Node>& root, const T& value) const {
    if(root == nullptr) {
        return false;
    }
    if(value < root->m_data) {
        return existsNode(root->m_leftNode,value);
    }
    else if (value > root->m_data) {
        return existsNode(root->m_rightNode,value);
    }
    else {
        return true;
    }
}

template <class T>
T MyBST<T>::getSum() const {
    if(m_root) {
        return getSumSubtree(m_root);
    }
    else {
        return T();
    }
}

template <class T>
T MyBST<T>::getSumSubtree(const std::unique_ptr<MyBST::Node>& root) const {
    T result = root->m_data;
    if(root->m_leftNode) {
        result += getSumSubtree(root->m_leftNode);
    }
    if(root->m_rightNode) {
        result += getSumSubtree(root->m_rightNode);
    }
    return result;
}

template <class T>
void MyBST<T>::deleteNode(std::unique_ptr<MyBST::Node>& root) {
    if(root == nullptr) {
        return;
    }
    deleteNode(root->m_leftNode);
    deleteNode(root->m_rightNode);
    root.reset();
    m_nodeCounter--;
}

template <class T>
int MyBST<T>::getNoOfNodes() const {
    return m_nodeCounter;
}

template <class T>
bool MyBST<T>::erase(const T& value) {
    return eraseNode(m_root,value);
}

template <class T>
bool MyBST<T>::eraseNode(std::unique_ptr<MyBST::Node>& node, const T& value) {
    if(node == nullptr) {
        return false;
    }
    if(value < node->m_data) {
        return eraseNode(node->m_leftNode,value);
    }
    else if(value > node->m_data) {
        return eraseNode(node->m_rightNode,value);
    }
    else {
        swapAndEraseNode(node);
        return true;
    }
}

template <class T>
void MyBST<T>::swapAndEraseNode(std::unique_ptr<MyBST::Node>& node) {
    std::unique_ptr<MyBST::Node> tmp = std::move(node);
    if(tmp->m_leftNode != nullptr && tmp->m_rightNode != nullptr) {
        node = std::move(tmp->m_leftNode);
        if(node->m_rightNode != nullptr) {
            node->m_rightNode.swap(tmp->m_rightNode);
            appendSubtree(tmp->m_rightNode,m_root);
        }
        else {
            node->m_rightNode = std::move(tmp->m_rightNode);
        }
    }
    else if(node->m_leftNode != nullptr) {
        node.swap(tmp->m_leftNode);
    }
    else {
        node.swap(tmp->m_rightNode);
    }
    tmp.reset();
    m_nodeCounter--;
}

template <class T>
void MyBST<T>::appendSubtree(std::unique_ptr<MyBST::Node>& node, std::unique_ptr<MyBST::Node>& parent) {
    if(node->m_data <= parent->m_data) {
        if(parent->m_leftNode == nullptr) {
            parent->m_leftNode = std::move(node);
            return;
        }
        else {
            appendSubtree(node,parent->m_leftNode);
        }
    }
    else {
        if(parent->m_rightNode == nullptr) {
            parent->m_rightNode = std::move(node);
        }
        else {
            appendSubtree(node,parent->m_rightNode);
            return;
        }
    }
}

#endif //EXERCISES_MYBST_H
