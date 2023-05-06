## Things to work on
- handling bad user input (check bounds on xy) ? 
- displaying the board with coordinates (spreadsheet style??)
- in the future, graphical user interface (sdl/wasm) or ncurses ?
- flags
- end screen (win/lose)
- multiple games?
- maybe move calc neighbors into init?


## Plan
board with 2d array?
- cells store type (flag/revealed/hidden/mine), neighbors

cell {
    type: char (f, r, h, m) enum?
    neighbors: int
}

input: 
print board with letters/numbers, user types coordinates and whether they
want to flag or reveal that square

revealing:
check if square is mine, lose
else call recursive function on square's neighbors
    check if this square's neighbors are mines
        if no mines, reveal and call function for neighbors
        else display number of mine neighbors, no function call for neighbors

check neighboring boxes for empty/mine squares

check win

