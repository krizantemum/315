#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Data_files
{
    int Email_ID;
    char Sender[51];
    char Recipient[51];
    int Day_of_the_month;
    int No_of_words;
};

//finds closest bigger prime number
int findPrime(int *size)
{

    int i, flag = 1;
    while(flag!=0)
    {
        (*size)++;
        flag = 0;
        for(i=2; i<(*size); i++)
        {
            if((*size)%(i) == 0)
            {
                flag++;
            }
        }
    }
    return *size;
}

//Gets the array, key and the size, does double hashing sends the index
int doubleHashing(struct Data_files *data, int key, int *size)
{
    int i = 1;
    int hash1 = key%(*size);
    int hash2 = 5 - (key%5);
    int doubleHash = i * hash2 + hash1;

    while(doubleHash<=(*size)-1)
    {
        if(data[doubleHash].Email_ID == -1)
        {
            return doubleHash;
        }
        else
        {
            i += 1;
            doubleHash = i * hash2 + hash1;
        }
    }
}

//Gives -1 to all Email-ID's, so we can understand which one is empty later on
struct Data_files *indexing(struct Data_files *data, int *size)
{
    int index;
    for(index = 0; index<(*size); index++)
    {
        data[index].Email_ID = -1;
    }
    return data;
}

//Gets the array, index and the size; returns new index
int linearProbing(struct Data_files *data, int index, int *size)
{
    int i = index;
    while(data[i].Email_ID != -1)
    {
        if(i == (*size)-1)
            i = 0;
        else
        {
            i++;
        }
    }
    return i;
}

//rehashing
struct Data_files *rehash(struct Data_files *data, int *size, double *elements, int choice)
{
    double a = *size;
    double *newSize = &a;
    *newSize = findPrime(size);
    struct Data_files *newData;
    newData = (struct Data_files *)malloc(sizeof(struct Data_files) * (*newSize));
    newData = indexing(newData, newSize);//IT MESSES UP WITH THIS PART(newSize), DOES NOT INITIALIZE TO -1
    int i;
    int key;

    for(i=0; i<(*size); i++)
    {
        if(data[i].Email_ID != -1)//Email-ID being -1 means it is empty
        {
            key = data[i].Email_ID + data[i].Sender[0] - 65;
            int sizeI = *newSize;
            key = key%(sizeI);
            if(newData[key].Email_ID == -1){//If no value in the selected index, put it there
                newData[key].Email_ID = data[i].Email_ID;
                newData[key].No_of_words = data[i].No_of_words;
                strcpy(newData[key].Sender,data[i].Sender);
                strcpy(newData[key].Recipient, data[i].Recipient);
                newData[key].Day_of_the_month = data[i].Day_of_the_month;
            }
            else if(choice == 1){// else if double hashing chosen (same algorithm)
                key = doubleHashing(data,key,size);
                newData[key].Email_ID = data[i].Email_ID;
                newData[key].No_of_words = data[i].No_of_words;
                strcpy(newData[key].Sender,data[i].Sender);
                strcpy(newData[key].Recipient, data[i].Recipient);
                newData[key].Day_of_the_month = data[i].Day_of_the_month;
            }
            else{
                key = linearProbing(data,key,size);//else for choice == 0 (same algorithm)
                newData[key].Email_ID = data[i].Email_ID;
                newData[key].No_of_words = data[i].No_of_words;
                strcpy(newData[key].Sender,data[i].Sender);
                strcpy(newData[key].Recipient, data[i].Recipient);
                newData[key].Day_of_the_month = data[i].Day_of_the_month;
            }
        }
    }
    return newData;
}

//Prints the table with its empty spaces
void printTable(struct Data_files *data, int *size)
{
    int i,j;
    j=*size;
    printf("Index        ID      Sender      Recipient       Date        Words\n");
    for(i=0; i<j; i++)
    {
        if(data[i].Email_ID == -1)
        {
            printf("%d\n",i);
        }
        else
        {
            printf("%d            %d        %s        %s        %d              %d\n",
                   i,data[i].Email_ID,data[i].Sender,data[i].Recipient,data[i].Day_of_the_month,data[i].No_of_words);
        }

    }
}

