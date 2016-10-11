/*    
**    URMulator v20161011
**    Copyright © 2016 Luka Aleksić
**    All rights reserved
**    
**    Web: <github.com/laleksic/URMulator>
**    E-mail: <laleksic@mail.ru>
*/

#ifndef REGISTER_H
    #define REGISTER_H

    #include <glib.h>

    GList* RegisterListCreate();

    void RegisterListPopulateFromArgs(GList *reg_list,
                                      char **args,
                                      int argc);

    void RegisterListPrint(GList *reg_list,
                           FILE *o_file);
    
    void RegisterListDestroy(GList *reg_list);

    typedef struct {
        int reg_num;
        int reg_val;
    } Register;

    Register* RegisterCreate(int reg_num,
                             int reg_val);

    void RegisterCreateAndAppend(GList *reg_list,
                                 int reg_num,
                                 int reg_val);

    void RegisterDestroy(Register *reg);
#endif
