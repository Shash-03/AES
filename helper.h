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

void HexaArrayToByteArray2(unsigned char key[24],char hexaKey[49] );

void ByteArrayToByteMatrix2(unsigned char keyMatrix[4][6],unsigned char key[24]);

void ByteMatrixToByteArray2(unsigned char key[24],unsigned char keyMatrix[4][6]);

void ByteArrayToHexaArray2(char hexaKey[49],unsigned char key[24]);

void HexaArrayToByteArray3(unsigned char key[32],char hexaKey[65]);

void ByteArrayToByteMatrix3(unsigned char keyMatrix[4][8],unsigned char key[32]);

void ByteMatrixToByteArray3( unsigned char keyMatrix[4][8],unsigned char key[32]);

void ByteArrayToHexaArray3(unsigned char key[32],char hexaKey[65]);

#endif