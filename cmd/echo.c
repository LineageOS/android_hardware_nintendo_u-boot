// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright 2000-2009
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 */

#include <common.h>
#include <command.h>

static void echo_char(const char c, bool err)
{
	if (!err)
		putc(c);
	else
		eputc(c);
}

static void echo_string(const char *s, bool err)
{
	if (!err)
		puts(s);
	else
		eputs(s);
}

static int do_echo(struct cmd_tbl *cmdtp, int flag, int argc,
		   char *const argv[])
{
	int i = 1;
    bool err = false;
	bool space = false;
	bool newline = true;

    if (argc > 1) {
        if (!strcmp(argv[1], "-n")) {
            newline = false;
            ++i;
        }
        if (!strcmp(argv[1], "-e")) {
            err = true;
            ++i;
        }
    }

    if (argc > 2 && (!newline || err)) {
        if (!strcmp(argv[2], "-n")) {
            newline = false;
            ++i;
        }
        if (!strcmp(argv[2], "-e")) {
            err = true;
            ++i;
        }
    }

	for (; i < argc; ++i) {
		if (space) {
			echo_char(' ', err);
		}
		echo_string(argv[i], err);
		space = true;
	}

	if (newline)
		echo_char('\n', err);

	return 0;
}

U_BOOT_CMD(
	echo, CONFIG_SYS_MAXARGS, 1, do_echo,
	"echo args to console",
	"[-e] [-n] [args..]\n"
	"    - echo args to console; -e prints to stderr; -n suppresses newline;"
);
