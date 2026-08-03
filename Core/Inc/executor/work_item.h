#pragma once

#include <cstdint>

namespace executor {

class WorkItem {
public:
    explicit constexpr WorkItem(const char* name) noexcept : name_(name) {}
    virtual ~WorkItem() = default;

    WorkItem(const WorkItem&) = delete;
    WorkItem& operator=(const WorkItem&) = delete;

    [[nodiscard]] const char* Name() const noexcept {
        return name_;
    }

    virtual void Run() noexcept = 0;

private:
    const char* const name_;
};

class FunctionWorkItem final : public WorkItem {
public:
    using Callback = void (*)(void* context);

    constexpr FunctionWorkItem(const char* name, Callback callback,
                               void* context = nullptr) noexcept
        : WorkItem(name), callback_(callback), context_(context) {}

    void Run() noexcept override {
        if (callback_ != nullptr) {
            callback_(context_);
        }
    }

private:
    Callback callback_;
    void* context_;
};

}  // namespace executor
