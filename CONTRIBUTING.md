# Contributing to AUS-CMake Data Structures Library

Thank you for your interest in contributing to the AUS-CMake Data Structures Library! This document provides guidelines and information for contributors.

## 📋 Table of Contents

- [Code of Conduct](#code-of-conduct)
- [Getting Started](#getting-started)
- [Development Setup](#development-setup)
- [Contributing Process](#contributing-process)
- [Coding Standards](#coding-standards)
- [Testing Guidelines](#testing-guidelines)
- [Documentation Guidelines](#documentation-guidelines)
- [Submitting Changes](#submitting-changes)

## Code of Conduct

This project follows a code of conduct based on respect, inclusion, and constructive collaboration:

- **Be Respectful**: Treat all contributors with respect and courtesy
- **Be Inclusive**: Welcome contributions from developers of all backgrounds and experience levels
- **Be Constructive**: Provide helpful feedback and suggestions for improvement
- **Be Professional**: Maintain a professional tone in all communications

## Getting Started

### Prerequisites

- **CMake 3.19+**: Build system
- **C++17 Compatible Compiler**: GCC 7+, Clang 5+, or MSVC 2017+
- **Git**: Version control
- **Doxygen** (optional): For generating documentation
- **Graphviz** (optional): For documentation diagrams

### Development Setup

1. **Fork and Clone**
   ```bash
   git clone <your-fork-url>
   cd AUS-CMake
   ```

2. **Build the Project**
   ```bash
   mkdir build && cd build
   cmake ..
   make -j4
   ```

3. **Run Tests**
   ```bash
   ./cli/main
   ```

4. **Generate Documentation** (optional)
   ```bash
   make docs
   ```

## Contributing Process

1. **Find or Create an Issue**
   - Check existing issues for bugs or feature requests
   - Create a new issue if your contribution doesn't have one
   - Discuss the approach before implementing large changes

2. **Create a Feature Branch**
   ```bash
   git checkout -b feature/your-feature-name
   ```

3. **Make Changes**
   - Follow coding standards (see below)
   - Write or update tests as needed
   - Update documentation if applicable

4. **Test Your Changes**
   ```bash
   # Build and test
   make
   ./cli/main
   
   # Generate documentation
   make docs
   ```

5. **Commit Changes**
   ```bash
   git add .
   git commit -m "Brief description of changes"
   ```

6. **Submit Pull Request**
   - Push your branch to your fork
   - Create a pull request with clear description
   - Reference related issues

## Coding Standards

### C++ Style Guidelines

1. **Naming Conventions**
   ```cpp
   // Classes: PascalCase
   class MyDataStructure {};
   
   // Functions and variables: camelCase
   auto insertElement(int value) -> void;
   int elementCount = 0;
   
   // Constants: UPPER_SNAKE_CASE
   constexpr int MAX_SIZE = 1000;
   
   // Private members: trailing underscore
   private:
       int size_;
       T* data_;
   ```

2. **Header Guards**
   ```cpp
   #pragma once
   ```

3. **Include Order**
   ```cpp
   // Standard library headers
   #include <vector>
   #include <memory>
   
   // Project headers
   #include <libds/adt/abstract_data_type.h>
   ```

4. **Template Usage**
   ```cpp
   // Prefer auto return type deduction
   template<typename T>
   auto getValue() const -> T&;
   
   // Use concepts where applicable (C++20+)
   template<typename T>
   requires std::copyable<T>
   class Container {};
   ```

### Modern C++ Features

- **RAII**: Use smart pointers and automatic resource management
- **Templates**: Prefer templates over macros for generic code
- **const correctness**: Mark methods const when they don't modify state
- **Exception safety**: Provide strong exception safety guarantees
- **STL compatibility**: Follow STL conventions for iterators and containers

## Testing Guidelines

### Test Structure

All tests inherit from `LeafTest` and follow this pattern:

```cpp
class MyStructureTest : public ds::tests::LeafTest 
{
public:
    MyStructureTest() : LeafTest("MyStructure") {}
    
protected:
    void test() override {
        // Test implementation
        testBasicOperations();
        testEdgeCases();
        testErrorConditions();
    }
    
private:
    void testBasicOperations() {
        // Basic functionality tests
        MyStructure<int> structure;
        structure.insert(42);
        assert_equals(1, structure.size(), "Size after insertion");
    }
    
    void testEdgeCases() {
        // Edge case testing
    }
    
    void testErrorConditions() {
        // Error condition testing
    }
};
```

### Assertion Guidelines

Use the provided assertion macros:

```cpp
// Basic assertions
assert_true(condition, "descriptive message");
assert_false(condition, "descriptive message");
assert_equals(expected, actual, "descriptive message");

// Null pointer checks
assert_not_null(pointer, "pointer should not be null");

// Container-specific assertions
assert_equals(expected_size, container.size(), "container size");
```

### Test Categories

- **Functionality Tests**: Verify correct behavior
- **Edge Case Tests**: Boundary conditions and special cases
- **Error Tests**: Invalid input and error handling
- **Performance Tests**: Complexity verification (use complexity framework)

## Documentation Guidelines

### Doxygen Comments

All public APIs should have comprehensive Doxygen documentation:

```cpp
/**
 * @brief Brief description of the class or function.
 * 
 * Detailed description providing context and usage information.
 * Can span multiple lines and include examples.
 * 
 * @tparam T The type parameter description
 * @param parameter_name Description of the parameter
 * @return Description of the return value
 * @throw std::exception_type When this exception is thrown
 * 
 * @code
 * // Usage example
 * MyClass<int> instance;
 * instance.doSomething(42);
 * @endcode
 */
template<typename T>
auto doSomething(const T& parameter) -> T;
```

### README Updates

When adding new components:

1. Update the main README.md with new features
2. Add component-specific documentation
3. Update usage examples if applicable
4. Document any new dependencies or requirements

## Submitting Changes

### Pull Request Guidelines

1. **Clear Title**: Descriptive title summarizing the change
2. **Detailed Description**: Explain what changes were made and why
3. **Reference Issues**: Link to related issues using `#issue-number`
4. **Test Results**: Confirm all tests pass
5. **Breaking Changes**: Clearly mark any breaking changes

### Pull Request Template

```markdown
## Description
Brief description of the changes made.

## Related Issues
Fixes #issue-number

## Type of Change
- [ ] Bug fix
- [ ] New feature  
- [ ] Breaking change
- [ ] Documentation update

## Testing
- [ ] All existing tests pass
- [ ] New tests added for new functionality
- [ ] Manual testing completed

## Documentation
- [ ] Code comments updated
- [ ] README updated if needed
- [ ] Doxygen documentation updated
```

### Review Process

1. **Automated Checks**: Ensure builds and tests pass
2. **Code Review**: Maintainer will review code quality and design
3. **Feedback**: Address any requested changes
4. **Approval**: Once approved, changes will be merged

## Additional Resources

- **Project Structure**: See README.md for overview
- **API Documentation**: Generated docs in `build/docs/html/`
- **Testing Framework**: See `tests/README.md`
- **Performance Analysis**: See `complexities/README.md`

## Questions or Help?

If you have questions or need help:

1. Check existing issues and documentation
2. Create an issue with your question
3. Reach out to maintainers

Thank you for contributing to the AUS-CMake Data Structures Library!