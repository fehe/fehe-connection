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
 
#include <stdio.h> 
#include <stdlib.h> 

#include "fehe_dllmain.h" 

#if defined(_WIN32) || defined(_WIN64)

BOOL APIENTRY DllMain (HINSTANCE hInst     /* Library instance handle. */ , 
                       DWORD reason        /* Reason this function is being called. */ , 
                       LPVOID reserved     /* Not used. */ ) 
{ 
    switch (reason) 
    { 
      case DLL_PROCESS_ATTACH: 
        break; 
  
      case DLL_PROCESS_DETACH: 
        break; 
  
      case DLL_THREAD_ATTACH: 
        break; 
  
      case DLL_THREAD_DETACH: 
        break; 
    } 
  
    /* Returns TRUE on success, FALSE on failure */ 
    return TRUE; 
}

#endif
