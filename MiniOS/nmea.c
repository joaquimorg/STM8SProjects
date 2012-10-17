/*
  mnea.c
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

#include "nmea.h"
#include "glcdlib.h"
#include <stdio.h>
#include <string.h>

/** Current GPS information based on decoded NMEA sentences */
gps_data_t gps_data;


// parsing state variables
char _parity;
bool _is_checksum_term;
char _term[16];
char _sentence_type;
char _term_number;
char _term_offset;
bool _gps_data_good;


enum {
    _GPS_SENTENCE_GPGGA, _GPS_SENTENCE_GPRMC, _GPS_SENTENCE_GPGSV, _GPS_SENTENCE_OTHER
};

#define _GPRMC_TERM   "GPRMC"
#define _GPGGA_TERM   "GPGGA"
#define _GPGSV_TERM   "GPGSV"

// properties

s32 _new_latitude;
s32 _new_longitude;
s32 _new_altitude;
u32 _new_speed;
u32 _new_course;
u8 _sat_pos;


//
// internal utilities
//

int gpsStrCmp(const char *str1, const char *str2) {
    while (*str1 && *str1 == *str2)
        ++str1, ++str2;
    return *str1;
}

int FromHex(char a) {
    if (a >= 'A' && a <= 'F')
        return a - 'A' + 10;
    else if (a >= 'a' && a <= 'f')
        return a - 'a' + 10;
    else
        return a - '0';
}


float _decimal(char* s) {
	// returns base-10 value of zero-termindated string
	// that contains only chars '+','-','0'-'9','.';
	// does not trap invalid strings!
  long  rl = 0;
  float rr = 0.0;
  float rb = 0.1;
  bool dec = FALSE;
  int i = 0;

  if ((s[i] == '-') || (s[i] == '+')) { i++; }
  while (s[i] != 0) {
    if (s[i] == '.') {
      dec = TRUE;
    }
    else{
      if (!dec) {
        rl = (10 * rl) + (s[i] - 48);
      }
      else {
        rr += rb * (float)(s[i] - 48);
        rb /= 10.0;
      }
    }
    i++;
		if ( i > 16 ) return 0;
  }
  rr += (float)rl;
  if (s[0] == '-') {
    rr = 0.0 - rr;
  }
  return rr;
}

//
//	Main Decoding
//

void DecodeInit(void) {

	u8 cnt;
    _parity = 0;
    _is_checksum_term = FALSE;
    _sentence_type = _GPS_SENTENCE_OTHER;
    _term_number = 0;
    _term_offset = 0;
    _gps_data_good = FALSE;
    _term[0] = '\0';
	
	
	/* clear GPS information */
	
	gps_data.pos.alt = ((s32)(0x00ul));
	gps_data.pos.lat = ((s32)(0x00ul));
	gps_data.pos.lon = ((s32)(0x00ul));
	gps_data.pos.cor = ((u16)(0x00u));
	gps_data.pos.spd = ((u16)(0x00u));	
	gps_data.status.siu = 0;
	strcpy(gps_data.datetime.time, "00:00:00\0");
	strcpy(gps_data.datetime.date, "2000-00-00\0");
	
	for (cnt = 0; cnt < 12; cnt++ ) {
			gps_data.sat_inview[cnt].prn = 0;
			gps_data.sat_inview[cnt].snr = 0;
			gps_data.sat_inview[cnt].ele = 0;
			gps_data.sat_inview[cnt].azi = 0;
	}
	
	_sat_pos = 0;
	
	UART2_DeInit();
	UART2_Init((u32)9600, UART2_WORDLENGTH_8D, UART2_STOPBITS_1, UART2_PARITY_NO, 
		UART2_SYNCMODE_CLOCK_DISABLE, UART2_MODE_RX_ENABLE);
	
	UART2_ITConfig(UART2_IT_RXNE, ENABLE);
	
	//enableInterrupts();
}

