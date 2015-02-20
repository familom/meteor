# Meteor

Mesosphere programming test.

Ideas.md contains open questions regarding the task.

The main idea behind new scheduler is to disallow burst in either job duration or desired resource consumption.
To achieve this goal we track all scheduled jobs and calculate average job duration / resource requirements.
If job scheduler is requested to schedule looks like 'burst' we reject it.

Simulator demonstrates our scheduler outperforms default 'first-come, first-served' scheduler by "num of rejected jobs" metrics.

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

# TODO
* Implement multi-resource scheduler
* Implement loadable job streams
