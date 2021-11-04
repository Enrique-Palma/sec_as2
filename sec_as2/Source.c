#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void argumentsCheck(int argc);
int myAtoi(char* str);
char* my_strcat(char* strg1, char* strg2);
int checkStringLength(const char* s);
void paddingMax80Chars(char* str);
unsigned long int calc8bit(char* input);
unsigned long int calc16bit(char* input);
unsigned long int calc32bit(char* input);

int main(int argc, char** argv)
{
	FILE* fileOpen_ptr;
	char* textFile;
	char c = 'x';
	unsigned long int res8bit = 0;
	unsigned long int res16bit = 0;
	unsigned long int res32bit = 0;
	int i = 0;
	int checksum_size = 0;

	//function call for arguments checker
	argumentsCheck(argc);

	// Make sure checksum size is correct
	checksum_size = myAtoi(argv[2]);
	if (!(checksum_size == 8 || checksum_size == 16 || checksum_size == 32))
	{
		fprintf(stderr, "Valid checksum sizes are 8, 16, or 32\n");
		return -1;
	}

	// Open the input file
	fileOpen_ptr = fopen(argv[1], "r");
	if (fileOpen_ptr == NULL)
	{
		fprintf(stdout, "Cannot connect to the specified files.");
		exit(0);
	}

	// Read in the input file
	textFile = calloc(sizeof(textFile), 1024);
	while ((c = getc(fileOpen_ptr)) != EOF)
	{
		textFile[i] = c;
		i++;
	}
	textFile[i] = '\0';

	// Close the file
	fclose(fileOpen_ptr);

	if (checksum_size == 8)
	{
		res8bit = calc8bit(textFile);
		paddingMax80Chars(textFile);
		printf("\n");
		// Use 0xff masking to display only last 2 hex values
		printf("%2d bit checksum is %8lx for all %4d chars\n", checksum_size, res8bit & 0xff, (int)checkStringLength(textFile));
	}
	else if (checksum_size == 16)
	{
		// Pad with a necessary X
		if (checkStringLength(textFile) % 2)
		{
			my_strcat(textFile, "X");
		}
		paddingMax80Chars(textFile);
		printf("\n");
		res16bit = calc16bit(textFile);
		// Use 0xffff masking to display only last 4 hex values
		printf("%2d bit checksum is %8lx for all %4d chars\n", checksum_size, res16bit & 0xffff, (int)checkStringLength(textFile));
	}
	else if (checksum_size)
	{
		// Pad with necessary X's
		while (checkStringLength(textFile) % 4)
		{
			my_strcat(textFile, "X");
		}
		paddingMax80Chars(textFile);
		printf("\n");
		res32bit = calc32bit(textFile);
		// Use 0xffffffff masking to display only last 8 hex values
		printf("%2d bit checksum is %8lx for all %4d chars\n", checksum_size, res32bit & 0xffffffff, (int)checkStringLength(textFile));
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
	if (argc != 3)
	{
		fprintf(stdout, "To run this program Please run as `checksum.c filename.txt 8/16/32`\n");
		exit(0);
	}
}


// A simple atoi() function
int myAtoi(char* str)
{
	int res = 0;
	for (int i = 0; str[i] != '\0'; ++i)
	{
		res = res * 10 + str[i] - '0';
	}
	return res;
}

//my strcat function
char* my_strcat(char* strg1, char* strg2)
{
	char* start = strg1;
	while (*strg1 != '\0')
	{
		strg1++;
	}

	while (*strg2 != '\0')
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
	while (*s != '\0')
	{
		count++;
		s++;
	}
	return count;
}

// Function that prints 80 characters per line
void paddingMax80Chars(char* str)
{
	int i = 0;
	while (str[i] != 0)
	{
		if (i % 80 == 0)
		{
			printf("\n");
		}
		printf("%c", str[i]);
		i++;
	}
}

// Calculate 8 bit checksum from the input string
unsigned long int calc8bit(char* input)
{
	int result = 0;
	for (int i = 0; i < checkStringLength(input); i++)
	{
		result += input[i];
	}
	return result;
}

// Calculate 16 bit checksum from the input string
unsigned long int calc16bit(char* input)
{
	int res16bit = 0;
	for (int i = 0; i < checkStringLength(input);)
	{
		res16bit += input[i] << 8;
		res16bit += (input[i + 1]);
		i += 2;
	}
	return res16bit;
}

// Calculate 32 bit checksum from the input string
unsigned long int calc32bit(char* input)
{
	unsigned long int res32bit = 0;
	for (int i = 0; i < checkStringLength(input);)
	{
		res32bit += input[i] << 24;
		res32bit += (input[i + 1]) << 16;
		res32bit += (input[i + 2]) << 8;
		res32bit += (input[i + 3]);
		i += 4;
	}

	return res32bit;
}