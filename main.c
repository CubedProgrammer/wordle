#include<stdint.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#ifdef _WIN32
#include<conio.h>
#include<windows.h>
#define mssleep Sleep
#else
#include<string.h>
#include<termios.h>
#include<unistd.h>
#define mssleep(ms)usleep((ms)*1000)
#endif
#include"load.h"
#define FACTOR 25214903917
#define ring putchar('\a')
uint64_t seed;
int rdchr(void);
int isword(char **dic, size_t diclen, const char *search)
{
    int found = 0, cmp;
    size_t hi = diclen - 1, lo = 0;
    size_t mid;
    while(hi > lo)
    {
        mid = hi + lo;
        mid /= 2;
        cmp = strcmp(dic[mid], search);
        if(cmp < 0)
            lo = mid + 1;
        else if(cmp == 0)
        {
            found = 1;
            hi = 0;
        }
        else
            hi = mid;
    }
    if(!found)
        found = strcmp(dic[lo], search) == 0;
    return found;
}
void play(int attempts, char anim)
{
    puts("Take your guesses!");
    size_t diclen = gh_cp_wordle____cnt, anslen = gh_cp_wordle____ac;
    char **dic = gh_cp_wordle____ls, **ans = gh_cp_wordle____a;
    uint32_t ind = seed % anslen;
    char *target = ans[ind], *tmp;
    char cpy[6];
    char txt[6];
    char ch;
    int chcnt = 0, lns = 0;
    putchar('\n');
    uint64_t possible_letters = 0x5555555555555;
    int possi, pos;
    while(attempts)
    {
        printf("\033\133%iF", lns + 1);
        for(int i = 'a'; i <= 'z'; i++)
        {
            possi = possible_letters >> (i - 'a') * 2 & 0b11;
            switch(possi)
            {
                case 3:
                    printf("\033\13332m%c", i);
                    break;
                case 2:
                    printf("\033\13333m%c", i);
                    break;
                case 1:
                    printf("\033\13334m%c", i);
                    break;
                default:
                    printf("\033\13331m%c", i);
            }
        }
        if(lns)
            printf("\033\133%iB", lns);
        puts("\033\133m");
        ch = rdchr();
        while(ch != '\n')
        {
            if(ch == 0177)
            {
                if(chcnt == 0)
                    ring;
                else
                {
                    --chcnt;
                    fputs("\b \b", stdout);
                }
            }
            else if(ch >= 'a' && ch <= 'z')
            {
                if(chcnt < 5)
                {
                    txt[chcnt] = ch;
                    ++chcnt;
                    putchar(ch);
                }
                else
                    ring;
            }
            else
                ring;
checkreturn:
            ch = rdchr();
            if(ch == '\n')
            {
                if(chcnt == 5)
                {
                    txt[5] = '\0';
                    if(!isword(dic, diclen, txt))
                    {
                        ring;
                        goto checkreturn;
                    }
                }
                else
                {
                    ring;
                    goto checkreturn;
                }
            }
        }
        --attempts;
        putchar('\r');
        strcpy(cpy, target);
        for(int i = 0; i < 5; i++)
        {
            if(target[i] == txt[i])
                cpy[i] = ' ';
        }
        for(int i = 0; i < 5; i++)
        {
            pos = (txt[i] - 'a') * 2;
            possi = possible_letters >> pos & 0b11;
            if(target[i] == txt[i])
            {
                fputs("\033\13332m", stdout);
                possi = 3;
            }
            else if(tmp = strchr(cpy, txt[i]))
            {
                *tmp = ' ';
                fputs("\033\13333m", stdout);
                if(possi <= 1)
                    possi = 2;
            }
            else
            {
                fputs("\033\13331m", stdout);
                if(possi == 1)
                    possi = 0;
            }
            possible_letters &= ~(0b11ull << pos);
            possible_letters |= (uint64_t)possi << pos;
            putchar(txt[i]);
            if(anim)
                mssleep(500);
        }
        puts("\033\1330m");
        chcnt = 0;
        ++lns;
        if(strcmp(target, txt) == 0)
            attempts = 0;
        else if(attempts == 0)
            puts(target);
    }
    seed *= FACTOR;
    seed += 11;
    seed &= 0xffffffffffff;
    puts("Press q to quit, any other key to continue");
}
int main(int argl, char *argv[])
{
#ifdef _WIN32
    HANDLE ho = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD cm;
    GetConsoleMode(ho, &cm);
    cm |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(ho, cm);
#else
    struct termios oldf, currf;
    tcgetattr(STDIN_FILENO, &currf);
    memcpy(&oldf, &currf, sizeof(struct termios));
    currf.c_lflag &= ~ECHO;
    currf.c_lflag &= ~ICANON;
    tcsetattr(STDIN_FILENO, TCSANOW, &currf);
#endif
    setvbuf(stdout, NULL, _IONBF, 0);
    seed = time(NULL) ^ FACTOR;
    int guesses = 6;
    char anim = 1;
    char dicts = 0, isguess = 0;
    const char *dictfn = NULL, *ansfn = NULL;
    char *arg;
    for(size_t i = 1; i < argl; ++i)
    {
        arg = argv[i];
        if(*arg == '-')
        {
            for(size_t j = 1; arg[j] != '\0'; ++j)
            {
                switch(arg[j])
                {
                    case'a':
                        anim = 0;
                        break;
                    case'd':
                        dicts = 2;
                        break;
                    case'g':
                        isguess = 1;
                        break;
                    default:
                        printf("Unregonized option -%c, ignoring...\n", arg[j]);
                }
            }
        }
        else if(dicts == 2)
            dictfn = arg, --dicts;
        else if(dicts == 1)
            ansfn = arg, dicts = 0;
        else if(isguess)
            guesses = atoi(arg), isguess = 0;
        else
            printf("\033\13331mUnused command line argument %s.\033\133m\n", arg);
    }
    if(dictfn == NULL && ansfn == NULL)
        find_and_load();
    else
    {
        loadfile(dictfn);
        loadans(ansfn);
    }
    play(guesses, anim);
    for(char again = rdchr(); again != 'q'; again = rdchr())
        play(guesses, anim);
#ifndef _WIN32
    tcsetattr(STDIN_FILENO, TCSANOW, &oldf);
#endif
    return 0;
}
