# URMulator v20161011
Copyright © 2016 Luka Aleksić  
All rights reserved
	
Web: <https://github.com/laleksic/URMulator>  
E-mail: <laleksic@mail.ru>

## About

URMulator is an Unlimited Register Machine (URM) simulator with a graphical interface and graph generation. I started it in early 2016 as a hobby project to learn C, and as an aid to solving URM problems
part of the curriculum of Informatics studies at the Faculty of Mathematics, University of Belgrade.
I release it publically in the hope that it will be useful to fellow students.

## Permissions

All rights reserved unless otherwise stated.

You **MAY** view, fork and download the source code. 
 
You **MAY** compile the unmodified source code and use the unmodified program for personal purposes.

You **MUST NOT** modify the source code, nor distribute it (modified or not), nor distribute executables (modified or not) without my prior permission.

You **MUST NOT** use the source code or any part of it in your own project(s) or otherwise without my prior permission.

Contact me at <laleksic@mail.ru>.

## Installation
### Requirements
* gcc, make and standard C libraries
* glib and development libraries
* graphviz and development libraries
* gtk2 and development libraries
* gtksourceview and development libraries
* goocanvas and development libraries

### Compiling
Navigating to the projects root directory, running `make` will compile the source and output executable files to the `Executable/` directory from which it can be run. Running `make install` as root will install the URMulator.

## Usage

### URM syntax

The URMulator outputs the resulting register values for a defined URM program and set starting register values, and (optionally) generates a graph in .png format.

For the purposes of the URMulator, an URM program is defined via a text file with commands written in my URM syntax

### Terminal

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

### Graphical interface
Starting the URMulator with the `-u` or `--gui` option will initiate the GUI,
and also all other options and arguments will be ignored, as they will be set
within the GUI. The usage of the GUI itself should be self explanitory.


## Changelog

*v20161011* - First public version. Essentially finished.

## Planned
*	Optimizing, cleaning up and consistently commenting the source
*	Fixing any bugs that pop up
*	Allowing user customization of aesthetic aspects of the GUI
*	Adding more functionality in general for the GUI
*	Adding ability to work with essentialy unlimitedly large numbers
	
## Known bugs
*	None so far. Report bugs to <laleksic@mail.ru>.



