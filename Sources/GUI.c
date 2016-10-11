/*	
**	URMulator v20161011
**	Copyright © 2016 Luka Aleksić
**	All rights reserved
**	
**	Web: <github.com/laleksic/URMulator>
**	E-mail: <laleksic@mail.ru>
*/

#include "Main.h"

gboolean delete_event(GtkWidget *widget, GdkEvent *event, gpointer data) {
	gtk_main_quit();
}
void destroy(GtkWidget *widget, gpointer data) {
	gtk_main_quit();
}

void FileNew(GtkWidget *widget, gpointer data) {
	FILE *def;
	def = fopen(".new","w");
	fprintf(def,"URMulator v20161011\n"
				"Copyright © 2016 Luka Aleksić\n"
				"All rights reserved\n\n"
				"Web: <github.com/laleksic/URMulator>\n"
				"E-mail: <laleksic@mail.ru>");
	fclose(def);
	char *filename = ".new";
    char *contents;
    g_file_get_contents(filename,&contents,NULL,NULL);
    gtk_text_buffer_set_text(ibuffer,contents,-1);
}

void FileSave(GtkWidget *widget, gpointer data) {
	GtkWidget *dialog=gtk_file_chooser_dialog_new("Save File",
									     		  window,
									     		  GTK_FILE_CHOOSER_ACTION_SAVE,
									     		  GTK_STOCK_CANCEL,
									     		  GTK_RESPONSE_CANCEL,
									     		  GTK_STOCK_SAVE,
									     		  GTK_RESPONSE_ACCEPT,
									     		  NULL);

	gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(dialog),
												   TRUE);

	if(gtk_dialog_run(GTK_DIALOG(dialog))==GTK_RESPONSE_ACCEPT)
	{
	    char *contents;
	    GtkTextIter start, end;
	    gtk_text_buffer_get_start_iter(ibuffer,&start);
	    gtk_text_buffer_get_end_iter(ibuffer,&end);
	    contents=gtk_text_buffer_get_text(ibuffer,&start,&end,TRUE);
	    char *filename=gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
	    g_file_set_contents(filename,contents,-1,NULL);
	}
	gtk_widget_destroy (dialog);
}

void FileOpen(GtkWidget *widget, gpointer data) {
	GtkWidget *dialog=gtk_file_chooser_dialog_new("Open File",
					      				  		  window,
				      					  		  GTK_FILE_CHOOSER_ACTION_OPEN,
				      					  		  GTK_STOCK_CANCEL,
				      					  		  GTK_RESPONSE_CANCEL,
				      					  		  GTK_STOCK_OPEN,
				      					  		  GTK_RESPONSE_ACCEPT,
				      					  		  NULL);

	if (gtk_dialog_run(GTK_DIALOG(dialog))==GTK_RESPONSE_ACCEPT)
	{
	    char *contents;
	    char *filename=gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
	    g_file_get_contents(filename,&contents,NULL,NULL);
	    gtk_text_buffer_set_text(ibuffer,contents,-1);  
	}
	gtk_widget_destroy (dialog);	
}

void GUIurmulate(GtkWidget *widget, gpointer data) {
	//Save text editor content to temp file !
    char *contents;
    GtkTextIter start, end;
    gtk_text_buffer_get_start_iter(ibuffer,&start);
    gtk_text_buffer_get_end_iter(ibuffer,&end);
    contents = gtk_text_buffer_get_text(ibuffer,&start,&end,TRUE);

    g_file_set_contents(".temp",contents,-1,NULL);

	struct arguments arguments;
	arguments.verbose = gtk_toggle_button_get_active(checkbox_verbose);
	arguments.graph = 1;
	arguments.gui = 0;
    arguments.argc=1;
    arguments.args = malloc(sizeof(char*));
 	arguments.args[0] = malloc(sizeof(char)*strlen(".temp")+sizeof(char));  
 	strcpy(arguments.args[0],".temp");

    char *reg_str=gtk_entry_get_text(input_reg);
    char *temp_str=malloc(strlen(reg_str)*sizeof(char)+sizeof(char));

    strcpy(temp_str,reg_str);

    char *pch = strtok(temp_str," ");
    while(pch!=NULL){
    	arguments.argc++;
    	arguments.args = realloc(arguments.args,arguments.argc*sizeof(char*));
    	arguments.args[arguments.argc-1]=malloc((strlen(pch)+1)*sizeof(char));
    	strcpy(arguments.args[arguments.argc-1],pch);
    	pch=strtok(NULL," ");
    }
    free(temp_str);

    arguments.ip_lim = atoi(gtk_entry_get_text(input_inst));

	Urmulate(arguments);

	for(int i=0; i<arguments.argc; i++)
		free(arguments.args[i]);
	free(arguments.args);

	char *gvc_o_fname = malloc(strlen(".temp")*sizeof(char)+7*sizeof(char));
	strcpy(gvc_o_fname,".temp");
	strcat(gvc_o_fname,".graph");	

	GdkPixbuf *pixbuf;
	pixbuf = gdk_pixbuf_new_from_file(gvc_o_fname,NULL);

	canv_image=goo_canvas_image_new(canv_root,pixbuf,0.0,0.0,NULL);

	goo_canvas_set_bounds(GOO_CANVAS(canvas),0.0,0.0,
						  gdk_pixbuf_get_width(pixbuf),
						  gdk_pixbuf_get_height(pixbuf));

	free(gvc_o_fname);

	char *o_fname = malloc(strlen(".temp")*sizeof(char)+6*sizeof(char));
	strcpy(o_fname,".temp");
	strcat(o_fname,".out");

	char *contents2;
	g_file_get_contents(o_fname,&contents2,NULL,NULL);
	gtk_text_buffer_set_text(obuffer,contents2,-1);

	free(o_fname);
}

