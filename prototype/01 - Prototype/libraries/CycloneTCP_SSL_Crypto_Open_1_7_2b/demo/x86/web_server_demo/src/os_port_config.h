/**
 * @file os_port_config.h
 * @brief RTOS port configuration file
 *
 * @section License
 *
 * Copyright (C) 2010-2016 Oryx Embedded SARL. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 * @author Oryx Embedded SARL (www.oryx-embedded.com)
 * @version 1.7.2
 **/

#ifndef _OS_PORT_CONFIG_H
#define _OS_PORT_CONFIG_H

//Select underlying RTOS
//#define _WIN32

//Miscellaneous definitions
#if defined(_WIN32)
   #define strlwr _strlwr
   #define strcasecmp _stricmp
   #define strncasecmp _strnicmp
   #define strtok_r(str, delim, p) strtok(str, delim)
#endif

#endif