/* @file config.h
 * This file implements stuff related to the configuration file.
 * It can read customizations on it and set the global variables.
 *
 * If the user specifies options at commandline, they have higher
 * precedence over the config file.
 *
 * Remember, DEFAULT_CONFIG_FILE is defined on the Makefile.
 * By default, it's '/home/<user>/.yetrisrc.ini'
 *
 */

#include <string.h>
#include "config.h"
#include "globals.h"


/** Deals with the config file, storing each option in memory.
 *  @see globals.h
 */
void config_handle()
{
//	if (global.config_filename != NULL)
//		config_parse(global.config_filename)
//	else
	if (!config_file_exists(DEFAULT_CONFIG_FILE))
	{
		config_create_default(DEFAULT_CONFIG_FILE);
		return;
	}

	config_parse(DEFAULT_CONFIG_FILE);
}

/** Tests if #filename exists. */
bool config_file_exists(char* filename)
{
	FILE* file = fopen(filename, "r");
	if (!file)
		return false;

	fclose(file);
	return true;
}

/** Reads the config file and stores It's contents in memory
 *  This is the main interface with the iniparser library
 */
void config_parse(char* filename)
{
	dictionary* ini;
	int n;

	ini = iniparser_load(filename);
	if (!ini)
		return; /* couldnt parse */

	/* defining some boolean flags */
	global.screen_use_colors          = iniparser_getboolean(ini, "interface:colors", -1);
	global.screen_center_vertically   = iniparser_getboolean(ini, "interface:center_vertical", -1);
	global.screen_center_horizontally = iniparser_getboolean(ini, "interface:center_horizontal", -1);
	global.screen_fancy_borders       = iniparser_getboolean(ini, "interface:fancy_borders", -1);
	global.screen_show_outer_border   = iniparser_getboolean(ini, "interface:outer_border", -1);
	global.game_has_statistics        = iniparser_getboolean(ini, "interface:statistics", -1);

	global.game_can_hold  = iniparser_getboolean(ini, "gameplay:hold",  -1);
	global.game_has_ghost = iniparser_getboolean(ini, "gameplay:ghost", -1);

	n = iniparser_getint(ini, "gameplay:next", -1);
	if ((n >= 0) && (n <= NEXT_PIECES_NO))
		global.game_next_no = n;

	n = iniparser_getint(ini, "gameplay:random", -1);
	if ((n >= 1) && (n <= 2))
		global.game_random_algorithm = n;

	global.theme_piece_has_colors = iniparser_getboolean(ini, "theming:piece_has_color",  -1);
	global.theme_ghost_has_colors = iniparser_getboolean(ini, "theming:ghost_has_color",  -1);
	global.theme_show_pivot_block = iniparser_getboolean(ini, "theming:show_pivot", -1);
	global.theme_lock_piece_color = iniparser_getboolean(ini, "theming:locked_piece_color", -1);

	/* copying custom piece theme */
	char piece_theme[3] = {};
	strncpy(piece_theme, iniparser_getstring(ini, "theming:piece", NULL), 3);
	if (piece_theme[0] != '\0')
		strncpy(global.theme_piece, piece_theme, 2);

	/* copying custom ghost theme */
	char ghost_theme[3] = {};
	strncpy(ghost_theme, iniparser_getstring(ini, "theming:ghost", NULL), 3);
	if (ghost_theme[0] != '\0')
		strncpy(global.theme_ghost, ghost_theme, 2);

	/* copying custom clear line theme */
	char clear_theme[3] = {};
	strncpy(clear_theme, iniparser_getstring(ini, "theming:clear_line", NULL), 3);
	if (clear_theme[0] != '\0')
		strncpy(global.theme_clear_line, clear_theme, 2);

	//attempt to parse colors
/* 	/\* getting colors *\/ */
/* 	int color; */
/* #define apply_if_valid(string, ok) { (color = iniparser_getint(ini, string, -1)        \ */
/* 		                             if ((color >= 0) && (color < 8)) { ok = color } } */

/* 	apply_if_valid("theming:color_titles_fg", global. */
/* 	color = iniparser_getint(ini, "theming:color_titles_fg", -1); */
/* 	if (is_valid(color)) global */


	iniparser_freedict(ini);
}

/** Creates a default config file on #filename */
void config_create_default(char* filename)
{
	FILE* file = fopen(filename, "w");

	if (!file)
		return;

	const char text[] =
		"# This is the default yetris configuration file.\n"
		"# Remember that commandline options take precedence over this file.\n"
		"#\n"
		"# Everything after '#' is ignored by the game.\n"
		"# Options are categorized like [this] and have 'key = value' pairs.\n"
		"# If some value is mistyped or left blank, the default value will be used.\n"
		"#\n"
		"# Boolean values accepted are case-insensitive (true, TRUE, tRuE...)\n"
		"\n"
		"[gameplay]\n"
		"\n"
		"# Enable/disable ghost piece (indication of where the piece will land)\n"
		"# default: true\n"
		"ghost = true\n"
		"\n"
		"# Enable/disable holding a piece (saving current piece for later use)\n"
		"# default: true\n"
		"hold  = true\n"
		"\n"
		"# How many next pieces are shown\n"
		"# default: 6\n"
		"next  = 6\n"
		"\n"
		"# The piece-generation algorithm used by the game. Invalid values\n"
		"# will fallback to the last valid one.\n"
		"# Valid values are:\n"
		"#  1: bag generator\n"
		"#  2: dummy random (srand)\n"
		"# default: 1\n"
		"random = 1\n"
		"\n"
		"[interface]\n"
		"\n"
		"# Enable/disable colors on the game.\n"
		"# default: true\n"
		"colors = true\n"
		"\n"
		"# Center vertically/horizontally the game screen.\n"
		"# default: false/false\n"
		"center_vertical   = false\n"
		"center_horizontal = false\n"
		"\n"
		"# If the game boxes have fancy borders\n"
        "# default: true\n"
		"fancy_borders = true\n"
		"\n"
		"# Show/hide outer border on the game screen\n"
		"# default: true\n"
		"outer_border = true\n"
		"\n"
		"# Show/hide piece statistics\n"
		"# default: true\n"
		"statistics = true\n"
		"\n"
		"[theming]\n"
		"\n"
		"# The appearance of a normal piece (two-char string)\n"
		"# default: \"  \"\n"
		"piece = \"  \"\n"
		"\n"
		"# The appearance of the ghost piece (two-char string)\n"
		"# default: \"[]\"\n"
		"ghost = \"[]\"\n"
		"\n"
		"# If normal pieces have colors (each individual color is defined by default)\n"
		"# default: true\n"
		"piece_has_color = true\n"
		"\n"
		"# If the ghost color has color\n"
		"# (it's color would be the same as the current piece's).\n"
		"# If 'piece_has_color' is false, the ghost gets the color the current piece\n"
		"# would have.\n"
		"# default: false\n"
		"ghost_has_color = false\n"
		"\n"
		"# The appearance of the line we clear, just before it disappears\n"
		"# default: \"::\"\n"
		"clear_line = \"::\"\n"
		"\n"
		"# Show/hide the pivot block (the center of the piece's rotation axis)\n"
		"# default: false\n"
		"show_pivot = false\n"
		"\n"
		"# When the piece locks on the board, will it have a custom color?\n"
		"# default: false\n"
		"locked_piece_color = false\n"
		"\n";

	fprintf(file, text);
}

