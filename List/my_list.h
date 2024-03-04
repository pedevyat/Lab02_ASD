#include <initializer_list>
#include <memory>
#include <iterator>
#include <map>


template<typename T>
class MyList
{
private:

    struct Node
    {
        T data;
        Node* next;
        Node* prev;
        Node(const T& data, Node* prev = nullptr, Node* next = nullptr) : data(data), prev(prev), next(next) {}
        Node(T&& data, Node* prev = nullptr, Node* next = nullptr) : data(std::move(data)), prev(prev), next(next) {}
    };
    Node* fict; // фиктивная вершина
    size_t sz;

public:
    using value_type      = T;
    using size_type       = size_t;
    using difference_type = ptrdiff_t;
    using pointer         = value_type*;
    using const_pointer   = const value_type*;
    using reference       = value_type&;
    using const_reference = const value_type&;

    template<typename Pointer, typename Reference>
    class BaseIterator : public std::iterator<std::bidirectional_iterator_tag, value_type, difference_type, Pointer, Reference>
    {    
    public:
        Node* p;
        BaseIterator(Node* p) : p(p) {}
        bool operator==(const BaseIterator& other) const
        {
            return p == other.p;
        }
        bool operator!=(const BaseIterator& other) const
        {
            return p != other.p;
        }
        pointer operator->()
        {
            return p->data;
        }
        BaseIterator& operator--()
        {
            p = p->prev;
            return *this;
        }
        BaseIterator& operator--(int)
        {
            BaseIterator& t(*this);
            --(*this);
            return t;
        }
        BaseIterator& operator++()
        {
            p = p->next;
            return *this;
        }
        BaseIterator& operator++(int)
        {
            BaseIterator& t (*this);
            ++(*this);
            return t;
        }
        Reference operator*() 
        {
            return p->data;
        }
    };

    

    using iterator               = BaseIterator<pointer, reference>;
    using const_iterator         = BaseIterator<const_pointer, const_reference>;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    MyList()
    {
        /*this->head = nullptr;
        this->tail = nullptr;
        this->sz = 0;*/
        void* elem = operator new(sizeof(Node));
        fict = static_cast<Node*>(elem);
        fict->next = fict;
        fict->prev = fict;
        this->sz = 0;
    }
    MyList(const MyList& other) : MyList(other.cbegin(), other.cend()) {}
    
    MyList(MyList&& other) : fict(other.fict), sz(other.sz)
    {
        other.fict = nullptr;
        other._size = 0;
    }

    MyList(std::initializer_list<T> init) : MyList() 
    {
        for (auto x : init)
            push_back(x);
    }

    MyList& operator=(const MyList& other)
    {
        if (this != &other)
        {
            MyList temp (other);
            swap(temp);
        }
        return *this;
    }
    MyList& operator=(MyList&& other) noexcept
    {
        if (this != &other)
        {
            clear();
            fict = other.fict;
            sz = other.sz;
        }
        return *this;
    }
    MyList& operator=(std::initializer_list<T> ilist)
    {
        MyList temp(ilist);
        swap(temp);
        return *this;
    }

    reference front()
    {
        return fict->next->data;
    }
    const_reference front() const
    {
        return fict->next->data;
    }
    reference back()
    {
        return fict->prev->data;
    }
    const_reference back() const
    {
        return fict->prev->data;
    }

    iterator begin() noexcept
    {
        return iterator(fict->next);
    }
    const_iterator begin() const noexcept
    {
        return const_iterator(fict->next);
    }
    const_iterator cbegin() const noexcept
    {
        return const_iterator(fict->next);
    }
    iterator end() noexcept
    {
        return iterator(fict);
    }
    const_iterator end() const noexcept
    {
        return const_iterator(fict);
    }
    const_iterator cend() const noexcept
    {
        return const_iterator(fict);
    }
    reverse_iterator rbegin() noexcept
    {
        return reverse_iterator(end());
    }
    const_reverse_iterator rbegin() const noexcept
    {
        return const_reverse_iterator(end());
    }
    const_reverse_iterator crbegin() const noexcept
    {
        return const_reverse_iterator(cend());
    }
    reverse_iterator rend() noexcept
    {
        return reverse_iterator(begin());
    }
    const_reverse_iterator rend() const noexcept
    {
        return const_reverse_iterator(begin());
    }
    const_reverse_iterator crend() const noexcept
    {
        return const_reverse_iterator(cbegin());
    }

    bool empty() const noexcept
    {
        return sz == 0;
    }
    size_type size() const noexcept
    {
        return sz;
    }

