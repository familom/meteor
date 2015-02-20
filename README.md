# Meteor

Mesosphere programming test.

Ideas.md contains open questions regarding the task.

# Dependencies

In order to build Meteor you will need C++11-compliant compiler (g++ and clang are supported),
CMake (2.8+) and Boost (1.50+).

# How to build

In the root Meteor directory run:
```
cmake .
make -j4
make check
```

# How to run simulator

In the root Meteor directory run:
```
cmake .
make simulate
```
