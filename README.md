# Number Manager

C++ daemon and CLI for managing positive integers with timestamps (Linux only).

## Files
- `daemon.cpp` - Background service managing number storage
- `cli.cpp` - Interactive command-line interface

## Data Structure
Uses `std::map<int, time_t>` for automatic sorting and O(log n) operations.

## IPC
Named Pipes (FIFO) for daemon-CLI communication.

## Build
```bash
g++ -std=c++17 -o daemon daemon.cpp
g++ -std=c++17 -o cli cli.cpp
```

## Usage
1. Start daemon: `./daemon`
2. Run CLI: `./cli`

CLI Menu:
1. Insert a number
2. Delete a number  
3. Print all numbers
4. Delete all numbers
5. Exit
