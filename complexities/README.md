# Complexity Analysis Framework

This directory contains tools for analyzing the time and space complexity of data structure operations and algorithms.

## Overview

The complexity analysis framework provides automated performance testing and statistical analysis of data structures and algorithms. It generates detailed reports with timing data that can be used to verify theoretical complexity bounds.

## Core Components

### `complexity_analyzer.h` - Universal Analyzer Template
Template-based analyzer that can measure the performance of any data structure or algorithm:

```cpp
template<class Structure>
class ComplexityAnalyzer : public LeafAnalyzer
```

**Key Features:**
- **Generic Design**: Works with any data structure
- **Configurable Test Sizes**: From small (1-3 elements) to large (10,000+ elements)  
- **Multiple Replications**: Statistical reliability through repeated measurements
- **Nanosecond Precision**: High-resolution timing using `std::chrono`
- **CSV Export**: Results saved for analysis in spreadsheet applications
- **Hook System**: Before/after operation callbacks for setup/cleanup

### `complexity_analyzer.cpp` - Implementation
Contains the concrete implementation of the analyzer framework.

### `list_analyzer.h` - Specialized List Analyzer  
Pre-configured analyzer specifically for list data structures with common operations:
- **Insert Operations**: At beginning, end, and random positions
- **Access Operations**: Random access by index
- **Remove Operations**: From beginning, end, and random positions  
- **Search Operations**: Linear search with various predicates

## Supported Analysis Types

### Time Complexity Analysis
Measures execution time for operations across different input sizes:

```cpp
// Example: Analyzing list insertion performance
auto analyzer = std::make_unique<ComplexityAnalyzer<ImplicitList<int>>>("List Insert");
analyzer->setOperationBatch([](auto& list, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        list.insertLast(i);
    }
});
```

### Space Complexity Analysis  
Can be extended to measure memory usage patterns through the hook system:

```cpp
analyzer->registerBeforeOperation([](auto& structure) {
    // Record memory state before operation
});

analyzer->registerAfterOperation([](auto& structure) {
    // Record memory state after operation
});
```

## Analysis Process

1. **Structure Creation**: Instantiate the data structure to be tested
2. **Size Progression**: Test with exponentially increasing input sizes
3. **Replication**: Perform multiple runs for statistical accuracy
4. **Measurement**: High-precision timing of operations
5. **Data Collection**: Aggregate results across all test runs
6. **CSV Export**: Save results for external analysis

## Configuration Options

### Test Parameters
- **Minimum Size**: Starting size for test progression (default: 1)
- **Maximum Size**: Ending size for test progression (configurable)
- **Size Multiplier**: Growth factor between test sizes (default: 10x)
- **Replication Count**: Number of runs per size (default: 100)

### Output Configuration
- **Output Directory**: Where to save CSV files (configurable via `ANALYZER_OUTPUT`)
- **File Naming**: Automatic generation based on analyzer name
- **Data Format**: CSV with headers for easy import

## Built-in Analyzers

### Lists Analyzer (`ListsAnalyzer`)
Comprehensive analysis of all list implementations:
- **Implicit Lists** (array-based)
- **Singly Linked Lists**  
- **Doubly Linked Lists**

**Measured Operations:**
- Insert at beginning/end/middle
- Access by index
- Remove from beginning/end/middle
- Linear search operations

### Integration with Main Application

Analyzers are automatically run after tests complete:

```cpp
// In main.cpp
std::vector<std::unique_ptr<ds::utils::Analyzer>> analyzers = createAnalyzers();
setAnalyzersParameters(analyzers, ANALYZER_OUTPUT);
runAnalyzers(analyzers);
```

## Interpreting Results

### CSV Output Format
```csv
Size,Replication,Time(ns)
1,1,245
1,2,198
1,3,223
...
10,1,1456
10,2,1398
...
```

### Analysis Guidelines
1. **Plot Size vs Average Time**: Visualize complexity trends
2. **Statistical Analysis**: Calculate mean, standard deviation, confidence intervals
3. **Complexity Verification**: Compare against theoretical bounds (O(1), O(n), O(log n), etc.)
4. **Performance Comparison**: Compare different implementations of the same ADT

## Usage Example

```cpp
// Create analyzer for a specific data structure
auto analyzer = std::make_unique<ComplexityAnalyzer<MyDataStructure<int>>>("MyStructure");

// Configure the operation to be measured
analyzer->setOperation([](MyDataStructure<int>& ds, size_t size) {
    // This operation will be timed
    ds.performOperation(size);
});

// Set test parameters
analyzer->setMinSize(1);
analyzer->setMaxSize(10000);
analyzer->setReplications(50);

// Run analysis
analyzer->analyze();
```

## Integration with CMake

The complexity analysis is automatically built and integrated:

```cmake
add_library(complexities SHARED complexity_analyzer.cpp)
target_compile_features(complexities PUBLIC cxx_std_17)
```

This framework provides essential tools for validating the theoretical complexity of data structure implementations and comparing the performance characteristics of different approaches.