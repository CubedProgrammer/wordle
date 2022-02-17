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
void play(int attempts)
{
    puts("Take your guesses!");
    size_t diclen = gh_cp_wordle____cnt;
    char **dic = gh_cp_wordle____ls;
    uint32_t ind = seed % diclen;
    char *target = dic[ind], *tmp;
    char cpy[6];
    char txt[6];
    char ch;
    int chcnt = 0, lns = 0;
    putchar('\n');
    uint32_t possible_letters = 0x3ffffff, possi;
    while(attempts)
    {
        printf("\033\133%iF", lns + 1);
        for(int i = 'a'; i <= 'z'; i++)
        {
            possi = possible_letters & 1 << (i - 'a');
            if(possi)
                printf("\033\13334m%c", i);
            else
                printf("\033\13331m%c", i);
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
            if(target[i] == txt[i])
                fputs("\033\13332m", stdout);
            else if(tmp = strchr(cpy, txt[i]))
            {
                *tmp = ' ';
                fputs("\033\13333m", stdout);
            }
            else
            {
                possible_letters &= ~(1 << (txt[i] - 'a'));
                fputs("\033\13331m", stdout);
            }
            putchar(txt[i]);
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
    if(argv[1] != NULL)
        guesses = atoi(argv[1]);
    if(argl >= 3)
        loadfile(argv[2]);
    else
        find_and_load();
    play(guesses);
    for(char again = rdchr(); again != 'q'; again = rdchr())
        play(guesses);
#ifndef _WIN32
    tcsetattr(STDIN_FILENO, TCSANOW, &oldf);
#endif
    return 0;
}
