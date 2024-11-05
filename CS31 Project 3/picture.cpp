#include "grid.h"
#include <iostream>
#include <cctype>
#include <cassert>
#include <string>
using namespace std;

bool isInside(int r, int c) {
    if (r > getRows() || r <= 0 || c > getCols() || c <= 0) { /* checks whether the end of the line will be within bounds */
        return false; /* if not in bounds, return false */
    }
    return true; /* if in bounds, return true */
}

void plotHorizontalLine(int r, int c, int distance, char ch) {
    if (distance < 0) { /* if distance is negative, return without doing anything */
        return; 
    }
    for (int i = 0; i <= distance; i++) { /* set a character in the location if it is in bounds */
        if (isInside(r, c)) {
            setChar(r, c, ch);
        }
        c++;
    }
}

void plotVerticalLine(int r, int c, int distance, char ch) {
    if (distance < 0) { /* if distance is negative, return without doing anything */
        return;
    }
    for (int i = 0; i <= distance; i++) { /* set a character in the location if it is in bounds */
        if (isInside(r, c)) {
            setChar(r, c, ch);
        }
        r++;
    }
}

void plotRectangle(int r, int c, int height, int width, char ch) {
    if (width < 0 || height < 0) { /* if width and/or height is negative, return without doing anything */
        return;
    }
    plotHorizontalLine(r, c, width, ch); /* call horizontal line function to draw horizontal line from beginning point */
    plotVerticalLine(r, c, height, ch); /* call vertical line function to draw vertical line from beginning point */
    plotHorizontalLine(r + height, c, width, ch); /* call horizontal line function to draw horizontal line from (r+height, c) */
    plotVerticalLine(r, c + width, height, ch); /* call vertical line function to draw vertical line from (r, c+width) */
}

const int HORIZ = 0; /* define global constants */
const int VERT = 1;
const int FG = 0;
const int BG = 1;

bool plotLine(int r, int c, int distance, int dir, char plotChar, int fgbg) {
    if (dir != HORIZ && dir != VERT) { /* if dir is not 0 or 1, then do not plot the line, return false */
        return false;
    }
    if (fgbg != FG && fgbg != BG) { /* if fgbg is not 0 or 1, do not do anything, return false */
        return false;
    }
    if (isprint(plotChar) == false) { /* if plotChar is not a printable character, return false */
        return false;
    }
    if (dir == HORIZ && (c + distance > getCols() || c + distance <= 0)) { /* if dir is HORIZ and the end of the line is not within the bounds of the columns, return false */
        return false;
    }
    if (dir == VERT && (r + distance > getRows() || r + distance <= 0)) { /* if dir is VERT and the end of the line is not within the bounds of the rows, return false */
        return false;
    }
    if (dir == HORIZ) { /* if direction is horizontal */
        if (distance < 0) { /* if distance is negative */
            for (int i = 0; i <= -distance; i++) {
                if (fgbg == FG || (fgbg == BG && getChar(r, c) == ' ')) { /* if fgbg is FG or if fgbg is BG and the current character there is ' ', set a character in the location and move along the distance backwards */
                    setChar(r, c, plotChar);
                }
                c--;
            }
        }
        else {
            for (int i = 0; i <= distance; i++) { /* do the same thing is distance is positive, but the other direction */
                if (fgbg == FG || (fgbg == BG && getChar(r, c) == ' ')) {
                    setChar(r, c, plotChar);
                }
                c++;
            }
        }
    }
    if (dir == VERT) { /* if distance is vertical */
        if (distance < 0) {
            for (int i = 0; i <= -distance; i++) {
                if (fgbg == FG || (fgbg == BG && getChar(r, c) == ' ')) { /* plot character upward only if fgbg is FG or fgbg is BG and the current character there is ' ' */
                    setChar(r, c, plotChar);
                }
                r--;
            }
        }
        else {
            for (int i = 0; i <= distance; i++) { /* again do the same thing but plots downward */
                if (fgbg == FG || (fgbg == BG && getChar(r, c) == ' ')) {
                    setChar(r, c, plotChar);
                }
                r++;
            }
        }
    }
    return true;
}

