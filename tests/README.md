# Testing Framework Documentation

This directory contains a comprehensive testing framework and test suites for the AUS-CMake data structures library.

## Framework Components

### Core Testing Infrastructure
- **`_details/test.hpp`** - Main testing framework classes and interfaces
- **`_details/test.cpp`** - Implementation of test framework
- **`_details/console_output.hpp`** - Console output formatting
- **`_details/console_output.cpp`** - Console output implementation  
- **`_details/console.hpp`** - Console utilities and color support
- **`_details/console.cpp`** - Console implementation
- **`_details/visitors.hpp`** - Visitor pattern for test traversal

### Test Organization

The testing framework uses a hierarchical structure:

```
root
├── mm (Memory Management)
│   ├── MemoryManager
│   └── CompactMemoryManager  
├── amt (Access Method Types)
│   ├── ImplicitSequence
│   ├── ExplicitSequence
│   ├── ImplicitHierarchy
│   ├── ExplicitHierarchy
│   └── Hierarchy
└── adt (Abstract Data Types)
    ├── List
    ├── Stack
    ├── Queue
    ├── PriorityQueue
    ├── Tree
    ├── Table
    └── Sorts
```

## Test Categories

### Memory Management Tests (`mm/`)
- **`memory_manager.test.h`** - Basic memory allocation/deallocation tests
- **`compact_memory_manager.test.h`** - Advanced memory management with compaction

### Access Method Types Tests (`amt/`)
- **`sequence.test.h`** - Base sequence testing utilities
- **`implicit_sequence.test.h`** - Array-based sequence tests
- **`explicit_sequence.test.h`** - Pointer-based sequence tests  
- **`hierarchy.test.h`** - Tree structure tests
- **`implicit_hierarchy.test.h`** - Array-based tree tests
- **`explicit_hierarchy.test.h`** - Pointer-based tree tests

### Abstract Data Types Tests (`adt/`)
- **`list.test.h`** - List implementation tests
- **`stack.test.h`** - Stack implementation tests
- **`queue.test.h`** - Queue implementation tests
- **`priority_queue.test.h`** - Priority queue tests
- **`tree.test.h`** - Tree ADT tests
- **`table.test.h`** - Table/dictionary tests
- **`sorts.test.h`** - Sorting algorithm tests

## Key Features

### Hierarchical Test Structure
Tests are organized in a tree structure using composite pattern:
- **`CompositeTest`** - Container for multiple tests
- **`LeafTest`** - Individual test implementation
- **`Test`** - Base class for all tests

### Rich Assertion Framework
```cpp
// Basic assertions
assert_true(condition, "message");
assert_false(condition, "message");
assert_equals(expected, actual, "message");

// Floating point comparisons with tolerance
assert_equals(3.14, actual, 0.01, "pi approximation");

// Container assertions
assert_not_null(pointer, "pointer should not be null");
```

### Flexible Output Options
- **Full Output**: Shows all test details including individual assertions
- **Summary Output**: Shows only test names and final results
- **Color-coded Results**: Pass (green), Fail (red), Info (blue)

### Test Policies
- **StopAtFirstFail**: Stops test execution on first failed assertion
- **RunAll**: Continues execution through all assertions

## Running Tests

Tests are integrated into the main CLI application:

```bash
# Build the project
cmake --build build

# Run all tests
./build/cli/main
```

## Writing New Tests

1. **Create Test Header**: Add test class in appropriate subdirectory
2. **Inherit from LeafTest**: Use the testing base class
3. **Implement test() method**: Write your test logic
4. **Use Assertions**: Validate expected behavior
5. **Register Test**: Add to the test hierarchy in `root.h`

### Example Test

```cpp
class MyDataStructureTest : public LeafTest
{
public:
    MyDataStructureTest() : LeafTest("MyDataStructure") {}

protected:
    void test() override 
    {
        // Setup
        MyDataStructure<int> ds;
        
        // Test insertion
        ds.insert(42);
        assert_equals(1, ds.size(), "Size after insertion");
        assert_true(ds.contains(42), "Contains inserted element");
        
        // Test removal  
        ds.remove(42);
        assert_equals(0, ds.size(), "Size after removal");
        assert_false(ds.contains(42), "Does not contain removed element");
    }
};
```

## Test Results

The framework provides detailed output showing:
- Test hierarchy with pass/fail indicators
- Individual assertion results with messages
- Performance timing for complexity verification
- Memory usage statistics where applicable

This comprehensive testing ensures the reliability and correctness of all data structure implementations in the library.