/*

=== GGA - Global Positioning System Fix Data ===

Time, Position and fix related data for a GPS receiver.

------------------------------------------------------------------------------
                                                      11
        1         2       3 4        5 6 7  8   9  10 |  12 13  14   15
        |         |       | |        | | |  |   |   | |   | |   |    |
 $--GGA,hhmmss.ss,llll.ll,a,yyyyy.yy,a,x,xx,x.x,x.x,M,x.x,M,x.x,xxxx*hh<CR><LF>
------------------------------------------------------------------------------

Field Number: 

1. Universal Time Coordinated (UTC)
2. Latitude
3. N or S (North or South)
4. Longitude
5. E or W (East or West)
6. GPS Quality Indicator,
     - 0 - fix not available,
     - 1 - GPS fix,
     - 2 - Differential GPS fix
           (values above 2 are 2.3 features)
     - 3 = PPS fix
     - 4 = Real Time Kinematic
     - 5 = Float RTK
     - 6 = estimated (dead reckoning)
     - 7 = Manual input mode
     - 8 = Simulation mode
7. Number of satellites in view, 00 - 12
8. Horizontal Dilution of precision (meters)
9. Antenna Altitude above/below mean-sea-level (geoid) (in meters)
10. Units of antenna altitude, meters
11. Geoidal separation, the difference between the WGS-84 earth
     ellipsoid and mean-sea-level (geoid), "-" means mean-sea-level
     below ellipsoid
12. Units of geoidal separation, meters
13. Age of differential GPS data, time in seconds since last SC104
     type 1 or 9 update, null field when DGPS is not used
14. Differential reference station ID, 0000-1023
15. Checksum



=== RMC - Recommended Minimum Navigation Information ===

------------------------------------------------------------------------------
                                                          12
        1         2 3       4 5        6  7   8   9    10 11|  13
        |         | |       | |        |  |   |   |    |  | |   |
 $--RMC,hhmmss.ss,A,llll.ll,a,yyyyy.yy,a,x.x,x.x,xxxx,x.x,a,m,*hh<CR><LF>
------------------------------------------------------------------------------

Field Number:

1. UTC Time
2. Status, V=Navigation receiver warning A=Valid
3. Latitude
4. N or S
5. Longitude
6. E or W
7. Speed over ground, knots
8. Track made good, degrees true
9. Date, ddmmyy
10. Magnetic Variation, degrees
11. E or W
12. FAA mode indicator (NMEA 2.3 and later)
13. Checksum

A status of V means the GPS has a valid fix that is below an internal
quality threshold, e.g. because the dilution of precision is too high 
or an elevation mask test failed.


=== GSV - Satellites in view ===

These sentences describe the sky position of a UPS satellite in view.
Typically they're shipped in a group of 2 or 3.

------------------------------------------------------------------------------
	1 2 3 4 5 6 7     n
	| | | | | | |     |
 $--GSV,x,x,x,x,x,x,x,...*hh<CR><LF>
------------------------------------------------------------------------------

Field Number: 

1. total number of GSV messages to be transmitted in this group
2. 1-origin number of this GSV message  within current group
3. total number of satellites in view (leading zeros sent)
4. satellite PRN number (leading zeros sent)
5. elevation in degrees (00-90) (leading zeros sent)
6. azimuth in degrees to true north (000-359) (leading zeros sent)
7. SNR in dB (00-99) (leading zeros sent)
   more satellite info quadruples like 4-7
   n) checksum

Example:           4         7  8        11 12        15 16        19
    $GPGSV,3,1,11,03,03,111,00,04,15,270,00,06,01,010,00,13,06,292,00*74
    $GPGSV,3,2,11,14,25,170,00,16,57,208,39,18,67,296,40,19,40,246,00*74
    $GPGSV,3,3,11,22,42,067,42,24,14,311,43,27,05,244,00,,,,*4D

Some GPS receivers may emit more than 12 quadruples (more than three
GPGSV sentences), even though NMEA-0813 doesn't allow this.  (The
extras might be WAAS satellites, for example.) Receivers may also
report quads for satellites they aren't tracking, in which case the
SNR field will be null; we don't know whether this is formally allowed
or not.
*/


// Processes a just-completed term
// Returns true if new sentence has just passed checksum test and is validated

