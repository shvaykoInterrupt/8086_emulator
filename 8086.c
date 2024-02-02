#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>


char* readEntireFile(const char *src)
{
	FILE* file = fopen(src, "rb");
	if (!file)
	{
		fprintf(stderr, "Cound't open file!\n");
		return 0;
	}

	fseek(file,0,SEEK_END);
	size_t fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);
	char* buffer = (char*)malloc(fileSize + 1);
	if (!buffer) exit(64);
	size_t elementsRead = fread(buffer, 1, fileSize, file);
	if (elementsRead != fileSize)
	{
		free(buffer);
		fprintf(stderr,"Have not read entire file\n");
		return 0;
	}

	fclose(file);
	buffer[fileSize] = '\0';
	return buffer;
}

void usage()
{
	printf("Usage: 8086.exe arg1");
}

enum
{
	OP_MOV = 0b100010
};

static uint16_t ip;
#define REGISTER_COUNT 8

#if 0
typedef struct
{
	union
	{
		uint16_t reg;
		
		struct
		{
			uint8_t highReg;
			uint8_t lowReg;
		};
	};
}Register;
#endif

char *regs[8][2] =
{
	{"AL", "AX"},
	{"CL", "CX"},
	{"DL", "DX"},
	{"BL", "BX"},
	{"AH", "SP"},
	{"CH", "BP"},
	{"DH", "SI"},
	{"BH", "DI"},
};

typedef enum
{
	MM_ND, // no displacement
	MM_8_BIT,
	MM_16_BIT,
	MM_RM
}Modes;

void decode(char *stream)
{
	uint16_t instruction = (stream[ip] << 8) | (stream[ip+1] & 0xff);

	switch (instruction >> 10)
	{
		case OP_MOV:
		{
			char destinationRegister = (instruction >> 9) & 0x1;
			char wideRegisters = (instruction >> 8) & 0x1;

			uint16_t mode = (instruction & 0xff) >> 6;
			uint8_t register1 = (instruction >> 3) & 0x7;
			uint8_t register2 = instruction & 0x7;
			if (mode == MM_RM)
			{
				//
			}

			printf("MOV %s, %s\n", regs[register2][wideRegisters], regs[register1][wideRegisters]);
			ip += 2;
		}break;
		default:
		{
			fprintf(stderr, "Unkown operation!\n");
		}break;
	}

	int foo = 5;
}


int main(int argc, char** argv)
{
	if (argc != 2)
	{
		usage();
		exit(61);
	}
	char* content = readEntireFile(argv[1]);
	if(content)
	{
		while(content[ip] != '\0')
		{
			decode(content);
		}
		
		free(content);
	}

	return 0;
}