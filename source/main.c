#include "immersive.h"

#include <assert.h>
#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

static void usage(char** argv)
{
    printf("\
%s [OPTIONS...]\n\
\n\
OPTIONS:\n\
    -h, --help\n\
        Print help text.\n\
\n\
    --pid <pid>\n\
        The main window of the specified process id and the target to immersive window.\n\
\n\
    --hwnd <hwnd>\n\
        The specified window handle to immersive window.\n\
", argv[0]);
}

int main(int argc, char** argv)
{
    assert(argc >= 1);
    assert(argv != NULL);

    HWND hwnd;

    if (argc == 1)
    {
        usage(argv);
        return EXIT_FAILURE;
    }

    if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)
    {
        usage(argv);
        return EXIT_SUCCESS;
    }
    else if (strcmp(argv[1], "--pid") == 0)
    {
        if (argc == 2)
        {
            usage(argv);
            return EXIT_FAILURE;
        }

        char* end = NULL;
        long const value = strtol(argv[2], &end, 10);
        if (end == NULL || *end != '\0')
        {
            fprintf(stderr, "ERROR: invalid format -- '%s'\n", argv[2]);
            return EXIT_FAILURE;
        }
        if (value == LONG_MIN && errno == ERANGE)
        {
            fprintf(stderr, "ERROR: underflow -- '%s'\n", argv[2]);
            return EXIT_FAILURE;
        }
        if (value == LONG_MAX && errno == ERANGE)
        {
            fprintf(stderr, "ERROR: overflow -- '%s'\n", argv[2]);
            usage(argv);
            return EXIT_FAILURE;
        }

        DWORD const pid = (DWORD)value;
        hwnd = GetWindowHandleFromProcessId(pid);
    }
    else if (strcmp(argv[1], "--hwnd") == 0)
    {
        if (argc == 2)
        {
            usage(argv);
            return EXIT_FAILURE;
        }

        char* end = NULL;
        long const value = strtol(argv[2], &end, 10);
        if (end == NULL || *end != '\0')
        {
            fprintf(stderr, "ERROR: invalid format -- '%s'\n", argv[2]);
            return EXIT_FAILURE;
        }
        if (value == LONG_MIN && errno == ERANGE)
        {
            fprintf(stderr, "ERROR: underflow -- '%s'\n", argv[2]);
            return EXIT_FAILURE;
        }
        if (value == LONG_MAX && errno == ERANGE)
        {
            fprintf(stderr, "ERROR: overflow -- '%s'\n", argv[2]);
            return EXIT_FAILURE;
        }

        hwnd = (HWND)value;
    }
    else
    {
        usage(argv);
        return EXIT_FAILURE;
    }

    DWORD const error = Immersive(hwnd);
    return error == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
