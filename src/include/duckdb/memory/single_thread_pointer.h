// Minimal my_shared_ptr implementation.

#include <cstdlib>  // nullptr_t
#include <utility>  // swap

//template <class T>
//T& move(T&& t) noexcept { return static_cast<T&&>(t); }

// static_cast to rvalue reference.
#define MOVE(...) std::static_cast<std::remove_reference_t<decltype(__VA_ARGS__)>&&>(__VA_ARGS__)

// static_cast to identity.
// The extra && aren't necessary, but make it more robust in case it's used with a non-reference.
#define FORWARD(...) static_cast<decltype(__VA_ARGS__)&&>(__VA_ARGS__)

// Type erasure for run-time polymorphic behavior of deleter.
class deleter_base {
public:
    virtual ~deleter_base() = default;
    virtual void operator()(void*) = 0;
};

template <typename T>
class deleter : public deleter_base {
public:
    void operator()(void* p) override { delete static_cast<T*>(p); }
};

template<typename T>
class my_shared_ptr {
public:
    T* _ptr{ nullptr };            // contained pointer
    long * _ref_count;             // reference counter
    deleter_base* _deleter;        // deleter

public:
    // Default ctor, constructs an empty my_shared_ptr.
    constexpr my_shared_ptr() noexcept = default;
    // Construct empty my_shared_ptr.
    constexpr my_shared_ptr(std::nullptr_t) noexcept { }
    // Ctor wraps raw pointer.
    my_shared_ptr(T* p) : _ptr{ p }, _ref_count{ new long{1} }, _deleter{ new deleter<T>() } { }
    // Ctor wraps raw pointer of convertible type.
    template<typename U>
    my_shared_ptr(U* p) : _ptr{ p }, _ref_count{ new long(1) }, _deleter{ new deleter<U>() } { }
    // Copy ctor.
    my_shared_ptr(const my_shared_ptr& sp) noexcept : _ptr{ sp._ptr }, _ref_count{ sp._ref_count }, _deleter{ sp._deleter } {
        if (_ptr)
            ++(*_ref_count);
    }
    // Conversion ctor.
    template<typename U>
    my_shared_ptr(const my_shared_ptr<U>& sp) noexcept : _ptr{sp._ptr}, _ref_count{sp._ref_count}, _deleter{sp._deleter} {
        if (_ptr)
            ++(*_ref_count);
    }
    // Ctor unique.
    my_shared_ptr(std::unique_ptr<T> up) : _ptr{ up.get() }, _ref_count{ new long{1} }, _deleter{ new deleter<T>() } { }

    // Ctor unique.
	template <class U>
	my_shared_ptr(std::unique_ptr<U> up) : _ptr{ up.get() }, _ref_count{ new long{1} }, _deleter{ new deleter<U>() } { }

    // Move ctor, move-construct my_shared_ptr from sp.
    my_shared_ptr(my_shared_ptr&& sp) noexcept : _ptr{ std::move(sp._ptr) }, _ref_count{ std::move(sp._ref_count) }, _deleter{ std::move(sp._deleter) } {
        sp._ptr = nullptr;
        sp._ref_count = nullptr;
        sp._deleter = nullptr;
    }
    template<typename U>
    explicit my_shared_ptr(my_shared_ptr<U>&& sp) noexcept : _ptr{ sp._ptr }, _ref_count{ sp._ref_count }, _deleter{ sp._deleter } {
        sp._ptr = nullptr;
        sp._ref_count = nullptr;
        sp._deleter = nullptr;
    }

    // No effect if my_shared_ptr is empty or use_count() > 1, otherwise release the resources.
    ~my_shared_ptr() {
        if (_ptr)
        {
            if (--(*_ref_count) == 0)
            {
                delete _ref_count;
                (*_deleter)(_ptr);
                delete _deleter;
            }
        }
    }

    // Copy assignment.
    my_shared_ptr& operator= (const my_shared_ptr& sp) noexcept {
        // Copy and swap idiom.
        my_shared_ptr tmp{sp};
        tmp.swap(*this);
        return *this;
    }
    // Move assignment.
    my_shared_ptr& operator=(my_shared_ptr&& sp) noexcept {
        my_shared_ptr{ std::move(sp) }.swap(*this);
        return *this;
    }
    template<typename U>
    my_shared_ptr& operator=(my_shared_ptr<U>&& sp) noexcept {
        my_shared_ptr{ std::move(sp) }.swap(*this);
        return *this;
    }

    // Dereference pointer to managed object.
    T& operator*() const noexcept { return *_ptr; }
    T* operator->() const noexcept { return _ptr; }

    // Return the contained pointer.
    T* get() const noexcept { return _ptr; }

    // Return use count (use count == 0 if my_shared_ptr is empty).
    long use_count() const noexcept {
        if (_ptr)
            return *_ref_count;
        else
            return 0;
    }

    // Check if solely owns managed object.
    bool unique() const noexcept { return (use_count() == 1) ? true : false; }

    // Check if there is an associated managed object.
    explicit operator bool() const noexcept { return (_ptr) ? true : false; }

    // Resets my_shared_ptr to empty.
    void reset() noexcept {
        my_shared_ptr tmp{};
        tmp.swap(*this);
    }
    // Reset my_shared_ptr to wrap raw pointer p.
    template<typename U>
    void reset(U* p) {
        my_shared_ptr tmp{p};
        tmp.swap(*this);
    }

    // Swap with another my_shared_ptr.
    void swap(my_shared_ptr& sp) noexcept {
        using std::swap;
        swap(_ptr, sp._ptr);
        swap(_ref_count, sp._ref_count);
        swap(_deleter, sp._deleter);
    }


};

// Operator overloading.
template<typename T, typename U>
inline bool operator==(const my_shared_ptr<T>& sp1, const my_shared_ptr<U>& sp2) { return sp1.get() == sp2.get(); }

template<typename T>
inline bool operator==(const my_shared_ptr<T>& sp, std::nullptr_t) noexcept { return !sp; }

template<typename T>
inline bool operator==(std::nullptr_t, const my_shared_ptr<T>& sp) noexcept { return !sp; }

template<typename T, typename U>
inline bool operator!=(const my_shared_ptr<T>& sp1, const my_shared_ptr<U>& sp2) { return sp1.get() != sp2.get(); }

template<typename T>
inline bool operator!=(const my_shared_ptr<T>& sp, std::nullptr_t) noexcept { return bool{sp}; }

template<typename T>
inline bool operator!=(std::nullptr_t, const my_shared_ptr<T>& sp) noexcept { return bool{sp}; }

// Create my_shared_ptr that manages a new object.
template<typename T, typename... Args>
inline my_shared_ptr<T> my_make_shared(Args&&... args) { return my_shared_ptr<T>{new T{ std::forward<Args>(args)... }}; }