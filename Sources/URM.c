/*	
**	URMulator v20161011
**	Copyright © 2016 Luka Aleksić
**	All rights reserved
**	
**	Web: <github.com/laleksic/URMulator>
**	E-mail: <laleksic@mail.ru>
*/

#include "Main.h"

void Z(GList *reg_list,
	   int *args,
	   int *inst_iter,
	   FILE *o_file,
	   gboolean verbose) {
	if(verbose) fprintf(o_file,"%d:R%d=0\n",*inst_iter,args[0]);

	Register *curr_reg;
	GFOREACH(curr_reg,reg_list) {
		if(curr_reg->reg_num == args[0]) {
			curr_reg->reg_val = 0;
			return;
		}
	}
	RegisterCreateAndAppend(reg_list, args[0], 0);
}

void S(GList *reg_list,
	   int *args,
	   int *inst_iter,
	   FILE *o_file,
	   gboolean verbose) {
	Register *curr_reg;
	GFOREACH(curr_reg,reg_list) {
		if(curr_reg->reg_num == args[0]) {
			curr_reg->reg_val++;
			if(verbose) fprintf(o_file,"%d:R%d=%d\n",*inst_iter,args[0],curr_reg->reg_val);
			return;
		}
	}
	RegisterCreateAndAppend(reg_list, args[0], 1);
	if(verbose) fprintf(o_file,"%d:R%d=1\n",*inst_iter,args[0]);
}

void T(GList *reg_list,
	   int *args,
	   int *inst_iter,
	   FILE *o_file,
	   gboolean verbose) {
	Register *curr_reg;
	gboolean found=FALSE;
	int tempval = 0;

	GFOREACH(curr_reg,reg_list) {
		if(curr_reg->reg_num == args[0]) {
			tempval = curr_reg->reg_val;
			found=TRUE;
		}
	}
	if (found==FALSE)
		RegisterCreateAndAppend(reg_list, args[0], tempval);

	GFOREACH(curr_reg,reg_list) {
		if(curr_reg->reg_num == args[1]) {
			curr_reg->reg_val = tempval;
			if(verbose) fprintf(o_file,"%d:R%d=%d\n",*inst_iter,args[1],tempval);
			return;
		}
	}
	RegisterCreateAndAppend(reg_list, args[1], tempval);
	if(verbose) fprintf(o_file,"%d:R%d=%d\n",*inst_iter,args[1],tempval);
}

void J(GList *reg_list,
	   int *args,
	   int *inst_iter,
	   FILE *o_file,
	   gboolean verbose) {
	Register *curr_reg;
	int tempval1=0;
	int tempval2=0;
	GFOREACH(curr_reg,reg_list) {
		if(curr_reg->reg_num == args[0])
			tempval1 = curr_reg->reg_val;
	}
	GFOREACH(curr_reg,reg_list) {
		if(curr_reg->reg_num == args[1])
			tempval2 = curr_reg->reg_val;
	}
	if (tempval1==tempval2) {
		if(verbose) fprintf(o_file,"%d:Jump to %d\n",*inst_iter,args[2]);
		*inst_iter=args[2]-1;
	}
	else {
		if(verbose) fprintf(o_file,"%d:No jump\n",*inst_iter);
	}
}