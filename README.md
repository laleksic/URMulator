![URMulator](https://raw.githubusercontent.com/laleksic/URMulator/master/Images/Screenshot.png "URMulator")
# URMulator v20161011
Copyright © 2016 Luka Aleksić  
All rights reserved
	
Web: <https://github.com/laleksic/URMulator>  
E-mail: <laleksic@mail.ru>

## About

URMulator is an Unlimited Register Machine (URM) simulator with a graphical interface and graph generation. I started it in early 2016 as a hobby project to learn C, and as an aid to solving URM problems
part of the curriculum of Informatics studies at the Faculty of Mathematics, University of Belgrade.
I release it publically in the hope that it will be useful to fellow students.

**What makes it different from other URM simulators** is the graphical interface with a built-in text editor for writing URM programs, and an image viewer for a dynamically generated graph based on your URM program, replacing pen and paper, making solving URM problems more fluid and hopefully more intuitive.

## Changelog

*v20161011* - First public version. Essentials finished.

## Planned
*       Fixing all trouble with the aesthetic features
*       Allowing customization of graph generation (colors, shapes, font, output format etc.)
*       Syntax highlighting in text editor
*	Optimizing, cleaning up and consistently commenting the source
*	Fixing any bugs that pop up
*	Allowing user customization of the GUI and adding more functionality in general for the GUI
*	Adding ability to work with essentialy unlimitedly large numbers

## Known bugs
*	None so far. Report bugs to <laleksic@mail.ru>.

## Permissions

All rights reserved unless otherwise stated.

You **MAY**:

* view, fork and download the source code. 
* compile the unmodified source code and use the unmodified program for personal purposes.

You **MUST NOT**:

* modify the source code, nor distribute it (modified or not), nor distribute executables (modified or not) without my prior permission.
* use the source code or any part of it in your own project(s) or otherwise without my prior permission.

For permissions contact me at <laleksic@mail.ru>.

## Installation
### Requirements
* **Linux**
* **gcc**, **make** and **standard C libraries**
* **glib** and development libraries
* **graphviz** and development libraries
* **gtk2** and development libraries
* **gtksourceview** and development libraries
* **goocanvas** and development libraries

On Ubuntu Linux (preferred distribution at my university) the following command covers them all.

    sudo apt install build-essential pkg-config x11proto-randr-dev libglib2.0-dev libgtk2.0-dev libgtksourceview2.0-dev libgoocanvas-dev graphviz-dev

### Compiling
Navigating to the projects root directory, running `make` will compile the source and output executable files to the `Executable/` directory from which it can be run. Running `make install` as root will install the URMulator.

If you have any trouble compiling or running the program contact me at <laleksic@mail.ru>.

## Usage

The URMulator outputs the resulting register values for a defined URM program and set starting register values, and (optionally) generates a graph in **.png** format.

For the purposes of the URMulator, an URM program is defined via a plaintext file with instructions written in the URM syntax.

### The URM syntax

Allowed instructions are as follows:

* `Z(x)` Sets register `x` to 0.

* `S(x)` Increases value of register `x` by 1.  
 If register `x` is unused, initializes it to 1.

* `T(x,y)` Sets value of register `y` to that of register `x`.  
If register `y` is unused, initializes it to the value of `x`.  
If `x` is unused, initializes both `x` and `y` to 0.
* `J(x,y,z)` Compares the values of registers `x` and `y`, and if they are equal, skips to instruction number `z`.  
If either of the registers `x` or `y` are unused, they are assumed to have a value of 0.  
 Jumping to instruction number 0, or an instruction larger than the total number of instructions in the program means stopping the program if the registers are equal.  
Jumps where `x=y` (e.g. `J(1,1,0)` are unconditional and will always jump to the designated instruction, or in this case, terminate the program.

Instructions **must** be preceded by a colon (`:`).  
Instructions may be written with both lower and upper case letters. `x`,`y` and `z` **must** be integers.  

Comments may be written before or after instructions, and require no special indication, however they **must not** include colons(`:`).  

You may, for example, number instructions as such `1: z(4)` for your convenience.  

Spaces and new lines are ignored, hence  

    This is a comment
    1    :
    Z (  4  )  This too

is equally valid.

### An example program
If the starting value for register 1 is a multiple of the starting value for register 2, returns 1 as new value for register 1. Otherwise returns 0 as new value for register 1.

    1	:j(3,1,8)	
    2	:j(4,2,6)

    3	:s(3)
    4	:s(4)
    5	:j(1,1,1)

    6	:z(4)
    7	:j(1,1,1)

    8	:z(1)
    9	:j(4,2,11)
    10	:j(1,1,0)
    11	:s(1)
    12	:j(1,1,0)

### Aesthetic features (experimental)

With the goal of generating a more pleasing graph, for printing purposes etc. the URMulator includes a way to group instructions into meaningful blocks on the graph, and to make certain comments visible on the graph, like so:

The following program,

    1: z(0)
    2: z(0)
    3: z(0)

    #FANCY COMMENT#
    4: j(1,2,8)

    { BRANCH_NO
        5: s(0)
        6: s(0)
        7: s(0)
    }

    { BRANCH_YES
        8: t(1,2)
        9: t(1,2)
       10: t(1,2)
    }

    11: z(0)

yields the following graph.

![URMulator](https://raw.githubusercontent.com/laleksic/URMulator/master/Images/Experimental.png "URMulator")

Comments enclosed with hash tags (`#`) will be displayed in a special note node on the graph itself. Such comments may not exceed 256 characters in length.

Instructions enclosed with curling braces (`{` and `}`) will be visually separated on the graph in their own block. A string must follow the opening brace to signify the block's name and it may not contain spaces and may not exceed 256 characters in length.

Neither of these influences the working of the program, and they are merely aesthetic. Both are barely implemented, and it is **not recommended** that you use them, though the next version hopes to remedy any problems.




### Invoking URMulator from terminal

    urm [-guv?V] [-l X] [--graph] [--limit=X][--gui] [--verbose] [--help]
    [--usage] [--version] INPUT REG1 REG2 REG3 ...
    
    -g, --graph                Generate Graphviz graph
    -l, --limit=X              Stop after X instructions (inf.loop safeguard)
    -u, --gui                  GTK2 powered graphic interface
    -v, --verbose              Print register information after each instruction
    -?, --help                 Give this help list
        --usage                Give a short usage message
    -V, --version              Print program version

Mandatory or optional arguments to long options are also mandatory or optional
for any corresponding short options.

Running `urm Multiples 25 6` will simply run the URM program defined in the plaintext file `Multiples` for the set starting register values of 25 and 6 for registers 1 and 2 respectively and print the resulting register values, both to standard terminal output and to a file entitled `Multiples.out` in the working directory.

Running the same with the `-g` or `--graph` option would produce a Graphviz generated graph in the **.png** in the working directory entitled `Multiples.graph`. Graph colors, size and output format are currently hardcoded, though they will be made customizable in future.

To prevent infinite loops, such as would be caused by a program like this,

    1:J(1,1,1)

the URMulator has a limit on how many functions to execute before it terminates the program. It defaults to 10000, though in some cases that will not suffice, and you can set it to any number you like with the `--limit=X` option, where `X` is your chosen limit.

The `--verbose` option influences output, and makes it print registers values after each instruction, rather than only the end values. Contrast:

Without `--verbose`:

    Reached end of program.

    Performed 144 instructions.

    Output:
    R1=0
    R2=6
    R3=25
    R4=1

With `--verbose`:

    Executing instructions:
    1:No jump
    2:No jump
    3:R3=1
    4:R4=1
    5:Jump to 1
    1:No jump
    2:No jump
    3:R3=2
    4:R4=2
    5:Jump to 1

    ... etc ...

    1:Jump to 8
    8:R1=0
    9:No jump
    10:Jump to 0
    Reached end of program.

    Performed 144 instructions.

    Output:
    R1=0
    R2=6
    R3=25
    R4=1

### Graphical interface
Starting the URMulator with the `-u` or `--gui` option will initiate the GUI,
and also all other options and arguments will be ignored, as they will be set
within the GUI. The usage of the GUI itself should be self explanitory.