void GUImain() {
	/*
	**	GTK initialization
	*/
	gtk_init(NULL,NULL);

	/*
	**	window
	*/
	window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title (GTK_WINDOW(window),"URMulator");
    gtk_container_set_border_width (GTK_CONTAINER (window), 10);

	g_signal_connect(window,"delete-event",G_CALLBACK(delete_event),NULL);
	g_signal_connect(window,"destroy",G_CALLBACK(destroy),NULL);

	/*
	**	vbox
	*/
	vbox=gtk_vbox_new(FALSE,0);
	gtk_container_add(GTK_CONTAINER(window),vbox);	

	/*
	**	menu_bar
	*/
    menu_bar=gtk_menu_bar_new();

    /*
    **	file_menu
    */
    file_menu = gtk_menu_new();
    new_item=gtk_menu_item_new_with_label("New");
    open_item=gtk_menu_item_new_with_label("Open");
    save_item=gtk_menu_item_new_with_label("Save");
    quit_item=gtk_menu_item_new_with_label("Quit");

    gtk_menu_shell_append(GTK_MENU_SHELL(file_menu),new_item);
    gtk_menu_shell_append(GTK_MENU_SHELL(file_menu),open_item);
    gtk_menu_shell_append(GTK_MENU_SHELL(file_menu),save_item);
    gtk_menu_shell_append(GTK_MENU_SHELL(file_menu),quit_item);

    g_signal_connect(new_item, "activate",G_CALLBACK(FileNew),NULL);
    g_signal_connect(open_item,"activate",G_CALLBACK(FileOpen),NULL);
    g_signal_connect(save_item,"activate",G_CALLBACK(FileSave),NULL);
    g_signal_connect(quit_item,"activate",G_CALLBACK(delete_event),NULL);

    gtk_widget_show(new_item);
    gtk_widget_show(open_item);
    gtk_widget_show(save_item);
    gtk_widget_show(quit_item);

    file_item=gtk_menu_item_new_with_label("File");
    gtk_widget_show(file_item);
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(file_item),file_menu);
    
    gtk_menu_bar_append(GTK_MENU_BAR(menu_bar),file_item);

    gtk_box_pack_start(GTK_BOX(vbox),menu_bar,FALSE,TRUE,0);
	gtk_widget_show(menu_bar);

	/*
	**	hpaned
	*/
	hpaned=gtk_hpaned_new();
    gtk_box_pack_start(GTK_BOX(vbox),hpaned,TRUE,TRUE,0);

    /*
    **	scrwin1
	*/
	scrwin1=gtk_scrolled_window_new(NULL,NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrwin1),
								   GTK_POLICY_AUTOMATIC,
								   GTK_POLICY_AUTOMATIC);

    /*	
    **	iview
    */
    iview=gtk_source_view_new();

	gtk_widget_set_size_request(iview,400,300);
	gtk_source_view_set_highlight_current_line(GTK_SOURCE_VIEW(iview),TRUE);

    PangoFontDescription *font_desc;
    font_desc=pango_font_description_from_string("mono 10");
	gtk_widget_modify_font(iview,font_desc);
	pango_font_description_free(font_desc);

	ibuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(iview));

    gtk_container_add(GTK_CONTAINER(scrwin1),iview);
	gtk_paned_add1(GTK_PANED(hpaned),scrwin1);
	gtk_widget_show(iview);
	gtk_widget_show(scrwin1);

	/*
	**	vpaned
	*/
    vpaned=gtk_vpaned_new();

    /*
    **	scrwin2
	*/
	scrwin2=gtk_scrolled_window_new(NULL,NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrwin2),
								   GTK_POLICY_AUTOMATIC,
								   GTK_POLICY_AUTOMATIC);

	/*
	**	canvas
	*/
	canvas=goo_canvas_new();
	gtk_widget_set_size_request(canvas,400,150);

	canv_root=goo_canvas_get_root_item (GOO_CANVAS (canvas));
	canv_image=NULL;

	gtk_container_add(GTK_CONTAINER(scrwin2),canvas);
	gtk_paned_add1(GTK_PANED(vpaned),scrwin2);
	gtk_widget_show(canvas);
	gtk_widget_show(scrwin2);

    /*
    **	scrwin3
	*/
	scrwin3=gtk_scrolled_window_new(NULL,NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrwin3),
								   GTK_POLICY_AUTOMATIC,
								   GTK_POLICY_AUTOMATIC);

    /*	
    **	oview
    */
    oview=gtk_text_view_new();
    gtk_container_add(GTK_CONTAINER(scrwin3),oview);

	gtk_widget_set_size_request(oview,400,150);
	gtk_text_view_set_editable(GTK_TEXT_VIEW(oview),FALSE);
	gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(oview),FALSE);

    font_desc=pango_font_description_from_string("mono 10");
	gtk_widget_modify_font(oview,font_desc);
	pango_font_description_free(font_desc);

	obuffer=gtk_text_view_get_buffer(GTK_TEXT_VIEW(oview));

	gtk_container_add(GTK_CONTAINER(scrwin3),oview);
	gtk_paned_add2(GTK_PANED(vpaned),scrwin3);
	gtk_widget_show(oview);
	gtk_widget_show(scrwin3);

	gtk_paned_add2(GTK_PANED(hpaned),vpaned);
	gtk_widget_show(vpaned);
	gtk_widget_show(hpaned);

	/*
	**	hbox1
	*/
	hbox1=gtk_hbox_new(FALSE,0);
    gtk_box_pack_start(GTK_BOX(vbox),hbox1,FALSE,TRUE,0);
 
    label_inst1=gtk_label_new("Stop after ");   

    input_inst=gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(input_inst),"10000");

    label_inst2=gtk_label_new(" instructions (inf.loop safeguard)");

    gtk_box_pack_start(GTK_BOX(hbox1),label_inst1,FALSE,TRUE,0);
    gtk_box_pack_start(GTK_BOX(hbox1),input_inst,FALSE,TRUE,0);
    gtk_box_pack_start(GTK_BOX(hbox1),label_inst2,FALSE,TRUE,0);

    gtk_widget_show(label_inst1);
    gtk_widget_show(input_inst);
    gtk_widget_show(label_inst2);
    gtk_widget_show(hbox1);

    /*
    **	hbox2
    */
    hbox2=gtk_hbox_new(FALSE,0);
    gtk_box_pack_start(GTK_BOX(vbox),hbox2,FALSE,TRUE,0);

    label_reg = gtk_label_new("Register values: ");
    input_reg = gtk_entry_new();

    gtk_box_pack_start(GTK_BOX(hbox2),label_reg,FALSE,TRUE,0);
    gtk_box_pack_start(GTK_BOX(hbox2),input_reg,FALSE,TRUE,0);

    gtk_widget_show(label_reg);
    gtk_widget_show(input_reg);
    gtk_widget_show(hbox2);

    /*
    **	hbox3
    */
    hbox3=gtk_hbox_new(FALSE,0);
    gtk_box_pack_start(GTK_BOX(vbox),hbox3,FALSE,TRUE,0);
    
   	checkbox_verbose=gtk_check_button_new_with_label(
   		"Show register values after each instruction");

    button_urmulate=gtk_button_new_with_label("Execute URM instructions");
    g_signal_connect(button_urmulate,"clicked",G_CALLBACK(GUIurmulate),NULL);

    gtk_box_pack_start(GTK_BOX(hbox3),checkbox_verbose,FALSE,TRUE,0);
    gtk_box_pack_end(GTK_BOX(hbox3),button_urmulate,FALSE,TRUE,0);

    gtk_widget_show(checkbox_verbose);
    gtk_widget_show(button_urmulate);
    gtk_widget_show(hbox3);

    gtk_widget_show(vbox);
	gtk_widget_show(window);

    /*
    **
    */
	FileNew(NULL,NULL);
	gtk_main();
}
