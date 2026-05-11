# C++ Design Patterns

Educational implementations of GoF (Gang of Four) design patterns in modern C++20. Each pattern includes a practical example and a coding exercise from the Udemy C++ course.

## Patterns Covered

### Creational Patterns
| Pattern | File | Description |
|---------|------|-------------|
| **SOLID Principles** | `solid.cpp` | Single Responsibility, Open/Closed, Liskov Substitution, Interface Segregation, Dependency Inversion |
| **Builder** | `builder.cpp` | Construct complex objects step by step |
| **Factory** | `factory.cpp` | Create objects without specifying exact class |
| **Prototype** | `prototype.cpp` | Clone existing objects |
| **Singleton** | `singleton.cpp` | Ensure only one instance exists |

### Structural Patterns
| Pattern | File | Description |
|---------|------|-------------|
| **Adapter** | `adapter.cpp` | Convert interface to another interface |
| **Bridge** | `bridge.cpp` | Separate abstraction from implementation |
| **Composite** | `composite.cpp` | Tree structures, part-whole hierarchies |
| **Decorator** | `decorator.cpp` | Add behavior dynamically without subclassing |
| **Facade** | `facade.cpp` | Simplified interface to complex subsystem |
| **Flyweight** | `flyweight.cpp` | Share common state between many objects |
| **Proxy** | `proxy.cpp` | Placeholder to control access to another object |

### Behavioral Patterns
| Pattern | File | Description |
|---------|------|-------------|
| **Chain of Responsibility** | `chain_of_responsibility.cpp` | Pass requests along a chain of handlers |
| **Command** | `command.cpp` | Encapsulate requests as objects, support undo/redo |
| **Interpreter** | `interpreter.cpp` | Evaluate language grammar with AST |
| **Iterator** | `iterator.cpp` | Traverse aggregate without exposing internals |
| **Mediator** | `mediator.cpp` | Central hub for component communication |
| **Memento** | `memento.cpp` | Capture and restore object state, undo/redo stacks |
| **Observer** | `observer.cpp` | Notify dependents of state changes automatically |
| **State** | `state.cpp` | Alter behavior when object's state changes |
| **Strategy** | `strategy.cpp` | Family of interchangeable algorithms |
| **Template Method** | `template_method.cpp` | Skeleton algorithm with customizable steps |
| **Visitor** | `visitor.cpp` | Double-dispatch operations on object structures |

## Build

```bash
mkdir -p build && cd build
cmake ..
cmake --build .
```

## Run a Pattern

```bash
./build/singleton
./build/flyweight
./build/mediator
./build/command
./build/observer
./build/state
./build/strategy
./build/visitor
```

## VS Code Debug Config

```json
{
    "version": "0.2.0",
    "configurations": [
        {
            "type": "lldb",
            "request": "launch",
            "name": "Launch",
            "program": "${workspaceFolder}/build/udemy_1_initial",
            "args": [],
            "cwd": "${workspaceFolder}"
        }
    ]
}
```

## Requirements

- CMake 3.10+
- C++20 compatible compiler (GCC 10+, Clang 12+, MSVC 2019+)

## License

MIT
