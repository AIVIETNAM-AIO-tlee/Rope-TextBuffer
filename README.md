# Assignment 2 - Text Buffer using Rope Data Structure

## Course: CO2003 - Data Structures and Algorithms  
**Ho Chi Minh City University of Technology – Faculty of Computer Science and Engineering**

---

## Objective

This assignment aims to help students:
- Master advanced C++ programming.
- Understand and implement the **Rope** data structure (a type of balanced AVL binary tree).
- Use the Rope to implement a simple **Text Buffer** that supports efficient text manipulation operations such as insertion, deletion, and cursor movement.

---

## Overview

Rope is a data structure designed for efficiently managing and manipulating long strings, especially when modifications often occur in the middle. This assignment focuses on:
- Implementing Rope as a binary tree with internal and leaf nodes.
- Using Rope to build a `RopeTextBuffer` that supports typical editor operations.
- Supporting additional features like **undo**, **redo**, and **search**.

---

## Project Structure

- `main.cpp` and `main.h`: Provided by instructor (do **not modify**).
- `RopeTextBuffer.h`: Your implementation header file.
- `RopeTextBuffer.cpp`: Your implementation source file.

---

## Key Classes

### Rope
Represents the Rope tree with operations like:
- `insert`, `deleteRange`, `charAt`, `substring`, etc.
- Maintains **AVL balance** after edits.
- Supports **O(log n)** operations through tree traversal and balancing.

### RopeTextBuffer
Wraps the Rope to simulate a text editor:
- Cursor-based editing with insert, delete, move, replace.
- Undo/redo operations.
- Find (first and all) characters.

### HistoryManager
Handles:
- Action history tracking (insert, delete, move, replace).
- Undo/redo with saved cursor positions and content.
- `printHistory()` for logging action history.

---

## Compilation

**Instructions:**

1. **Navigate to the project directory:**
```bash
cd path/to/Rope-TextBuffer-master
```

2. **Compile the project:**
```bash
g++ -o main main.cpp RopeTextBuffer.cpp -I . -std=c++17
```

3. **Run the application:**
```bash
./main
```
