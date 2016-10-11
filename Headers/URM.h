/*    
**    URMulator v20161011
**    Copyright © 2016 Luka Aleksić
**    All rights reserved
**    
**    Web: <github.com/laleksic/URMulator>
**    E-mail: <laleksic@mail.ru>
*/

#ifndef URM_H
    #define URM_H

    #include <glib.h>
    
    void Z(GList *reg_list,
           int *args,
           int *inst_iter,
           FILE *o_file,
           gboolean verbose);

    void S(GList *reg_list,
           int *args,
           int *inst_iter,
           FILE *o_file,
           gboolean verbose);

    void T(GList *reg_list,
           int *args,
           int *inst_iter,
           FILE *o_file,
           gboolean verbose);

    void J(GList *reg_list,
           int *args,
           int *inst_iter,
           FILE *o_file,
           gboolean verbose);
#endif
