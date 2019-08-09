/*
METAR Decoder Software Package Library: Parses Aviation Routine Weather Reports
Copyright (C) 2003  Eric McCarthy

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "metar_structs.h"
 
/********************************************************************/
/*                                                                  */
/*  Title:         fracPart                                         */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          13 Jun 1995                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:      Convert a character string fraction into a       */
/*                 decimal (floating point) number.                 */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         string - a pointer to a character string frac-   */
/*                          tion.                                   */
/*  Output:        A decimal (floating point) number.               */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/
 
float fracPart( const char *instring )
{
   char buf[ 32 ],
        *slash;
 
   float numerator,
         denominator;
 
   slash = strchr(instring, '/');

   if (slash == NULL) {
    return ((float) atoi(instring));
   }
 
   memset(buf , '\0', sizeof(buf));
   if (slash - instring >= sizeof(buf)) {
     /* prevent buffer overflow */
     return ((float) MAXINT);
   }
   strncpy( buf, instring, slash - instring);
 
   numerator = (float) atoi(buf);
 
   denominator = (float) atoi(slash + 1);
 
   if( denominator == 0.0 )
      return ((float) MAXINT);
   else
      return (numerator/denominator);
 
}
 
// vim: set ts=4 sw=4 sts=4 noet :
