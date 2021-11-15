/*=============================================================================
| Assignment: pa02 - Calculating an 8, 16, or 32 bit
| checksum on an ASCII input file
|
| Author: Enrique Palma
| Language: c
|
| To Compile: javac pa02.java
| gcc -o pa02 pa02.c
| g++ -o pa02 pa02.cpp
|
| To Execute: java -> java pa02 inputFile.txt 8
| or c++ -> ./pa02 inputFile.txt 8
| or c -> ./pa02 inputFile.txt 8
| where inputFile.txt is an ASCII input file
| and the number 8 could also be 16 or 32
| which are the valid checksum sizes, all
| other values are rejected with an error message
| and program termination
|
| Note: All input files are simple 8 bit ASCII input
|
| Class: CIS3360 - Security in Computing - Fall 2021
| Instructor: McAlpin
| Due Date: 11/22/2021
|
+=============================================================================*/

#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define MAX 1024

//Function prototypes
void argumentsCheck(int argc);
int stringToInteger(char* str);
char* stringAppend(char* strg1, char* strg2);
int checkStringLength(const char* s);
void paddingMax80Chars(char* str);
long int eightBits(char* user_input8);
long int sixteenBits(char* user_input16);
long int thirtyTwoBits(char* user_input32);

int main(int argc, char** argv)
{
	FILE* fileOpen_ptr;
	char* textFile;
	char c = 'x';
	long int main8bit = 0;
	long int main16bit = 0;
	long int main32bit = 0;
	int i = 0;
	int checksumSizeCheck = 0;

	//function call for arguments checker
	argumentsCheck(argc);

	// checksum check
	checksumSizeCheck = stringToInteger(argv[2]);

	if(checksumSizeCheck != 8 && checksumSizeCheck != 16 && checksumSizeCheck != 32)
	{
		fprintf(stdout, "Correct checksum sizes are 8, 16, or 32\n");
		exit(0);
	}

	// Open the input file
	fileOpen_ptr = fopen(argv[1], "r");
	if(fileOpen_ptr == NULL)
	{
		fprintf(stdout, "Cannot connect to the specified files.\n");
		exit(0);
	}

	//input file
	textFile = calloc(sizeof(textFile), MAX);
	while((c = getc(fileOpen_ptr)) != EOF)
	{
		textFile[i] = c;
		i++;
	}
	textFile[i] = '\0';

	// Close file
	fclose(fileOpen_ptr);

	if(checksumSizeCheck == 8)
	{
		main8bit = eightBits(textFile);
		paddingMax80Chars(textFile);
		printf("\n");
		// Use 0xff masking to display only last 2 hex values
		printf("%2d bit checksum is %8lx for all %4d chars\n", checksumSizeCheck, main8bit & 0xff, checkStringLength(textFile));
	}
	else if(checksumSizeCheck == 16)
	{
		// Pad with X
		if(checkStringLength(textFile) % 2)
		{
			stringAppend(textFile, "X");
		}
		paddingMax80Chars(textFile);
		printf("\n");
		main16bit = sixteenBits(textFile);
		//  0xffff - only last 4 hex 
		printf("%2d bit checksum is %8lx for all %4d chars\n", checksumSizeCheck, main16bit & 0xffff, checkStringLength(textFile));
	}
	else if(checksumSizeCheck)
	{
		// Pad with X
		while (checkStringLength(textFile) % 4)
		{
			stringAppend(textFile, "X");
		}
		paddingMax80Chars(textFile);
		printf("\n");
		main32bit = thirtyTwoBits(textFile);
		//  0xffffffff - only last 8 hex 
		printf("%2d bit checksum is %8lx for all %4d chars\n", checksumSizeCheck, main32bit & 0xffffffff, checkStringLength(textFile));
	}
	else
	{
		fprintf(stdout, "error");
	}

	free(textFile);
	return 0;
}

// Checking for the correct arguments to run the program correctly
void argumentsCheck(int argc)
{
	if(argc != 3)
	{
		fprintf(stdout, "To Run Program ->  `checksum.c filename.txt 8/16/32`\n");
		exit(0);
	}
}

// stringToInteger function
int stringToInteger(char* str)
{
	int res = 0;
	for(int i = 0; str[i] != '\0'; ++i)
	{
		res = res * 10 + str[i] - '0';
	}
	return res;
}

//stringAppend function
char* stringAppend(char* strg1, char* strg2)
{
	char* start = strg1;
	while(*strg1 != '\0')
	{
		strg1++;
	}
	while(*strg2 != '\0')
	{
		*strg1 = *strg2;
		strg1++;
		strg2++;
	}
	*strg1 = '\0';
	return start;
}

//checkStringLength function
int checkStringLength(const char* s)
{
	unsigned int count = 0;
	while(*s != '\0')
	{
		count++;
		s++;
	}
	return count;
}

// only 80 characters per line
void paddingMax80Chars(char* str)
{
	int i = 0;
	while(str[i] != 0)
	{
		if(i % 80 == 0)
		{
			printf("\n");
		}
		printf("%c", str[i]);
		i++;
	}
}

// 8 bit checksum calcualtion
long int eightBits(char* user_input8)
{
	int result8bit = 0;
	int i = 0;
	while(i < checkStringLength(user_input8))
	{
		result8bit = result8bit + user_input8[i];
		i++;
	}
	return result8bit;
}

// 16 bit checksum calcualtion
long int sixteenBits(char* user_input16)
{
	int result16bit = 0;
	int i = 0;
	while(i < checkStringLength(user_input16))
	{
		result16bit = result16bit + (user_input16[i] << 8);
		result16bit = result16bit + (user_input16[i + 1]);
		i += 2;
	}	
	return result16bit;
}

// 32 bit checksum calcualtion
long int thirtyTwoBits(char* user_input32)
{
	long int result32bit = 0;
	int i = 0;
	while(i < checkStringLength(user_input32))
		{
			result32bit = result32bit + (user_input32[i] << 24);
			result32bit = result32bit + ((user_input32[i + 1]) << 16);
			result32bit = result32bit + ((user_input32[i + 2]) << 8);
			result32bit = result32bit + (user_input32[i + 3]);
			i = i + 4;
		}
	return result32bit;
}

/*=============================================================================
| I [Enrique Palma] ([en769144]) affirm that this program is
| entirely my own work and that I have neither developed my code together with
| any another person, nor copied any code from any other person, nor permitted
| my code to be copied or otherwise used by any other person, nor have I
| copied, modified, or otherwise used programs created by others. I acknowledge
| that any violation of the above terms will be treated as academic dishonesty.
+=============================================================================*/