bool TermComplete(void) {
	int _tmp;
	s32 temp_snum = 0;        /* temporary signed number for string conversion */
	s32 temp_snum2 = 0;       /* temporary signed number for string conversion */
	u8  temp_char = 0;
	u8  temp_char2 = 0;
	s16 temp_word = 0;
   
    char checksum;
    if (_is_checksum_term) {
		
        checksum = 16 * FromHex(_term[0]) + FromHex(_term[1]);
        if (checksum == _parity) {
			
			//
			// bypass from status for testing...
			//
			//
			_gps_data_good = TRUE;
            if (_gps_data_good) {
                //_last_time_fix = _new_time_fix;
                //_last_position_fix = _new_position_fix;

                switch (_sentence_type) {
                    case _GPS_SENTENCE_GPRMC:
                        gps_data.pos.lat = _new_latitude;
                        gps_data.pos.lon = _new_longitude;
                        gps_data.pos.spd = _new_speed;
                        gps_data.pos.cor = _new_course;
                        break;
                    case _GPS_SENTENCE_GPGGA:
                        gps_data.pos.alt = _new_altitude;
                        gps_data.pos.lat = _new_latitude;
                        gps_data.pos.lon = _new_longitude;
                        break;
                }

                return TRUE;
            }
        }

        return FALSE;
    }

    // the first term determines the sentence type
    if (_term_number == 0) {
				
        if (!gpsStrCmp(_term, _GPRMC_TERM))
            _sentence_type = _GPS_SENTENCE_GPRMC; // 100
        else if (!gpsStrCmp(_term, _GPGGA_TERM))
            _sentence_type = _GPS_SENTENCE_GPGGA; // 200
		else if (!gpsStrCmp(_term, _GPGSV_TERM))
            _sentence_type = _GPS_SENTENCE_GPGSV; // 300
        else
            _sentence_type = _GPS_SENTENCE_OTHER;
        return FALSE;
    }
    
	if (_sentence_type != _GPS_SENTENCE_OTHER && _term[0]) {
		//_tmp = (_sentence_type == _GPS_SENTENCE_GPGGA ? 200 : 100) + _term_number;
		
		if ( _sentence_type == _GPS_SENTENCE_GPRMC ) _tmp = 100;
		else if ( _sentence_type == _GPS_SENTENCE_GPGGA )  _tmp = 200;
		else if ( _sentence_type == _GPS_SENTENCE_GPGSV )  _tmp = 300;
		else  _tmp = 900;
		_tmp = _tmp + _term_number;
		
        switch ( _tmp ) {
            case 101: // Time in both sentences
            case 201: // 

				gps_data.datetime.time[0] = _term[0];
				gps_data.datetime.time[1] = _term[1];

				gps_data.datetime.time[3] = _term[2];
				gps_data.datetime.time[4] = _term[3];

				gps_data.datetime.time[6] = _term[4];
				gps_data.datetime.time[7] = _term[5];
				
                break;
            case 102: // GPRMC A = Valid position, V = Warning
                _gps_data_good = _term[0] == 'A';
                break;			
            case 103: // Latitude
            case 202:
				_new_latitude = _decimal( _term );
                break;
            case 104: // N/S
            case 203:
                if (_term[0] == 'S')
                    _new_latitude = -_new_latitude;
                break;								
            case 105: // Longitude
            case 204:
				_new_longitude = _decimal( _term );
                break;
            case 106: // E/W
            case 205:
                if (_term[0] == 'W')
                    _new_longitude = -_new_longitude;
                break;
            case 107: // Speed (GPRMC)
				_new_speed = _decimal( _term ) * _GPS_KMPH_PER_KNOT;
                break;	
			case 207: //total number of satellites in use
					gps_data.status.siu = _decimal( _term );
                break;				
            case 108: // Course (GPRMC)
				_new_course = _decimal( _term );
                break;
            case 109: // Date (GPRMC)

				gps_data.datetime.date[8] = _term[0];
				gps_data.datetime.date[9] = _term[1];

				gps_data.datetime.date[5] = _term[2];
				gps_data.datetime.date[6] = _term[3];

				gps_data.datetime.date[2] = _term[4];
				gps_data.datetime.date[3] = _term[5];				
                break;
            case 206: // Fix data (GPGGA)
                _gps_data_good = _term[0] > '0';
                break;			
            case 209: // Altitude (GPGGA)			
				_new_altitude = _decimal( _term );
                break;
				
			case 302: //1-origin number of this GSV message  within current group
                _sat_pos = (_decimal( _term ) - 1) * 4;							
                break;
			//Example:           4         7  8        11 12        15 16        19
			case 304: //satellite PRN number
				if (_term[0] != 0)
					gps_data.sat_inview[_sat_pos].prn = _decimal( _term );
					break;
			case 305: //elevation in degrees
				if (_term[0] != 0)
					gps_data.sat_inview[_sat_pos].ele = _decimal( _term );                
                break;
			case 306: //azimuth in degrees to true north
				if (_term[0] != 0)
					gps_data.sat_inview[_sat_pos].azi = _decimal( _term );		
				break;					
			case 307: //SNR in dB (00-99)
				if (_term[0] != 0)
					gps_data.sat_inview[_sat_pos].snr = _decimal( _term );
                break;
				
			case 308: //satellite PRN number
				if (_term[0] != 0)
					gps_data.sat_inview[_sat_pos + 1].prn = _decimal( _term );
                
                break;
			case 309: //elevation in degrees
				if (_term[0] != 0)
					gps_data.sat_inview[_sat_pos + 1].ele = _decimal( _term );                
                break;
			case 310: //azimuth in degrees to true north
				if (_term[0] != 0)
					gps_data.sat_inview[_sat_pos + 1].azi = _decimal( _term );		
				break;
			case 311: //SNR in dB (00-99)
				if (_term[0] != 0)
					gps_data.sat_inview[_sat_pos + 1].snr = _decimal( _term );
                break;
				
			case 312: //satellite PRN number
				if (_term[0] != 0)
					gps_data.sat_inview[_sat_pos + 2].prn = _decimal( _term );
                
                break;
			case 313: //elevation in degrees
				if (_term[0] != 0)
					gps_data.sat_inview[_sat_pos + 2].ele = _decimal( _term );                
                break;
			case 314: //azimuth in degrees to true north
				if (_term[0] != 0)
					gps_data.sat_inview[_sat_pos + 2].azi = _decimal( _term );		
				break;
			case 315: //SNR in dB (00-99)
				if (_term[0] != 0)
					gps_data.sat_inview[_sat_pos + 2].snr = _decimal( _term );
                break;
				
			case 316: //satellite PRN number
				if (_term[0] != 0)
					gps_data.sat_inview[_sat_pos + 3].prn = _decimal( _term );
                
                break;
			case 317: //elevation in degrees
				if (_term[0] != 0)
					gps_data.sat_inview[_sat_pos + 3].ele = _decimal( _term );                
                break;
			case 318: //azimuth in degrees to true north
				if (_term[0] != 0)
					gps_data.sat_inview[_sat_pos + 3].azi = _decimal( _term );		
				break;
			case 319: //SNR in dB (00-99)
				if (_term[0] != 0)
					gps_data.sat_inview[_sat_pos + 3].snr = _decimal( _term );
                break;
        }
		
	}
	
    return FALSE;
}

bool EncodeNMEA(char c) {
    bool valid_sentence = FALSE;

    //++_encoded_characters;
    switch (c) {
        case ',': // term terminators
            _parity ^= c;
        case '\r':
        case '\n':
        case '*':
            if (_term_offset < sizeof(_term)) {
                _term[_term_offset] = 0;						
                valid_sentence = TermComplete();				
            }
            ++_term_number;
						_term[0] = '\0';
            _term_offset = 0;
            _is_checksum_term = c == '*';
            return valid_sentence;

        case '$': // sentence begin
            _term_number = _term_offset = 0;
            _parity = 0;
            _sentence_type = _GPS_SENTENCE_OTHER;
            _is_checksum_term = FALSE;
            _gps_data_good = FALSE;
			_term[0] = '\0';
            return valid_sentence;
    }

    // ordinary characters
    if (_term_offset < sizeof(_term) - 1)
        _term[_term_offset++] = c;
    if (!_is_checksum_term)
        _parity ^= c;

    return valid_sentence;
}

//
//
//


void get_gps_data(gps_data_t * external_gps_data)
{
   *external_gps_data = gps_data;
   return;
}
