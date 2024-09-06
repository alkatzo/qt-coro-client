#pragma once

// container
template<typename T>
concept Container = requires(T x) {
    { x.begin() };
    { x.end() };
};

// Qt container
template<typename T>
concept QContainer = Container<T> && requires(T x) {
    { x.operator<<(std::declval<typename T::value_type>()) };
};

template<typename T>
concept PtrType = std::is_pointer_v<T>;

// container of pointers
template<typename T>
concept PtrContainer = Container<T> && requires {
    requires PtrType<typename T::value_type>;
};

// Qt container of pointers
template<typename T>
concept QPtrContainer = QContainer<T> && requires(T x) {
    requires PtrType<typename T::value_type>;
};

