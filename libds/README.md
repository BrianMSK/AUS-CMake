# Data Structures Library (libds) Documentation

This directory contains the core implementation of data structures and algorithms for the AUS-CMake project.

## Directory Structure

### `adt/` - Abstract Data Types
Contains high-level interface definitions and implementations:

- **`abstract_data_type.h`** - Base ADT interface
- **`array.h`** - Array implementations 
- **`list.h`** - List data structures (Implicit, Singly/Doubly Linked)
- **`stack.h`** - Stack (LIFO) implementations
- **`queue.h`** - Queue (FIFO) implementations  
- **`priority_queue.h`** - Priority queue implementations
- **`tree.h`** - Tree data structures (Binary, K-way, Multi-way)
- **`table.h`** - Key-value table implementations
- **`sorts.h`** - Sorting algorithm implementations

### `amt/` - Access Method Types  
Contains low-level data organization implementations:

- **`sequence.h`** - Sequential data organization base
- **`implicit_sequence.h`** - Array-based sequential access
- **`explicit_sequence.h`** - Pointer-based sequential access (Singly/Doubly Linked)
- **`hierarchy.h`** - Hierarchical data organization base  
- **`implicit_hierarchy.h`** - Array-based tree structures
- **`explicit_hierarchy.h`** - Pointer-based tree structures
- **`network.h`** - Graph-like data organization base
- **`explicit_network.h`** - Graph implementations

### `mm/` - Memory Management
Contains memory allocation and management utilities:

- **`memory_manager.h`** - Basic memory manager with allocation tracking
- **`compact_memory_manager.h`** - Optimized memory manager with compaction
- **`memory_omanip.h`** - Memory output manipulation utilities

### Shared Components

- **`constants.h`** - System-wide constants and configuration
- **`heap_monitor.h`** - Memory usage monitoring and debugging

## Key Design Principles

1. **Template-Based**: All data structures are templated for type safety and performance
2. **RAII Compliance**: Automatic resource management with proper constructors/destructors  
3. **Exception Safety**: Strong exception safety guarantees where possible
4. **Iterator Support**: STL-compatible iterators for applicable structures
5. **Modular Design**: Clear separation between interfaces (ADT) and implementations (AMT)

## Usage Example

```cpp
#include <libds/adt/list.h>
#include <libds/adt/stack.h>

// Create a list with automatic memory management
ds::adt::ImplicitList<int> numbers;
numbers.insertLast(1);
numbers.insertLast(2);
numbers.insertLast(3);

// Use a stack
ds::adt::ExplicitStack<std::string> words;
words.push("Hello");
words.push("World");

std::cout << words.top() << std::endl; // "World"
words.pop();
std::cout << words.top() << std::endl; // "Hello"
```

## Testing

All components have comprehensive test suites in the `../tests/` directory that verify:
- Basic functionality
- Edge cases and error conditions  
- Memory management correctness
- Performance characteristics

## Performance Analysis

The `../complexities/` directory contains tools for analyzing the performance characteristics of all data structures and algorithms implemented in this library.