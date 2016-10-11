/*    
**    URMulator v20161011
**    Copyright © 2016 Luka Aleksić
**    All rights reserved
**    
**    Web: <github.com/laleksic/URMulator>
**    E-mail: <laleksic@mail.ru>
*/

#ifndef INSTRUCTION_H
    #define INSTRUCTION_H

    #include <glib.h>

    GList* InstructionListCreate();

    void InstructionListPopulate(GList *inst_list,
                                 FILE* i_file);

    void InstructionListExecute(GList *reg_list,
                                GList *inst_list,
                                char *i_fname,
                                gboolean verbose,
                                int ip_lim);

    void InstructionListDestroy(GList*);

    typedef enum {
        URM_INST_TYPE_DUMMY,
        URM_INST_TYPE_Z,
        URM_INST_TYPE_S,
        URM_INST_TYPE_T,
        URM_INST_TYPE_J
    } inst_type;

    typedef struct {
        inst_type type;
        int *args;

        gboolean connector;
        gint connector_id;
        char *connector_name;

        gboolean comment;
        char *comment_text;
    } Instruction;

    Instruction* InstructionCreate(inst_type type,
                                   int *args,
                                   gboolean connector,
                                   gint connector_id,
                                   char *connector_name,
                                   gboolean comment,
                                   char *comment_text);

    void InstructionCreateAndAppend(GList *inst_list,
                                    inst_type type,
                                    int *args,
                                    gboolean connector,
                                    gint connector_id,
                                    char *connector_name,
                                    gboolean comment,
                                    char *comment_text);

    void InstructionDestroy(Instruction *inst);
#endif
