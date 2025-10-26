# Mini-VCS: A Terminal-Based Version Control System

> A custom-built, terminal-based Version Control System (VCS) inspired by Git. This project allows users to track and manage different versions of their files directly from the command line.

**Author:** Reihane Ebrhimzade
**Student ID:** 402105568

---

## Overview

This project is a from-scratch implementation of a Version Control System, similar in concept to **Git**. It operates entirely within the terminal, providing a lightweight and powerful tool for managing file history, tracking changes, and reverting to previous versions.

## Key Features

* **File Tracking:** Monitor changes to files within a project directory.
* **Versioning:** Save "snapshots" (like commits) of your files at different points in time.
* **History Management:** View the history of changes and navigate between different versions.
* **Git-like Commands:** Uses an intuitive command-line interface that will feel familiar to Git users.

## Getting Started

### Prerequisites

*(Add any prerequisites here, e.g., C++ compiler, Python 3, etc.)*

* Example: `g++` (for C++)
* Example: `Python 3.10`

### Installation

1.  Clone the repository:
    ```bash
    git clone [your-repository-link-here]
    cd [repository-folder-name]
    ```
2.  Build the project:
    *(Add your build commands here)*
    ```bash
    # Example:
    make
    ```

## Usage

The system is operated via a set of simple commands, similar to Git.

*(This is a great place to list your actual commands)*

### Example Commands

* **Initialize a new repository:**
    ```bash
    ./mini-vcs init
    ```
* **Add files to be tracked:**
    ```bash
    ./mini-vcs add [file-name]
    ```
* **Save the current version:**
    ```bash
    ./mini-vcs commit -m "Your commit message"
    ```
* **View version history:**
    ```bash
    ./mini-vcs log
    ```

---

*(Feel free to add other sections like "Project Goals" or "Technical Details" if you want to elaborate more!)*
