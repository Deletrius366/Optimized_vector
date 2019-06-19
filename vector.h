//
// Created by User on 18.06.2019.
//

#include <memory>
#include <variant>

#ifndef SET_VECTOR_H
#define SET_VECTOR_H

template<typename T>
struct vector_iterator {

    typedef T value_type;
    typedef T &reference;
    typedef size_t difference_type;
    typedef T *pointer;
    typedef std::random_access_iterator_tag iterator_category;

    template<typename, typename> friend
    class vector;

    template<typename> friend
    class const_vector_iterator;

    vector_iterator() = default;

    vector_iterator(vector_iterator const &) = default;

    vector_iterator &operator=(vector_iterator const &) = default;

    vector_iterator &operator++() {
        ++ptr;
        return (*this);
    }

    const vector_iterator operator++(int) {
        vector_iterator aux(*this);
        ++(*this);
        return (aux);
    }

    vector_iterator &operator--() {
        --ptr;
        return (*this);
    }

    const vector_iterator operator--(int) {
        vector_iterator aux(*this);
        --(*this);
        return (aux);
    }

    vector_iterator &operator+=(size_t n) {
        ptr += n;
        return (*this);
    }

    vector_iterator &operator-=(size_t n) {
        ptr -= n;
        return (*this);
    }

    friend vector_iterator operator+(vector_iterator other, size_t n) {
        other += n;
        return other;
    }

    friend vector_iterator operator+(size_t n, vector_iterator other) {
        other += n;
        return other;
    }

    friend vector_iterator operator-(vector_iterator other, size_t n) {
        other -= n;
        return other;
    }

    friend difference_type operator-(vector_iterator const &it1, vector_iterator const &it2) {
        return it1.ptr - it2.ptr;
    }

    reference operator[](size_t ind) {
        return ptr[ind];
    };

    reference operator*() {
        return *ptr;
    }

    pointer operator->() {
        return ptr;
    }

    friend bool operator==(vector_iterator const &it1, vector_iterator const &it2) {
        return it1.ptr == it2.ptr;
    }

    friend bool operator!=(vector_iterator const &it1, vector_iterator const &it2) {
        return it1.ptr != it2.ptr;
    }

    friend bool operator<(vector_iterator const &it1, vector_iterator const &it2) {
        return it1.ptr < it2.ptr;
    }

    friend bool operator>(vector_iterator const &it1, vector_iterator const &it2) {
        return it2.ptr < it1.ptr;
    }

    friend bool operator<=(vector_iterator const &it1, vector_iterator const &it2) {
        return it1.ptr <= it2.ptr;
    }

    friend bool operator>=(vector_iterator const &it1, vector_iterator const &it2) {
        return it1.ptr >= it2.ptr;
    }

private:
    explicit vector_iterator(pointer p) : ptr(p) {}

    pointer ptr = nullptr;

};

template<typename T>
struct const_vector_iterator {

    typedef T value_type;
    typedef T const &reference;
    typedef size_t difference_type;
    typedef T const *pointer;
    typedef std::random_access_iterator_tag iterator_category;

    template<typename, typename> friend
    class vector;

    const_vector_iterator() = default;

    const_vector_iterator(const_vector_iterator const &) = default;

    const_vector_iterator &operator=(const_vector_iterator const &) = default;

    const_vector_iterator(vector_iterator<T> const &other) : ptr(other.ptr) {}

    const_vector_iterator &operator++() {
        ++ptr;
        return (*this);
    }

    const const_vector_iterator operator++(int) {
        vector_iterator aux(*this);
        ++(*this);
        return (aux);
    }

    const_vector_iterator &operator--() {
        --ptr;
        return (*this);
    }

    const const_vector_iterator operator--(int) {
        const_vector_iterator aux(*this);
        --(*this);
        return (aux);
    }

    const_vector_iterator &operator+=(size_t n) {
        ptr += n;
        return (*this);
    }

    const_vector_iterator &operator-=(size_t n) {
        ptr -= n;
        return (*this);
    }

    friend const_vector_iterator operator+(const_vector_iterator other, size_t n) {
        other += n;
        return other;
    }

    friend const_vector_iterator operator+(size_t n, const_vector_iterator other) {
        other += n;
        return other;
    }

    friend const_vector_iterator operator-(const_vector_iterator other, size_t n) {
        other -= n;
        return other;
    }

    friend difference_type operator-(const_vector_iterator const &it1, const_vector_iterator const &it2) {
        return it1.ptr - it2.ptr;
    }

    reference operator[](size_t ind) {
        return ptr[ind];
    };

    reference operator*() {
        return *ptr;
    }

    pointer operator->() {
        return ptr;
    }

