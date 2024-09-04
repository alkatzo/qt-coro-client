#pragma once

// container
template<typename T>
concept QContainer = requires(T x) {
    { x.begin() };
    { x.end() };
    { x.operator<<(std::declval<typename T::value_type>()) };
};

template<typename T>
concept PtrType = std::is_pointer_v<T>;

// container of pointers
template<typename T>
concept QPtrContainer = QContainer<T> && requires {
    requires PtrType<typename T::value_type>;
};

namespace db {

/**
 * @brief The Deleter class
 * Default deleter class to dealocate results when ctx object is gone
 */
class Deleter {
public:
    template<typename T>
    static void free(const T &) {}

    template<QPtrContainer T>
    static void free(const T &arg) {
        qDeleteAll(arg);
    }

    template<PtrType T>
    static void free(const T &arg) {
        delete arg;
    }
};


}

