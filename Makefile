#	
#	URMulator v20161011
#	Copyright © 2016 Luka Aleksić
#	All rights reserved
#	
#	Web: <github.com/laleksic/URMulator>
#	E-mail: <laleksic@mail.ru>
#

SourceDirectory=Sources
_Sources=Main.c Register.c Instruction.c URM.c Graph.c GUI.c
Sources=$(patsubst %,$(SourceDirectory)/%,$(_Sources))

HeaderDirectory=Headers

InstallDirectory=/usr/local/bin/

ExecutableDirectory=Executable
ExecutableName=urm

Urmulator:
	gcc -g -w -std=c99 \
	 `pkg-config --cflags --libs glib-2.0 libgvc gtk+-2.0 gtksourceview-2.0 goocanvas` \
	 -I$(HeaderDirectory) \
	 -o $(ExecutableDirectory)/$(ExecutableName) \
	 $(Sources)

install: Urmulator
	install -m 0755 $(ExecutableDirectory)/$(ExecutableName) $(InstallDirectory)
   
.PHONY: install
