/*
 * Midiel Rodriguez
 * ID: xxxxxx
 * Course: COP-4338
 * Lab: 3
 *
 * This program reads a text file and sorts the words ASCII order. 
 *
 * Usage: mywords [-cs] [-f substring] filename
 *
 * -c flag means to count the number of words in the file.
 * -s option means to print the words in the file sorted by ASCII order.
 * -f option will find the number of occurrences of the given substring.
 * 
 *
 * Disclaimer:
 * I hereby certify that this code is my work and my work alone and understand
 * the syllabus regarding plagiarized code.
 * Copyright 2018 Midiel
 */

/*
 * File:   mywords.c
 * Author: Midiel Rodriguez
 *
 * Created on June 17, 2018
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORDS 20
#define MAX_CHARACTERS 200
#define DELAMINATORS " !?,.:;\" \t\n  "

// It displays/prints the proper usage to run the program.
static void print_usage() {
    printf("Usage: mywords [-cs] [-f substring] filename\n");
}

// Displays a memory error message
static void memoryError() {
    printf("Error. Allocation of memory was unsuccessful.\n");
}

/* It sorts the list of words by ASCII order.
 * It accepts a pointer to a string pointer the represents the list of words,
 * and an integer pointer that represents the number of words.
 */
static void sortWords(char** words, int* numWords) {
    
    // To iterate through the list
    int i;
    int j;
    
    // Iterates through the list and sorts them by comparing the ASCII value
    for(i = 0; i < *numWords; i++) {
        for(j = 0; j <= i; j++) {
            if(strcmp(words[j], words[i]) > 0) { 
                char* temp;                                 // holds temp words
                temp = (char*)malloc(strlen(words[i]));     // allocates memory
                temp = words[i];                            // points to the word
                words[i] = (char*)malloc(strlen(words[j])); // ... and so on
                words[i] = words[j];
                words[j] = (char*)malloc(strlen(temp));
                words[j] = temp;
                                        
                // Validate allocation of memory
                if(temp == NULL || words[i] == NULL || words[j] == NULL) {
                    memoryError();
                    free(temp);
                    free(words);
                    free(numWords);
                    exit (0);
                }
            }
        }
    }
}

// Prints the entire array of words
static void printWords(char** words, int* countWd) {
    int i;
    for(i = 0; i < *countWd; i++) {
        printf("%d: %s\n", (i + 1), words[i]);
    }
}


// Find matching word
static int findWord(char** list, char* word, int* n) {
    int count = 0;
    int i;
    for(i = 0; i < *n; i++) {
        if(strcmp(list[i], word) == 0) {
            count++;
        }
    }
    return count;
}


/*
 * 
 */
int main(int argc, char** argv) {
    
    // Validates for a minimum of two arguments.
    if(argc < 2){
        printf("Error: Minimum two arguments required.\n");
        print_usage();
	exit(0);
    }
    
    // To hold the flags and variables
    extern char *optarg;
    int c;
    int cFlag = 0, sFlag = 0, fFlag = 0;
    
    
    FILE* inFile = NULL;
    char* wordPtr[MAX_WORDS] = {NULL};
    int* numWords = (int*)malloc(sizeof(int));
    
    // Validate that memory allocation was successful
    if(numWords == NULL) {
        memoryError();
        return -1;
    }
        
    // Try to open the file in read mode
    inFile = fopen(argv[argc - 1], "r");
    
    // Validate that file did open
    if(inFile == NULL) {
        printf("Could not open file %s.\n", argv[argc - 1]);
        print_usage();
        free(numWords);
        return -1;
    }
    
    // Copy the text file into a string
    char character;
    char textString[MAX_CHARACTERS];
    int i = 0;
    while((character = (char)fgetc(inFile)) != EOF) {
        textString[i] = character;
        i++;
    }
    textString[i] = '\0';                     // Make last character null
    
    
    // Close the file
    if(fclose(inFile) == EOF) {
        printf("Error closing the file.\n");
        free(numWords);
        return -1;
    }
    
    
    // Delaminate/split the string and save the words
    char* string = (char*)malloc(sizeof(char) * MAX_CHARACTERS);    // allocate memory
    strcpy(string, textString);                                     // copy the string
    char *tempPtr;                                                  // temp pointer
    
    // split the string into a word
    tempPtr = strtok(string, DELAMINATORS);
    
    // while there are more words
    while(tempPtr != NULL) {
        
        // Allocate memory
        wordPtr[*numWords] = (char*)malloc(strlen(tempPtr));
        
        // Validate allocation of memory
        if(wordPtr[*numWords] == NULL) {
            memoryError();
            free(string);
            return -1;
        }
        wordPtr[*numWords]= tempPtr;            // Point to temporary word
        (*numWords)++;                          // Increase count
        tempPtr = strtok(NULL, DELAMINATORS);   // Keep splitting
    }
        
    
    /* It iterates through the array of characters using "getopt" to parse the
     * command line. It will not read the last element because the last element
     * must be the name of the text file.
     */ 
    char* findSub;                              // Substring to look for 

    while ((c = getopt(argc - 1, argv, "csf:")) != -1){
        switch (c) {
            case 'c':                                   // sets cFlag
                cFlag = 1;
                break; 
            case 's':                                   // sets sFlag
                sFlag = 1;
                break;
            
            // sets the fFlag and gets the substring to search for
            case 'f':
                findSub = (char*)malloc(strlen(optarg) * sizeof(char));
                findSub = optarg;
                if(strlen(findSub) < 1) {
                    printf("You need to include the word to search for.\n");
                    print_usage();
                    free(findSub);
                    exit(0);
                }
                fFlag = 1;
                break;
                
            /* If getopt can't read the input, then an error is printed and
             * the program is terminated.
             */
            case '?':
                printf("Error: Invalid format.\n");
                print_usage();
                exit (0);
            
            // Default case prints the usage.
            default:
                print_usage();
                exit (0);        
        }
    }
    
    // Process the flags
    if(sFlag) {
        // sorts the words by ASCII order
        sortWords(wordPtr, numWords);
        
        // print the sorted words
        printf("\nList of words sorted by ASCII order:\n");
        printWords(wordPtr, numWords);    
    }
    
    // Prints the number of words in the text file
    if(cFlag) {
        printf("\nNumber of words in the file \"%s\": %d\n\n", argv[argc - 1], *numWords);  
    }
    
    // Finds the number of occurrences of the given substring
    if(fFlag) {
        int count = 0;                              // count keeper    
        char* textPtr = textString;                 // pointer to the text string

        // search for the substring provided.
        // If found, increase count and move pointer to the end of the substring
        while ((textPtr = strstr(textPtr, findSub)) != NULL ) {
            textPtr += strlen(findSub);
            count++;
        }
        printf("The substring \"%s\" was found: %d\n\n", findSub, count);
    }

    return (0);
}