    friend bool operator==(const_vector_iterator const &it1, const_vector_iterator const &it2) {
        return it1.ptr == it2.ptr;
    }

    friend bool operator!=(const_vector_iterator const &it1, const_vector_iterator const &it2) {
        return it1.ptr != it2.ptr;
    }

    friend bool operator<(const_vector_iterator const &it1, const_vector_iterator const &it2) {
        return it1.ptr < it2.ptr;
    }

    friend bool operator>(const_vector_iterator const &it1, const_vector_iterator const &it2) {
        return it2.ptr < it1.ptr;
    }

    friend bool operator<=(const_vector_iterator const &it1, const_vector_iterator const &it2) {
        return it1.ptr <= it2.ptr;
    }

    friend bool operator>=(const_vector_iterator const &it1, const_vector_iterator const &it2) {
        return it1.ptr >= it2.ptr;
    }

private:
    explicit const_vector_iterator(pointer p) : ptr(p) {}

    pointer ptr = nullptr;

};

template<class T, class Alloc = std::allocator<T>>
class vector {
    typedef std::allocator_traits<Alloc> alloc_traits;
    typedef typename alloc_traits::template rebind_alloc<char> rebind_allocator;

    struct aux {
        size_t capacity = 0;
        size_t size = 0;
        size_t count = 0;

        // T* shared;
        T *get_ptr() {
            return reinterpret_cast<T *>(&count + 1);
        };

        // aux() = default;

    };

    typedef std::variant<aux *, T> variant_;

    variant_ data_;
    // rebind_allocator alloc;

    void *alloc(size_t size) {
        return operator new(size);
    }

    aux *aux_alloc(size_t size) {
        aux *now = static_cast<aux *>(alloc(sizeof(aux) + sizeof(T) * size));
        now->capacity = size;
        now->size = 0;
        now->count = 0;
        return now;
    }

    template<typename ...Args>
    void construct(T *ptr, Args const &... args) {
        new(ptr) T(args...);
    }

    void construct(T *ptr, T *other) {
        new(ptr) T(*other);
    }

    void dealloc(aux *ptr) {
        operator delete(static_cast<void *> (ptr));
    }

    void deconstruct(T *ptr) {
        ptr->~T();
    }

    bool small() const noexcept {
        return data_.index() == 1 || aux_ptr() == nullptr;
    }

    bool unique() const noexcept {
        // assert(aux_ptr() == nullptr);
        return data_.index() == 1 || aux_ptr() == nullptr || aux_ptr()->count == 0;
    }

    size_t aux_capacity() const noexcept {
        return aux_ptr()->capacity;
    }

    size_t aux_size() const noexcept {
        assert(aux_ptr() != nullptr);
        return aux_ptr()->size;
    }

    aux *aux_ptr() const noexcept {
        assert(data_.index() == 0);
        return std::get<0>(data_);
    }

    aux *copy(size_t ind) {
        aux *new_aux = aux_alloc(aux_capacity());
        size_t i = 0;
        try {
            for (; i < std::min(ind, size()); ++i) {
                construct(new_aux->get_ptr() + i, aux_ptr()->get_ptr() + i);
            }
        } catch (...) {
            for (size_t j = 0; j < i; j++) {
                deconstruct(new_aux->get_ptr() + j);
            }
            dealloc(new_aux);
            throw;
        }
        new_aux->size = aux_size();
        new_aux->capacity = aux_capacity();
        new_aux->count = 0;
        return new_aux;
    }

    void detach() {
        if (data_.index() == 0 && aux_ptr() != nullptr && aux_ptr()->count > 0) {
            aux *new_aux = copy(aux_capacity());
            --aux_ptr()->count;
            data_ = new_aux;
        }
    }

    void push_back_long(T const &elem) {
        try {
            construct(aux_ptr()->get_ptr() + size(), elem);
            ++aux_ptr()->size;
        } catch (...) {
            throw;
        }
    }

public:

    typedef vector_iterator<T> iterator;
    typedef const_vector_iterator<T> const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    vector() noexcept = default;

    ~vector() noexcept {
        if (!small()) {
            if (aux_ptr()->count == 0) {
                for (size_t i = 0; i < size(); ++i) {
                    deconstruct(aux_ptr()->get_ptr() + i);
                }
                dealloc(aux_ptr());
            } else {
                --aux_ptr()->count;
            }
        }
    }

    size_t size() const noexcept {
        if (data_.index() == 0) {
            // std::cout << "rofl " << aux_ptr() << "\n";
            return aux_ptr() != nullptr ? aux_size() : 0;
        } else {
            return 1;
        }
    }

    bool empty() const noexcept {
        return data_.index() == 0 && (aux_ptr() == nullptr || aux_ptr()->size == 0);
    }

