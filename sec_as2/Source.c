#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define MAX 1024

void argumentsCheck(int argc);
int stringToInteger(char* str);
char* stringAppend(char* strg1, char* strg2);
int checkStringLength(const char* s);
void paddingMax80Chars(char* str);
long int calc8bit(char* input);
long int calc16bit(char* input);
long int calc32bit(char* input);

int main(int argc, char** argv)
{
	FILE* fileOpen_ptr;
	char* textFile;
	char c = 'x';
	long int res8bit = 0;
	long int res16bit = 0;
	long int res32bit = 0;
	int i = 0;
	int checksum_size = 0;

	//function call for arguments checker
	argumentsCheck(argc);

	// Make sure checksum size is correct
	checksum_size = stringToInteger(argv[2]);
	if(!(checksum_size == 8 || checksum_size == 16 || checksum_size == 32))
	{
		fprintf(stderr, "Valid checksum sizes are 8, 16, or 32\n");
		return -1;
	}

	// Open the input file
	fileOpen_ptr = fopen(argv[1], "r");
	if(fileOpen_ptr == NULL)
	{
		fprintf(stdout, "Cannot connect to the specified files.");
		exit(0);
	}

	// Read in the input file
	textFile = calloc(sizeof(textFile), MAX);
	while((c = getc(fileOpen_ptr)) != EOF)
	{
		textFile[i] = c;
		i++;
	}
	textFile[i] = '\0';

	// Close the file
	fclose(fileOpen_ptr);

	if(checksum_size == 8)
	{
		res8bit = calc8bit(textFile);
		paddingMax80Chars(textFile);
		printf("\n");
		// Use 0xff masking to display only last 2 hex values
		printf("%2d bit checksum is %8lx for all %4d chars\n", checksum_size, res8bit & 0xff, checkStringLength(textFile));
	}
	else if(checksum_size == 16)
	{
		// Pad with a necessary X
		if(checkStringLength(textFile) % 2)
		{
			stringAppend(textFile, "X");
		}
		paddingMax80Chars(textFile);
		printf("\n");
		res16bit = calc16bit(textFile);
		// Use 0xffff masking to display only last 4 hex values
		printf("%2d bit checksum is %8lx for all %4d chars\n", checksum_size, res16bit & 0xffff, checkStringLength(textFile));
	}
	else if(checksum_size)
	{
		// Pad with necessary X's
		while (checkStringLength(textFile) % 4)
		{
			stringAppend(textFile, "X");
		}
		paddingMax80Chars(textFile);
		printf("\n");
		res32bit = calc32bit(textFile);
		// Use 0xffffffff masking to display only last 8 hex values
		printf("%2d bit checksum is %8lx for all %4d chars\n", checksum_size, res32bit & 0xffffffff, checkStringLength(textFile));
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
		fprintf(stdout, "To run this program Please run as `checksum.c filename.txt 8/16/32`\n");
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

// 8 bit checksum from the input string
long int calc8bit(char* input)
{
	int result8bit = 0;
	int i = 0;
	while(i < checkStringLength(input))
	{
		result8bit += input[i];
		i++;
	}
	return result8bit;
}

// 16 bit checksum from the input string
long int calc16bit(char* input)
{
	int result16bit = 0;
	int i = 0;
	while(i < checkStringLength(input))
	{
		result16bit += input[i] << 8;
		result16bit += (input[i + 1]);
		i += 2;
	}	
	return result16bit;
}

// 32 bit checksum from the input string
long int calc32bit(char* input)
{
	long int result32bit = 0;
	int i = 0;
	while(i < checkStringLength(input))
		{
			result32bit += input[i] << 24;
			result32bit += (input[i + 1]) << 16;
			result32bit += (input[i + 2]) << 8;
			result32bit += (input[i + 3]);
			i += 4;
		}
	return result32bit;
}