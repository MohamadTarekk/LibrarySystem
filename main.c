#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <windows.h>
#include <conio.h>
#include "struct.h"
#include "members.h"
#include "books.h"
#include "borrows.h"
#include "validation.h"
#include "mainMenu.h"
#include "Consfunctions.h"
#define MaxNumOfUsers 100

/// Global Variables

//Users
int arrayCount;
int currentID;
char numTest[20];
int NumOfUser;
int runs;
user users[MaxNumOfUsers];

//Books
book library[MaxNumOfUsers];
int BooksNO;
int bosearch;
int bofound;
FILE *fbook;
book *search_results[10];
char validISBN[14];
char input_receiver[MaxNumOfUsers]; // Receive inputs and Enter a series of validation functions to return a valid input

//Borrows
Borrowed bBooks[MaxNumOfUsers];
int borrowedNo;
int day,month,year;
FILE *fborrowed;
Borrowed *Isbnsearch[MaxNumOfUsers];

int main()
{
    filesCheck();
    fillArray();
    load_books();
    load_borrowed();
    system("title Library System");
    cursor(0);
    mainmenu();
    return 0;
}
