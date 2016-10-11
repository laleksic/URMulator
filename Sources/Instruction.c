/*	
**	URMulator v20161011
**	Copyright © 2016 Luka Aleksić
**	All rights reserved
**	
**	Web: <github.com/laleksic/URMulator>
**	E-mail: <laleksic@mail.ru>
*/

#include "Main.h"

GList* InstructionListCreate() {
	GList *inst_list=NULL;
	char* connector_name = calloc(256,sizeof(char));
	char* comment_text = calloc(256,sizeof(char));
	int* args = calloc(3,sizeof(int));
	Instruction* dummy_inst = InstructionCreate(0,
												args,
												FALSE,
												0,
												connector_name,
												FALSE,
												comment_text);
	inst_list=g_list_append(inst_list,dummy_inst);
	free(connector_name);
	free(comment_text);
	free(args);	
	return inst_list;
}

void InstructionListPopulate(GList *inst_list,
							 FILE* i_file) {
	gboolean connector = FALSE;
	gint connector_current = 0;
	char *connector_name = calloc(256,sizeof(char));

	gboolean comment = FALSE;
	char *comment_text = calloc(256,sizeof(char));

	while (!feof(i_file)) {
		char tempc;
		int *args = calloc(3,sizeof(int));
		if (fscanf(i_file," %c ",&tempc)) {
			if (tempc=='{') {
				connector=TRUE;
				connector_current=g_list_length(inst_list);
				fscanf(i_file," %256[?/!£$%&*()-_=+~@';,.¬`0-9a-zA-Z] ",
					   connector_name);
			}
			if (tempc=='}') {
				connector=TRUE;
				connector_current=0;
			}
			if (tempc=='#') {
				comment=TRUE;
				fscanf(i_file," %256[?/!£$%&*()-_=+~@';,.¬`0-9a-zA-Z \n\t] # ",
					   comment_text);
			}
			if (tempc==':') {
				fscanf(i_file," %c ",&tempc);
				tempc=tolower(tempc);
				switch(tempc) {
					case 'z':
						if (fscanf(i_file," ( %d ) ",&args[0])) {
							InstructionCreateAndAppend(inst_list,
											  		   URM_INST_TYPE_Z,
											  		   args,
											  		   connector,
											  		   connector_current,
											  		   connector_name,
											  		   comment,
											  		   comment_text);
						}
						break;
					case 's':
						if (fscanf(i_file," ( %d ) ",&args[0])) {
							InstructionCreateAndAppend(inst_list,
											  		   URM_INST_TYPE_S,
											  		   args,
											  		   connector,
											  		   connector_current,
											  		   connector_name,
											  		   comment,
											  		   comment_text);
						}
						break;
					case 't':
						if (fscanf(i_file," ( %d , %d ) ",&args[0],&args[1])) {
							InstructionCreateAndAppend(inst_list,
											  		   URM_INST_TYPE_T,
											  		   args,
											  		   connector,
											  		   connector_current,
											  		   connector_name,
											  		   comment,
											  		   comment_text);
						}
						break;
					case 'j':
						if (fscanf(i_file," ( %d , %d , %d ) ",
							&args[0],&args[1],&args[2])) {
							InstructionCreateAndAppend(inst_list,
											  		   URM_INST_TYPE_J,
											  		   args,
											 		   connector,
											 		   connector_current,
											 		   connector_name,
											  		   comment,
											  		   comment_text);
						}
						break;
					default:
						break;
				}
				free(connector_name);
				free(comment_text);
				connector_name = calloc(256,sizeof(char));
				comment_text = calloc(256,sizeof(char));
				connector = FALSE;
				comment = FALSE;
			}
		}
		else
			fseek(i_file,1,SEEK_CUR);
		free(args);
	}
}

void InstructionListExecute(GList *reg_list,
							GList *inst_list,
							char *i_fname,
							gboolean verbose,
							int ip_lim) {
	int inst_num = g_list_length(inst_list);
	int inst_iter = 1;

	char *o_fname = malloc(strlen(i_fname)*sizeof(char)+6*sizeof(char));
	strcpy(o_fname,i_fname);
	strcat(o_fname,".out");

	FILE* o_file = fopen(o_fname,"w");

	int ip = 0;
	//int ip_lim = 10000;

	if(verbose) fprintf(o_file,"Executing instructions:\n");
	while (1) {
		Instruction* curr_inst = g_list_nth_data(inst_list,inst_iter);
		int *args = curr_inst->args;
		switch(curr_inst->type) {
			case URM_INST_TYPE_Z:
				Z(reg_list,args,&inst_iter,o_file, verbose);
				break;
			case URM_INST_TYPE_S:
				S(reg_list,args,&inst_iter,o_file, verbose);
				break;
			case URM_INST_TYPE_T:
				T(reg_list,args,&inst_iter,o_file, verbose);
				break;
			case URM_INST_TYPE_J:
				J(reg_list,args,&inst_iter,o_file, verbose);
				break;
			default:
				break;
		}

		inst_iter++;
		if(inst_iter<=0 || inst_iter>inst_num-1) {
			fprintf(o_file,"Reached end of program.\n");
			break;
		}
		ip++;
		if (ip>ip_lim) {
			fprintf(o_file,"Did not reach end of program.\n");
			break;
		}		
	}
	fprintf(o_file,"\nPerformed %d instructions.\n",ip);
	fprintf(o_file,"\nOutput:\n");
	RegisterListPrint(reg_list,o_file);
	fclose(o_file);

	char *contents;
	g_file_get_contents(o_fname,&contents,NULL,NULL);
	printf("%s",contents);

	free(o_fname);
}

void InstructionListDestroy(GList* inst_list) {
	Instruction* curr_inst;
	GFOREACH(curr_inst,inst_list) {
		InstructionDestroy(curr_inst);
	}
	g_list_free(inst_list);	
}

Instruction* InstructionCreate(inst_type type,
	                   		   int *args,
	                   		   gboolean connector,
	                   		   gint connector_id,
	                   		   char *connector_name,
	                   		   gboolean comment,
	                   		   char *comment_text) {

	Instruction *new_inst = malloc(sizeof(Instruction));

	new_inst->type = type;

	new_inst->args = calloc(3,sizeof(int));
	new_inst->args[0]=args[0];
	new_inst->args[1]=args[1];
	new_inst->args[2]=args[2];	

	new_inst->connector = connector;
	new_inst->connector_id = connector_id;
	new_inst->connector_name = calloc(256,sizeof(char));
	strcpy(new_inst->connector_name,connector_name);

	new_inst->comment = comment;
	new_inst->comment_text = calloc(256,sizeof(char));
	strcpy(new_inst->comment_text,comment_text);

	return new_inst;
}

void InstructionCreateAndAppend(GList *inst_list,
					   			inst_type type,
	                   			int *args,
	                   			gboolean connector,
	                   			gint connector_id,
	                   			char *connector_name,
	                   			gboolean comment,
	                   			char *comment_text) {

	Instruction *new_inst = InstructionCreate(type,
											  args,
											  connector,
											  connector_id,
											  connector_name,
											  comment,
											  comment_text);
	
	g_list_append(inst_list,new_inst);
}

void InstructionDestroy(Instruction *inst) {
	free(inst->args);
	free(inst->connector_name);
	free(inst->comment_text);
	free(inst);
}