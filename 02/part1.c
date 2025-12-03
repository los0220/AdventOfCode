#include <stdio.h>
#include <string.h>
#include <stdbool.h>

int isInvalid(long value);
int readRange(FILE* fptr, long* lower, long* upper);


int main(int argc, char **argv)
{
	bool verbose = false;
	char VERBOSE_FLAG[] = "-v";
	char* fileName = NULL;

	if (argc == 2)
	{
		fileName = argv[1];
	}
	else if (argc == 3)
	{
		fileName = argv[2];

		if (strcmp(argv[1], VERBOSE_FLAG) == 0)
			verbose = true;
	}
	else
	{
		return -1;	
	}

	FILE* fptr = fopen(fileName, "r");

	long first, last, i;
	long sum = 0;
	
	while (fptr != NULL && !feof(fptr) )
	{
		if (readRange(fptr, &first, &last) <= 0)
			break;

		if (verbose)
			printf("%ld - %ld:", first, last);

		for (i = first; i <= last; i++)
		{
			if (isInvalid(i) == 1)
			{
				sum += i;
				if (verbose)
					printf(" %ld", i);
			}
		} 

		if (verbose)
			printf("\n");
	}

	fclose(fptr);

	printf("%ld\n", sum);

	return 0;
}

int isInvalid(long value)
{
	const int BUFFER_SIZE = 32;
	char buffer[BUFFER_SIZE];
	int len;

	len = snprintf(buffer, BUFFER_SIZE, "%ld", value);

	if (len <= 0)
		return -1;

	if (len % 2 != 0)
		return 0;

	for (int i = 0; i < len / 2; i++)
	{
		if (buffer[i] != buffer[i + len / 2])
			return 0;
	}
	
	return 1;
}

int readRange(FILE* fptr, long* first, long* last)
{
	const char VALUES_SEPARATOR = '-';

	int count = 0;
	long* value = first;
	char c;

	*first = 0;
	*last = 0;

	while (fptr != NULL && (c = fgetc(fptr)) != EOF)
	{
		if ('0' <= c && c <= '9')
		{
			*value *= 10;
			*value += (long)(c - '0');

			count++;
		}
		else if (c == VALUES_SEPARATOR)
		{
			value = last;
		}
		else 
		{
			break;
		}
	}

	return count;
}
