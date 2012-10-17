/*
  mnea.h
  Copyright (C) 2010 Joaquim Pereira - http://www.joaquim.org
  All rights reserved.

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
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/


/*
  Original Code From 
  TinyGPS - a small GPS library for Arduino providing basic NMEA parsing
  Mikal Hart

*/

#ifndef __NMEA_H__
#define __NMEA_H__

#include "stm8s.h"


/** statical and dynamical position information */
typedef struct
{
   /** Latitude -90000000..90000000 (90° S to 90° N) [1E-6 °] */
   s32 lat;
   /** Longitude -180000000..180000000 (180° W to 180° E) [1E-6 °] */
   s32 lon;
   /** Altitude -2147483648..2147483647 [1E-3 m] (above Mean-Sea-Level - MSL) */
   s32 alt;
   /** Course 0..36000 [1E-2 °] */
   u16 cor;
   /** Speed 0..65536 [1E-2 m/s] */
   u16 spd;
} position_t;

/** date and time */
typedef struct
{
   u8 time[9];
   u8 date[11];
} date_t;


/** GPS precision and status information */
typedef struct
{
   
   /** Used satellites (satellites-in-use) */
   int siu;
   
} status_t;

/** Satellites in view status information */
typedef struct
{
   
   /** satellite PRN number */
   int prn;   
   /** SNR in dB */
   int snr;
	 
	 int ele;
	 int azi;
   
} sat_inview_t;


   /**
 * Generic structure holding supported GPS information from decoded NMEA
 * sentences. This can be used to be logged on memory as a LB file
 * (locus-binary).
 */
typedef struct
{
   position_t pos;
   date_t datetime;
   status_t status;
   sat_inview_t sat_inview[12];

} gps_data_t;


extern char *debug;

void DecodeInit(void);
bool EncodeNMEA(char c);

void get_gps_data(gps_data_t * external_gps_data);

#endif /*__NMEA_H__*/