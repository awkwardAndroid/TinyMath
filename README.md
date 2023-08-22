# TinyMath
Tiny language for math

This is my first time dipping into anything resembling language creation so this is far from good, error proof or complete.

## How it works
Currently the language holds very basic math.

!You ether need to have g++ or change the compiler!

The compiled math.cpp program takes files with .mh extension as input(./math filename.mh)
and creates a new cpp file and compiles it.



You can do
* Addition
* Subtraction
* Multiply
* Divide

and create variables to hold values.

The print function outputs what ever you put after it(cant do strings yet).

All the values are currently [double's](https://learn.microsoft.com/en-us/office/vba/language/reference/user-interface-help/double-data-type)

### Example of a .mh file
Remember semicolons!

filename = test.mh
```
x = 100;
other = 8;

print x * other;
print 2.0 / 10;
print -2 -2:
```
Then if you do
```
./math test.mh
800
0.2
-4
```
