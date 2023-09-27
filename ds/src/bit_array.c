/********************
Developer: Avri Kehat
Reviewed by: Yossi Matzliah
Date: 31.01.23
*********************/

#include <assert.h> /*assert*/  
#include "bit_array.h"

#define MAX_SET_SIZE_T (0xffffffffffffffff)
#define ONE_IN_64_BIT ((size_t)1)


bit_array_t BitArraySetOn(bit_array_t bitarr, size_t index)
{
	return (bitarr | ONE_IN_64_BIT << index);
}

bit_array_t BitArraySetOff(bit_array_t bitarr, size_t index)
{
	return ~(~bitarr | ONE_IN_64_BIT << index);
}

bit_array_t BitArraySetVal(bit_array_t bitarr, size_t index, int bool_value)
{
	return ((bool_value) ? (BitArraySetOn(bitarr, index)) : (BitArraySetOff(bitarr, index)));
}

bit_array_t BitArraySetAll(bit_array_t bitarr)
{
	return (bitarr | MAX_SET_SIZE_T);				
}

bit_array_t BitArrayResetAll(bit_array_t bitarr)
{
	return (bitarr & ~bitarr);
}

bit_array_t BitArrayFlip(bit_array_t bitarr, size_t index)
{
	return (bitarr ^ (ONE_IN_64_BIT << index));
}

bit_array_t BitArrayMirror(bit_array_t bitarr)
{
	size_t mirror = 0;								
	size_t i = 0;
	
	for(i = 0; BITS_IN_SIZE_T > i; ++i) 			
	{
		mirror <<= 1;
		if(bitarr & 1)
		{
			++mirror;								
		}
		bitarr >>= 1; 								
	}
	return mirror;
}

void BitArrayMirrorLUT(bit_array_t bitarr, size_t lut_bit_array[])
{
	size_t i = 0;
	
	for(i = 0; BITS_IN_SIZE_T > i; ++i) 			
	{
		lut_bit_array[i] = BitArrayGet(bitarr, i);
	}
	return;
}

bit_array_t BitArrayRotateLeft(bit_array_t bitarr, size_t step)
{
	return bitarr << step | (bitarr >> (BITS_IN_SIZE_T - step));
}

bit_array_t BitArrayRotateRight(bit_array_t bitarr, size_t step)
{
	return BitArrayRotateLeft(bitarr, -step);
}

size_t BitArrayCountOn(bit_array_t bitarr)
{
	size_t count = 0;

	while(0 != bitarr)
	{
		bitarr = bitarr & (bitarr - 1);
		++count;
	}
	
	return count;
}

size_t BitArrayCountOnWithLUT(bit_array_t bitarr, size_t lut_bit_array[])
{
	size_t count = 0;
	size_t i = 0;
	
	for(i = 0; BITS_IN_SIZE_T > i; ++i) 			
	{
		if (lut_bit_array[i] != (size_t) BitArrayGet(bitarr, i))
		{
			++count;
		}
	}
	return count;
}

size_t BitArrayCountOff(bit_array_t bitarr)
{
	return BITS_IN_SIZE_T - (BitArrayCountOn(bitarr));
}

int BitArrayGet(bit_array_t bitarr, size_t index)
{
	return (bitarr >> index) & 1;
}

char *BitArrayToString(bit_array_t bitarr, char *dest)
{
	size_t i = 0;  																									
	
	assert(NULL != dest);
	
	for (i = 0; BITS_IN_SIZE_T > i; ++i)
	{
		dest[i] = (char) (BitArrayGet(bitarr, BITS_IN_SIZE_T - i - 1) + '0');
	}
	dest[BITS_IN_SIZE_T] = '\0';
	return dest;
}
