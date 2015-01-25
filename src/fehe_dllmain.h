/* *************************************************************************
 *
 * Copyright (C) 2015 by fehe (F. Fröhling, H. Heimers)
 *
 * Contact: felix.froehling1@gmail.com
 *			hendriku31@gmail.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 * 
 ************************************************************************* */
 
#ifndef FEHE_DLLMAIN_H_ 
#define FEHE_DLLMAIN_H_ 

#if defined(_WIN32) || defined(_WIN64)
 #include <windows.h>
#define DllExport __declspec( dllexport )
#define DllImport __declspec( dllimport )

#elif __linux

#define DllExport 
#define DllImport 

#endif

#endif /* FEHE_DLLMAIN_H_  */
