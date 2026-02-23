# System Software Project: Assembler, Linker & Emulator

## Project Overview

This project is a comprehensive system software suite featuring a two-pass assembler, a static linker, and an emulator for a custom-defined CPU architecture. It's designed to take assembly language code, process it through assembling and linking stages, and finally execute it on a simulated processor. This project was developed as a part of the System Software course at the University of Belgrade, School of Electrical Engineering.

The core components are:
*   **Assembler**: A two-pass assembler that translates assembly language source files (`.s`) into relocatable object files (`.o`).
*   **Linker**: A static linker that combines one or more object files into a single executable file in HEX format. It resolves symbol dependencies and performs relocation.
*   **Emulator**: A CPU emulator that loads and executes the generated HEX file, simulating the hardware environment.

## Tech Stack

*   **Languages**: C++
*   **Build Tools**: `make`
*   **Parsing**: `flex` (lexical analyzer), `bison` (parser generator)

## Features

*   **Two-Pass Assembler**: Resolves forward references for symbols and labels efficiently.
*   **Relocatable Code Generation**: Produces object files with symbol tables and relocation information, allowing code to be placed anywhere in memory.
*   **Static Linking**: Merges multiple object files, resolves external symbol references, and places sections at specified memory addresses.
*   **Instruction Set Emulation**: A full-featured emulator that simulates a CPU with a set of general-purpose registers and memory, capable of executing the linked program.
*   **Support for Directives**: The assembler supports common directives like `.global`, `.extern`, `.section`, `.word`, `.skip`, and `.end`.
*   **Modular Architecture**: The project is organized into distinct components for assembling, linking, and emulation, each with its own source and header files.

## Getting Started

Follow these instructions to get the project up and running on your local machine.

### Prerequisites

Ensure you have the following tools installed on your system:
*   `g++` (GNU C++ Compiler)
*   `make`
*   `flex`
*   `bison`

You can typically install these on a Debian-based Linux distribution with:
```bash
sudo apt-get update
sudo apt-get install build-essential flex bison
```

### Building the Project

1.  Navigate to the `resenje` directory:
    ```bash
    cd resenje
    ```

2.  Run the `make` command to build the assembler, linker, and emulator executables:
    ```bash
    make
    ```
    This will create three executables in the `resenje` directory: `assembler`, `linker`, and `emulator`.

### Running an Example

The `tests` directory contains sample assembly files. The `start.sh` script demonstrates the full workflow. Here's a manual breakdown of the steps:

1.  **Assemble the source files**:
    The assembler takes an input assembly file (`.s`) and produces an object file (`.o`).
    ```bash
    ./assembler -o main.o ../tests/main.s
    ./assembler -o math.o ../tests/math.s
    # ... and so on for other test files
    ```

2.  **Link the object files**:
    The linker takes the generated object files and links them into a single executable hex file.
    ```bash
    ./linker -hex -o program.hex main.o math.o isr_timer.o isr_terminal.o isr_software.o handler.o 
    ```

3.  **Emulate the executable**:
    The emulator loads and runs the final `program.hex` file.
    ```bash
    ./emulator program.hex
    ```

## Project Structure

The codebase is organized into the following main directories within `resenje/`:

```
resenje/
├── inc/            # Header files for all components
│   ├── assembler.hpp
│   ├── emulator.hpp
│   ├── linker.hpp
│   └── structures.hpp
├── src/            # Source code for the main components
│   ├── assembler.cpp
│   ├── emulator.cpp
│   └── linker.cpp
├── misc/           # Flex (.l) and Bison (.y) grammar files
│   ├── bison.y
│   └── flex.l
├── tests/          # Sample assembly files for testing
│   ├── main.s
│   └── ...
└── makefile        # The build script for the project
```

## Usage

The primary way to use the tools is via the command line.

### Assembler
```
./assembler -o <output_file.o> <input_file.s>
```

### Linker
```
./linker -hex [-place=<section_name>@<address> ...] -o <output_file.hex> <input_file1.o> [<input_file2.o> ...]
```
*   `-hex`: Specifies that the output should be in hex format.
*   `-place`: (Optional) Places a section at a specific memory address.

### Emulator
```
./emulator <input_file.hex>
```
This will load the hex file and start execution from the program's entry point. The emulator will print the final state of the registers upon halting.
