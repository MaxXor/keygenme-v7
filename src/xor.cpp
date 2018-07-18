#include "program.h"
#include "xor.h"

void decrypt(WCHAR* input, int key, int length)
{
	for (int i = 0; i < length; i++)
		input[i] = input[i] ^ (key + i) % 255;
}