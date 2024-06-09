#include <bits/stdc++.h>
#include "helper.h"

using namespace std;

//Add Round key operation
void AddRoundKey(unsigned char cipherText[4][4],unsigned char key[4][4]){
    for (int i = 0; i < 4;i++){
        for (int j = 0; j < 4;j++){
            cipherText[j][i] ^= key[j][i];
        }
    }
}

//Inverse sub bytes operation
void InverseSubBytes(unsigned char cipherText[4][4]){
    for (int i = 0; i < 4;i++){
        for (int j = 0; j < 4;j++){
            cipherText[j][i] = inverseSBox[cipherText[j][i]];
        }
    }
}

//Inverse shift rows operation
void InverseShiftRows(unsigned char cipherText[4][4]){
    unsigned char shift[4][4];

    for (int i = 0; i < 4;i++){
        for (int j = 0;j < 4;j++){
            shift[i][j] = cipherText[i][(j-i + 4)%4];
        }
    }

    for (int i = 0; i < 4;i++){
        for (int j = 0; j < 4;j++){
            cipherText[j][i] = shift[j][i];
        }
    }
}

void KeyExpansion128(unsigned char expandedKeys[11][4][4]) {
    for (int round = 1; round <= 10; round++) {
        for (int j = 3; j >= 1 ; j--){
            for (int i = 0; i < 4;i++){
                expandedKeys[round][i][j] = expandedKeys[round - 1][i][j-1] ^ expandedKeys[round - 1][i][j];
            }
        }
        unsigned char col[4];

        for (int i = 0; i < 4;i++){
            col[i] = expandedKeys[round - 1][i][2] ^ expandedKeys[round - 1][i][3];
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

        col[0] ^= rc[10 - round];

        for (int i = 0; i < 4;i++){
            expandedKeys[round][i][0] = col[i] ^ expandedKeys[round - 1][i][0];
        }


    }
    
}



int main(){

    unsigned char cipherText[16];
    char hexaCipher[33] = "69c4e0d86a7b0430d8cdb78070b4c55a";
    unsigned char cipherMatrix[4][4];

    unsigned char key[16];
    unsigned char keyMatrix[4][4];
    char hexaKey[33] = "13111d7fe3944a17f307a78b4d2b30c5";

    HexaArrayToByteArray(cipherText, hexaCipher);
    ByteArrayToByteMatrix(cipherMatrix,cipherText);

    HexaArrayToByteArray(key, hexaKey);
    ByteArrayToByteMatrix(keyMatrix,key);

    unsigned char expandedKeys[11][4][4];

    for (int i = 0; i < 4;i++){
        for (int j = 0; j < 4;j++){
            expandedKeys[0][i][j] = keyMatrix[i][j];
        }
    }

    KeyExpansion128(expandedKeys);



    AddRoundKey(cipherMatrix,expandedKeys[0]);

    InverseShiftRows(cipherMatrix);

    InverseSubBytes(cipherMatrix);

    AddRoundKey(cipherMatrix,expandedKeys[1]);




    

    ByteMatrixToByteArray(cipherMatrix,cipherText);
    ByteArrayToHexaArray(cipherText,hexaCipher);

    for (int i = 0; i < 32;i++){
        cout << hexaCipher[i] ;
    }


    
    
    





    return 0;
}