    vector(vector const &other) noexcept(false) : vector() {
        if (&other != this) {
            if (other.small()) {
                if (!other.empty())
                    push_back(other[0]);
            } else {
                data_ = other.aux_ptr();
                // data_ = std::get<0>(other.data_);
                ++aux_ptr()->count;
            }
        }
    }

    void clear() {
        if (!small() && aux_ptr()->count == 0) {
            for (size_t i = 0; i < size(); ++i) {
                deconstruct(aux_ptr()->get_ptr() + i);
            }
            dealloc(aux_ptr());
        }
        if (aux_ptr()->count != 0) {
            --aux_ptr()->count;
        }
        data_ = nullptr;
    }

    vector &operator=(vector const &other) {
        if (&other != this) {
            if (aux_ptr() != nullptr) clear();
            // std::cout << "after clear";
            data_ = other.data_;
            if (!other.small())
                ++other.aux_ptr()->count;

        }
        return (*this);
    }

    void reserve(size_t size) {
        if (size == 0) return;
        if (empty()) {
            aux *new_aux = aux_alloc(size + 8);
            new_aux->size = 0;
            data_ = new_aux;
        } else if (small()) {
            aux *new_aux = aux_alloc(size + 8);
            try {
                construct(aux_ptr()->get_ptr(), std::get<1>(data_));
            } catch (...) {
                dealloc(new_aux);
                throw;
            }
            new_aux->size = 1;
        } else {
            aux *new_aux = copy(size + aux_capacity());
            clear();
            data_ = new_aux;
        }
    }

    T const &front() const noexcept {
        return (*this)[0];
    }

    T &front() {
        detach();
        return (*this)[0];
    }

    T const &back() const noexcept {
        return (*this)[size() - 1];
    }

    T &back() {
        detach();
        return (*this)[size() - 1];
    }

    T *data() {
        detach();
        if (empty()) return nullptr;
        return &(*this)[0];
    }

    const T *data() const {
        if (empty()) return nullptr;
        return &(*this)[0];
    }

    iterator begin() {
        detach();
        if (empty()) return iterator(nullptr);
        return small() ? iterator(&(std::get<1>(data_))) : iterator(aux_ptr()->get_ptr());
    }

    const_iterator begin() const noexcept {
        if (empty()) return iterator(nullptr);
        return small() ? const_iterator(&(std::get<1>(data_))) : const_iterator(aux_ptr()->get_ptr());
    }

    iterator end() {
        detach();
        if (empty()) return iterator(nullptr);
        return small() ? iterator(&(std::get<1>(data_)) + 1) : iterator(aux_ptr()->get_ptr() + size());
    }

    const_iterator end() const noexcept {
        if (empty()) return iterator(nullptr);
        return small() ? const_iterator(&(std::get<1>(data_)) + 1) : const_iterator(aux_ptr()->get_ptr() + size());
    }

    const_iterator cbegin() const noexcept {
        return begin();
    }

    const_iterator cend() const noexcept {
        return end();
    }

    reverse_iterator rbegin() {
        detach();
        return reverse_iterator(end());
    }

    reverse_iterator rend() noexcept(false) {
        return reverse_iterator(begin());
    }

    const_reverse_iterator rbegin() const noexcept {
        return const_reverse_iterator(cend());
    }

    const_reverse_iterator rend() const noexcept {
        return const_reverse_iterator(cbegin());
    }

    const_reverse_iterator crbegin() const noexcept {
        return const_reverse_iterator(cend());
    }

    const_reverse_iterator crend() const noexcept {
        return const_reverse_iterator(cbegin());
    }

    void swap(vector &other) {
        if (this != &other) {
            bool b1 = data_.index() == 0;
            bool b2 = other.data_.index() == 0;
            if (b1 && b2) {
                std::swap(data_, other.data_);
                return;
            }
            if (!b1 && b2) {
                other.swap((*this));
                return;
            }
            if (b1 && !b2) {
                aux *prev = aux_ptr();
                try {
                    data_ = other.data_;
                } catch (...) {
                    other.data_ = nullptr;
                    data_ = prev;
                    throw;
                }
                other.data_ = prev;
            } else {
                try {
                    std::swap(data_, other.data_);
                } catch (...) {
                    data_ = nullptr;
                    other.data_ = nullptr;
                    throw;
                }
            }
        }
    }

    void push_back(T const &elem) {
        detach();
        if (empty() && aux_ptr() == nullptr) {
            try {
                data_ = elem;
            } catch (...) {
                data_ = nullptr;
                throw;
            }
        } else {
            // std::cout << "second elem" << "\n";
            if (small()) {
                aux *new_aux = aux_alloc(32);
                new_aux->size = 2;
                try {
                    construct(new_aux->get_ptr(), std::get<1>(data_));
                } catch (...) {
                    dealloc(new_aux);
                    throw;
                }
                try {
                    construct(new_aux->get_ptr() + 1, elem);
                } catch (...) {
                    deconstruct(new_aux->get_ptr());
                    dealloc(new_aux);
                    throw;
                }
                data_ = new_aux;
            } else {
                push_back_long(elem);
            }
        }
    }

