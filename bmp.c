#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include "bmp.h"
#define BALPHA "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define SIZE 26

size_t snoc(const char* text);
int XOR(char *binary);
int bin_to_dec(int n);
void dec_to_bin(int number, char *binary);
char* add_special_symbols(char *text, char *specialSymbols, size_t symSize);
//*************************************************************************//
size_t snoc(const char* text){
			if (!text) return 0;
			if (strcmp(text , "")==0){return 0;}
			else return 1;}

//*******************************************************************//

char* add_special_symbols(char *text, char *specialSymbols, size_t symSize){
	char buf, next;
	size_t i = 0;
	while(i < symSize){
		if(specialSymbols[i] != 'a'){
			next = text[i];
			text[i] = specialSymbols[i];
			for(size_t j = i; j < strlen(text); j++){
				buf = text[j+1];
				text[j+1] = next;
				next = buf;
			}
		}
		i++;
	}
	char *done = calloc(strlen(text)+1, sizeof(char));
	if(!done || done == NULL) return NULL;
	strcpy(done, text);
	return done;
}
char* vigenere_encrypt(const char* key, const char* text) {
    if (!snoc(text)  || !snoc(key) ){ return NULL;}
	//some useful variables
	size_t uniSize = strlen(text);

	//text operations
	char textToOperate[uniSize];
	char specialSymbols[uniSize];
	strcpy(textToOperate, text);
	size_t sizeT = strlen(textToOperate);
	for(size_t i = 0; i < sizeT; i++){
		if(textToOperate[i] >= 97 && textToOperate[i] < 123)
			textToOperate[i] = textToOperate[i] - 32;
	}
	for(size_t i = 0; i < uniSize; i++){
		specialSymbols[i] = 'a';
	}
	specialSymbols[uniSize] = '\0';
	for(size_t j = 0; j < uniSize; j++){
		if(textToOperate[j] < 65 || textToOperate[j] > 90){
			specialSymbols[j] = textToOperate[j];
		}
	}
	size_t symSize = uniSize;
	for(size_t i = 0; i < uniSize; i++){
		if(textToOperate[i] < 65 || textToOperate[i] > 90){
			for(size_t j = i; j < uniSize-1; j++){
				textToOperate[j] = textToOperate[j+1];
			}
			(uniSize)--;
			i--;
			textToOperate[uniSize] = '\0';
		}
	}

	//key operations
	char keyMod[uniSize];
	strcpy(keyMod, key);
	size_t realSize = strlen(keyMod);
	size_t j = 0;
	for(size_t i = realSize; i < uniSize; i++){
		keyMod[i] = keyMod[j];
		j = (j < realSize-1) ? j+1 : 0;
	}
	keyMod[uniSize] = '\0';
	size_t size = strlen(keyMod);
	for(size_t i = 0; i < size; i++){
		if(keyMod[i] >= 97 && keyMod[i] < 123)
			keyMod[i] = keyMod[i] - 32;
	}

	//magic
	size_t k;
	for(k = 0; k < uniSize; ++k)
        textToOperate[k] = ((textToOperate[k] + keyMod[k]) % 26) + 'A'; 
    textToOperate[k] = '\0';

	return add_special_symbols(textToOperate, specialSymbols, symSize);
}

char* vigenere_decrypt(const char* key, const  char* text) {
    if (!snoc(key) || !snoc(text)) {return NULL;}
	//some useful variables
	size_t uniSize = strlen(text);

	//text operations
	char textToOperate[uniSize];
	char specialSymbols[uniSize];
	strcpy(textToOperate, text);
	size_t sizeT = strlen(textToOperate);
	for(size_t i = 0; i < sizeT; i++){
		if(textToOperate[i] >= 97 && textToOperate[i] < 123)
			textToOperate[i] = textToOperate[i] - 32;
	}
	for(size_t i = 0; i < uniSize; i++){
		specialSymbols[i] = 'a';
	}
	specialSymbols[uniSize] = '\0';
	for(size_t j = 0; j < uniSize; j++){
		if(textToOperate[j] < 65 || textToOperate[j] > 90){
			specialSymbols[j] = textToOperate[j];
		}
	}
	size_t symSize = uniSize;
	for(size_t i = 0; i < uniSize; i++){
		if(textToOperate[i] < 65 || textToOperate[i] > 90){
			for(size_t j = i; j < uniSize-1; j++){
				textToOperate[j] = textToOperate[j+1];
			}
			(uniSize)--;
			i--;
			textToOperate[uniSize] = '\0';
		}
	}

	//key operations
	char keyMod[uniSize];
	strcpy(keyMod, key);
	size_t realSize = strlen(keyMod);
	size_t j = 0;
	for(size_t i = realSize; i < uniSize; i++){
		keyMod[i] = keyMod[j];
		j = (j < realSize-1) ? j+1 : 0;
	}
	keyMod[uniSize] = '\0';
	size_t size = strlen(keyMod);
	for(size_t i = 0; i < size; i++){
		if(keyMod[i] >= 97 && keyMod[i] < 123)
			keyMod[i] = keyMod[i] - 32;
	}	
	//magic
	size_t k;
	for(k = 0; k < uniSize; ++k)
        textToOperate[k] = (((textToOperate[k] - keyMod[k]) + 26) % 26) + 'A';
    textToOperate[k] = '\0';
	return add_special_symbols(textToOperate, specialSymbols, symSize);
}

