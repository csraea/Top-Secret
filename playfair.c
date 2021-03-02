#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include "playfair.h"


void find_location(char matrix[5][5], char symbol, size_t *iCol, size_t *iLine);
void fill_matrix(char matrix[5][5], const char keyWord[], size_t keySize, const char textToEncrypt[], size_t textSize);
//**********************************************************************//
char* playfair_encrypt(const char* key, const char* text) {
    if (key == NULL || text == NULL){return NULL;} 
    if ((int)strlen(key) < 1){return NULL;} 
    if((int)strlen(key) == 1 && strcmp(key," ")==0)return NULL;
    int counter = 0;
    if(key[0] == '\0') return NULL;
    for(int i = 0; i < strlen(key); i++){
        for(int j = 0; j < strlen(ALPHA); j++){
            if(key[i] == ALPHA[j]) counter++;
        }
        if(counter == 0) return NULL;
    }
    
    //copying data
    size_t keySize = strlen(key);
    size_t textSize = strlen(text);
    char keyWord[keySize];
    char textToEncrypt[textSize];
    strcpy(keyWord, key);
    strcpy(textToEncrypt, text);

    //case up both the key and the text
    size_t size = strlen(keyWord);
    for(size_t i = 0; i < size; i++){
        if(keyWord[i] >= 97)
            keyWord[i] = keyWord[i] - 32;
    }
    size = strlen(textToEncrypt);
    for(size_t i = 0; i < size; i++){
        if(textToEncrypt[i] >= 97)
            textToEncrypt[i] = textToEncrypt[i] - 32;
    }

    //skip unnecessary symbols
    for(size_t i = 0; i < keySize; i++){
        if(keyWord[i] == ' '){
            for(size_t j = i; j < keySize-1; j++){
                keyWord[j] = keyWord[j+1];
            }
            (keySize)--;
            keyWord[keySize] = '\0';
        }
    }
    for(size_t i = 0; i < keySize; i++){
        if(keyWord[i] == 'W')
            keyWord[i] = 'V';
    }
    //skip letters
    char letter;
    bool repetition = false;
    for(size_t i = 0; i < keySize+1; i++){
        letter = keyWord[i];
        for(size_t idx = 0; idx < i; idx++){
            if(keyWord[idx] == letter){
                repetition = true;
                break;
            }
        }
        if(repetition == true){  
            for(size_t j = i; j < keySize; j++){
                keyWord[j] = keyWord[j+1];          
            }
            (keySize)--;
            i--;
            keyWord[keySize] = '\0';
            repetition = false;
        }
    }

    for(size_t i = 0; i < textSize; i++){
        if(textToEncrypt[i] == ' '){
            for(size_t j = i; j < textSize-1; j++){
                textToEncrypt[j] = textToEncrypt[j+1];
            }
            (textSize)--;
            textToEncrypt[textSize] = '\0';
        }
    }
    for(size_t i = 0; i < textSize; i++){
        if(textToEncrypt[i] == 'W')
            textToEncrypt[i] = 'V';
    }
    for(size_t i = 0; i < textSize; i++){
        if(textToEncrypt[i] == 'W')
           (textToEncrypt[i] = 'V');
    }
    //insert 'X' inside the text
    char leter, buffer;
    for(size_t i = 0; i < textSize; i += 2){
        if(textToEncrypt[i] == textToEncrypt[i+1] && textToEncrypt[i] != 'X'){
            leter = textToEncrypt[i];
            textToEncrypt[i+1] = 'X';
            (textSize)++;
            for(size_t j = i+2; j < textSize; j++){
                buffer = textToEncrypt[j];
                textToEncrypt[j] = leter;
                leter = buffer;
            }
        }
    }
    //adding "X" at the ending if necessary
    if(textSize % 2 == 1){
        textToEncrypt[textSize] = 'X';
    }
    //matrix operations
    char matrix[5][5];
    fill_matrix(matrix, keyWord, keySize, textToEncrypt, textSize);
    //manipulate matrix
    size_t aCol, aLine, bCol, bLine;
    for(size_t i = 0; i < textSize; i += 2){
        find_location(matrix, textToEncrypt[i], &aCol, &aLine);
        find_location(matrix, textToEncrypt[i+1], &bCol, &bLine);
        if(aCol == bCol){
            textToEncrypt[i] = (aLine < 4) ? matrix[aLine+1][aCol] : matrix[0][aCol];
            textToEncrypt[i+1] = (bLine < 4) ? matrix[bLine+1][bCol] : matrix[0][bCol];
        } else if(aLine == bLine){
            textToEncrypt[i] = (aCol < 4) ? matrix[aLine][aCol+1] : matrix[aLine][0];
            textToEncrypt[i+1] = (bCol < 4) ? matrix[bLine][bCol+1] : matrix[bLine][0];
        } else {
            textToEncrypt[i] = matrix[aLine][bCol];
            textToEncrypt[i+1] = matrix[bLine][aCol];
        }
    }
    size_t Size_space = (strlen(textToEncrypt) /2)*3 -1;
    char * textToReturn = ( char*) calloc(Size_space+1, sizeof(char));
    size_t k = 0;
    for(size_t i=0,j=0; textToEncrypt[j] != '\0'; i++, j++ ){
        if(k == 2){textToReturn[i] = ' '; i++;k=0;}
        if(k != 2){textToReturn[i] = textToEncrypt[j];k++;}
    }    
    return textToReturn;
}
//**********************************************************************//
char* playfair_decrypt(const char* key, const char* text) {
    while (key == NULL || key == NULL){return NULL;} 
    while ((int)strlen(key) < 1) {return NULL;}
    if((int)strlen(key) == 1 && strcmp(key," ")==0)return NULL;
     int counter = 0;
    if(key[0] == '\0') return NULL;
    for(int i = 0; i < strlen(key); i++){
        for(int j = 0; j < strlen(ALPHA); j++){
            if(key[i] == ALPHA[j]) counter++;
        }
        if(counter == 0) return NULL;
    }
    //copying data
    size_t keySize = strlen(key);
    size_t textSize = strlen(text);
    char keyWord[keySize];
    char textToDecrypt[textSize];
    strcpy(keyWord, key);
    strcpy(textToDecrypt, text);

    //keyword operations
    for(size_t i = 0; i < keySize; i++){
        if(keyWord[i] >= 97)
            keyWord[i] = keyWord[i] - 32;
    }
    for(size_t i = 0; i < keySize; i++){
        if(keyWord[i] == ' '){
            for(size_t j = i; j < keySize-1; j++){
                keyWord[j] = keyWord[j+1];
            }
            (keySize)--;
            keyWord[keySize] = '\0';
        }
    }
    for(size_t i = 0; i < keySize; i++){
        if(keyWord[i] == 'W')
            keyWord[i] = 'V';
    }
    //skip letters
    char letter;
    bool repetition = false;
    for(size_t i = 0; i < keySize+1; i++){
        letter = keyWord[i];
        for(size_t idx = 0; idx < i; idx++){
            if(keyWord[idx] == letter){
                repetition = true;
                break;
            }
        }
        if(repetition == true){  
            for(size_t j = i; j < keySize; j++){
                keyWord[j] = keyWord[j+1];          
            }
            (keySize)--;
            i--;
            keyWord[keySize] = '\0';
            repetition = false;
        }
    }
    //matrix operations
    for(size_t i = 0; i < textSize; i++){
        if(textToDecrypt[i] == ' '){
            for(size_t j = i; j < textSize-1; j++){
                textToDecrypt[j] = textToDecrypt[j+1];
            }
            (textSize)--;
            textToDecrypt[textSize] = '\0';
        }
    }
    char matrix[5][5];
    fill_matrix(matrix, keyWord, keySize, textToDecrypt, textSize);
    size_t aCol, aLine, bCol, bLine;
    for(size_t i = 0; i < textSize; i += 2){
        find_location(matrix, textToDecrypt[i], &aCol, &aLine);
        find_location(matrix, textToDecrypt[i+1], &bCol, &bLine);
        if(aCol == bCol){
            textToDecrypt[i] = (aLine > 0) ? matrix[aLine-1][aCol] : matrix[4][aCol];
            textToDecrypt[i+1] = (bLine > 0) ? matrix[bLine-1][bCol] : matrix[4][bCol];
        } else if(aLine == bLine){
            textToDecrypt[i] = (aCol > 0) ? matrix[aLine][aCol-1] : matrix[aLine][4];
            textToDecrypt[i+1] = (bCol > 0) ? matrix[bLine][bCol-1] : matrix[bLine][4];
        } else {
            textToDecrypt[i] = matrix[aLine][bCol];
            textToDecrypt[i+1] = matrix[bLine][aCol];
        }
    }
    char *textToReturn = malloc(sizeof(char)*strlen(textToDecrypt)+1);
    strcpy(textToReturn, textToDecrypt);
    return textToReturn;
}

