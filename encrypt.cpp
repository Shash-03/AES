#include <bits/stdc++.h>
#include "helper.h"

using namespace std;




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

    cout << "\n";

    return 0;
}