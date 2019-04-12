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

void load_borrowed()
{
    int n=0;
    FILE *fborrowed = fopen("borrowed.txt","r");
    char B[500];
    while(!feof(fborrowed))
    {
        fgets(B,sizeof(B),fborrowed);
        sscanf(B,"%[^,],%d,%d/%d/%d, %d/%d/%d, %d/%d/%d",bBooks[n].bookISBN,&bBooks[n].memberID,
            &bBooks[n].issued.day,&bBooks[n].issued.month,&bBooks[n].issued.year,&bBooks[n].duetoreturn.day,&bBooks[n].duetoreturn.month,
            &bBooks[n].duetoreturn.year,&bBooks[n].returned.day,&bBooks[n].returned.month,&bBooks[n].returned.year);
        n++;
    }
    borrowedNo=--n;
    fclose(fborrowed);
}

void save_borrowed()
{
    FILE *fborrowed;
    fborrowed = fopen("borrowed.txt", "w"); // "w" because every time the array changes the file change with it overwriting the old data
    int n=0;
    for(n=0; n<borrowedNo; n++)
    {
        fprintf(fborrowed,"%s,",bBooks[n].bookISBN);
        fprintf(fborrowed,"%d,",bBooks[n].memberID);
        fprintf(fborrowed,"%d/%d/%d,",bBooks[n].issued.day,bBooks[n].issued.month,bBooks[n].issued.year);
        fprintf(fborrowed,"%d/%d/%d,",bBooks[n].duetoreturn.day,bBooks[n].duetoreturn.month,bBooks[n].duetoreturn.year);
        fprintf(fborrowed,"%d/%d/%d\n",bBooks[n].returned.day,bBooks[n].returned.month,bBooks[n].returned.year);
    }
    fclose(fborrowed);
}
void searchb4Borrow()
{
    searchMenu(3);
}
void Borrow_book(book *temp)
{
    int position=1;
    char Keypressed=0;
    int id;
    if(temp->available<=0)
    {
        color(12);
        printf("There are no enough copies of this book");
    }
    else
    {
        color(15);
        printf("Enter your ID: ");
        scanf("%d",&id);
        int j;
        j = userIDverfication(id);
        if(j==-1)
        {
            color(12);
            printf("You can't borrow more than 3 books\n");
            color(14);
            printf("Press any key to go back to main menu ..");
            Keypressed=_getch();
            if(Keypressed)
                mainmenu();
        }
        while(j==0)
        {
            color(15);
            printf("You've entered a wrong ID.\n");
            system("cls");
            while(Keypressed!=(char)13)
            {
                setcoordinates(0,0);
                arrowHere(1,position);
                printf("1.Enter a new id");
                setcoordinates(0,2);
                arrowHere(2,position);
                printf("2.Back to borrow menu");
                Keypressed=_getch();
                if(Keypressed==(char)224)
                {
                    Keypressed = _getch();
                    if(Keypressed == (char)80 && position != 2)
                        position++;
                    else if(Keypressed == (char)72 && position != 1)
                        position--;
                    else
                        position = position;
                }
            }
            if(position==1)
            {
                system("cls");
                color(15);
                printf("Enter your id:");
                scanf("%d",&id);
                j = userIDverfication(id);
                if(j==-1)
                {
                    color(15);
                    printf("You cant borrow more than 3 books");
                    borrowmenu();
                }
            }
            else if(position==2)
                borrowmenu();
        }
        int rDay,rMonth,rYear;
        color(15);
        printf("Enter the date due to return the book (DD/MM/YYYY): ");
        scanf("%d/%d/%d",&rDay,&rMonth,&rYear);
        while(!dateValidation(rDay, rMonth, rYear) || !duetoReturnDateValidation(rDay, rMonth, rYear))
        {
            color(12);
            printf("This is not a valid date , please provide a valid one!\n");
            color(15);
            printf("Date due to return the book: ");
            scanf("%d/%d/%d",&rDay,&rMonth,&rYear);
        }
        bBooks[borrowedNo].duetoreturn.day=rDay;
        bBooks[borrowedNo].duetoreturn.month=rMonth;
        bBooks[borrowedNo].duetoreturn.year=rYear;
        currentDate(&day,&month,&year);
        bBooks[borrowedNo].issued.day=day;
        bBooks[borrowedNo].issued.month=month;
        bBooks[borrowedNo].issued.year=year;
        temp->nBorrowed++;
        temp->available--;
        temp->aBorrows++;
        strcpy(bBooks[borrowedNo].bookISBN,temp->isbn);
        bBooks[borrowedNo].memberID=id;
        bBooks[borrowedNo].returned.day=0;
        bBooks[borrowedNo].returned.month=0;
        bBooks[borrowedNo].returned.year=0;
        borrowedNo++;
    }
}
void return_date()
{
    system("cls");
    int ID, posInArray;
    char Keypressed=0;
    char numTest[20];
    strcpy(numTest, "a");
    color(15);
    printf("Enter User ID: ");
    fflush(stdin);
    myfgets(numTest, 20);
    while(!numberValid(numTest))
    {
        color(12);
        printf("This is not a valid ID, please provide a valid one.\n");
        color(15);
        printf("Enter User ID: ");
        fflush(stdin);
        myfgets(numTest, 20);
    }
    ID = atoi(numTest);
    posInArray=searchUser(ID);
    if(posInArray < 0)
    {
        color(12);
        printf("User Doesn't Exist!\n");
        color(14);
        printf("Press any key to go back to main menu ..");
        Keypressed=_getch();
        if(Keypressed)
            mainmenu();
    }
    else
    {
        if(users[posInArray].booksBorrowed<1)
        {
            color(12);
            printf("\nThis user doesn't have any borrowed books.\n");
            color(14);
            printf("Press any key to go back to main menu ..");
            Keypressed=_getch();
            if(Keypressed)
                mainmenu();
        }
        else
        {
            color(15);
            printf("Enter Book ISBN:");
            fflush(stdin);
            book x;
            strcpy(x.isbn, isbnValidation());
            color(15);
            int i,posInLibrary,j;
            int day,year,month,flag=0;
            for (i=0; i<borrowedNo; i++)
            {
                if(bBooks[i].returned.day==0&&bBooks[i].returned.year==0&&bBooks[i].returned.year==0)
                  {
                        if ( atoll(bBooks[i].bookISBN) == atoll(x.isbn) )
                        {
                            Isbnsearch[i] = &bBooks[i];
                            if(Isbnsearch[i]->memberID==ID)
                            {
                                color(15);
                                printf("The user borrowed this book");
                                color(10);
                                printf("\nBook[%d]",i);
                                color(15);
                                printf("\nBook's ISBN: %s",Isbnsearch[i]->bookISBN);
                                printf("\nUser's ID: %d",Isbnsearch[i]->memberID);
                                printf("\nBook's Issued date: %d/%d/%d",Isbnsearch[i]->issued.day,Isbnsearch[i]->issued.month,Isbnsearch[i]->issued.year);
                                printf("\nBook's due to return date: %d/%d/%d",Isbnsearch[i]->duetoreturn.day,Isbnsearch[i]->duetoreturn.month,Isbnsearch[i]->duetoreturn.year);
                                currentDate(&day,&month,&year);
                                Isbnsearch[i]->returned.day=day;
                                Isbnsearch[i]->returned.month=month;
                                Isbnsearch[i]->returned.year=year;
                                printf("\nBook's return date: %d/%d/%d\n",Isbnsearch[i]->returned.day,Isbnsearch[i]->returned.month,Isbnsearch[i]->returned.year);
                                color(10);
                                printf("Book Returned successfully");
                                users[posInArray].booksBorrowed--;
                                for(j=0; j<=BooksNO; j++)
                                {
                                    if(atoll(library[j].isbn)==atoll(x.isbn))
                                        posInLibrary=j;
                                }
                                library[posInLibrary].nBorrowed--;
                                library[posInLibrary].available++;
                                flag = 1;
                            }
                        }
                  }

            }
            if(flag==0)
            {
                color(12);
                printf("The user didn't borrow this book");
            }
            color(14);
            printf("\nPress any key to go back to main menu");
            Keypressed=0;
            Keypressed=_getch();
            if(Keypressed)
                mainmenu();
        }
    }
}
void printBorrowed()
{
    system("cls");
    int i;
    char Keypressed=0;
    if(borrowedNo==0)
    {
        setcoordinates(0,0);
        color(12);
        printf("Sorry, No data in our system, yet!\n");
        color(14);
        printf("Press any key to go back to main menu ..");
        Keypressed=_getch();
        if(Keypressed)
            mainmenu();
    }
    else
    {
        setcoordinates(0,0);
        color(15);
        for(i=0; i<borrowedNo; i++)
        {
            color(10);
            printf("Book[%d]",i+1);
            color(15);
            printf("\nBook's ISBN: %s",bBooks[i].bookISBN);
            printf("\nUser's ID: %d",bBooks[i].memberID);
            printf("\nBook's Issued date: %d/%d/%d",bBooks[i].issued.day,bBooks[i].issued.month,bBooks[i].issued.year);
            printf("\nBook's due to return date: %d/%d/%d",bBooks[i].duetoreturn.day,bBooks[i].duetoreturn.month,bBooks[i].duetoreturn.year);
            printf("\nBook's return date: %d/%d/%d\n",bBooks[i].returned.day,bBooks[i].returned.month,bBooks[i].returned.year);
        }
        color(14);
        printf("Press any key to go back to main menu ..");
        Keypressed=_getch();
        if(Keypressed)
            mainmenu();
    }
}
void overdueBooks()
{
    system("cls");
    int i,k;
    char Keypressed=0;
    if(borrowedNo==0)
    {
        setcoordinates(0,0);
        color(12);
        printf("There are no borrowed books.\n");
        color(14);
        printf("Press any key to go back to main menu..");
        Keypressed=_getch();
        if(Keypressed)
            mainmenu();
    }
    setcoordinates(0,0);
    for(i=0;i<borrowedNo;i++)
    {
        if(bBooks[i].returned.day==0 && bBooks[i].returned.month==0 && bBooks[i].returned.year==0)
        {
            k = returnDateValidation(bBooks[i].duetoreturn.day,bBooks[i].duetoreturn.month,bBooks[i].duetoreturn.year);
            if(k)
            {
                color(11);
                printf("Overdue Books");
                color(10);
                printf("\nBook[%d]",i+1);
                color(15);
                printf("\nBook's ISBN: %s",bBooks[i].bookISBN);
                printf("\nUser's ID: %d",bBooks[i].memberID);
                printf("\nBook's Issued date: %d/%d/%d",bBooks[i].issued.day,bBooks[i].issued.month,bBooks[i].issued.year);
                printf("\nBook's due to return date: %d/%d/%d\n",bBooks[i].duetoreturn.day,bBooks[i].duetoreturn.month,bBooks[i].duetoreturn.year);
            }
        }
        else
        {
            color(12);
            printf("There are no overdue books.\n");
            color(14);
            printf("Press any key to go back to main menu..");
            Keypressed=_getch();
            if(Keypressed)
                mainmenu();
        }
    }
        color(14);
        printf("Press any key to go back to main menu..");
        Keypressed=_getch();
        if(Keypressed)
            mainmenu();
}
