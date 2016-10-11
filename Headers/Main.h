/*    
**    URMulator v20161011
**    Copyright © 2016 Luka Aleksić
**    All rights reserved
**    
**    Web: <github.com/laleksic/URMulator>
**    E-mail: <laleksic@mail.ru>
*/

#ifndef MAIN_H
    #define MAIN_H

    /*
    **    Standard libraries
    */
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdarg.h>

    /*
    **    Third party libraries
    */
    #include <argp.h>
    #include <glib.h>
    #include <gvc.h>
    #include <gtk/gtk.h>
    #include <gtksourceview/gtksourceview.h>
    #include <goocanvas.h>

    /*
    **    Local headers
    */
    #include "Register.h"
    #include "Instruction.h"
    #include "URM.h"
    #include "Graph.h"
    #include "GUI.h"

    /*
    **    Macro for iterating over a GList
    **    Source :
    **    <stackoverflow.com/questions/1611585/macro-for-iterating-over-a-glist>
    */
    #define GFOREACH(item, list) for(GList *__glist = list;\
                                     __glist && (item = __glist->data, TRUE);\
                                     __glist = __glist->next)

    struct arguments {
        char** args;
        int argc;

        gboolean verbose;        //    Verbose output (print register values
                                 //    after each instruction)

        gboolean graph;          //    Generate graph using GraphViz
                                 //    (output format is .png)

        gboolean gui;            //    Launch in GTK2 powered GUI mode

        int ip_lim;              //    Max number of instructions to execute
                                 //    (infinite loop safeguard)
    };

    void Urmulate(struct arguments arguments);
#endif
