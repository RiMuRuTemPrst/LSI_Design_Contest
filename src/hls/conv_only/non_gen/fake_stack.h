#include <inttypes.h>
#include <cstddef>
#include <cassert>

#define byte char

/**
 * Arena: A simple stack-based memory allocator (linear allocator).
 * Used for temporary memory management during simulation.
 */
struct Arena {
    byte* base;             // Start of memory pool
    size_t size;            // Total size of pool
    size_t max_runtime_size;// Maximum memory reached during execution
    size_t offset;          // Current allocation offset
    struct List {
        size_t data[30];    // Stack to store previous offsets (for pop functionality)
        int top;
    } manage;

    // Constructor: Initializes the arena with a block of memory
    Arena(void* mem, size_t sz) : base((byte*)mem), size(sz), offset(0) {
        manage.top = -1;
        max_runtime_size = 0;
    }

    // Resets the allocator to the beginning of the pool
    void reset() {
        offset = 0;
        manage.top = -1;
    }

    // Reverts the last allocation (simple stack pop)
    void pop() {
        if (manage.top < 0) 
            assert(0 && "Error: Fake stack underflow !!\n");
        offset = manage.data[manage.top--];
    }

    // Allocates a block of memory for type T
    template<typename T>
    T* alloc(size_t count) {
        constexpr size_t align = alignof(T);
        uintptr_t current = (uintptr_t)(base + offset);
        uintptr_t aligned = (current + align - 1) & ~(align - 1);

        size_t new_offset = (aligned - (uintptr_t)base) + count * sizeof(T);

        // Check for pool overflow or management stack overflow
        if (new_offset > size || manage.top >= 29)
            assert(0 && "Error: Fake stack overflow !!\n");

        manage.data[++manage.top] = offset;
        offset = new_offset;

        // Track peak memory usage
        if (offset > max_runtime_size) max_runtime_size = offset;

        return (T*)aligned;
    }
};
