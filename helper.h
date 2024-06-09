#ifndef HELPER_H
#define HELPER_H

extern unsigned char sBox[256];
extern unsigned char rc[10];
extern unsigned char inverseSBox[256];

int HexaToInt(char hexa);

char IntToHexa(int number);

char* ByteArrayToHexaArray(unsigned char* byte, char* hexa);

void HexaArrayToByteArray(unsigned char* byte, char* hexa);

void ByteArrayToByteMatrix(unsigned char matrix[4][4],unsigned char* byte);

void ByteMatrixToByteArray(unsigned char matrix[4][4],unsigned char* byte);

#endif
