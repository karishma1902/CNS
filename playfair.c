#include <stdio.h>
#include <stdlib.h>
#include <string.h>// Function for converting a string to uppercase
void toUpperCase(char encrypt[], int ps) {
    for (int i = 0; i < ps; i++) {
        if (encrypt[i] >= 'a' && encrypt[i] <= 'z')
            encrypt[i] -= 32; // Convert lowercase to uppercase
    }
}// Function for removing spaces from a string
int removeSpaces(char* plain, int ps) {
    int count = 0;
    for (int i = 0; i < ps; i++) {
        if (plain[i] != ' ')
            plain[count++] = plain[i];
    }
    plain[count] = '\0';
    return count;
}// Function for creating the key matrix
void createMatrix(char keystr[], int ks, char keyMat[5][5]) {
    int dict[26] = {0};
    int i = 0, j = 0;
    for (int k = 0; k < ks; k++) {
        if (dict[keystr[k] - 'A'] == 0 && keystr[k] != 'J') {
            keyMat[i][j] = keystr[k];
            dict[keystr[k] - 'A'] = 1;
            j++;
            if (j == 5) {
                i++;
                j = 0;
            }
        }
    }
    for (int k = 0; k < 26; k++) {
        if (dict[k] == 0 && (char)(k + 'A') != 'J') {
            keyMat[i][j] = (char)(k + 'A');
            j++;
            if (j == 5) {
                i++;
                j = 0;
            }
        }
    }
}// Function for searching characters in the key matrix
void search(char keyMat[5][5], char a, char b, int arr[]) {
    if (a == 'J')
        a = 'I';
    else if (b == 'J')
        b = 'I';
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (keyMat[i][j] == a) {
                arr[0] = i;
                arr[1] = j;
            } else if (keyMat[i][j] == b) {
                arr[2] = i;
                arr[3] = j;
            }
        }
    }
}// Function for preparing the plaintext for encryption
int prep(char str[], int p) {
    int sub = p;
    for (int i = 0; i < sub; i += 2) {
        if (str[i] == str[i + 1]) {
            for (int j = sub; j > i + 1; j--) {
                str[j] = str[j - 1];
            }
            // Instead of inserting 'X', shift elements to the right
            if (str[i] != 'X') {
                str[i + 1] = 'X';
            } else {
                str[i + 1] = 'Z'; // Use 'Z' if 'X' is already present
            }
            sub++;
        }
    }// If the length is odd, append 'Z' to the end
    if (sub % 2 != 0) {
        str[sub++] = 'Z';
    }
    str[sub] = '\0';
    return sub;
}// Function for encryption using Playfair Cipher
void encrypt(char str[], char keyMat[5][5], int pos) {
    int a[4];
    for (int i = 0; i < pos; i += 2) {
        search(keyMat, str[i], str[i + 1], a);
        if (a[0] == a[2]) {
            str[i] = keyMat[a[0]][(a[1] + 1) % 5];
            str[i + 1] = keyMat[a[0]][(a[3] + 1) % 5];
        } else if (a[1] == a[3]) {
            str[i] = keyMat[(a[0] + 1) % 5][a[1]];
            str[i + 1] = keyMat[(a[2] + 1) % 5][a[1]];
        } else {
            str[i] = keyMat[a[0]][a[3]];
            str[i + 1] = keyMat[a[2]][a[1]];
        }
    }
}// Function for decryption using Playfair Cipher// Function for decryption using Playfair Cipher
void decrypt(char str[], char keyMat[5][5], int pos) {
    int a[4];
    for (int i = 0; i < pos; i += 2) {
        search(keyMat, str[i], str[i + 1], a);
        if (a[0] == a[2]) {
            str[i] = keyMat[a[0]][(a[1] + 4) % 5];
            str[i + 1] = keyMat[a[0]][(a[3] + 4) % 5];
        } else if (a[1] == a[3]) {
            str[i] = keyMat[(a[0] + 4) % 5][a[1]];
            str[i + 1] = keyMat[(a[2] + 4) % 5][a[1]];
        } else {
            str[i] = keyMat[a[0]][a[3]];
            str[i + 1] = keyMat[a[2]][a[1]];
        }
    } // Remove 'X' characters added during encryption but not part of original plaintext
    for (int i = 0; i < pos; i++) {
        if (str[i] == 'X' && str[i + 1] == str[i - 1]) {
            for (int j = i; j < pos; j++) {
                str[j] = str[j + 1];
            }
            pos--;
        }
    }
    str[pos] = '\0';
}// Main function
int main() {
    char string[200], keyString[200];
    printf("Enter key: ");
    scanf("%[^\n]s", keyString);
    printf("Enter plaintext: ");
    scanf("\n");
    scanf("%[^\n]s", string);// Prepare key and plaintext
    int ks = strlen(keyString);
    int ps = strlen(string);
    toUpperCase(keyString, ks);
    toUpperCase(string, ps);
    ks = removeSpaces(keyString, ks);
    ps = removeSpaces(string, ps);
    ps = prep(string, ps);// Create key matrix
    char keyMat[5][5];
    createMatrix(keyString, ks, keyMat);// Encrypt the plaintext
    encrypt(string, keyMat, ps);
    printf("Cipher text: %s\n", string); // Decrypt the cipher text
    decrypt(string, keyMat, ps);
    printf("Decrypted text: %s\n", string);
    return 0;
}
