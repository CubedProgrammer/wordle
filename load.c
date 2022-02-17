#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#ifdef _WIN32
#include<windows.h>
#include<shlwapi.h>
#else
#include<unistd.h>
#endif
#include"load.h"
char **gh_cp_wordle____ls;
size_t gh_cp_wordle____cnt;
void loadfile(const char *fname)
{
    gh_cp_wordle____ls = loadfile_actual(&gh_cp_wordle____cnt, fname);
}
void find_and_load(void)
{
    const char *choice = "fivedict.txt";
#ifdef _WIN32
    if(PathFileExistsA(choice))
#else
    if(access(choice, F_OK) == 0)
#endif
        loadfile(choice);
    else
#ifdef _WIN32
        fputs("File fivedict.txt not found.\n", stderr);
#else
    {
        char buf[2601];
        strcpy(buf, getenv("HOME"));
        int blen = strlen(buf);
        strcpy(buf + blen, "/.fivedict.txt");
        choice = buf;
        if(access(choice, F_OK) == 0)
            loadfile(choice);
        else
            fputs("File fivedict.txt not found.\n", stderr);
    }
#endif
}
char **loadfile_actual(size_t *lenp, const char *fname)
{
    FILE *fh = fopen(fname, "r");
    char **vec = malloc(5 * sizeof(const char*));
    size_t len = 0;
    size_t capa = 5, ocapa = 3;
    char arr[6];
    while(!feof(fh))
    {
        fscanf(fh, "%s", arr);
        if(len == capa)
        {
            capa += ocapa;
            ocapa = len;
            vec = realloc(vec, capa * sizeof(const char*));
        }
        vec[len] = malloc(6 * sizeof(char));
        strcpy(vec[len], arr);
        ++len;
    }
    *lenp = len;
    char *tmp;
    int cmp;
    size_t ind;
    for(size_t i = 1; i < len; i++)
    {
        ind = i;
        cmp = strcmp(vec[ind-1], vec[ind]);
        while(cmp > 0)
        {
            tmp = vec[ind-1];
            vec[ind-1] = vec[ind];
            vec[ind] = tmp;
            --ind;
            cmp = ind == 0 ? 0 : strcmp(vec[ind-1], vec[ind]);
        }
    }
    fclose(fh);
    return vec;
}
