#pragma once

template<typename T>
concept PtrType = std::is_pointer_v<T>;

// QT container of pointers
template<typename T>
concept QPtrContainer = requires(T x) {
    { x.append(std::declval<typename T::value_type>()) } -> std::same_as<void>;
    requires PtrType<typename T::value_type>;
};

