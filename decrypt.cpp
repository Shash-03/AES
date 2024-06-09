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
        //We can simply find w5,w6,w7 by w0 ^ w1 and so on
        for (int j = 3; j >= 1 ; j--){

            for (int i = 0; i < 4;i++){
                expandedKeys[round][i][j] = expandedKeys[round - 1][i][j-1] ^ expandedKeys[round - 1][i][j];
            }
        }

        //To find w4, we take g(w3) ^ w0
        unsigned char col[4];

        for (int i = 0; i < 4;i++){
            col[i] = expandedKeys[round - 1][i][2] ^ expandedKeys[round - 1][i][3];
        }


        
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

void InverseMixColumns(unsigned char cipherText[4][4]) {

    //For the inverse mix columns, we multiply by 11x^3 + 13x^2 + 9x + 14
    unsigned char mul[4][4];

    for (int i = 0; i < 4; i++) {
        unsigned char col[4];
        for (int j = 0; j < 4; j++) {
            col[j] = cipherText[j][i];
        }

        unsigned char Times_2[4], Times_4[4], Times_8[4];
        unsigned char Times_9[4], Times_11[4], Times_13[4], Times_14[4];

        for (int j = 0; j < 4; j++) {
            if (col[j] & 0x80) {
                Times_2[j] = (col[j] << 1) ^ 0x1b;
            } else {
                Times_2[j] = col[j] << 1;
            }
        }

        for (int j = 0; j < 4; j++) {
            if (Times_2[j] & 0x80) {
                Times_4[j] = (Times_2[j] << 1) ^ 0x1b;
            } else {
                Times_4[j] = Times_2[j] << 1;
            }
        }

        for (int j = 0; j < 4; j++) {
            if (Times_4[j] & 0x80) {
                Times_8[j] = (Times_4[j] << 1) ^ 0x1b;
            } else {
                Times_8[j] = Times_4[j] << 1;
            }
        }

        for (int j = 0; j < 4; j++) {
            Times_9[j] = Times_8[j] ^ col[j];
            Times_11[j] = Times_8[j] ^ Times_2[j] ^ col[j];
            Times_13[j] = Times_8[j] ^ Times_4[j] ^ col[j];
            Times_14[j] = Times_8[j] ^ Times_4[j] ^ Times_2[j];
        }

        mul[0][i] = Times_14[0] ^ Times_11[1] ^ Times_13[2] ^ Times_9[3];
        mul[1][i] = Times_9[0] ^ Times_14[1] ^ Times_11[2] ^ Times_13[3];
        mul[2][i] = Times_13[0] ^ Times_9[1] ^ Times_14[2] ^ Times_11[3];
        mul[3][i] = Times_11[0] ^ Times_13[1] ^ Times_9[2] ^ Times_14[3];
    }

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            cipherText[j][i] = mul[j][i];
        }
    }
}

void Round(unsigned char cipherMatrix[4][4],unsigned char keyMatrix[4][4] ){
    InverseShiftRows(cipherMatrix);
    InverseSubBytes(cipherMatrix);
    AddRoundKey(cipherMatrix, keyMatrix);
    InverseMixColumns(cipherMatrix);
}

void LastRound(unsigned char cipherMatrix[4][4],unsigned char keyMatrix[4][4] ){
    InverseShiftRows(cipherMatrix);
    InverseSubBytes(cipherMatrix);
    AddRoundKey(cipherMatrix, keyMatrix);
}

void AESDecryption128(unsigned char cipherMatrix[4][4],unsigned char expandedKeys[11][4][4] ){
    AddRoundKey(cipherMatrix,expandedKeys[0]);

    for (int i = 1; i <= 9;i++){
        Round(cipherMatrix,expandedKeys[i]);
    }

    LastRound(cipherMatrix,expandedKeys[10]);
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

    AESDecryption128(cipherMatrix,expandedKeys);


    ByteMatrixToByteArray(cipherMatrix,cipherText);
    ByteArrayToHexaArray(cipherText,hexaCipher);

    for (int i = 0; i < 32;i++){
        cout << hexaCipher[i] ;
    }

    return 0;
}