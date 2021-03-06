#pragma once
/*
 * $Id: decompile.h,v 1.4 2000/08/10 21:02:50 danny Exp $
 *
 * Copyright � 1993 Free Software Foundation, Inc.
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "cell.h"

#ifdef __cplusplus
//extern "C" {
#endif

extern void decomp_free ();
extern char * backslash_a_string (char *string, int add_quote);
char * decomp (CELLREF r, CELLREF c, CELL *cell);
char * decomp_formula (CELLREF r, CELLREF c, CELL *cell, int tog);

#ifdef __cplusplus
//}
#endif

