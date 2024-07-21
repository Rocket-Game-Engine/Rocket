# Contributing

## Coding Standard

In general, follow [these guidelines](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines).

## Formatting Rules

### Classes

- Class names should be in pascal-case, trying to emphasize the class' overall functionality as best as possible.
- The `class` identifier is for classes, the `struct` identifier is for data.
- Attributes should be prefixed with an underscore if they are `protected` or `private`.
- Attributes are named with snake case.
- Methods are named in CamelCase, trying to explain the method's function as best as possible.
- Method parameters are named like attributes, aka in snake case.
- The order of definitions in a class go
  - Protected Methods
  - Private Methods
  - Public Methods
  - Protected Attributes
  - Private Attributes
  - Public Attributes

```c++
class ExampleClass
{
protected:
    int getProtectedInt();

private:
    int getPrivateInt();

public:
    ExampleClass();
    int getPublicInt();

protected:
    int _protected_int;

private:
    int _private_int;

public:
    int public_int;

};
```

### Filesystem

- Header files go under `include/`, and source files go under `src/`.
- The filesystems under `include/` and `src/` should be roughly mirrored (aka `include/test.hpp` should have a matching `src/test.cpp` if applicable).
- Files and directories (except toplevel directories and `main.cpp`) will be written in CamelCase.
- Header files have the `.hpp` extension, and source files have the `.cpp` extension.

Example filesystem:
```
| ./
| include/
| |--- OtherDir/
| |--- |--- Other.hpp
| |--- Test.hpp
| src/
| |--- main.cpp
| |--- OtherDir/
| |--- |--- Other.cpp
| |--- Test.cpp
| ...
```
