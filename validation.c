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
extern int arrayCount;
extern int currentID;
extern char numTest[20];
extern int NumOfUser;
extern int runs;
extern user users[MaxNumOfUsers];

//Books
extern book library[MaxNumOfUsers];
extern int BooksNO;
extern int bosearch;
extern int bofound;
extern FILE *fbook;
extern book *search_results[10];
char validISBN[14];
char input_receiver[MaxNumOfUsers]; // Receive inputs and Enter a series of validation functions to return a valid input

//Borrows
extern Borrowed bBooks[MaxNumOfUsers];
extern int borrowedNo;
extern int day,month,year;
extern FILE *fborrowed;
extern Borrowed *Isbnsearch[MaxNumOfUsers];

void filesCheck()
{
    int f;
    f = (int)fopen("usersFile.txt", "r");
    if(!f)
    {
        FILE *usersFile;
        usersFile = fopen("usersFile.txt", "w");
        fprintf(usersFile, "%d\n", currentID);
        fclose(usersFile);
    }
    f = (int)fopen("Books.txt", "r");
    if(!f)
    {
        FILE *fbook;
        fbook = fopen("Books.txt", "w");
        fclose(fbook);
    }
    f = (int)fopen("borrowed.txt", "r");
    if(!f)
    {
        FILE *fborrowed;
        fborrowed = fopen("borrowed.txt", "w");
        fclose(fborrowed);
    }
}
int natIDverification(char *ID) ///MS
{
    int dy, mnth, yr;
    dy = (int)(ID[5]-48)*10+(int)(ID[6]-48);
    mnth = (int)(ID[3]-48)*10+(int)(ID[4]-48);
    yr = (int)(ID[1]-48)*10+(int)(ID[2]-48);
    if(yr>17)
        yr+=1900;
    else
        yr+=2000;
    if(dateValidation(dy, mnth, yr))
        return 1;
    else
        return 0;
}
int mailVaildation(char *mail) // Right E-mail form validation
{
    int i, count=0, flag=0;
    if(!isalpha(mail[0]))
        return 0;
    for(i=0; mail[i]!='\0'; i++)
    {
        if(!isalpha(mail[i]) && !isdigit(mail[i]) && mail[i]!='_' && mail[i]!='.' && mail[i]!='@') // The only characters available in an E-mail
            return 0;
        if(mail[i]=='@')
            count++;
    }
    if(count!=1) // An E-mail should contain only one "@"
        return 0;
    i=1;
    while(mail[i]!='\0')
    {
        if(mail[i]=='@')
        {
            if((!isalpha(mail[i-1]) && !isdigit(mail[i-1])) || !isalpha(mail[i+1]))
                return 0;
            while(mail[++i]!='\0')
            {
                if(mail[i]=='.' && mail[i+1]!='.') // Case of 2 consecutive dots is refused
                {
                    if(isalpha(mail[i-1]) && isalpha(mail[i+1]))
                        flag = 1;
                    else
                        return 0;
                }
            }
        }
        else
            i++;
    }
    return flag;
}
int numberValid(char *p) // Number Validation
{
    int i;
    for(i=0; p[i]!='\0'; i++);
    if(p[i-1]==' ')
        p[i-1]='\0';
    for(i=0; p[i]!='\0'; i++){
        if(isdigit(p[i]))
            continue;
        else
            return 0;
    }
    return atoi(p);
}
int stringValidation(char *str) // A string should only contain letters
{
    int i, count=0;
    for(i=0; str[i]!='\0'; i++)
    {
        if(isalpha(str[i]))
            count++;
        if(!isalpha(str[i]) && str[i]!=' ')
            return 0;
    }
    if(count==0)
        return 0;
    return 1;
}
int phoneNumValidation(char *num)
{
    if(!numberValid(num))
        return 0;
    if(strlen(numTest)!=11)
        return 1;
    if(numTest[0]!='0' || numTest[1]!='1')
        return 2;
    return 5;
}
int dateValidation(int D, int M, int Y)
{
    if(Y>=1967 && Y<=2018)
    {
        if(M>=1 && M<=12)
        {
            if((D>=1 && D<=31) && (M==1 || M==3 || M==5 || M==7 || M==8 || M==10 || M==12))
                return 1;
            else if((D>=1 && D<=30) && (M==4 || M==6 || M==9 || M==11))
                return 1;
            else if((D>=1 && D<=28) && (M==2))
                return 1;
            else if(D==29 && M==2 && (Y%400==0 ||(Y%4==0 && Y%100!=0)))
                return 1;
            else
                return 0;
        }
        else
            return 0;
    }
    else
        return 0;
}
int duetoReturnDateValidation(int dd,int mm,int yy)
{
    currentDate(&day,&month,&year);
    if(yy>year&&yy<=2018)
        return 1;
    if(mm>month&&yy==year)
        return 1;
    if(dd>day&&mm==month&&yy==year)
        return 1;
    return 0;
}
int userIDverfication(int id)
{
    int i;
    for(i=0;i<arrayCount;i++)
    {
        if(users[i].ID==id)
        {
            if(users[i].booksBorrowed<3)
            {
                users[i].booksBorrowed++;
                return 1;
            }
            else
                return -1; //max books borrowed
        }
    }
    return 0; //wrong id
}
char* isbnValidation()
{
    fgets(validISBN,sizeof(validISBN),stdin);
    while((!numberValid(validISBN)) || (strlen(validISBN)!=13))
    {
        color(12);
        printf("Invalid Input!\n");
        fflush(stdin);
        color(15);
        printf("Please enter a 13-digit number: ");
        fgets(validISBN,sizeof(validISBN),stdin);
    }
    return validISBN;
}
int returnDateValidation(int d,int m,int y)
{
    currentDate(&day,&month,&year);
    if(year>y)
        return 1;
    if(y>year)
        return 0;
    if(month>m&&year==y)
        return 1;
    if(m>month&&year==y)
        return 0;
    if(day>d&&month==m&&year==y)
        return 1;
    if(d>day&&month==m&&year==y)
        return 0;
    return 5;
}
int publicationDate(int dd,int mm,int yy)
{
    currentDate(&day,&month,&year);
    if(yy<year)
        return 1;
    else if(yy==year)
    {
        if(mm<month)
            return 1;
        else if(mm==month)
        {
            if(dd<=day)
                return 1;
        }
    }
    return 0;
}
void myfgets(char *input, int n) ///MS
{
    int i;
    fgets(input, n, stdin);
    for(i=0; input[i]!='\0'; i++);
    if(input[i-1]=='\n')
        input[i-1]='\0';
}
