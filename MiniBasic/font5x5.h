//	
//		font5x5.h
//      
//      Copyright 2012 joaquim <mail@joaquim.org>
//      
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 2 of the License, or
//      (at your option) any later version.
//      
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//      
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, write to the Free Software
//      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
//      MA 02110-1301, USA.



// Font 5x5 ( largura da fonte não é fixa )
extern const u8 Font5x5[289] = {
0, 				// Code for char Espaço   
23, 			// Code for char !  
3,0,3, 			// Code for char "  
10,31,10,31,10, // Code for char #  
4,14,27,10, 	// Code for char $  
9,4,2,9, 		// Code for char %  
10,21,17, 		// Code for char &  
3,4, 			// Code for char '  
14,27,17, 		// Code for char (  
17,27,14, 		// Code for char )  
21,14,14,21, 	// Code for char *  
4,14,4, 		// Code for char +  
16,8, 			// Code for char ,  
4,4,4, 			// Code for char -  
16, 			// Code for char .  
16,12,3, 		// Code for char "/"  
31,17,31, 		// Code for char 0  
18,31,16, 		// Code for char 1  
25,21,18, 		// Code for char 2  
17,21,14, 		// Code for char 3  
14,8,30, 		// Code for char 4  
23,21,9, 		// Code for char 5  
31,21,29, 		// Code for char 6  
18,10,6, 		// Code for char 7  
10,21,10, 		// Code for char 8  
23,21,31, 		// Code for char 9  
10, 			// Code for char :  
16,10, 			// Code for char ;  
4,10,17, 		// Code for char <  
10,10,10, 		// Code for char =  
17,10,4, 		// Code for char >  
1,21,2, 		// Code for char ?  
14,19,22, 		// Code for char @  
30,9,30, 		// Code for char A  
31,21,10, 		// Code for char B  
14,17,17, 		// Code for char C  
31,17,14, 		// Code for char D  
31,21,21, 		// Code for char E  
31,5,5, 		// Code for char F  
14,17,29, 		// Code for char G  
31,4,31, 		// Code for char H  
17,31,17, 		// Code for char I  
24,16,31, 		// Code for char J  
31,4,27, 		// Code for char K  
31,16,16, 		// Code for char L  
31,2,4,2,31, 	// Code for char M  
31,1,30, 		// Code for char N  
14,17,14, 		// Code for char O  
31,5,2, 		// Code for char P  
31,17,31,16, 	// Code for char Q  
31,5,26, 		// Code for char R  
18,21,9, 		// Code for char S  
1,31,1, 		// Code for char T  
15,16,15, 		// Code for char U  
31,8,7, 		// Code for char V  
15,16,12,16,15, // Code for char W  
27,4,27, 		// Code for char X  
3,28,3, 		// Code for char Y  
25,21,19, 		// Code for char Z  
31,17,17, 		// Code for char [  
1,6,24, 		// Code for char "\"  
17,17,31, 		// Code for char ]  
2,1,2, 			// Code for char ^  
16,16,16, 		// Code for char _  
1,2, 			// Code for char `  
8,26,28, 		// Code for char a  
31,20,8, 		// Code for char b  
12,18,18, 		// Code for char c  
8,20,31, 		// Code for char d  
12,22,20, 		// Code for char e  
30,5,1, 		// Code for char f  
20,26,12, 		// Code for char g  
31,4,24, 		// Code for char h  
29, 			// Code for char i  
16,16,13, 		// Code for char j  
31,8,20, 		// Code for char k  
17,31,16, 		// Code for char l  
30,2,12,2,28, 	// Code for char m  
30,2,28, 		// Code for char n  
12,18,12, 		// Code for char o  
30,10,4, 		// Code for char p  
4,10,30, 		// Code for char q  
30,4,2, 		// Code for char r  
20,22,10, 		// Code for char s  
4,30,4, 		// Code for char t  
14,16,14, 		// Code for char u  
30,8,6, 		// Code for char v  
14,16,12,16,14, // Code for char w  
18,12,18, 		// Code for char x  
22,20,14, 		// Code for char y  
26,22,22, 		// Code for char z  
4,14,17, 		// Code for char {  
27, 			// Code for char |  
17,14,4, 		// Code for char }  
1,2,1,2, 		// Code for char ~  
14,14,14,14 	// Code for char " "
};

// Char position and char size table
//
// CharSize = Font5x5WidthPos["2"] - Font5x5WidthPos["2" - 1]
// CharPos  = Font5x5WidthPos["2" - 1]
//
extern const u16 Font5x5WidthPos[96] = {
1,2,5,10,14,18,21,23,26,29,33,36,38,41,
42,45,48,51,54,57,60,63,66,69,72,75,76,
78,81,84,87,90,93,96,99,102,105,108,111,
114,117,120,123,126,129,134,137,140,143,
147,150,153,156,159,162,167,170,173,176,
179,182,185,188,191,193,196,199,202,205,
208,211,214,217,218,221,224,227,232,235,
238,241,244,247,250,253,256,259,264,267,
270,273,276,277,280,284,289
};
