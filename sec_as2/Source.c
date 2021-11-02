#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

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
unsigned int checkStringLength(const char* s)
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
void print80(char* str)
{
	for (int i = 0; i < checkStringLength(str); i++)
	{
		if (i % 80 == 0)
			printf("\n");
		printf("%c", str[i]);
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

int main(int argc, char** argv)
{
	FILE* inputfile;
	char* input, * output, * checksum, c = 'x';
	unsigned long int res8bit = 0;
	unsigned long int res16bit = 0;
	unsigned long int res32bit = 0;
	int i = 0;
	int checksum_size = 0;

	// Wrong number of arguments passed
	if (argc != 3)
	{
		fprintf(stderr, "Please run as `checksum.c filename.txt 8/16/32`\n");
		return -1;
	}

	// Make sure checksum size is correct
	checksum_size = atoi(argv[2]);
	if (!(checksum_size == 8 || checksum_size == 16 || checksum_size == 32))
	{
		fprintf(stderr, "Valid checksum sizes are 8, 16, or 32\n");
		return -1;
	}

	// Open the input file	

	inputfile = fopen(argv[1], "r");

	if (inputfile == NULL)
	{
		fprintf(stdout, "Cannot connect to the specified files.");
		exit(0);
	}

	// Read in the input file
	input = malloc(sizeof(char) * 1024);
	while (fscanf(inputfile, "%c", &c) != EOF && i < 1024)
	{
		input[i] = c;
		i++;
	}
	input[i] = '\0';

	// Close the file
	fclose(inputfile);

	switch (checksum_size)
	{
		case 8:
			res8bit = calc8bit(input);
			print80(input);
			printf("\n");
			// Use 0xff masking to display only last 2 hex values
			printf("%2d bit checksum is %8lx for all %4d chars\n", checksum_size, res8bit & 0xff, (int)checkStringLength(input));
			break;
		case 16:
			// Pad with a necessary X
			if (checkStringLength(input) % 2)
				my_strcat(input, "X");
			
			print80(input);
			printf("\n");
			res16bit = calc16bit(input);
			// Use 0xffff masking to display only last 4 hex values
			printf("%2d bit checksum is %8lx for all %4d chars\n", checksum_size, res16bit & 0xffff, (int)checkStringLength(input));
			break;
		case 32:
			// Pad with necessary X's
			while (checkStringLength(input) % 4)
				my_strcat(input, "X");
			
			print80(input);
			printf("\n");
			res32bit = calc32bit(input);
			// Use 0xffffffff masking to display only last 8 hex values
			printf("%2d bit checksum is %8lx for all %4d chars\n", checksum_size, res32bit & 0xffffffff, (int)checkStringLength(input));
			break;
	}

	return 0;
}

///commentsadsadad asdas dasd