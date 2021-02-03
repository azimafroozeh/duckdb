#include <cstdlib>  // nullptr_t

namespace azim {
template<typename T>
class shared_ptr {
public:
    T *ptr;            // contained pointer
    uint32_t *ref_count;             // reference counter

public:
    // Default constructor, constructs an empty shared_ptr.
    constexpr shared_ptr() : ptr(nullptr), ref_count(nullptr) {}

    // Construct empty shared_ptr.
    constexpr shared_ptr(std::nullptr_t) : ptr(nullptr), ref_count(nullptr) {}

    // Construct a shared_ptr that wraps raw pointer.
    explicit shared_ptr(T *p) {
        if (p) {
            ptr = p;
            ref_count = new uint32_t(1);
        }
    }

    shared_ptr(uint32_t *r, T *p) {
        ptr = p;
        ref_count = r;
    }

    template<class U>
    shared_ptr(uint32_t *r, U *p) {
        ptr = p;
        ref_count = r;
    }

    // Construct a shared_ptr that wraps raw pointer of convertible type.
    template<typename U>
    shared_ptr(U *p) {
        if (p) {
            ptr = p;
            ref_count = new uint32_t(1);
        }
    }

    // Copy  constructor.
    shared_ptr(const shared_ptr &sp) : ptr(nullptr), ref_count(nullptr) {
        if (sp.ptr) {
            ptr = sp.ptr;
            ref_count = sp.ref_count;
            ++(*ref_count);
        }

    }

    // Conversion constructor.
    template<typename U>
    shared_ptr(const shared_ptr<U> &sp) : ptr(nullptr), ref_count(nullptr) {
        if (sp.ptr) {
            ptr = sp.ptr;
            ref_count = sp.ref_count;
            ++(*ref_count);
        }

    }

    // move  constructor.
    shared_ptr(shared_ptr &&sp) noexcept: ptr{sp.ptr}, ref_count{sp.ref_count} {
        sp.ptr = nullptr;
        sp.ref_count = nullptr;
    }

    // move  constructor.
    template<class U>
    shared_ptr(shared_ptr<U> &&sp) noexcept : ptr{sp.ptr}, ref_count{sp.ref_count} {
        sp.ptr = nullptr;
        sp.ref_count = nullptr;
    }

    // No effect if shared_ptr is empty or use_count() > 1, otherwise release the resources.
    ~shared_ptr() {
        release();
    }

    void release() {
        if (ptr && ref_count) {
            if (--(*ref_count) == 0) {
                delete ptr;
            }
        }
        ptr = nullptr;
    }

    // Copy assignment.
    shared_ptr &operator=(shared_ptr sp) noexcept {
        std::swap(this->ptr, sp.ptr);
        std::swap(this->ref_count, sp.ref_count);
        return *this;
    }


    // Dereference pointer to managed object.
    T &operator*() const noexcept { return *ptr; }

    T *operator->() const noexcept { return ptr; }

    // Return the contained pointer.
    T *get() const noexcept { return ptr; }

    // Return use count (use count == 0 if shared_ptr is empty).
    long use_count() const noexcept {
        if (ptr)
            return *ref_count;
        else
            return 0;
    }

    // Check if there is an associated managed object.
    explicit operator bool() const noexcept { return (ptr); }

    // Resets shared_ptr to empty.
    void reset() noexcept {
        release();
    }
};

template<class T>
struct _object_and_block {
    T object;
    uint32_t pn = 1;

    template<class ... Args>
    explicit _object_and_block(Args &&...args) :
        object(std::forward<Args>(args)...) {
    }
};

// Operator overloading.
template<typename T, typename U>
inline bool operator==(const shared_ptr<T> &sp1, const shared_ptr<U> &sp2) {
    return sp1.get() == sp2.get();
}

template<typename T>
inline bool operator==(const shared_ptr<T> &sp, std::nullptr_t) noexcept { return !sp; }


template<typename T, typename U>
inline bool operator!=(const shared_ptr<T> &sp1, const shared_ptr<U> &sp2) {
    return sp1.get() != sp2.get();
}

template<typename T>
inline bool operator!=(const shared_ptr<T> &sp, std::nullptr_t) noexcept { return bool{sp}; }

template<typename T>
inline bool operator!=(std::nullptr_t, const shared_ptr<T> &sp) noexcept { return bool{sp}; }

template<class T, class ... Args>
shared_ptr<T> make_shared(Args &&... args) {
    auto tmp_object = new _object_and_block<T>(std::forward<Args>(args)...);
    return shared_ptr<T>(&tmp_object->pn, &(tmp_object->object));
}
}