void fill_matrix(char matrix[5][5], const char keyWord[], size_t keySize, const char textToEncrypt[], size_t textSize){
    size_t idx = 0;
    size_t line, col;
    //fill matrix with the keyword
    bool keyWordFilled = false;
    for(line = 0; line < 5; line++){
        for(col = 0; col < 5; col++){
            if(idx < keySize){
                matrix[line][col] = keyWord[idx];
                idx++;
            } else {
                keyWordFilled = true;
                break;
            }
        }
        if(keyWordFilled == true)
            break;
        col++;
    }
    //fill matrix with the ALPHA
    size_t idxAlpha = 0;
    char alpha[26];
    strcpy(alpha, ALPHA);
    size_t alphaSize = strlen(alpha);
    size_t j;
    for(j = 0; j < alphaSize; j++){
        for(size_t i = 0; i < keySize; i++){
            if(alpha[j] == keyWord[i]){
                for(size_t q = j; q < alphaSize-1; q++){
                    alpha[q] = alpha[q+1];
                }
                alphaSize--;
                j--;
                alpha[alphaSize] = '\0';
                break;
            }
        }
    }
    for(line = line; line < 5; line++){
        for(col = col; col < 5; col++, idxAlpha++){
            matrix[line][col] = alpha[idxAlpha];
        }
        if(col == 5) col = 0;
    }
}

void find_location(char matrix[5][5], char symbol, size_t *iCol, size_t *iLine){
    for(size_t i = 0; i < 5; i++){
        for(size_t j = 0; j < 5; j++){
            if(matrix[i][j] == symbol){
                *iCol = j;
                *iLine = i;
                return;
            }
        }
    }
}