    void pop_back() {
        if (!small()) {
            deconstruct(aux_ptr()->get_ptr() + size() - 1);
            --aux_ptr()->size;
        }
    }

    void insert(const_iterator it, const T& val) {
        detach();
        if (empty() || it == end() ) {
            std::cout << "end" << "\n";
            push_back(val);
        } else {
            vector tmp;
            // std::cout << "a" << "\n";
            tmp.reserve(size() + 2);
            // std::cout << "b" << "\n";
            // size_t pl = it-begin();
            auto i = cbegin();
            try {
                for (i = begin(); i < it; ++i) {
                    tmp.push_back(*i);
                }
                tmp.push_back(val);
                for (i = it; i != end(); ++i) {
                    tmp.push_back(*i);
                }
            } catch (...) {
                tmp.clear();
                throw;
            }
            *this = tmp;
        }
    }

    void erase(const_iterator it1, const_iterator it2) {
        detach();
        if (it2 == end()) {
            for (; it2 != it1; it2--) {
                pop_back();
            }
        } else {
            size_t pl = it1 - cbegin();
            size_t pr = it2 - cbegin();
            // size_t size = it1 - cbegin() + cend() - it2;
            size_t sizeNow = size() - (it2 - it1);
            vector tmp;
            tmp.reserve(sizeNow + 8);
            try {
                for (size_t i = 0; i < pl; i++) {
                    tmp.push_back((*this)[i]);
                }
                for (size_t i = pr; i < size(); i++) {
                    tmp.push_back((*this)[i]);
                }
            } catch (...) {
                tmp.clear();
                throw;
            }
            *this = tmp;
        }
    }

    void erase(const_iterator it1) {
        erase(it1, it1 + 1);
    }

    T &operator[](size_t ind) {
        detach();
        if (small()) {
            assert(ind == 0);
            return std::get<1>(data_);
        } else {
            assert(data_.index() == 0);
            assert(aux_ptr() != nullptr);
            assert(ind < size());
            return aux_ptr()->get_ptr()[ind];
        }
    }

    const T &operator[](size_t ind) const {
        if (small()) {
            assert(ind == 0);
            return std::get<1>(data_);
        } else {
            return aux_ptr()->get_ptr()[ind];
        }
    }

    template<typename C>
    friend bool operator==(vector<C> const &, vector<C> const &) noexcept;

    template<typename C>
    friend bool operator!=(vector<C> const &, vector<C> const &) noexcept;

    template<typename C>
    friend bool operator<=(vector<C> const &, vector<C> const &) noexcept;

    template<typename C>
    friend bool operator>=(vector<C> const &, vector<C> const &) noexcept;

    template<typename C>
    friend bool operator<(vector<C> const &, vector<C> const &) noexcept;

    template<typename C>
    friend bool operator>(vector<C> const &, vector<C> const &) noexcept;

    void cout() {
        for (size_t i = 0; i < size(); i++) {
            T const a = const_cast<const vector &>(*this)[i];
            std::cout << a << " ";
        }
        std::cout << "size: " << this->size() << "\n";
    }

};

template<typename T>
void swap(vector<T> &v1, vector<T> &v2) {
    return v1.swap(v2);
}

template<typename T>
bool operator==(vector<T> const &v1, vector<T> const &v2) noexcept {
    if (v1.size() != v2.size())
        return false;
    for (size_t i = 0; i < v1.size(); ++i) {
        if (v1[i] != v2[i])
            return false;
    }
    return true;
}

template<typename T>
bool operator!=(vector<T> const &v1, vector<T> const &v2) noexcept {
    return !(v1 == v2);
}

template<typename T>
bool operator<(vector<T> const &v1, vector<T> const &v2) noexcept {
    for (size_t i = 0; i < std::min(v1.size(), v2.size()); ++i) {
        if (v1[i] != v2[i]) return v1[i] < v2[i];
    }
    return v1.size() < v2.size();
}

template<typename T>
bool operator>(vector<T> const &v1, vector<T> const &v2) noexcept {
    return v2 < v1;
}

template<typename T>
bool operator<=(vector<T> const &v1, vector<T> const &v2) noexcept {
    return v1 < v2 || v1 == v2;
}

template<typename T>
bool operator>=(vector<T> const &v1, vector<T> const &v2) noexcept {
    return v1 > v2 || v2 == v1;
}


#endif //SET_VECTOR_H
