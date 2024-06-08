#include <bits/stdc++.h>

using namespace std;

//S-box data
unsigned char sBox[256] =
{
	0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
	0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
	0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
	0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
	0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
	0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
	0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
	0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
	0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
	0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
	0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
	0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
	0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
	0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
	0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
	0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16
};

unsigned char rc[10] = {
    0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,0x1B,0x36
};


//Add Round key operation
void AddRoundKey(unsigned char primaryText[4][4],unsigned char key[4][4]){
    for (int i = 0; i < 4;i++){
        for (int j = 0; j < 4;j++){
            primaryText[j][i] ^= key[j][i];
        }
    }

}

//Sub Bytes operation
void SubBytes(unsigned char primaryText[4][4]){
    for (int i = 0; i < 4;i++){
        for (int j = 0; j < 4;j++){
            primaryText[j][i] = sBox[primaryText[j][i]];
        }
    }

}

//Shift Rows
void ShiftRows(unsigned char primaryText[4][4]){
    unsigned char shift[4][4];

    for (int i = 0; i < 4;i++){
        for (int j = 0; j < 4;j++){
            shift[i][j] = primaryText[i][(j+i)%4]; // The value in (i,j) would be the value in the original matrix
                                                    // which after left shifting by i would land on (i,j)
        }
    }

    for (int i = 0; i < 4;i++){
        for (int j = 0; j < 4;j++){
            primaryText[j][i] = shift[j][i];
        }
    }

}

//Mix Column Operation
void MixColumns(unsigned char primaryText[4][4]){
    unsigned char mul[4][4];

    for (int i = 0; i < 4; i++){
        unsigned char col[4];
        for (int j = 0; j < 4; j++){
            col[j] = primaryText[j][i];
        }

        unsigned char Times_2[4];
        for (int j = 0; j < 4; j++){
            if (col[j] & 0x80) { // If the highest bit is set, it will overflow
                Times_2[j] = (col[j] << 1) ^ 0x1b; // Left shift and XOR with 0x1b(Taking modulo x**8 + x** 4 + x**3 + x + 1)
            } else {
                Times_2[j] = col[j] << 1; // Just left shift
            }
        }

        unsigned char Times_3[4];
        for (int j = 0; j < 4; j++){
            Times_3[j] = Times_2[j] ^ col[j]; // Multiplying by 3 is equivalent to (2 * value) ^ value
        }
        /* The mix column operation involves the multiplication of 
        each column by a matrix*/
        /* The values of the matrix can be derived by multiplying 
        the polynomial 3x^3 + x^2 + x + 2 by the polynomial represented
        by a column modulo (x^4 + 1) */
        //Over GF(2**8), x^i mod (x^4 + 1) = x^(i mod 4)
        mul[0][i] = Times_2[0] ^ Times_3[1] ^ col[2] ^ col[3];
        mul[1][i] = col[0] ^ Times_2[1] ^ Times_3[2] ^ col[3];
        mul[2][i] = col[0] ^ col[1] ^ Times_2[2] ^ Times_3[3];
        mul[3][i] = Times_3[0] ^ col[1] ^ col[2] ^ Times_2[3];
    }

    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            primaryText[j][i] = mul[j][i];
        }
    }
}


//Round function
void Round(unsigned char primaryText[4][4],unsigned char key[4][4]){
    SubBytes(primaryText);
    ShiftRows(primaryText);
    MixColumns(primaryText);
    AddRoundKey(primaryText,key);
}

//Round function for the last round without the mix columns operation
void LastRound(unsigned char primaryText[4][4],unsigned char key[4][4]){
    SubBytes(primaryText);
    ShiftRows(primaryText);
    AddRoundKey(primaryText,key);  
}



int HexaToInt(char hexa){
    if (hexa >= '0' && hexa <= '9'){
        return hexa - '0';
    }

    else if (hexa >= 'a' && hexa <= 'f'){
        return hexa - 'a' + 10;
    }

    else {
        throw invalid_argument("Invalid hexadecimal character");
    }

}

char IntToHexa(int number){
    if (number >= 0 && number <= 9){
        return number + '0';
    }
    else if (number >= 10 && number <= 15){
        return (number - 10) + 'a';
    }
    else{
        throw invalid_argument("Invalid number");
    }
}

