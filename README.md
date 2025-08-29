# AUS-CMake: Data Structures and Algorithms Library

![Build Status](https://img.shields.io/badge/build-passing-brightgreen)
![Version](https://img.shields.io/badge/version-1.1.0-blue)
![C++17](https://img.shields.io/badge/C%2B%2B-17-blue)

A comprehensive C++17 library implementing various data structures, algorithms, and performance analysis tools. This project provides robust implementations of fundamental computer science concepts with extensive testing and complexity analysis capabilities.

## 📋 Table of Contents

- [Features](#features)
- [Project Structure](#project-structure)
- [Building](#building)
- [Usage](#usage)
- [Documentation](#documentation)
- [Testing](#testing)
- [Performance Analysis](#performance-analysis)
- [Components](#components)
- [Contributing](#contributing)
- [License](#license)

## ✨ Features

- **Complete Data Structure Implementations**: Arrays, Lists, Stacks, Queues, Trees, Tables, Priority Queues
- **Multiple Storage Strategies**: Implicit (array-based) and explicit (pointer-based) implementations
- **Comprehensive Algorithm Collection**: Various sorting algorithms (QuickSort, MergeSort, HeapSort, etc.)
- **Memory Management**: Custom memory managers with allocation tracking
- **Performance Analysis Tools**: Built-in complexity analyzers with CSV output
- **Extensive Testing Framework**: Comprehensive test suites for all components
- **Modular Design**: Clean separation between abstract data types (ADT) and access methods (AMT)

## 📁 Project Structure

```
AUS-CMake/
├── libds/                  # Core data structures library
│   ├── adt/               # Abstract Data Types (List, Stack, Queue, Tree, etc.)
│   ├── amt/               # Access Method Types (Sequence, Hierarchy, etc.)
│   └── mm/                # Memory Management
├── tests/                 # Comprehensive test suite
├── complexities/          # Performance analysis tools
├── cli/                   # Command-line interface and demos
├── sem/                   # Semester project (bus stop system)
├── lab3/                  # Laboratory assignment
└── CMakeLists.txt         # Main CMake configuration
```

## 🔧 Building

### Prerequisites

- CMake 3.19 or higher
- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)

### Build Instructions

```bash
# Clone the repository
git clone <repository-url>
cd AUS-CMake

# Create build directory
mkdir build && cd build

# Configure with CMake
cmake ..

# Build the project
make -j4

# Or use CMake to build
cmake --build . --parallel 4
```

### Build Targets

- `main` - CLI interface with test runner and demos
- `tests` - Test library (libtests.so)
- `complexities` - Performance analysis library (libcomplexities.so)
- `ds` - Core data structures library (header-only)
- `sem` - Semester project executable
- `lab3` - Lab assignment executable

## 🚀 Usage

### Running Tests and Demos

```bash
# Run all tests and performance analysis
./cli/main

# Run semester project
./sem/sem

# Run lab3
./lab3/lab3
```

### Using the Library in Your Code

```cpp
#include <libds/adt/list.h>
#include <libds/adt/stack.h>

// Using an implicit list (array-based)
ds::adt::ImplicitList<int> list;
list.insertLast(42);
list.insertLast(24);

// Using a stack
ds::adt::ExplicitStack<std::string> stack;
stack.push("Hello");
stack.push("World");
```

## 📚 Documentation

### API Documentation

The library uses Doxygen-style comments throughout the codebase. Key documentation includes:

- **Class Documentation**: All major classes have detailed descriptions
- **Method Documentation**: Parameters, return values, and behavior explained
- **Usage Examples**: Practical examples for complex functionality

### Component Overview

#### Abstract Data Types (ADT)
- **List**: Dynamic arrays with insertion/deletion operations
- **Stack**: LIFO (Last In, First Out) container
- **Queue**: FIFO (First In, First Out) container  
- **Priority Queue**: Elements ordered by priority
- **Tree**: Hierarchical data structure with various traversal methods
- **Table**: Key-value storage with different implementation strategies

#### Access Method Types (AMT)
- **Sequence**: Linear data organization (implicit/explicit)
- **Hierarchy**: Tree-like organization with parent-child relationships
- **Network**: Graph-like structures with complex relationships

#### Memory Management
- **MemoryManager**: Basic memory allocation and tracking
- **CompactMemoryManager**: Optimized memory management with compaction

## 🧪 Testing

The project includes an extensive testing framework with hierarchical test organization:

```bash
# Tests are automatically run when executing the main CLI
./build/cli/main
```

### Test Categories

- **Memory Management Tests**: Allocation, deallocation, tracking
- **AMT Tests**: Sequence and hierarchy operations
- **ADT Tests**: All data structure implementations
- **Algorithm Tests**: Sorting algorithms with various data types
- **Performance Tests**: Complexity verification

## 📊 Performance Analysis

Built-in complexity analyzers provide detailed performance metrics:

- **Automatic CSV Generation**: Results saved to CSV files for analysis
- **Multiple Data Types**: Support for integers and strings
- **Configurable Test Sizes**: From small (1-3 elements) to large (10,000+ elements)
- **Operation Timing**: Nanosecond precision timing

### Supported Algorithms

- **Selection Sort**: O(n²) comparison-based sorting
- **Bubble Sort**: O(n²) simple exchange sorting
- **Insert Sort**: O(n²) insertion-based sorting
- **Quick Sort**: O(n log n) average case divide-and-conquer
- **Heap Sort**: O(n log n) heap-based sorting
- **Shell Sort**: O(n^1.25) gap-based insertion sort
- **Merge Sort**: O(n log n) stable divide-and-conquer
- **Radix Sort**: O(d×n) non-comparison integer sorting

## 🏗️ Components

### Core Library (libds)

Header-only library providing all data structure implementations:
- Template-based for type safety
- RAII compliance for automatic resource management  
- Iterator support where applicable
- Exception safety guarantees

### Testing Framework

Custom testing framework with:
- Hierarchical test organization
- Detailed test result reporting
- Assertion macros with custom messages
- Console output with color coding
- Test visitor pattern implementation

### Performance Analysis

Sophisticated analysis tools featuring:
- Template-based analyzers for any data structure
- Configurable test parameters
- Statistical data collection
- CSV export functionality
- Memory usage monitoring

## 🤝 Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

### Code Style Guidelines

- Follow existing naming conventions
- Add Doxygen comments for new public APIs
- Include appropriate tests for new functionality
- Ensure all tests pass before submitting

## 📄 License

This project is licensed under the MIT License - see the LICENSE file for details.

## 🎓 Academic Context

This library was developed as part of algorithms and data structures coursework, providing practical implementations of fundamental computer science concepts. It serves as both a learning tool and a foundation for more advanced projects.

---

**Note**: This library is designed for educational purposes and provides clean, well-documented implementations of classic data structures and algorithms. While suitable for learning and small projects, consider production-ready libraries for critical applications.