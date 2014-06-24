/* for stdio and stdlib */
#include "pacf_log.h"
/* for filter interface */
#include "pacf_interface.h"
/* for pc_info definition */
#include "main.h"
/* for errno */
#include <errno.h>
/* for string funcs */
#include <string.h>
/* for getopt() */
#include <unistd.h>
/*---------------------------------------------------------------------*/
/* global variable to indicate start/stop for lua interpreter */
volatile uint32_t stop_processing = 0;
/* function to call lua interpreter */
extern int lua_kickoff(void);
/* global variable definitions */
static PacfInfo pc_info = {
	.batch_size 		= DEFAULT_BATCH_SIZE,
	.lua_startup_file 	= NULL
};
/*---------------------------------------------------------------------*/
/**
 * Program termination point
 * XXX - This function will free up all the previously 
 * 	allocated resources.
 */
void
clean_exit(int exit_val)
{
	TRACE_FUNC_START();
	fprintf(stdout, "Goodbye!\n");
	TRACE_FUNC_END();
	exit(exit_val);

}
/*---------------------------------------------------------------------*/
void
print_help(char *progname)
{
	TRACE_FUNC_START();
	fprintf(stdout, "Usage: %s [-b batch_size] "
		"[-f start_lua_script_file] [-h]\n", progname);
	
	TRACE_FUNC_END();
	clean_exit(EXIT_SUCCESS);
}
/*---------------------------------------------------------------------*/
/**
 * Main entry point
 */
int
main(int argc, char **argv)
{
	TRACE_FUNC_START();
	int c;

	/* accept command-line arguments */
	while ( (c = getopt(argc, argv, "b:hf:")) != -1) {
		switch(c) {
		case 'f':
			pc_info.lua_startup_file = (uint8_t *)strdup(optarg);
			if (NULL == pc_info.lua_startup_file) {
				TRACE_ERR("Can't strdup string for lua_startup_file: %s\n",
					  strerror(errno));
			}
			TRACE_DEBUG_LOG("Taking file %s as startup\n",
					pc_info.lua_startup_file);
			break;
		case 'b':
			pc_info.batch_size = atoi(optarg);
			TRACE_DEBUG_LOG("Taking batch_size as %d\n", 
					pc_info.batch_size);
			break;
		case 'h':
			print_help(*argv);
			break;
		default:
			print_help(*argv);
			break;
		}
	}

	/* warp to lua interface */
	lua_kickoff();

	TRACE_FUNC_END();

	clean_exit(EXIT_SUCCESS);

	/* control will never come here */
	return EXIT_SUCCESS;
}
/*---------------------------------------------------------------------*/