    void clear() noexcept
    {
        while (!empty())
            pop_back();
    }
    iterator insert(iterator pos, const T& value)
    {
        Node* newNode = new Node(value);
        newNode->prev = pos.p->prev;
        newNode->next = pos.p;
        pos.p->prev->next = newNode;
        pos.p->prev = newNode;
        return iterator(newNode);
    }
    iterator insert(const_iterator pos, const T& value)
    {
        Node* newNode = new Node(value);
        newNode->prev = pos.p->prev;
        newNode->next = pos.p;
        pos.p->prev->next = newNode;
        pos.p->prev = newNode;
        return iterator(newNode);
    }
    iterator insert(const_iterator pos, T&& value)
    {
        Node* newNode = new Node(value);
        newNode->prev = pos.p->prev;
        newNode->next = pos.p;
        pos.p->prev->next = newNode;
        pos.p->prev = newNode;
        return iterator(newNode);
    }

    iterator erase(iterator pos)
    {
        Node* node = pos.p;
        if (node->prev)
            node->prev->next = node->next;
        else
            fict->next = node->next;
        if (node->next)
            node->next->prev = node->prev;
        else
            fict->prev = node->prev;
        iterator nextIterator(node->next);
        delete node;
        sz--;
        return nextIterator;
    }
    iterator erase(const_iterator pos)
    {
        Node* node = pos.p;
        if (node->prev)
            node->prev->next = node->next;
        else
            fict->next = node->next;
        if (node->next)
            node->next->prev = node->prev;
        else
            fict->prev = node->prev;
        iterator nextIterator(node->next);
        delete node;
        sz--;
        return nextIterator;
    }
    iterator erase(iterator first, iterator last)
    {
        while (first != last)
            first = erase(first);
        return last;
    }
    iterator erase(const_iterator first, const_iterator last)
    {
        while (first != last)
            first = erase(first);
        return last;
    }

    void push_back(const T& value)
    {
        Node* newNode = new Node(value);
        newNode->next = fict;
        newNode->prev = fict->prev;
        fict->prev->next = newNode;
        fict->prev = newNode;
        sz++;
    }
    void push_back(T&& value)
    {
        Node* newNode = new Node(std::move(value));
        newNode->next = fict;
        newNode->prev = fict->prev;
        fict->prev->next = newNode;
        fict->prev = newNode;
        sz++;
    }
    void pop_back()
    {
        Node* tmp = fict->prev;
        fict->prev = tmp->prev;
        tmp->prev->next = fict;
        delete tmp;
        sz--;
    }
    void push_front(const T& value)
    {
        Node* newNode = new Node(value);
        newNode->next = fict->next;
        newNode->prev = fict;
        fict->next->prev = newNode;
        fict->next = newNode;
        sz++;
    }
    void push_front(T&& value)
    {
         Node* newNode = new Node(std::move(value));
         newNode->next = fict->next;
         newNode->prev = fict;
         fict->next->prev = newNode;
         fict->next = newNode;
         sz++;
    }
    void pop_front()
    {
        Node* tmp = fict->next;
        fict->next = tmp->next;
        tmp->next->prev = fict;
        delete tmp;
        sz--;
    }

    void swap(MyList& other) noexcept
    {
        std::swap(fict, other.fict);
        std::swap(sz, other.sz);
    }

    // Разворачивает список в обратном направлении, меняя указатели на элементы
    void reverse()
    {
        Node* n = fict->next;
        Node* temp = fict->prev;
        fict->prev = fict->next;
        fict->next = temp;
        while (n != fict) 
        {
            temp = n->prev;
            n->prev = n->next;
            n->next = temp;
            n = n->prev;
        }
    }

    // Подсчитывает количество положительных элементов в списке
    size_t CountPosElems() 
    {
        Node* n = fict->next;
        size_t count = 0;
        while (n != fict)
        {
            if (n->data > 0)
                count++;
            n = n->next;
        }
        return count;
    }

    // Находит в списке максимальный элемент, и число вхождений этого элемента в список;
    std::map<T, int> SearchMaxElemAndCountEntry()
    {
        Node* n = fict->next;
        int count_entry = 1;
        T max_elem = n->data;
        while (n != fict)
        {
            if (max_elem < n->data)
            {
                max_elem = n->data;
                count_entry = 1;
            }
            else if (max_elem == n->data)
                count_entry++;
            n = n->next;
        }
        std::map<T, int> m{ {max_elem, count_entry} };
        return m;
    }

    // Делит список на два, переместив в один отрицательные элементы, а в другой — неотрицательные.
    void SplitNegativeAndNonNegative(MyList& neg, MyList& nneg)
    {
        Node* n = fict->next;
        while (n != fict)
        {
            if (n->data >= 0)
            {
                nneg.push_back(n->data);
            }
            else
            {
                neg.push_back(n->data);
            }               
            //n->data = nullptr;
            n = n->next;
        }
    }


    ~MyList()
    {
        clear();
    }

};
