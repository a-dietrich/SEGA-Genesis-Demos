# Console Library

The console library provides functions for creating a TTY style text console on the Genesis that can be used for debug output on real hardware.

Characters are written as a stream, where lines are automatically wrapped if the horizontal border of the screen is reached. When at the bottom of the screen window, the console content is moved up by one text line and a blank row is inserted. Per default, the console window occupies a standad screen of 40x28 tiles. All text attributes, such as font, palette, plane etc., are taken from SGDK text settings. Screen updates are done using DMA transfer mode (which can be changed with `CON_setTransferMethod()`).

One of the use cases are assert messages. To this end, the Genesis state can be automatically reset before text is displayed.

## Usage

All console functions have been collected in a separate module (`console.h` and `console.c`). A text string can be written to the console by simply calling `CON_write()`, which works just like a standard printf() function. For string formatting this makes use of [stb_sprintf](http://github.com/nothings/stb).

```c
int CON_write(const char *fmt, ...)  __attribute__ ((format (printf, 1, 2)));
```

* **fmt** <br> C-string that contains the text to be written. It can optionally contain embedded format specifiers that are replaced by the values specified in subsequent additional arguments and formatted as requested. (See [printf format string](https://en.wikipedia.org/wiki/Printf_format_string).)

* ***... (additional arguments)*** <br> The function may expect a sequence of additional arguments, each containing a value to be used to replace a format specifier in the format string.

* ***returns*** <br> The total number of characters written is returned. This count does no include the additional null-character automatically appended at the end of the string.


This function also evaluates and executes escape control sequences. The following table lists the control characters that are supported:

| Escape code | Function             |
|-------------|----------------------|
| \\b         | backspace            |
| \\n         | new line (line feed) |
| \\r         | carriage return      |
| \\t         | horizontal tab       |
| \\v         | vertical tab