//Converting array of bytes into array of hexadecimals
char* ByteArrayToHexaArray(unsigned char* byte, char* hexa){
    for (int i = 0; i < 16;i++){
        int number = byte[i];

        int first = number / 16;
        int second = number % 16;

        char a = IntToHexa(first);
        char b = IntToHexa(second);

        hexa[2*i] = a;
        hexa[2*i+1] = b;

    }
}

//Converting array of hexadecimals to array of bytes
void HexaArrayToByteArray(unsigned char* byte, char* hexa){
    for (int i = 0; i < 16;i++){
        int first = HexaToInt(hexa[2*i]);
        int second = HexaToInt(hexa[2*i+1]);
        byte[i] = (unsigned char)((first << 4) + second);
    }
}

//Converting the 16 sized array into the 4 by 4 matrix
void ByteArrayToByteMatrix(unsigned char matrix[4][4],unsigned char* byte){
    int ind = 0;
    for (int i = 0; i < 4;i++){
        for (int j = 0; j < 4;j++){
            matrix[j][i] = byte[ind++];
        }
    }
}

//Converting the 4 by 4 matrix back to the array
void ByteMatrixToByteArray(unsigned char matrix[4][4],unsigned char* byte){
    int ind = 0;
    for (int i = 0; i < 4;i++){
        for (int j = 0;j < 4;j++){
            byte[ind++] = matrix[j][i];
        }
    }
}

void KeyExpansion(unsigned char expandedKeys[11][4][4]) {
    for (int round = 1; round <= 10; round++) {
        unsigned char col[4];

        // Get the last column from the previous round key
        for (int i = 0; i < 4; i++) {
            col[i] = expandedKeys[round - 1][i][3];
        }

        // Perform the key schedule core (rotWord and subWord)
        unsigned char temp = col[0];
        for (int i = 0; i < 3; i++) {
            col[i] = col[i + 1];
        }
        col[3] = temp;

        for (int i = 0; i < 4; i++) {
            col[i] = sBox[col[i]];
        }

        col[0] ^= rc[round - 1];

        // Compute the first column of the new round key
        for (int i = 0; i < 4; i++) {
            expandedKeys[round][i][0] = expandedKeys[round - 1][i][0] ^ col[i];
        }

        // Compute the rest of the columns of the new round key
        for (int j = 1; j < 4; j++) {
            for (int i = 0; i < 4; i++) {
                expandedKeys[round][i][j] = expandedKeys[round][i][j - 1] ^ expandedKeys[round - 1][i][j];
            }
        }
    }
}



void AESEncryption(unsigned char primaryMatrix[4][4], unsigned char expandedKeys[11][4][4]){
    AddRoundKey(primaryMatrix,expandedKeys[0]);

    for (int i = 1; i <= 9;i++){
        Round(primaryMatrix,expandedKeys[i]);
    }

    LastRound(primaryMatrix,expandedKeys[10]);
}



int main(){

    unsigned char primaryText[16];
    char hexaPrimary[33] = "00112233445566778899aabbccddeeff";
    unsigned char primaryMatrix[4][4];

    unsigned char key[16];
    unsigned char keyMatrix[4][4];
    char hexaKey[33] = "000102030405060708090a0b0c0d0e0f";

    HexaArrayToByteArray(primaryText, hexaPrimary);
    ByteArrayToByteMatrix(primaryMatrix,primaryText);

    HexaArrayToByteArray(key, hexaKey);
    ByteArrayToByteMatrix(keyMatrix,key);
    
    unsigned char expandedKeys[11][4][4];

    for (int i = 0; i < 4;i++){
        for (int j = 0; j < 4;j++){
            expandedKeys[0][i][j] = keyMatrix[i][j];
        }
    }

    KeyExpansion(expandedKeys);

    AESEncryption(primaryMatrix,expandedKeys);

    ByteMatrixToByteArray(primaryMatrix,primaryText);
    ByteArrayToHexaArray(primaryText,hexaPrimary);

    for (int i = 0; i < 32;i++){
        cout << hexaPrimary[i] ;
    }

    return 0;
}