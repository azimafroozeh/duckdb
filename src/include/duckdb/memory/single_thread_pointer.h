
#include <cstdlib>  // nullptr_t

template<typename T>
class my_shared_ptr {
public:
    T* _ptr{ nullptr };            // contained pointer
    uint64_t * _ref_count;             // reference counter

public:
    // Default ctor, constructs an empty my_shared_ptr.
    constexpr my_shared_ptr() noexcept = default;
    // Construct empty my_shared_ptr.
    constexpr my_shared_ptr(std::nullptr_t) noexcept { }
    // Ctor wraps raw pointer.
    explicit my_shared_ptr(T* p) : _ptr{ p }, _ref_count{ new uint64_t(1) } { }
    // Ctor wraps raw pointer of convertible type.
    template<typename U>
    my_shared_ptr(U* p) : _ptr{ p }, _ref_count{ new uint64_t(1) } { }
    // Copy ctor.
    my_shared_ptr(const my_shared_ptr& sp) noexcept : _ptr{ sp._ptr }, _ref_count{ sp._ref_count } {
        if (_ptr)
            ++(*_ref_count);
    }
    // Conversion ctor.
    template<typename U>
    my_shared_ptr(const my_shared_ptr<U>& sp) noexcept : _ptr{sp._ptr}, _ref_count{sp._ref_count} {
        if (_ptr)
            ++(*_ref_count);
    }
    // Ctor unique.
    my_shared_ptr(std::unique_ptr<T> up) : _ptr{ up.get() }, _ref_count{ new uint64_t {1} } { }

    // Ctor unique.
	template <class U>
	my_shared_ptr(std::unique_ptr<U> up) : _ptr{ up.get() }, _ref_count{ new uint64_t {1} } { }


    // No effect if my_shared_ptr is empty or use_count() > 1, otherwise release the resources.
    ~my_shared_ptr() {
        if (_ptr)
        {
            if (--(*_ref_count) == 0)
            {
                delete _ref_count;
                delete _ptr;
            }
        }
    }

    // Copy assignment.
    my_shared_ptr& operator= (const my_shared_ptr& sp) noexcept {
        // Copy and swap idiom.
		this->_ptr = sp._ptr;
		this->_ref_count = sp._ref_count;
        if (_ptr)
            ++(*_ref_count);
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
    bool unique() const noexcept { return (use_count() == 1); }

    // Check if there is an associated managed object.
    explicit operator bool() const noexcept { return (_ptr); }

    // Resets my_shared_ptr to empty.
    void reset() noexcept {
		this->_ptr = nullptr;
        this->_ref_count = nullptr;
    }
    // Reset my_shared_ptr to wrap raw pointer p.
    template<typename U>
    void reset(U* p) {
		this->_ptr = p;
		this->_ref_count = new uint64_t (1);
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