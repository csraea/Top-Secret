#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "playfair.h"
#include "bmp.h"

#define KEYBUF 100
#define TEXTBUF 1000

void clear_buffer(){
    while(getchar()!='\n');
}

int main(){
    char key[KEYBUF];
    char text[TEXTBUF];
    char c;
    printf("Choose encryption or decryption:\n\tE - to encrypt\n\tD - to decrypt\n\tQ - to exit\nChoose:\t");
    c = getchar();
    if(c =='E' || c == 'e'){
        printf("Choose an encryption method:\n\tP - to use playfair_encrypt\n\tB - to use bmp_encrypt\n\tQ - to exit\nChoode:\t");
        clear_buffer();
        c = getchar();
        if(c =='Q' || c == 'q')
            return 0;
        clear_buffer();
        printf("Enter key: ");
        fgets(key, KEYBUF, stdin);
        key[strlen(key)-1]='\0';
        printf("Enter text: ");
        fgets(text, TEXTBUF, stdin);
        text[strlen(text)-1]='\0';
        if(c =='P' || c == 'p'){
            char* buffer = playfair_encrypt(key, text);
            printf("Encrypted text: %s\n", buffer);
            free(buffer);
        }else if(c =='B' || c == 'b'){
            unsigned char* buffer1 = bmp_encrypt(key, text);
            printf("Encrypted text: ");
            for(int i=0; 1==1;i++) {
                if(buffer1[i]=='\0')
                    break;
                printf("%x ", buffer1[i]);
            }
            printf("\n");
            free(buffer1);
        }
    }else if(c =='D' || c == 'd'){
        printf("Choose an decryption method:\n\tP - to use playfair_decrypt\n\tB - to use bmp_decrypt\n\tQ - to exit\nChoose:\t");
        clear_buffer();
        c = getchar();
        if(c =='Q' || c == 'q')
            return 0;
        clear_buffer();
        printf("Enter key: ");
        fgets(key, KEYBUF, stdin);
        key[strlen(key)-1]='\0';
        printf("Enter text: ");
        fgets(text, TEXTBUF, stdin);
        text[strlen(text)-1]='\0';
        if(c =='P' || c == 'p'){
            char* buffer2 = playfair_decrypt(key, text);
            printf("Decrypted text: %s\n", buffer2);
            free(buffer2);
        }else if(c =='B' || c == 'b'){
            unsigned char text1[TEXTBUF];
            int i, l, k;
            for(i=0, l=strlen(text), k=0; i<l; i+=3, k++){
                text1[k]=0;
                if(text[i]>'9')
                    text1[k]+=(text[i]-87)*16;
                else
                    text1[k]+=(text[i]-48)*16;
                if(text[i+1]>'9')
                    text1[k]+=text[i+1]-87;
                else
                    text1[k]+=(text[i+1]-48);
            }
            text1[k]='\0';
            char* buffer3 = bmp_decrypt(key, text1);
            printf("Decrypted text: %s\n", buffer3);
            free(buffer3);
        }
    }
    return 0;
}