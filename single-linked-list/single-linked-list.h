#pragma once

#include <algorithm>
#include <cstddef>

template <typename Type>
class SingleLinkedList {
private:
    struct Node {
        Node() = default;
        Node(const Type& val, Node* next) : value(val), next_node(next) { }
        Type value;
        Node* next_node = nullptr;
    };

    mutable Node head_;
    size_t size_ = 0;

    template <typename ValueType>
    class BasicIterator {
        friend class SingleLinkedList;

    private:
        explicit BasicIterator(Node* node) noexcept : node_(node) { }

    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = Type;
        using difference_type = std::ptrdiff_t;
        using pointer = ValueType*;
        using reference = ValueType&;

        BasicIterator() = default;

        BasicIterator(const BasicIterator<Type>& other) noexcept {
            this->node_ = other.node_;
        }

        BasicIterator<ValueType>& operator=(const BasicIterator<ValueType>& rhs) = default;

        [[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept {
            return &this->node_->value == &*rhs ? true : false;
        }

        [[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept {
            return !(*this == rhs);
        }

        [[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept {
            return &this->node_->value == &*rhs ? true : false;
        }

        [[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept {
            return !(*this == rhs);
        }

        BasicIterator<ValueType>& operator++() noexcept {
            node_ = node_->next_node;
            return *this;
        }

        BasicIterator<ValueType> operator++(int) noexcept {
            BasicIterator previous(*this);
            node_ = node_->next_node;
            return previous;
        }

        [[nodiscard]] reference operator*() const noexcept {
            return node_->value;
        }

        [[nodiscard]] pointer operator->() const noexcept {
            return &node_->value;
        }
    private:
        Node* node_ = nullptr;
    };

    template <class T>
    void FillIn(const T& from) {
        try {
            SingleLinkedList<Type> temp;
            Node* it = &temp.head_;
            for (const Type& node : from) {
                it->next_node = new Node(node, nullptr);
                it = it->next_node;
                ++temp.size_;
            }
            this->swap(temp);
        }
        catch (const std::bad_alloc&) {
            throw std::bad_alloc();
        }
    }

public:
    using value_type = Type;
    using reference = value_type&;
    using const_reference = const value_type&;
    using Iterator = BasicIterator<Type>;
    using ConstIterator = BasicIterator<const Type>;

    SingleLinkedList() noexcept { }

    ~SingleLinkedList() noexcept {
        Clear();
    }

    SingleLinkedList(std::initializer_list<Type> values) {
        try {
            FillIn(values);
        }
        catch (const std::bad_alloc&) {
            throw std::bad_alloc();
        }
    }

    SingleLinkedList(const SingleLinkedList<Type>& other) {
        try {
            FillIn(other);
        }
        catch (const std::bad_alloc&) {
            throw std::bad_alloc();
        }
    }

    SingleLinkedList<Type>& operator=(const SingleLinkedList<Type>& other) {
        if (this != &other) {
            try {
                SingleLinkedList<Type> temp(other);
                this->swap(temp);
            }
            catch (const std::bad_alloc&) {
                throw std::bad_alloc();
            }
        }
        return *this;
    }

    void PushFront(const Type& value) {
        try {
            head_.next_node = new Node(value, head_.next_node);
            ++size_;
        }
        catch (const std::bad_alloc&) {
            throw std::bad_alloc();
        }
    }

    void Clear() noexcept {
        while (head_.next_node) {
            Node* to_delete = head_.next_node;
            head_.next_node = to_delete->next_node;
            delete to_delete;
        }
        size_ = 0;
    }

    [[nodiscard]] size_t GetSize() const noexcept {
        return size_;
    }

    [[nodiscard]] bool IsEmpty() const noexcept {
        return !size_ ? true : false;
    }

    Iterator InsertAfter(ConstIterator pos, const Type& value) {
        try {
            pos.node_->next_node = new Node(value, pos.node_->next_node);
            ++size_;
        }
        catch (const std::bad_alloc&) {
            throw std::bad_alloc();
        }
        return Iterator(pos.node_->next_node);
    }

    void PopFront() noexcept {
        if (size_ > 0) {
            Node* required = head_.next_node;
            head_.next_node = required->next_node;
            delete required;
        }
    }

    Iterator EraseAfter(ConstIterator pos) noexcept {
        Node* required = pos.node_->next_node;
        pos.node_->next_node = required->next_node;
        delete required;
        --size_;
        return Iterator(pos.node_->next_node);
    }

    void swap(SingleLinkedList<Type>& other) noexcept {
        std::swap(this->head_.next_node, other.head_.next_node);
        size_t temp_size = this->size_;
        this->size_ = other.size_;
        other.size_ = temp_size;
    }

    [[nodiscard]] Iterator begin() noexcept {
        Node* begin = &head_;
        return begin->next_node ? Iterator(begin->next_node) : Iterator(nullptr);
    }

    [[nodiscard]] Iterator end() noexcept {
        return Iterator(nullptr);
    }

    [[nodiscard]] ConstIterator begin() const noexcept {
        const Node* begin = &head_;
        return begin->next_node ? ConstIterator(begin->next_node) : ConstIterator(nullptr);
    }

    [[nodiscard]] ConstIterator end() const noexcept {
        return ConstIterator(nullptr);
    }

    [[nodiscard]] ConstIterator cbegin() const noexcept {
        const Node* begin = &head_;
        return begin->next_node ? ConstIterator(begin->next_node) : ConstIterator(nullptr);
    }

    [[nodiscard]] ConstIterator cend() const noexcept {
        return ConstIterator(nullptr);
    }

    [[nodiscard]] Iterator before_begin() noexcept {
        return Iterator(&head_);
    }

    [[nodiscard]] ConstIterator cbefore_begin() const noexcept {
        return ConstIterator(&head_);
    }

    [[nodiscard]] ConstIterator before_begin() const noexcept {
        return ConstIterator(&head_);
    }

    bool operator==(const SingleLinkedList<Type>& other) {
        return std::equal(this->begin(), this->end(), other.begin(), other.end());
    }

    bool operator!=(const SingleLinkedList<Type>& other) {
        return !std::equal(this->begin(), this->end(), other.begin(), other.end());;
    }

    bool operator<(const SingleLinkedList<Type>& other) {
        return std::lexicographical_compare(this->begin(), this->end(), other.begin(), other.end());
    }

    bool operator>(const SingleLinkedList<Type>& other) {
        return !std::lexicographical_compare(this->begin(), this->end(), other.begin(), other.end()) && *this != other;
    }

    bool operator<=(const SingleLinkedList<Type>& other) {
        return std::lexicographical_compare(this->begin(), this->end(), other.begin(), other.end()) || *this == other;
    }

    bool operator>=(const SingleLinkedList<Type>& other) {
        return !std::lexicographical_compare(this->begin(), this->end(), other.begin(), other.end());
    }
};

template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept {
    lhs.swap(rhs);
}