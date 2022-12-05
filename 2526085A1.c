#include <stdio.h>
#include "stdlib.h"
// C:\Users\Public\data

/*
 * heap sort is from: https://www.programiz.com/dsa/heap-sort
 * binary search is from https://www.geeksforgeeks.org/binary-search/
 * number holds how many words each mail contains
 * binary search is O(log n) because it divides the list with 2 every time it is called.
 * heapsort is O(n log n)
 * Binary tree's height is log n.
 * We compare our top value to its children until all of its children are smaller.
 * Worst case, we do this operation as much as height of the tree.
 * While building the heap, we do this for half of the tree, n/2 log n.
 */

struct Data_files{
    int mail_id;
    char sender[50];
    char receipt[50];
    int date;
    int number;
};

struct Data_files *readEmails(int num, char directory[200]){

    FILE *inFile;
    struct Data_files *data;
    data = (struct Data_files *)malloc(sizeof(struct Data_files) * num);
    int i;
    char str[50];
    char dummy[50];
    char ch;
    for(i=1;i<=num;i++){
        sprintf(str, "%s\\%d.txt",directory, i);
        inFile = fopen(str,"r");
        if (inFile == NULL)
        {
            printf("\nThe file was not successfully opened.");
            printf("\nPlease check that the file %s currently exists.\n", str);
            exit(1);
        }

        fscanf(inFile, "%d\n", &data[i-1].mail_id);
        fscanf(inFile, "%s %s\n", dummy, data[i-1].sender);
        fscanf(inFile, "%s %s\n", dummy, data[i-1].receipt);
        fscanf(inFile, "%s %d\n", dummy, &data[i-1].date);

        data[i-1].number = 0;

        while((ch = fgetc(inFile)) != EOF){
            if(ch == ' '){
                data[i-1].number++;
            }
        }
    }
    return data;
}

void swap(struct Data_files *data, struct Data_files *new) {
    struct Data_files temp = *data;
    *data = *new;
    *new = temp;
}

void heapify(struct Data_files *data, int size, int i, int choice) {
    // Find largest among root, left child and right child
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    struct Data_files *temp = data;
    struct Data_files *new = data;

    if(choice == 1){//by ID
        temp = &data[left];
        new = &data[largest];
        if (left < size && temp->mail_id > new->mail_id)
            largest = left;

        temp = &data[right];
        new = &data[largest];
        if (right < size && temp->mail_id > new->mail_id)
            largest = right;
    }
    if(choice == 2){//by number
        temp = &data[left];
        new = &data[largest];
        if (left < size && temp->mail_id > new->mail_id)
            largest = left;

        temp = &data[right];
        new = &data[largest];
        if (right < size && temp->number > new->number)
            largest = right;
    }

    if(choice == 3){//by date
        temp = &data[left];
        new = &data[largest];
        if (left < size && temp->date > new->date)
            largest = left;

        temp = &data[right];
        new = &data[largest];
        if (right < size && temp->date > new->date)
            largest = right;
    }

    // Swap and continue heapifying if root is not largest
    if (largest != i) {
        swap(&data[i], &data[largest]);
        heapify(data, size, largest, choice);
    }
}

void heapSort(struct Data_files *data, int size, int choice) {
    for (int i = size / 2 - 1; i >= 0; i--)
        heapify(data, size, i, choice);

    for (int i = size - 1; i >= 0; i--) {
        swap(&data[0], &data[i]);
        heapify(data, i, 0, choice);
    }
}

void printEmails(struct Data_files *data, int num) {
    int index;
    for(index = 0; index<num; index++){
        printf("Id: %d\n",data[index].mail_id);
        printf("Sender: %s\n",data[index].sender);
        printf("Recipient: %s\n",data[index].receipt);
        printf("Date: %d\n",data[index].date);
        printf("Words: %d\n\n",data[index].number);
    }
}

void binSearch(struct Data_files *data, int l, int r, int x) {
    if (r >= l) {
        int mid = l + (r - l) / 2;

        if (data[mid].mail_id == x){
            printf("Id: %d\n"
                   "Sender: %s\n"
                   "Recipient: %s\n"
                   "Date: %d\n"
                   "Words: %d\n\n", data[mid].mail_id, data[mid].sender, data[mid].receipt, data[mid].date,
                   data[mid].number);
            return;
        }

        if (data[mid].mail_id > x)
            return binSearch(data, l, mid - 1, x);

        return binSearch(data, mid + 1, r, x);
    }

    printf("Element not present in the files.\n");
}

int main() {
    int files, choice = 0;
    char directory[200];
    struct Data_files *data;
    printf("Enter the path of the directory which contains data files(Please enter the full path):");
    scanf("%s",directory);
    printf("Enter the number of data files:");
    scanf("%d",&files);
    data = readEmails(files, directory);
    while(choice != 5){
        printf("Please choose one of the following options:\n");
        printf("(1) Display emails sorted by id\n");
        printf("(2) Display emails sorted by number of words\n");
        printf("(3) Display emails sorted by date\n");
        printf("(4) Search email by ID\n");
        printf("(5) Exit\n\n");
        printf("Command:");
        scanf("%d",&choice);
        switch (choice) {
            case (1):{
                heapSort(data, files, 1);
                printEmails(data, files);
                break;
            }
            case (2):{
                heapSort(data, files, 2);
                printEmails(data, files);
                break;
            }
            case (3):{
                heapSort(data, files, 3);
                printEmails(data, files);
                break;
            }
            case (4):{
                heapSort(data, files, 1);
                int key;
                printf("Enter key: ");
                scanf("%d", &key);
                binSearch(data, 0, files-1, key);
                break;
            }
            case (5):{
                printf("Goodbye!!");
                exit(1);
            }
            default:{
                printf("Try again.\n");
            }

        }
    }
}
