#ifndef VALIDATION_H_INCLUDED
#define VALIDATION_H_INCLUDED

void filesCheck(); // Creating the necessary files at the first run
int natIDverification(char*);
int mailVaildation(char*);
int numberValid(char*);
int stringValidation(char*);
int phoneNumValidation (char*);
int dateValidation(int, int, int);
int duetoReturnDateValidation(int dd,int mm,int yy);
int userIDverfication(int);
char* isbnValidation();
int returnDateValidation(int ,int ,int);
int publicationDate(int dd,int mm,int yy);
void myfgets(char*, int);

#endif // VALIDATION_H_INCLUDED
