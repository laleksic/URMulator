/*	
**	URMulator v20161011
**	Copyright © 2016 Luka Aleksić
**	All rights reserved
**	
**	Web: <github.com/laleksic/URMulator>
**	E-mail: <laleksic@mail.ru>
*/

#ifndef GUI_H
	#define GUI_H

	#include <gvc.h>	
	#include <gtk/gtk.h>
	#include <goocanvas.h>
	#include <gtksourceview/gtksourceview.h>

	/*
	**	Hierarchy of gtk widgets in gui:
	**
	**	window --> vbox --+--> menu_bar --> file_menu --+--> new_item
	**                    |                             +--> open_item
	**                    |                             +--> save_item
	**                    |                             +--> quit_item
	**                    |
	**                    +--> hpaned --+--> scrwin1 --> iview
	**	                  |             |
	**	                  |             +--> vpaned --+--> scrwin2 --> canvas
	**	                  |                           +--> scrwin3 --> oview
	**	                  |
	**                    +--> hbox1 --+--> label_reg
	**	                  |            +--> input_reg
	**	                  |
	**                    +--> hbox2 --+--> label_inst1
	**	                  |            +--> input_inst
	**	                  |            +--> label_inst2
	**	                  |
	**                    +--> hbox3 --+--> checkbox_verbose
	**	                               +--> button_urmulate
	**
	**	The window and all widgets are declared as globals to allow callback
	**	functions to access them effortlessly
	*/
	GtkWidget *window,
			  *vbox,
			  *menu_bar,
			  *file_item,
			  *file_menu,
			  *new_item,
			  *open_item,
			  *save_item,
			  *quit_item,
			  *hpaned,
			  *scrwin1,
			  *iview,
			  *vpaned,
			  *scrwin2,
			  *scrwin3,
			  *canvas,
			  *oview,
			  *hbox1,
			  *label_reg,
			  *input_reg,
			  *hbox2,
			  *label_inst1,
			  *input_inst,
			  *label_inst2,
			  *hbox3,
			  *checkbox_verbose,
			  *button_urmulate;
			  
	GtkTextBuffer *ibuffer, *obuffer;
	GooCanvasItem *canv_root, *canv_image;

	/*
	**	Functions
	*/
	gboolean delete_event(GtkWidget *widget, GdkEvent *event, gpointer data);
	void destroy(GtkWidget *widget, gpointer data);

	void FileNew(GtkWidget *widget, gpointer data);
	void FileSave(GtkWidget *widget, gpointer data);
	void FileOpen(GtkWidget *widget, gpointer data);

	void GUIurmulate(GtkWidget *widget, gpointer data);

	void GUImain();
#endif