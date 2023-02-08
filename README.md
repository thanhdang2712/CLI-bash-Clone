# Programming Project: Shell

## Overview
A simple shell that operates in the following basic way: when you type in a command (in response to the shell prompt), the shell creates a child process that executes the command that was entered, then prompts for more user input when finished. The shell you implement will be similar to one that you use every day in Linux (e.g., `bash`), but with many simplifications. 

## Getting started 
Clone your repository on your RPi/Personal Computer.

Compile the shell using `make`, and run the shell using `./shell`. 

Your shell will run in an interactive fashion. It will display a prompt (`shell>`) and wait for the user to enter a command. The user will enter the program to execute (e.g., `/bin/ls`) or a "built-in" command (e.g., `exit`). Non-built-in commands should contain the full path to the executable file (e.g., `/bin/ls` rather than just `ls`) and may contain an arbitrary number of command-line options (e.g., `/bin/ls -l -r`).  If the user enters a non-built-in command, a new process will be created and the specified program will be started. The shell will wait for the program to finish (unless the command includes an ampersand `&` at the end to indicate it should run in the background), display the prompt, and allow the user to enter another command.

## Acknowledgements
This project was part of the COSC208: Introduction to Computer Systems, taught by Professor Aaron Gember-Jacobson. The objective of the assignment is to implement threads, processes, and understand how to build a CLI tool to deploy bash commands.