bool hasSyntaxError(string commandString, int &i) {
    while (i < int(commandString.length())) { /* check the full string for syntax errors */
        char command = commandString[i];
        if (command == 'F' || command == 'f' || command == 'B' || command == 'b') { /* if this command is f or b */
            i++;
            if (i == commandString.length() || !isprint(commandString[i])) { /* return true if the character is not printable or if eol */
                return true;
            }
            else {
                i++; /* continue loop */
            }
        }
        else if (command == 'V' || command == 'v' || command == 'H' || command == 'h') { /* if command is v or h */
            i++;
            if (i == commandString.length()) { /* return true if eol */
                return true;
            }
            if (commandString[i] == '-') { /* check if next character is minus sign */
                i++;
                if (i == commandString.length()) { /* return true if eol */
                    return true;
                }
            }
            if (!isdigit(commandString[i])) { /* return true if there is no digit */
                return true;
            }
            i++;
            if (i < commandString.length() && isdigit(commandString[i])) { /* check if there is another digit */
                i++;
            }
        }
        else if (command == 'C' || command == 'c') { /* if command is c, move on */
            i++; 
        }
        else { /* any other character beginning a command, return true */
            return true;
        }
    }
    return false; /* or else there is no syntax error */
}

void validString(string commandString, int& i, string& argument, char &command) {
    command = commandString[i];
    if (command == 'F' || command == 'f' || command == 'B' || command == 'b') { /* if command is f or b, argument is the first character after */
        argument += commandString[i + 1];
        i += 2;
    }
    else if (command == 'V' || command == 'v' || command == 'H' || command == 'h') { /* if command is v or h */
        i++;
        if (commandString[i] == '-') { /* add this to argument if it is a minus sign */
            argument += commandString[i++];
        }
        argument += commandString[i++]; /* add the first digit because we checked for syntax errors already */
        if (i < commandString.length() && isdigit(commandString[i])) { /* if the next char is in bounds and a digit, add to argument */
            argument += commandString[i++];
        }
    }
    else if (command == 'C' || command == 'c') { /* just move on if command is c*/
        i++;
    }
}

int performCommands(string commandString, char& plotChar, int& mode, int& badPos) {
    int r = 1; /* initialize r and c */
    int c = 1;
    if (isprint(plotChar) == false || (mode != FG && mode != BG)) { /* return 2 if not a valid character or mode */
        return 2;
    }
    int n = 0;
    if (hasSyntaxError(commandString, n)) { /* call this function to determine if there are syntax errors */
        badPos = n;
        return 1;
    }
    int i = 0;
    while (i < commandString.length()) {
        string argument = "";
        char command;
        validString(commandString, i, argument, command); /* call this function to get the full command and argument */
        int distance = 0;
        if (tolower(command) == 'v') {
            distance = stoi(argument); /* change argument to a digit */
            if (r + distance > getRows() || r + distance <= 0) { /* check if out of bounds */
                badPos = i - int(argument.length()) - 1;
                return 3;
            }
            plotLine(r, c, distance, VERT, plotChar, mode); /* plot the line */
            r += distance; /* change position */
        }
        else if (tolower(command) == 'h') {
            distance = stoi(argument); /* change argument to a digit */
            if (c + distance > getCols() || c + distance <= 0) { /* check if out of bounds */
                badPos = i - int(argument.length()) - 1;
                return 3;
            }
            plotLine(r, c, distance, HORIZ, plotChar, mode); /* plot the line */
            c += distance; /* change position */
        }
        else if (tolower(command) == 'f') {
            mode = FG; /* change mode to FG */
            plotChar = argument[0]; /* set new character to the argument, only the first character is needed since argument here is only one character */
        }
        else if (tolower(command) == 'b') {
            mode = BG; /* change mode to BG */
            plotChar = argument[0]; /* set new character to argument */
        }
        else if (tolower(command) == 'c') { /* reset everything if command is c */
            plotChar = '*';
            r = 1;
            c = 1;
            mode = FG;
        }
    }
    return 0;
}

int main() {
    setSize(12, 15);
    cout << "enter thing: ";
    string test;
    getline(cin, test);
    int badPos = 9999;
    int m = FG;
    char pc = 'c';
    cout << performCommands(test, pc, m, badPos) << " " << badPos << endl;
    draw();

    /* setSize(12, 15);
    assert(plotLine(3, 5, 2, HORIZ, '@', FG));
    for (int c = 5; c <= 7; c++)
        assert(getChar(3, c) == '@');
    assert(getChar(3, 8) == ' ');
    clearGrid();
    char pc = '%';
    int m = FG;
    int bad = 999;
    // A successful command string should not change bad
    assert(performCommands("V2", pc, m, bad) == 0 && getChar(3, 1) == '%' && bad == 999);
    assert(performCommands("V2H2Q2", pc, m, bad) == 1 && bad == 4);
    assert(performCommands("H3V99CF\t", pc, m, bad) == 1 && bad == 7);
    cout << "All tests succeeded." << endl; */
}
