#include <stdio.h>
#include "kv.h"

int main(int argc, char *argv[])
{
    if (argc == 1) {
	return 0;
    }

    for (int i = 1; i < argc; ++i) {
	if (argv[i][0] == 'p') {
	    put_value(argv[i]);
	} else if (argv[i][0] == 'g') {
	    get_value(argv[i]);
	} else if (argv[i][0] == 'd') {
	    delete_value(argv[i]);
	} else if (argv[i][0] == 'c') {
	    clear_all();
	} else if (argv[i][0] == 'a') {
	    print_all();
	} else {
		printf("bad command\n");
	}
    }
    return 0;
}
