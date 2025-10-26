# 16-Bit Multi-Cycle CPU for Digital Systems Design

This repository contains the Verilog implementation of a 16-bit multi-cycle CPU, designed as a project for the Digital Systems Design course. The processor is built from the ground up, featuring a custom Instruction Set Architecture (ISA), a pipelined Arithmetic Logic Unit (ALU) with specialized algorithms, and a complete control unit. [cite_start]The entire design is written in synthesizable Verilog. [cite: 19, 21, 22]

## 📋 Table of Contents
- [Project Overview](#-project-overview)
- [Core Features](#-core-features)
- [Instruction Set Architecture (ISA)](#-instruction-set-architecture-isa)
  - [Instruction Formats](#instruction-formats)
  - [Supported Instructions](#supported-instructions)
- [CPU Architecture](#-cpu-architecture)
  - [Arithmetic Logic Unit (ALU)](#arithmetic-logic-unit-alu)
  - [Register File](#register-file)
  - [Main Memory](#main-memory)
  - [Control Unit](#control-unit)
- [Directory Structure](#-directory-structure)
- [Testing & Synthesis](#-testing--synthesis)
- [Project Information](#-project-information)

---

## 🚀 Project Overview

[cite_start]The primary goal of this project is to design and analyze a simple 16-bit multi-cycle processor. [cite: 19] This involves a deep dive into the fundamental concepts of computer architecture, including:
* [cite_start]Instruction Set Architecture (ISA) design [cite: 19]
* [cite_start]Datapath component design (ALU, Register File) [cite: 19]
* [cite_start]Memory addressing and interfacing [cite: 19]
* [cite_start]Finite-State-Machine (FSM) based control logic [cite: 21]

The processor supports a curated set of computational and memory instructions, with ALU operations implemented using specific, efficient hardware algorithms. 

## ✨ Core Features

* **16-bit Datapath:** All registers and memory I/O are 16-bit wide.
* **Multi-Cycle Design:** Instructions are executed in a series of sequential steps, optimizing resource usage. 
* **Custom ISA:** A simple RISC-like ISA with two main instruction formats: **R-Type** and **M-Type**. 
* **Advanced ALU:** The ALU supports 16-bit signed (2's complement) arithmetic, including specialized algorithms for multiplication and division.
* **Unified Memory:** A single 64K x 16-bit memory is used for both instruction fetching and data (load/store) operations. 
* **Synthesizable Code:** The entire design is written in Verilog and is synthesizable, allowing for implementation on an FPGA or ASIC. 

---

## 🖥️ Instruction Set Architecture (ISA)

The ISA is defined with two primary 16-bit instruction formats.

### Instruction Formats

**R-Type (Register-Type)** 
Used for register-to-register computational operations.
| Bits  | 15:13  | 12:11 | 10:9 | 8:7  | 6:0    |
| :---- | :----- | :---- | :--- | :--- | :----- |
| Field | Opcode | rd    | rs1  | rs2  | Unused |
| Size  | 3 bits | 2 bits| 2 bits| 2 bits| 7 bits |

**M-Type (Memory-Type)**
Used for `LOAD` and `STORE` operations.
| Bits  | 15:13  | 12:11 | 10:9 | 8:0       |
| :---- | :----- | :---- | :--- | :-------- |
| Field | Opcode | rd/rs | base | Address   |
| Size  | 3 bits | 2 bits| 2 bits| 9 bits    |


### Supported Instructions

The processor supports the following 6 instructions:

| Instruction | Type   | Opcode | Description |
| :--- | :--- | :--- | :--- |
| **ADD** | R-Type | `000` | 16-bit Addition: `reg[rd] = reg[rs1] + reg[rs2]` |
| **SUB** | R-Type | `001` | 16-bit Subtraction: `reg[rd] = reg[rs1] - reg[rs2]` |
| **MUL** | R-Type | `010` | 16-bit Multiplication (Karatsuba Algorithm) |
| **DIV** | R-Type | `011` | 16-bit Division (Restoring Algorithm) |
| **LOAD** | M-Type | `100` | Load Word: `reg[rd] = mem[reg[base] + sign_ext(Address)]` |
| **STORE** | M-Type | `101` | Store Word: `mem[reg[base] + sign_ext(Address)] = reg[rd/rs]` |


---

## 🏛️ CPU Architecture

The processor is composed of four primary components: the ALU, the Register File, the Main Memory, and the Control Unit.

### Arithmetic Logic Unit (ALU)

The ALU handles all 16-bit signed (2's complement) arithmetic operations. To optimize performance, specific algorithms were implemented.

* **Addition (ADD) & Subtraction (SUB):**
    * [cite_start]Implemented using a **Carry Select Adder** at the gate level.
    * [cite_start]The 16-bit inputs are split into four 4-bit blocks. 
    * [cite_start]Each block (except the first) performs two parallel additions: one assuming a carry-in of `0` and one assuming a carry-in of `1`. 
    * [cite_start]The correct result is chosen by a multiplexer once the true carry from the previous block is known. 
    * Subtraction is achieved by 2's complementing the second operand and performing an addition.

* **Multiplication (MUL):**
    * Uses the **Karatsuba Algorithm** for 16-bit inputs. 
    * This recursive algorithm breaks the 16-bit multiplication into three 8-bit multiplications, reducing complexity. 
    * Each 8-bit multiplication is performed using the standard **Shift-and-Add** algorithm.

* **Division (DIV):**
    * Implemented using the classic **Restoring Division** algorithm. 
    * This iterative algorithm performs 16 cycles of shifting the remainder, subtracting the divisor, and conditionally restoring the remainder based on the result's sign.

### Register File

The register file is the CPU's high-speed local memory.
* **Size:** Contains **4 registers** (x0-x3).
* **Width:** Each register is **16 bits** wide. 
* **Ports:** Features **two read ports** (for simultaneous operand fetching) and **one write port**. 
* **Clocking:**
    * Read operations are performed on the **falling edge** of the clock. 
    * Write operations are performed on the **rising edge** of the clock, contingent on the `write_enable` signal being active. 

### Main Memory

The processor uses a single, unified memory for instructions and data.
* [cite_start]**Type:** Word-addressable, with a 2-byte (16-bit) word size. [cite: 82, 85]
* [cite_start]**Size:** $2^{16}$ addresses, for a total of 64K words (128 KB). [cite: 84]
* [cite_start]**Interface:** Accessed for instruction fetching (via the PC) and for data operations via `LOAD` and `STORE` instructions.

### Control Unit

The Control Unit is the brain of the processor, orchestrating the execution of instructions.
* [cite_start]**Program Counter (PC):** Contains a PC register that points to the current instruction in memory. [cite: 89]
* [cite_start]**Instruction Decoding:** Decodes the fetched instruction's opcode to generate all necessary control signals for the datapath components (ALU, Register File, Memory). [cite: 90]
* **Multi-Cycle Execution:** Manages the state transitions for each instruction.
* [cite_start]**`ready` Signal:** Asserts a `ready` signal upon completion of an instruction, which triggers the PC to increment and fetch the next instruction. 

---

## 📁 Directory Structure

[cite_start]The project files are organized as specified in the assignment documentation. 

---
## 🔬 Testing & Synthesis

* **Verification:** The design was verified using a comprehensive testbench (or multiple testbenches) designed to execute all supported instructions (`ADD`, `SUB`, `MUL`, `DIV`, `LOAD`, `STORE`) and validate their functionality. [cite: 92] The results were confirmed by analyzing the output waveforms.
* **Synthesis:** The Verilog code is fully synthesizable. [cite: 22] The project was synthesized using [Your Synthesis Tool, e.g., Xilinx Vivado or Intel Quartus], and the output files are located in the `/syn` directory.

---

## 🎓 Project Information

* **University:** [Sharif University Of Technology]
* **Course:** Digital Systems Design 
* **Professor:** Dr. Amin Fasahti
* **Semester:** Spring 1404 
