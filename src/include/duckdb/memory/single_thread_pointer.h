//
// Created by Azim Afroozeh on 1/27/21.
/*
 * it works
 * all test are passed
 * but has leakage
 */
//


namespace azim {

#include <cstdlib>  // nullptr_t

template<typename T>
class shared_ptr {
public:
    T* ptr;            // contained pointer
    uint64_t * ref_count;             // reference counter

public:
    // Default constructor, constructs an empty shared_ptr.
    shared_ptr() : ptr (nullptr), ref_count(nullptr){
    }
    // Construct empty shared_ptr.
    shared_ptr(std::nullptr_t) : ptr (nullptr), ref_count(nullptr){
    }
    // Construct a shared_ptr that wraps raw pointer.
    shared_ptr(T* p) : ptr{ p } , ref_count{nullptr}{
        ref_count = p ? new uint64_t(1) : nullptr;
    }
    // Construct a shared_ptr that wraps raw pointer of convertible type.
    template<typename U>
    shared_ptr(U* p) : ptr{ p }, ref_count{ p ? new uint64_t(1) : nullptr} { }

    // Copy  constructor.
    shared_ptr(const shared_ptr& sp) noexcept : ptr{ sp.ptr }, ref_count{ sp.ref_count } {
        if (ptr)
            ++(*ref_count);
    }
    // Conversion constructor.
    template<typename U>
    shared_ptr(const shared_ptr<U>& sp) noexcept : ptr{sp.ptr}, ref_count{sp.ref_count} {
        if (ptr)
            ++(*ref_count);
    }

    // Copy  constructor.
    shared_ptr(shared_ptr&& sp) noexcept : ptr{ sp.ptr }, ref_count{ sp.ref_count } {
        sp.ptr = nullptr;
        sp.ref_count = nullptr;
    }
    // Conversion constructor.
    template<typename U>
    shared_ptr(shared_ptr<U>&& sp) noexcept : ptr{sp.ptr}, ref_count{sp.ref_count} {
        sp.ptr = nullptr;
        sp.ref_count = nullptr;
    }


    // No effect if shared_ptr is empty or use_count() > 1, otherwise release the resources.
    ~shared_ptr() {
        if (ptr && --(*ref_count) == 0)
        {
            delete ref_count;
            delete ptr;
        }
    }



    // Copy assignment.
    shared_ptr& operator= (const shared_ptr& sp) noexcept {
        this->ptr = sp.ptr;
        this->ref_count = sp.ref_count;
        if (ptr)
            ++(*ref_count);
        return *this;
    }


    // Dereference pointer to managed object.
    T& operator*() const noexcept { return *ptr; }
    T* operator->() const noexcept { return ptr; }

    // Return the contained pointer.
    T* get() const noexcept { return ptr; }

    // Return use count (use count == 0 if shared_ptr is empty).
    long use_count() const noexcept {
        if (ptr)
            return *ref_count;
        else
            return 0;
    }

    // Check if solely owns managed object.
    bool unique() const noexcept { return (use_count() == 1); }

    // Check if there is an associated managed object.
    explicit operator bool() const noexcept { return (ptr); }

    // Resets shared_ptr to empty.
    void reset() noexcept {
        this->ptr = nullptr;
        this->ref_count = nullptr;
    }
    // Reset shared_ptr to wrap raw pointer p.
    template<typename U>
    void reset(U* p) {
        this->ptr = p;
        this->ref_count = new uint64_t (1);
    }
};

// Operator overloading.
template<typename T, typename U>
inline bool operator==(const shared_ptr<T>& sp1, const shared_ptr<U>& sp2) { return sp1.get() == sp2.get(); }

template<typename T>
inline bool operator==(const shared_ptr<T>& sp, std::nullptr_t) noexcept { return !sp; }

template<typename T>
inline bool operator==(std::nullptr_t, const shared_ptr<T>& sp) noexcept { return !sp; }

template<typename T, typename U>
inline bool operator!=(const shared_ptr<T>& sp1, const shared_ptr<U>& sp2) { return sp1.get() != sp2.get(); }

template<typename T>
inline bool operator!=(const shared_ptr<T>& sp, std::nullptr_t) noexcept { return bool{sp}; }

template<typename T>
inline bool operator!=(std::nullptr_t, const shared_ptr<T>& sp) noexcept { return bool{sp}; }

// Create shared_ptr that manages a new object.
template<typename T, typename... Args>
inline shared_ptr<T> make_shared(Args&&... args) {
    return shared_ptr<T>{
        new T{
            std::forward<Args>(args)... }
    };
}
}