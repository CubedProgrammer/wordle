#include<ctype.h>
#ifdef _WIN32
#include<windows.h>
#else
#include<termios.h>
#include<unistd.h>
#endif
int rdchr(void)
{
    char arr[4];
    int ch;
#ifdef _WIN32
    arr[0] = getch();
    if(arr[0] == -040)
        arr[0] = getch();
    else if(arr[0] == '\b')
        arr[0] = 127;
    else if(arr[0] == '\r')
        arr[0] = '\n';
#else
    read(STDIN_FILENO, arr, sizeof(arr));
#endif
    ch = tolower(arr[0]);
    return ch;
}
