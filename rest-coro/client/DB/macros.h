#pragma once

#define DISABLE_COPY(ClassName)               \
ClassName(const ClassName&) = delete;     \
    ClassName& operator=(const ClassName&) = delete;

#define DEFAULT_MOVE(ClassName)               \
ClassName(ClassName&&) = default;         \
    ClassName& operator=(ClassName&&) = default;