unsigned char* bit_encrypt(const char* text) {
    if (!text) return NULL;
    char full_binary[9];
	char xor_binary[8];
	size_t size = strlen(text);
	unsigned char* encrypted = calloc(size, sizeof(char));
	if(encrypted == NULL) return NULL;
	for(size_t i = 0; i < size; i++){
		dec_to_bin(text[i], full_binary);
		full_binary[8] = '\0';
		char buffer;
		size_t j = 0;
		while( j < 4 ){
			buffer = full_binary[j];
			full_binary[j] = full_binary[j+1];
			full_binary[j+1] = buffer;
			j += 2;
		}
		dec_to_bin(XOR(full_binary), xor_binary);
		for(size_t i = 4; i < 8; i++){
			full_binary[i] = xor_binary[i];
		}
		encrypted[i] = bin_to_dec(atoi(full_binary));
	}
	encrypted[size] = '\0';
	return encrypted;

}

char* bit_decrypt(const unsigned char* text) {
    while (!text) {return NULL;}
    char full_binary[9];
	char xor_binary[8];
	size_t size = strlen((char*)text);
	char* decrypted = calloc(size, sizeof(char));
	if(decrypted == NULL) return NULL;
	for(size_t i = 0; i < size; i++){
		dec_to_bin(text[i], full_binary);
		dec_to_bin(XOR(full_binary), xor_binary);
		char buffer;
		for(size_t i = 0; i < 4; i += 2){
			buffer = full_binary[i];
			full_binary[i] = full_binary[i+1];
			full_binary[i+1] = buffer;
		}
		for(size_t i = 4; i < 8; i++){
				full_binary[i] = xor_binary[i];
		}
		decrypted[i] = bin_to_dec(atoi(full_binary));
	}
	decrypted[size] = '\0';
	return decrypted;

}

/////////////////////////////////////////////////////////////////////

unsigned char* bmp_encrypt(const char* key, const char* text) {
	if (!text || !snoc(key) ){return NULL;}
	if (!key) return NULL;
	if(strlen(key)<1)return NULL;
	char *reversed, *encrypted;
	unsigned char *final;
	reversed = reverse(text);
	encrypted = vigenere_encrypt(key, reversed);
	free(reversed);
	final = bit_encrypt(encrypted);
	free(encrypted);
	return final;
}

char* bmp_decrypt(const char* key, const unsigned char* text) {
    if (!text || !snoc(key) ){ return NULL;}
    if (!key) return NULL;
    if(strlen(key)<1){return NULL;}
	char *reversed, *decrypted, *final;
	decrypted = bit_decrypt(text);
	reversed = vigenere_decrypt(key, decrypted);
	free(decrypted);
	final = reverse(reversed);
	free(reversed);
	return final;
}


int XOR(char *binary){
	char first_b[5], second_b[5];
	for(size_t i = 0; i < 4; i++){
		first_b[i] = binary[i];
		second_b[i] = binary[i+4];
	}
	first_b[4] = '\0';
	second_b[4] = '\0';
	return (bin_to_dec(atoi(first_b)) ^ (bin_to_dec(atoi(second_b))));
}


int bin_to_dec(int n){
	int decimalNumber = 0, i = 0, remainder;
    while (n != 0)
    {
        remainder = n%10;
        n /= 10;
        decimalNumber += remainder*pow(2,i);
        ++i;
    }
    return decimalNumber;
}

void dec_to_bin(int number, char *binary){
	int c, k, i;
	for (c = 7, i = 0; c >= 0; c--, i++){
    	k = number >> c;
    	if(k & 1) binary[i] = '1';
    	else binary[i] = '0';
    }
}

char* reverse(const char* text){
	if(!text || !*text) return NULL;
	size_t size = strlen(text);
	char *reversed = calloc(size, sizeof(char));
	if(reversed == NULL) return NULL;
	for(size_t i = 0; i < size; i++){
		reversed[i] = text[size-i-1];
	}
	for(size_t i = 0; i < size; i++){
		if(reversed[i] >= 97 && reversed[i] < 123)
			reversed[i] = reversed[i] - 32;
	}
	reversed[size] = '\0';
	return reversed;
}
