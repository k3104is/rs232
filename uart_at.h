/*
***************************************************************************
*
* Author: Teunis van Beelen
*
* Copyright (C) 2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013, 2014, 2015, 2016, 2017 Teunis van Beelen
*
* Email: teuniz@gmail.com
*
***************************************************************************
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
***************************************************************************
*/

/* Last revision: August 5, 2017 */

/* For more info and how to use this library, visit: http://www.teuniz.net/RS-232/ */


#ifndef _UART_AT_
#define _UART_AT_


#include <stdio.h>
#include <string.h>
enum{
    eOK = 0,
    eNG,
};

enum{
    eOFF = 0,
    eON
};
typedef struct
{
    unsigned char   u1_cport_nr;
    unsigned long   u4_bdrate;
    char            s1_mode[4];
}S_SETTING;



typedef struct
{
    char            *s1_command;
    char            *s1_discription;
    // void           (*vdStrFunc)(char*);
    // char            *s1_res_ok;
    // char            *s1_res_ng;
}S_AT;

#endif


