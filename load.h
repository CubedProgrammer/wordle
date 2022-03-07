#ifndef load_h_included
#define load_h_included
#include<stddef.h>
extern char **gh_cp_wordle____ls, **gh_cp_wordle____a;
extern size_t gh_cp_wordle____cnt, gh_cp_wordle____ac;
char **loadfile_actual(size_t *lenp, const char *fname);
void loadfile(const char *fname);
void loadans(const char *fname);
void find_and_load(void);
#endif
