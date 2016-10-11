/*  
**  URMulator v20161011
**  Copyright © 2016 Luka Aleksić
**  All rights reserved
**  
**  Web: <github.com/laleksic/URMulator>
**  E-mail: <laleksic@mail.ru>
*/

#include "Main.h"

GList* RegisterListCreate() {
    GList *reg_list=NULL;
    Register *dummy_reg = RegisterCreate(0,0);
    reg_list=g_list_append(reg_list,dummy_reg);
    return reg_list;
}

void RegisterListPopulateFromArgs(GList* reg_list,
                                  char** args,
                                  int argc) {
    for(int i=1; i<argc; i++) {
        char* argv = args[i];
        RegisterCreateAndAppend(reg_list,i,atoi(argv));
    }
}

void RegisterListPopulateFromFile(GList *reg_list,
                                  FILE* i_file) {
    while (!feof(i_file)) {
        int reg_num,reg_val;
        if(fscanf(i_file," : r %u = %u ",&reg_num,&reg_val)) {
            //we iterate through all elements in the register list
            //if a register with same reg_num exists, we write value to it
            //and return true (finished with this register)
            Register* curr_reg;
            GFOREACH(curr_reg,reg_list) {   
                if(curr_reg->reg_num == reg_num) {
                    curr_reg->reg_val = reg_val;
                    continue;
                }
            }
            //otherwise, we append a new register with reg_num and reg_val
            RegisterCreateAndAppend(reg_list, reg_num, reg_val);
            continue;       
        }
        fseek(i_file,1,SEEK_CUR);
    }
}

void RegisterListPrint(GList *reg_list,
                       FILE *o_file) {
    Register* curr_reg;
    GFOREACH(curr_reg,reg_list) {       
        if(curr_reg->reg_num!=0)
            fprintf(o_file,"R%d=%d\n",curr_reg->reg_num, curr_reg->reg_val);
    }   
}

void RegisterListDestroy(GList *reg_list) {
    Register* curr_reg;
    GFOREACH(curr_reg,reg_list) {
        RegisterDestroy(curr_reg);
    }
    g_list_free(reg_list);  
}

Register* RegisterCreate(int reg_num,
                         int reg_val) {
    Register *new_reg = malloc(sizeof(Register));
    new_reg->reg_num = reg_num;
    new_reg->reg_val = reg_val;
    return new_reg;
}

void RegisterCreateAndAppend(GList *reg_list,
                             int reg_num,
                             int reg_val) {
    Register *new_reg = RegisterCreate(reg_num,reg_val);
    g_list_append(reg_list,new_reg);
}

void RegisterDestroy(Register* reg) {
    free(reg);
}




