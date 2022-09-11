# CS246E final project

This program (named vm) is a simpler version of the vim editor. It implements many of the common key command and features in vim (quick movement, search, text delete/replace, copy/paste, undo, command multiplier, syntax highlighting, etc.).

The program is written in C++ and demonstrates good object-oriented design (SOLID principles) as well as good coding practices in general. A design plan and a UML diagram were written prior to development. The program is designed so that new actions, functionalities, and visual layouts can be easily added or extended in the future. A MVC designed architecture is employed.

The RAII idiom is followed with the use of smart pointers and STL containers. The program design also follows the NVI idiom as much as possible.

The ncurses library is used to enable display capabilities for the program.

This program was written as the final project for CS246E during the Fall 2021 term.
