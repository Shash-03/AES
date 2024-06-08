#include <bits/stdc++.h>
#include <helper.h>

using namespace std;






int main(){

    unsigned char cipherText[16];
    char hexaCipher[33] = "69c4e0d86a7b0430d8cdb78070b4c55a";
    unsigned char cipherMatrix[4][4];

    unsigned char key[16];
    unsigned char keyMatrix[4][4];
    char hexaKey[33] = "000102030405060708090a0b0c0d0e0f";

    HexaArrayToByteArray(cipherText, hexaCipher);
    ByteArrayToByteMatrix(cipherMatrix,cipherText);

    HexaArrayToByteArray(key, hexaKey);
    ByteArrayToByteMatrix(keyMatrix,key);
    



    ByteMatrixToByteArray(cipherMatrix,cipherText);
    ByteArrayToHexaArray(cipherText,hexaCipher);

    for (int i = 0; i < 32;i++){
        cout << hexaCipher[i] ;
    }

    return 0;
}