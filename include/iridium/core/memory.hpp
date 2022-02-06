#pragma once

#include <cstdint>
#include <cstdlib>
#include <vector>

namespace ir
{
    template <typename T>
    class Memory
    {
    public:
        using iterator = T*;
        using const_iterator = const T*;
    public:
        Memory() : size(1), capacity(1)
        {
            memory = (T*) malloc(sizeof(T));
        }

        Memory(const Memory&) = delete;
        Memory(Memory&&) = delete;
        Memory& operator=(const Memory&) = delete;
        Memory& operator=(Memory&&) = delete;

        ~Memory()
        {
            free(memory);
        }

        T* grow()
        {
            if (size == capacity)
            {
                capacity *= 2;
                memory = (T*) realloc(memory, capacity * sizeof(T));
            }

            return &memory[size++];
        }

        constexpr iterator begin() { return memory; }
        constexpr iterator end() { return memory + size + 1; }
        constexpr const_iterator begin() const { return memory; }
        constexpr const_iterator end() const { return memory + size + 1; }
    private:
        size_t size;
        size_t capacity;
        T* memory;
    };

    template <typename T>
    struct RefCount
    {
        uint32_t count;
        T value;
    };

    template <typename T>
    class Pool;

    template <typename T>
    class Ref
    {
        friend class Pool<T>;
    private:
        Ref(RefCount<T>* ref) : ref(ref) {}
    public:
        Ref(const Ref& other) { *this = other; }
        Ref(Ref&& other) { *this = std::move(other); }
        Ref(std::nullptr_t) { ref = nullptr; }

        Ref& operator=(const Ref& other)
        {
            ref = other.ref;
            if (ref != nullptr)
            {
                ++ref->count;
            }
            return *this;
        }

        Ref& operator=(Ref&& other)
        {
            ref = other.ref;
            other.ref = nullptr;
            return *this;
        }

        ~Ref()
        {
            if (ref != nullptr)
            {
                --ref->count;
                if (ref->count == 0)
                {
                    ref->value.~T();
                }
            }
        }

        constexpr bool operator==(const Ref<T>& other) const { return ref == other.ref; }

    public:
        constexpr const T* value() const { return ref == nullptr ? nullptr : &ref->value; }
        constexpr T* value() { return ref == nullptr ? nullptr : &ref->value; }
        constexpr const T* operator->() const { return &ref->value; }
        constexpr T* operator->() { return &ref->value; }
        constexpr const T& operator*() const { return ref->value; }
        constexpr T& operator*() { return ref->value; }
        constexpr uint32_t refcount() const { return ref->count; }
    private:
        RefCount<T>* ref;
    };

    template <typename T>
    class Pool
    {
    public:
        template <typename... Args>
        Ref<T> create(Args&&... args)
        {
            auto slot = find_slot();
            slot->count = 1;
            new(&slot->value) T(std::forward<Args>(args)...);
            return Ref<T>(slot);
        }

    private:
        RefCount<T>* find_slot()
        {
            for (auto& item : items)
            {
                if (item.count == 0)
                {
                    return &item;
                }
            }

            return items.grow();
        }

    private:
        Memory<RefCount<T>> items;
    };

} // namespace ir
