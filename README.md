# Minishell Project Description

## Overview

**Title**: Minishell  
**Summary**: This project involves creating a simple shell, akin to a basic version of bash. It focuses on teaching about processes and file descriptors.

## Key Components

- **Programming Language**: C
- **Core Objective**: To write a shell that can interpret and execute basic command line instructions.

## Features

1. **Basic Shell Functionality**: 
   - Display a prompt for new commands.
   - Execute the correct executable based on PATH or given path.
   - Handle command history.
   - Manage input/output redirection and pipes.
   - Interpret environment variables and return exit status of commands.

2. **Built-in Commands**: 
   - `echo` (with `-n` option)
   - `cd` (with relative or absolute paths)
   - `pwd` (no options)
   - `export`, `unset`, `env` (with no options or arguments)
   - `exit` (no options)

3. **Signal Handling**: 
   - Implement behavior for Ctrl-C, Ctrl-D, and Ctrl-\ in interactive mode.

4. **Error Handling and Memory Management**: 
   - Avoid unexpected exits (e.g., segmentation faults).
   - Ensure no memory leaks occur.

5. **Compliance**: 
   - Follow specified coding standards and submission guidelines.

## Additional Features (Bonus Part)

- Implement logical operators (`&&`, `||`) with priority handling.
- Support wildcards (`*`) for the current working directory.
