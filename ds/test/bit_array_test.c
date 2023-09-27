/********************
Developer: Avri Kehat
Reviewed by: Yossi Matzliah
Date: 31.01.23
*********************/

#include<stdio.h> /*printf*/

#include "bit_array.h"

enum bit_set {OFF, SET};

static void TestBitArray(void);

int main(void)
{
	TestBitArray();
	
	return 0;
}

static void TestBitArray(void)
{
	size_t lut_bit_array[BITS_IN_SIZE_T] = {0};
	
	bit_array_t bit = 0;
	char dest[BITS_IN_SIZE_T+1] = {0};
	int index = 2;
	size_t i = 0;
	
	bit = 251;
	
	printf("\n%ld with set bit on at index %d is %ld\n\n", bit, index, BitArraySetOn(bit, index)); 
	
	index = 1;
	
	printf("%ld with set bit off at index %d is %ld\n\n", bit, index, BitArraySetOff(bit, index));
	
	printf("%ld with set index 63 to SET is:\n%s\n\n", bit, BitArrayToString(BitArraySetVal(bit, 63, SET), dest));
																					
	bit = 34359738367;

	printf("%ld with set index 25 to OFF is:\n%s\n\n", bit, BitArrayToString(BitArraySetVal(bit, 25, OFF), dest));
	
	printf("Count Set Bits with LUT: %ld\n\n", BitArrayCountOnWithLUT(bit, lut_bit_array));
	
	printf("%ld with BitArrayResetAll is:\n%s\n\n", bit, BitArrayToString(BitArrayResetAll(bit), dest));
	
	bit = 256;
		
	printf("%ld with BitArraySetAll is:\n%s\n\n", bit, BitArrayToString(BitArraySetAll(bit), dest));
	
	bit = 247;
	
	printf("%ld Before Flip of index 3 is:\n%s\n\n", bit, BitArrayToString(bit, dest));
	
	printf("%ld After Flip of index 6 is:\n%s\n\n", bit, BitArrayToString(BitArrayFlip(bit, 6), dest));
	
	printf("%ld After Flip of index 15 is:\n%s\n\n", bit, BitArrayToString(BitArrayFlip(bit, 15), dest));
	
	printf("%ld Mirror is:\n%s\n\n", bit, BitArrayToString(BitArrayMirror(bit), dest));
	
	printf("Miror with LUT\n");
	BitArrayMirrorLUT(bit, lut_bit_array);
	
	for(i = 0; i < BITS_IN_SIZE_T; ++i)
	{
		printf("%ld", lut_bit_array[i]);
	}
	printf("\n");
	
	printf("\n%d Rotate Left 58 steps:\n%s\n\n",255 , BitArrayToString(BitArrayRotateLeft(255, 60), dest));
	
	printf("%d Rotate Right 3 steps:\n%s\n\n",255 , BitArrayToString(BitArrayRotateRight(255, 3), dest));
		
	printf("Bit at index 4 of %ld is: %d\n\n", bit, BitArrayGet(bit, 4));
	
	printf("%ld: Bits on: %ld\n\n",bit ,BitArrayCountOn(bit));
	
	printf("%ld: Bits off: %ld\n\n",bit ,BitArrayCountOff(bit));
	
	printf("Test BitArray to string:\n%s\n", BitArrayToString(bit, dest));
}
