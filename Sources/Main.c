/*	
**	URMulator v20161011
**	Copyright © 2016 Luka Aleksić
**	All rights reserved
**	
**	Web: <github.com/laleksic/URMulator>
**	E-mail: <laleksic@mail.ru>
*/

#include "Main.h"

/*
**	ARGP setup
*/
const char *argp_program_version = "URMulator v20161011";
const char *argp_program_bug_address = "<laleksic@mail.ru>";

static struct argp_option options[] = {
	{"verbose",'v',0,0,"Print register information after each instruction"},
	{"graph",'g',0,0,"Generate Graphviz graph"},
	{"gui",'u',0,0,"GTK2 powered graphic interface"},
	{"limit",'l',"X",0,"Stop after X instructions (inf.loop safeguard)"},
	{0}
};

static error_t parse_opt (int key, char *arg, struct argp_state *state) {
	struct arguments *arguments = state->input;
	switch (key) {
		case 'v':
			arguments->verbose=TRUE;
			break;
		case 'g':
			arguments->graph=TRUE;
			break;
		case 'u':
			arguments->gui=TRUE;
			break;
		case 'l':
			arguments->ip_lim=atoi(arg);
		case ARGP_KEY_ARG:
			if(state->arg_num==0) {
				arguments->args=calloc(1,sizeof(char*));
				arguments->args[state->arg_num]=arg;
				arguments->argc=1;
			}
			else {
				realloc(arguments->args,(state->arg_num+1)*sizeof(char*));
				arguments->args[state->arg_num]=arg;
				arguments->argc+=1;
			}
			break;
		case ARGP_KEY_END:
			if (state->arg_num<1&&!arguments->gui)
				argp_usage(state);
			break;
		default:
			return ARGP_ERR_UNKNOWN;
	}
	return 0;
}

static char args_doc[] = "INPUT REG1 REG2 REG3 ...";

static char doc[] =
"urm -- An Unlimited Register Machine (URM) simulator with a graphical\
interface and graph generation.";

static struct argp argp = {options, parse_opt, args_doc, doc};

/*
**	Main
*/
int main (int argc,char **argv) {
	//argp argument default values
	struct arguments arguments;
	arguments.verbose = 0;
	arguments.graph = 0;
	arguments.gui = 0;
	arguments.ip_lim = 10000;

	argp_parse (&argp, argc, argv, 0, 0, &arguments);

	if(arguments.gui) {
		//GTK loop, see GTK.c
		GUImain();
		return 1;
	}

	Urmulate(arguments);
	return 1;
}

void Urmulate(struct arguments arguments) {
	char *i_fname = arguments.args[0];

	GList* reg_list = RegisterListCreate();
	GList* inst_list = InstructionListCreate();
	
	FILE* i_file;
	i_file=fopen(i_fname,"r");

	RegisterListPopulateFromArgs(reg_list,arguments.args,arguments.argc);
	//RegisterListPopulateFromFile(reg_list, i_file);

	rewind(i_file);

	InstructionListPopulate(inst_list, i_file);
	
	fclose(i_file);

	//Append another dummy instruction to end of instruction list
	//(this one makes graph generation work)
	char* connector_name = calloc(256,sizeof(char));
	char* comment_text = calloc(256,sizeof(char));
	int* args = calloc(3,sizeof(int));
	InstructionCreateAndAppend(inst_list,
					  		   0,
					  		   args,
					  		   FALSE,
					  		   0,
					  		   connector_name,
					  		   FALSE,
					  		   comment_text);
	free(connector_name);
	free(comment_text);
	free(args);

	InstructionListExecute(reg_list,
						   inst_list,
						   i_fname,
						   arguments.verbose,
						   arguments.ip_lim);

	if(arguments.graph) GraphGenerate(inst_list, i_fname);

	RegisterListDestroy(reg_list);
	InstructionListDestroy(inst_list);
}