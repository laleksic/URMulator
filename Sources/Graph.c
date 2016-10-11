/*	
**	URMulator v20161011
**	Copyright © 2016 Luka Aleksić
**	All rights reserved
**	
**	Web: <github.com/laleksic/URMulator>
**	E-mail: <laleksic@mail.ru>
*/

#include "Main.h"

void GraphGenerate(GList *inst_list, char *i_fname) {
	Agraph_t *G;
	GVC_t *gvc;

	FILE *input, *output;

	char *gvc_i_fname, *gvc_o_fname;
	gvc_i_fname = malloc(strlen(i_fname)*sizeof(char)+6*sizeof(char));
	strcpy(gvc_i_fname,i_fname);
	strcat(gvc_i_fname,".dot");

	/*
	**	Here we begin writing a DOT file for Graphviz to use to generate
	**	a graph image. Default graph settings are hardcoded though some
	**	will be made configurable by the user in a later version.
	*/
	input = fopen(gvc_i_fname,"w");

	fprintf(input,"strict digraph\n\
				   {\n\
				   \
				   node [\
				   height=0.25, \
				   width=0.25, \
				   fontsize=10, \
				   fontname=\"Monospace\"\
				   ];\n\
				   \
				   graph [\
				   newrank=true,\
				   fontname=\"Monospace\",\
				   fontsize=12,\
				   remincross=true,\
				   clusterrank=local, \
				   ranksep=0.2,\
				   nodesep=.2\
				   ];\n\
				   \
				   edge [\
				   style=tapered,\
				   color=slateblue1,\
				   penwidth=3,\
				   arrowsize=.5, \
				   arrowhead=dot, \
				   arrowtail=open\
				   ];\n");

	/*
	**	Nodes from the same branch are grouped to provide a more neat
	**	and straight graph.
	*/
	gint group;
	group=0;

	/*
	**	In my URM syntax instructions can be grouped into blocks ('{' & '}')
	**	for aesthetic purposes. Each block translates into a Graphviz
	**	subgraph. The "root" block is a subgraph labeled "main".
	*/
	fprintf(input,"subgraph cluster0\n\
				   {\n\
				   style=\"filled,rounded\"; \
				   fillcolor=lightgray; \
				   color=lightgray; \
				   label=\"main\";\n");

	for(int i=0;i<=g_list_length(inst_list)-1;i++)
	{
		/*
		**	For every iteration we have pointers used to insure proper linking
		**	of nodes.
		*/
		Instruction *curr_inst, *curr_connector_inst,
					*next_inst, *next_connector_inst,
					*prev_inst,
					*target_inst, *target_connector_inst;
		curr_inst=curr_connector_inst=
		next_inst=next_connector_inst=
		prev_inst=
		target_inst=target_connector_inst=
		NULL;

		curr_inst = g_list_nth_data(inst_list,i);
		curr_connector_inst =
		g_list_nth_data(inst_list,curr_inst->connector_id);

		if (i!=g_list_length(inst_list)-1)
			next_inst = g_list_nth_data(inst_list,i+1);
		if (next_inst!=NULL)
			next_connector_inst = 
			g_list_nth_data(inst_list,next_inst->connector_id);	

		if (i!=0)
			prev_inst = g_list_nth_data(inst_list,i-1);

		if (curr_inst->type==URM_INST_TYPE_J &&
			curr_inst->args[2]>0 &&
			curr_inst->args[2]<=g_list_length(inst_list)-1)
			target_inst = g_list_nth_data(inst_list,curr_inst->args[2]);

		if (target_inst!=NULL)
			target_connector_inst = 
			g_list_nth_data(inst_list,target_inst->connector_id);

		/*
		**	Now we begin writing per node settings to the Graphviz input file
		**
		**	If the current instruction is preceded by a block opener '{', open
		**	a new graphviz cluster
		*/
		if(curr_inst->connector==TRUE && i!=g_list_length(inst_list)-1)
		{
			fprintf(input,"}\nsubgraph cluster%d\n{\n",
						  curr_inst->connector_id);
			fprintf(input,"fontname=\"Monospace\"; \
						   fontsize=12; \
						   style=\"filled,rounded\"; \
						   fillcolor=lightgray; \
						   color=lightgray; ");
			if(curr_inst->connector_id==0) fprintf(input,"label=\"main\";\n");
			else fprintf(input,"label=\"%s\";\n",curr_inst->connector_name);
		}

		/*
		**	Start node
		*/
		if (i==0)
			fprintf(input,"0 [\
						   group=g0,\
						   style=filled,\
						   color=limegreen,\
						   fillcolor=limegreen,\
						   shape=oval, \
						   label=\"START\"\
						   ];\n"	);

		/*
		**	Main end node
		*/
		else if (i==g_list_length(inst_list)-1 &&
				(prev_inst->type!=URM_INST_TYPE_J ||
				prev_inst->args[0]!=prev_inst->args[1]))
			fprintf(input,"%d [\
						   group=g0,\
						   style=filled,\
						   color=limegreen,\
						   fillcolor=limegreen, \
						   shape=oval, \
						   label=\"END\"\
						   ];\n",g_list_length(inst_list)-1	);

		/*
		**	Instruction nodes
		*/
		else if (curr_inst->type==URM_INST_TYPE_Z)
			fprintf(input,"%d [\
						   group=g%d,\
						   style=filled,\
						   color=slateblue1,\
						   fillcolor=slateblue1,\
						   shape=box,\
						   label=\"%d: r%d<--0\"\
						   ];\n",i,group,i,curr_inst->args[0]);

		else if (curr_inst->type==URM_INST_TYPE_S)
			fprintf(input,"%d [\
						   group=g%d,\
						   style=filled,\
						   color=slateblue1,\
						   fillcolor=slateblue1,\
						   shape=box,\
						   label=\"%d: r%d<--r%d+1\"\
						   ];\n",
						   i,group,i,curr_inst->args[0],curr_inst->args[0]);

		else if (curr_inst->type==URM_INST_TYPE_T)
			fprintf(input,"%d [\
						   group=g%d,\
						   style=filled,\
						   color=slateblue1,\
						   fillcolor=slateblue1,\
						   shape=box,\
						   label=\"%d: r%d<--r%d\"\
						   ];\n",
						   i,group,i,curr_inst->args[1],curr_inst->args[0]);

		/*
		**	Conditional jump
		*/
		else if (curr_inst->type==URM_INST_TYPE_J &&
				curr_inst->args[0]!=curr_inst->args[1])
			fprintf(input,"%d [\
						   group=g%d,\
						   style=filled,\
						   color=indianred1,\
						   fillcolor=indianred1,\
						   shape=diamond,\
						   label=\"%d: r%d=r%d?\"\
						   ];\n",
						   i,group,i,curr_inst->args[0],curr_inst->args[1]);

		/*
		**	Unconditional jump (jump where condition always = true)
		*/
		else if (curr_inst->type==URM_INST_TYPE_J &&
				curr_inst->args[0]==curr_inst->args[1])
			fprintf(input,"%d [\
						   group=g%d,\
						   style=filled,\
						   color=lightsalmon,\
						   fillcolor=lightsalmon,\
						   shape=box,\
						   label=\"%d: goto %d\"\
						   ];\n",i,group,i,curr_inst->args[2]);

		/*
		**	Alternate ending points for algorithm
		*/
		if(curr_inst->type==URM_INST_TYPE_J &&
		  (curr_inst->args[2]==0 ||
		  curr_inst->args[2]>g_list_length(inst_list)-1))
			fprintf(input,"END%d [\
						   group=g0,\
						   style=filled,\
						   color=limegreen,\
						   fillcolor=limegreen, \
						   shape=oval, \
						   label=\"END\"\
						   ];\n",i);

		/*
		**	Connector nodes which connect instructions from seperate blocks
		*/
		else if(curr_inst->type==URM_INST_TYPE_J &&
				target_inst->connector_id!=curr_inst->connector_id )
			fprintf(input,"c%dIN%dYES [\
						   group=g%d,\
						   label=\"goto %d [%s]\", \
						   style=filled,\
						   color=lightsalmon,\
						   fillcolor=lightsalmon,\
						   shape=box\
						   ];\n",
						   curr_inst->args[2],i,group,curr_inst->args[2],
						   target_connector_inst->connector_name);

		if(curr_inst->type==URM_INST_TYPE_J &&
		   curr_inst->args[0]!=curr_inst->args[1] &&
		   next_inst->connector_id!=curr_inst->connector_id)
			fprintf(input,"c%dIN%dNO [\
						   group=g%d,\
						   label=\"goto %d [%s]\", \
						   style=filled,\
						   color=lightsalmon,\
						   fillcolor=lightsalmon,\
						   shape=box\
						   ];\n",
						   i+1,i,group,i+1,next_connector_inst->connector_name);

		if(next_inst!=NULL &&
		   curr_inst->type!=URM_INST_TYPE_J && 
		   next_inst->connector_id!=curr_inst->connector_id && 
		   i!=g_list_length(inst_list)-1)
			fprintf(input,"c%dIN%d [\
						   group=g%d,\
						   label=\"goto %d [%s]\", \
						   style=filled,\
						   color=lightsalmon,\
						   fillcolor=lightsalmon,\
						   shape=box\
						   ];\n",
						   i+1,i,group,i+1,next_connector_inst->connector_name);

		if(curr_inst->type==URM_INST_TYPE_J)
			group++;
		
		/*
		**	Node links
		*/
		if(i!=g_list_length(inst_list)-1)
		{
			if(curr_inst->type!=URM_INST_TYPE_J)
			{
				if(curr_inst->connector_id==next_inst->connector_id ||
				   i==g_list_length(inst_list)-1)
					fprintf(input,"%d -> %d;\n",i,i+1);
				else if(curr_inst->connector_id!=next_inst->connector_id)
					fprintf(input,"%d -> c%dIN%d;\n",i,i+1,i);					
			}
			if(curr_inst->type==URM_INST_TYPE_J && 
			   curr_inst->args[0]==curr_inst->args[1])
			{
				if(curr_inst->args[2]==0 ||
				   curr_inst->args[2] > g_list_length(inst_list)-1)
					fprintf(input,"%d -> END%d;\n",i,i);
				else
				{
					if(curr_inst->connector_id==target_inst->connector_id)
						fprintf(input,"%d -> %d;\n",i,curr_inst->args[2]);
					if(curr_inst->connector_id!=target_inst->connector_id)
						fprintf(input,"%d -> c%dIN%dYES;\n",
									  i,curr_inst->args[2],i);	
				}
			}	
			if(curr_inst->type==URM_INST_TYPE_J &&
			   curr_inst->args[0]!=curr_inst->args[1])
			{
				if(curr_inst->connector_id==next_inst->connector_id)
					fprintf(input,"%d -> %d [color=indianred1];\n",i,i+1);
				if(curr_inst->connector_id!=next_inst->connector_id)
					fprintf(input,"%d -> c%dIN%dNO [color=indianred1];\n",
								  i,i+1,i);				
				
				if(curr_inst->args[2]==0 ||
				   curr_inst->args[2] > g_list_length(inst_list)-1)
					fprintf(input,"%d -> END%d [color=limegreen];\n",i,i);
				else
				{
					if(curr_inst->connector_id==target_inst->connector_id)
						fprintf(input,"%d -> %d [color=limegreen];\n",
									  i,curr_inst->args[2]);
					if(curr_inst->connector_id!=target_inst->connector_id)
						fprintf(input,"%d -> c%dIN%dYES [color=limegreen];\n",
									  i,curr_inst->args[2],i);	
				}					
			}
		}

		/*
		**	Comment nodes and links to their respective node
		*/
		if (curr_inst->comment==TRUE)
		{
			fprintf(input,"}\n");
			fprintf(input,"c%d -> %d [style=dotted,dir=none];\n",i,i);
			fprintf(input,"c%d [\
						   shape=note,\
						   color=khaki4,\
						   style=filled,\
						   fillcolor=khaki1,\
						   label=\"%s\"\
						   ];\n",i,curr_inst->comment_text);

			fprintf(input,"{ rank=same; %d; c%d; }\n",i,i);
			fprintf(input,"subgraph cluster%d\n{\n",curr_inst->connector_id);
			fprintf(input,"fontname=\"Monospace\"; \
						   fontsize=12; \
						   style=\"filled,rounded\"; \
						   fillcolor=lightgray; \
						   color=lightgray; ");
			if(curr_inst->connector_id==0) fprintf(input,"label=\"main\";\n");
			else fprintf(input,"label=\"%s\";\n",
							   curr_connector_inst->connector_name);			
		}					
		
	}
	fprintf(input,"}\n");	
	fprintf(input,"}\n");
		
	fclose(input);

	input = fopen(gvc_i_fname,"r");

	gvc_o_fname = malloc(strlen(i_fname)*sizeof(char)+8*sizeof(char));
	strcpy(gvc_o_fname,i_fname);
	strcat(gvc_o_fname,".graph");	
	output = fopen(gvc_o_fname,"w");

	/*
	**	Graphviz does its magic here
	*/
	gvc = gvContext();
	G = agread(input,0);

	gvLayout(gvc,G,"dot");
	gvRender(gvc,G,"png",output);
	gvFreeLayout(gvc,G);
	agclose(G);
	gvFreeContext(gvc);

	fclose(input);
	fclose(output);
	free(gvc_i_fname);
	free(gvc_o_fname);
}