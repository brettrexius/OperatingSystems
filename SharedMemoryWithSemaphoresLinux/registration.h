/*	Header file to be used with
 *	shmp1.c and shmc1.c
 */

struct CLASS { //initiate a struct "CLASS"
	char class_number[6]; // declare char array "class_number" size 6
	char date[7]; // declare char array "date" size 7
	char title[50]; // declare char array "title" size 50
	int seats_left; // declare int "seats_left"
};
