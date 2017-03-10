#include <string>
#include <vector>

//extern "C" {
#include "atlast.h"
#include "atoleo.h"
#include "basic.h"
#include "cmd.h"
#include "defuns.h"
#include "init.h"
#include "io-abstract.h"
#include "io-term.h"
#include "io-utils.h"
#include "mdi.h"
#include "neoleo_swig.h"
#include "graph.h"
#include "mysql.h"
#include "print.h"
//}

using std::string;
using std::vector;

#define _(x) (x) // TODO get rid of this line

void
init_native_language_support()
{
#if 0	/* ENABLE_NLS */ // mcarter
	setlocale(LC_ALL, "");
	bindtextdomain(PACKAGE, LOCALEDIR);
	textdomain(PACKAGE);
#endif
}

/* A bland signal handler. */
static RETSIGTYPE
got_sig (int sig)
{
}


static void
init_maps (void)
{
  num_maps = 0;
  the_maps = 0;
  map_names = 0;
  map_prompts = 0;

  the_funcs = (cmd_func**) ck_malloc (sizeof (struct cmd_func *) * 2);
  num_funcs = 1;

  //the_funcs[0] = &cmd_funcs[0]; mcarter
  // TODO LOW mcarter: unhappy about diagnostic warning tweaks, but seems OK
//#pragma GCC diagnostic push
//#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"
  the_funcs[0] = (cmd_func *) get_cmd_funcs();
//#pragma GCC diagnostic pop  

  find_func (0, &end_macro_cmd, "end-macro");
  find_func (0, &digit_0_cmd, "digit-0");
  find_func (0, &digit_9_cmd, "digit-9");
  find_func (0, &break_cmd, "break");
  find_func (0, &universal_arg_cmd, "universal-argument");

  create_keymap ("universal", 0);
  push_command_frame (0, 0, 0);
}



int 
main(int argc, char **argv)
{
	volatile int ignore_init_file = 0;
	FILE * init_fp[2];
	char * init_file_names[2];
	volatile int init_fpc = 0;
	int command_line_file = 0;	/* was there one? */


	init_atoleo();
	/*
	{
		string s = "4life";
		vector<char> v(s.begin(), s.end());
		v.push_back(0);
		char *cmd = &v[0];
		atl_eval(cmd);
	}
	*/

	init_native_language_support();
	MdiInitialize();	/* Create initial Global structure */
	PlotInit();
	AllocateDatabaseGlobal();
	InitializeGlobals();
	Global->argc = argc;
	Global->argv = argv;
	parse_command_line(argc, argv, &ignore_init_file);
	init_basics();

	if(get_option_tests()) {
		headless_tests();
		exit(EXIT_SUCCESS);
	}


	/* Find the init files. 
	 * This is done even if ignore_init_file is true because
	 * it effects whether the disclaimer will be shown.
	 */
	{
		char *ptr, *home;

		home = getenv ("HOME");
		if (home)
		{
			ptr = mk_sprintf ("%s/%s", home, RCFILE);
			init_fp[init_fpc] = fopen (ptr, "r");
			init_file_names[init_fpc] = ptr;
			if (init_fp[init_fpc])
				++init_fpc;
		}

		init_fp[init_fpc] = fopen (RCFILE, "r");
		if (init_fp[init_fpc])
			++init_fpc;
	}

	FD_ZERO (&read_fd_set);
	FD_ZERO (&read_pending_fd_set);
	FD_ZERO (&exception_fd_set);
	FD_ZERO (&exception_pending_fd_set);

	bool force_cmd_graphics = false;
	choose_display(force_cmd_graphics);
	io_open_display ();

	init_graphing ();
	PrintInit();
	OleoSetEncoding(OLEO_DEFAULT_ENCODING);

	if (setjmp (Global->error_exception))
	{
		fprintf (stderr, _("Error in the builtin init scripts (a bug!).\n"));
		io_close_display(69);
		exit (69);
	}
	else
	{
		init_maps ();
		init_named_macro_strings ();
		run_init_cmds ();
	}

	oleo_catch_signals(&got_sig);

	/* Read the init file. */
	{
		volatile int x;
		for (x = 0; x < init_fpc; ++x)
		{
			if (setjmp (Global->error_exception))
			{
				fprintf (stderr, _("   error occured in init file %s near line %d."),
						init_file_names [x], Global->sneaky_linec);
				io_info_msg(_("   error occured in init file %s near line %d."),
						init_file_names [x], Global->sneaky_linec);
			}
			else
				if (!ignore_init_file)
					read_cmds_cmd (init_fp[x]);
			fclose (init_fp[x]);
		}
	}


	if (option_filter) {
		read_file_and_run_hooks(stdin, 0, "stdin");
	} else if (argc - optind == 1) {
		FILE * fp;
		/* fixme: record file name */

		if ((fp = fopen (argv[optind], "r"))) {
			if (setjmp (Global->error_exception)) {
				fprintf (stderr, _(", error occured reading '%s'\n"), argv[optind]);
				io_info_msg(_(", error occured reading '%s'\n"), argv[optind]);
			} else
				read_file_and_run_hooks (fp, 0, argv[optind]);
			fclose (fp);
			command_line_file = 1;
			FileSetCurrentFileName(argv[optind]);
		} else {
			fprintf (stderr, _("Can't open %s: %s\n"), argv[optind], err_msg ());
			io_info_msg(_("Can't open %s: %s\n"), argv[optind], err_msg ());
		}

		optind++;
	}

	/* Force the command frame to be rebuilt now that the keymaps exist. */
	{
		struct command_frame * last_of_the_old = the_cmd_frame->next;
		while (the_cmd_frame != last_of_the_old)
			free_cmd_frame (the_cmd_frame);
		free_cmd_frame (last_of_the_old);
	}

	io_recenter_cur_win ();
	Global->display_opened = 1;
	io_run_main_loop();

	return (0); /* Never Reached! */
}