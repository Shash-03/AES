#include <bits/stdc++.h>
#include "helper.h"

using namespace std;

class AES_Operations{
public:

    //Add Round key operation
    static void AddRoundKey(unsigned char cipherText[4][4],unsigned char key[4][4]){
        for (int i = 0; i < 4;i++){
            for (int j = 0; j < 4;j++){
                cipherText[j][i] ^= key[j][i];
            }
        }
    }

    //Inverse sub bytes operation
    static void InverseSubBytes(unsigned char cipherText[4][4]){
        for (int i = 0; i < 4;i++){
            for (int j = 0; j < 4;j++){
                cipherText[j][i] = inverseSBox[cipherText[j][i]];
            }
        }
    }

    //Inverse shift rows operation
    static void InverseShiftRows(unsigned char cipherText[4][4]){
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

    static void InverseMixColumns(unsigned char cipherText[4][4]) {

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

    static void Round(unsigned char cipherMatrix[4][4],unsigned char keyMatrix[4][4] ){
        InverseShiftRows(cipherMatrix);
        InverseSubBytes(cipherMatrix);
        AddRoundKey(cipherMatrix, keyMatrix);
        InverseMixColumns(cipherMatrix);
    }

    static void LastRound(unsigned char cipherMatrix[4][4],unsigned char keyMatrix[4][4] ){
        InverseShiftRows(cipherMatrix);
        InverseSubBytes(cipherMatrix);
        AddRoundKey(cipherMatrix, keyMatrix);
    }


};


class AES128_Decryption{
private:
    char hexaCipher[33];
    char hexaKey[33];

public:

    AES128_Decryption(const char *cipher, const char *key){
        setHexaCipher(cipher);
        setHexaKey(key);
    }

    char* getHexaCipher() {
        return hexaCipher;
    }

   
    void setHexaCipher(const char* value) {
        strncpy(hexaCipher, value, 32);
        hexaCipher[32] = '\0'; 
    }

    char* getHexaKey() {
        return hexaKey;
    }

   
    void setHexaKey(const char* value) {
        strncpy(hexaKey, value, 32);
        hexaKey[32] = '\0';
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


    void Decryption128(unsigned char cipherMatrix[4][4],unsigned char expandedKeys[11][4][4] ){
        AES_Operations::AddRoundKey(cipherMatrix,expandedKeys[0]);

        for (int i = 1; i <= 9;i++){
            AES_Operations::Round(cipherMatrix,expandedKeys[i]);
        }

        AES_Operations::LastRound(cipherMatrix,expandedKeys[10]);
    }

    void Decrypt(){

        unsigned char cipherText[16];
        unsigned char cipherMatrix[4][4];

        unsigned char key[16];
        unsigned char keyMatrix[4][4];

        char* hexaCipher = getHexaCipher();
        char* hexaKey = getHexaKey();

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

        Decryption128(cipherMatrix,expandedKeys);

        ByteMatrixToByteArray(cipherMatrix,cipherText);
        ByteArrayToHexaArray(cipherText,hexaCipher);

        for (int i = 0; i < 32;i++){
            cout << hexaCipher[i] ;
        }

    }

};


int main(){
    const char* hexaCipher = "69c4e0d86a7b0430d8cdb78070b4c55a";
    const char* hexaKey = "13111d7fe3944a17f307a78b4d2b30c5";

    AES128_Decryption aes128(hexaCipher,hexaKey);

    aes128.Decrypt();


    return 0;
}