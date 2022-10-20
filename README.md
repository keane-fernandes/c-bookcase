<h1 align="center">
  <br>
    <img src=./docs/bookcase.png alt="bookcase.png" width="100"></a>
  <br>
    Happy Bookcases
  <br>
</h1>

<h4 align="center">A breadth-first search (BFS) algorithm implementation to sort an unsorted bookcase.</h4>

<p align="center">
  <a href="#Features">Features</a> |
  <a href="#Overview">Overview</a> |
  <a href="#Design">Design</a>     |
  <a href="#Usage">Usage</a>       |
  <a href="#License">License</a>   |
</p>

# Features
- Optimised breadth-first search algorithm
- Central focus on memory and performance
- Test driven development using assertions

# Overview
This program reads in a bookcase definition file (found in the [bookcases](bookcases) directory in this repository), and shows the 'moves' to make the bookcase happy.

A bookcase is only happy if:
- Each shelf only has books of one colour (or is empty).
- All books of the same colour are on the same shelf.
- The only books that exist are black (K), red (R), green (G), yellow (Y), blue (B), magenta (M), cyan (C) or white (W).

The following rules apply with regards to 
1. You can only move one book at a time.
2. The only book that can move is the rightmost one from each shelf. 
3. The book must move to become the rightmost book on its new shelf. 
4. You can’t put more books on a shelf than its maximum size.

A bookcase definition file looks something like this:

```bc
4 3 7
RG.
GR.
CY.
YC.
```

where the first row will always have three digits where: 
- the first digit refers to the number of shelves in the bookcase
- the second digit refers to the number of books a shelf can hold
- the third digit is optional and refers to the minimum number of moves needed to make the bookcase happy 

and the second row and beyond represent the shelves (books and free spaces) according to the table below:

| Character   | Description      |
| ----------- | ---------------- |
| .           | Free space       |
| K           | Black book       |
| R           | Red book         |
| G           | Green book       |
| Y           | Yellow book      |
| B           | Blue book        |
| M           | Magenta book     |
| C           | Cyan book        |
| W           | White book       |

# Design
A brute-force algorithm is used for searching over all moves to make the bookcase happy. The algorithm is very similar to a queue and hence is a breadth-first search. Here's a summary of the steps:
1. Create a linked list of bookcases.
2. The initial bookcase is put into the front of this linked list.
3. Take a bookcase from the front of the list.
4. For this (parent) bookcase, find the resulting (child) bookcases which can be created from all the valid possible single book moves. Put each of these bookcases into the end of the list. There may be as many as height × (height − 1) of these. If a happy bookcase is found, stop. Else, go to 3.

# Usage
Ensure you have [GCC](https://gcc.gnu.org) installed on your machine. Once you have done so, run the following command on your terminal:

```bash
make bookcase
```

There are ten bookcases in the [bookcases directory](bookcases) on which you can run the sorting algorithm and there are commands in the [Makefile](Makefile) to run each of them. As an example, if you want to run it on bookcase five from the [bookcases directory](bookcases), type in the following command on your terminal:

```bash
make sort_bc_five
```
to which the output on the terminal would be:

<p align="center">
  <img width=50 src="docs/bc_five.jpg">
</p>

Another example would be:

```bash
make sort_bc_six
```
to which the output on the terminal would be:

<p align="center">
  <img width=40 src="docs/bc_six.png">
</p>

Make your own bookcase files and try this sorting algorithm out!

# License

```
Copyright (c) 2021 Keane Fernandes

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