//Gets array, id and name to be searched and the size, prints
void searchEmails(struct Data_files *data, int id, char name[51], int *size)
{
    int key = id + name[0] - 65;
    int hash1 = key%(*size);
    int hash2 = 5-(key%5);

    if(data[hash1].Email_ID == id && (strcmpi(data[hash1].Sender,name)==0))
    {
        printf("Recipient: %s\n"
               "Date: %d\n"
               "Number of words: %d",data[hash1].Recipient,data[hash1].Day_of_the_month,data[hash1].No_of_words);
        return;
    }
    while(data[hash1+1].Email_ID != -1)
    {
        hash1 += 1;
        if(data[hash1].Email_ID == id && !strcmp(data[hash1].Sender, name))
        {
            printf("Recipient: %s\n"
                   "Date: %d\n"
                   "Number of words: %d\n",data[hash1].Recipient,data[hash1].Day_of_the_month,data[hash1].No_of_words);
            return;
        }
    }
    while (data[hash2].Email_ID != -1)
    {
        if(data[hash2].Email_ID == id && (strcmpi(data[hash2].Sender,name)==0))
        {
            printf("Recipient: %s\n"
                   "Date: %d\n"
                   "Number of words: %d",data[hash1].Recipient,data[hash1].Day_of_the_month,data[hash1].No_of_words);
            return;
        }
        hash2 += 1;
    }
    printf("Student is not found!");
}

//Reads the mails
struct Data_files *readEmail(int choice, int *size)
{
    double b = 0;
    double *elements;
    elements = &b;

    double load_factor;
    int num = 6;
    char directory[200];
    printf("Enter the path to the file");
    scanf("%s",directory);
    printf("Enter the num of files");
    scanf("%d", &num);
    FILE *inFile;
    struct Data_files *data;
    data = (struct Data_files *)malloc(sizeof(struct Data_files) * (*size));
    data = indexing(data, size);
    struct Data_files *temp = (struct Data_files *)malloc(sizeof(struct Data_files) * 1);
    char str[50];
    char dummy[50];

    int i;
    int hash2;
    for(i=1; i<=num; i++)
    {
        sprintf(str, "%s\\%d.txt",directory, i);
        inFile = fopen(str,"r");
        if (inFile == NULL)
        {
            printf("\nThe file was not successfully opened.");
            printf("\nPlease check that the file %s currently exists.\n", str);
            exit(1);
        }

        fscanf(inFile, "%d\n", &temp[0].Email_ID);
        fscanf(inFile, "%s %s\n", dummy, temp[0].Sender);
        fscanf(inFile, "%s %s\n", dummy, temp[0].Recipient);
        fscanf(inFile, "%s %d\n", dummy, &temp[0].Day_of_the_month);

        temp[0].No_of_words = 0;

        char ch;
        while((ch = fgetc(inFile)) != EOF)
        {
            if(ch == ' ')
            {
                temp[0].No_of_words++;
            }
        }
        temp[0].No_of_words++;
        int key = temp->Email_ID + temp->Sender[0] - 65;
        int hash1 = key%(*size);

        if(data[hash1].Email_ID == -1)
        {
            data[hash1].Email_ID = temp->Email_ID;
            data[hash1].No_of_words = temp->No_of_words;
            strcpy(data[hash1].Sender, temp->Sender);
            strcpy(data[hash1].Recipient, temp->Recipient);//broken
            data[hash1].Day_of_the_month = temp->Day_of_the_month;
        }
        else if(choice == 1)
        {
            hash2 = doubleHashing(data,key,size);
            data[hash2].Email_ID = temp->Email_ID;
            data[hash2].No_of_words = temp->No_of_words;
            strcpy(data[hash2].Sender, temp->Sender);
            strcpy(data[hash2].Recipient, temp->Recipient);
            data[hash2].Day_of_the_month = temp->Day_of_the_month;
        }
        else
        {
            hash2 = linearProbing(data,hash1,size);
            data[hash2].Email_ID = temp->Email_ID;
            data[hash2].No_of_words = temp->No_of_words;
            strcpy(data[hash2].Sender, temp->Sender);
            strcpy(data[hash2].Recipient, temp->Recipient);
            data[hash2].Day_of_the_month = temp->Day_of_the_month;
        }

        (*elements)++;
        load_factor = (*elements)/(*size);
        if(load_factor>0.5)
        {
            *size *= 2;
            data = rehash(data,size,elements,choice);
            printTable(data,size);
        }
    }
    return data;
}

int main()
{
    struct Data_files *data;
    int a = 11;
    int *size = &a;
    int hash, choice = 0;
    printf("Enter 1 for double hashing, 2 for linear probing.\n");
    scanf("%d", &hash);
    while (choice!=4)
    {
        printf("1 for Read an e-mail\n2 for Search an e-mail\n3 for print table\n4 for Exit\n");
        scanf("%d", &choice);
        switch (choice)
        {
            case 1:
            {
                data = readEmail(hash, size);
                break;
            }
            case 2:
            {
                int id;
                char name[51];
                printf("Enter the email id:");
                scanf("%d",&id);
                printf("Enter the name:");
                scanf("%s",name);
                searchEmails(data, id, name, size);
                break;
            }
            case 3:
            {
                printTable(data, size);
                break;
            }
        }
    }
    return 0;
}
