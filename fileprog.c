#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define FILENAME "students.txt"

typedef struct {
    int roll;
    char name[50];
    int scores[5];
    int isDeleted;  
} Student;

void addRecord();
void displayRecords();
void searchRecord(int roll);
void editRecord(int roll);
void logicalDelete(int roll);
void physicalDelete(int roll);
int calculateTotal(int scores[]);

int main() {
    int choice, roll;

    do {
        printf("\n--- Student Management System ---\n");
        printf("1. Add Record\n");
        printf("2. Display All Records\n");
        printf("3. Search Record by Roll\n");
        printf("4. Edit Record\n");
        printf("5. Logical Delete\n");
        printf("6. Physical Delete\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addRecord();
                break;
            case 2:
                displayRecords();
                break;
            case 3:
                printf("Enter roll number to search: ");
                scanf("%d", &roll);
                searchRecord(roll);
                break;
            case 4:
                printf("Enter roll number to edit: ");
                scanf("%d", &roll);
                editRecord(roll);
                break;
            case 5:
                printf("Enter roll number to logically delete: ");
                scanf("%d", &roll);
                logicalDelete(roll);
                break;
            case 6:
                printf("Enter roll number to physically delete: ");
                scanf("%d", &roll);
                physicalDelete(roll);
                break;
            case 0:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice! Try again.\n");
        }
    } while (choice != 0);

    return 0;
}

int calculateTotal(int scores[]) {
    int total = 0;
    for (int i = 0; i < 5; i++) {
        total += scores[i];
    }
    return total;
}

void addRecord() {
    FILE *fp = fopen(FILENAME, "a");
    if (!fp) {
        printf("Error opening file!\n");
        return;
    }

    Student s;
    printf("Enter roll number: ");
    scanf("%d", &s.roll);
    
    printf("Enter name: ");
    scanf("%s", s.name);

    printf("Enter scores in 5 subjects: ");
    for (int i = 0; i < 5; i++) {
        scanf("%d", &s.scores[i]);
    }

    s.isDeleted = 0;  

    fprintf(fp, "%d %s %d %d %d %d %d\n", s.roll, s.name, s.scores[0], s.scores[1], s.scores[2], s.scores[3], s.scores[4]);

    printf("Record added successfully!\n");

    fclose(fp);
}

void displayRecords() {
    FILE *fp = fopen(FILENAME, "r");
    if (!fp) {
        printf("Error opening file!\n");
        return;
    }

    Student s;
    printf("\nRoll\tName\tTotal Score\n");
    printf("------------------------------\n");

    while (fscanf(fp, "%d %s %d %d %d %d %d", &s.roll, s.name, &s.scores[0], &s.scores[1], &s.scores[2], &s.scores[3], &s.scores[4]) != EOF) {
        if (s.isDeleted == 0) {  
            int total = calculateTotal(s.scores);
            printf("%d\t%s\t%d\n", s.roll, s.name, total);
        }
    }

    fclose(fp);
}

void searchRecord(int roll) {
    FILE *fp = fopen(FILENAME, "r");
    if (!fp) {
        printf("Error opening file!\n");
        return;
    }

    Student s;
    int found = 0;

    while (fscanf(fp, "%d %s %d %d %d %d %d", &s.roll, s.name, &s.scores[0], &s.scores[1], &s.scores[2], &s.scores[3], &s.scores[4]) != EOF) {
        if (s.roll == roll && s.isDeleted == 0) {
            found = 1;
            int total = calculateTotal(s.scores);
            printf("\nRoll: %d\nName: %s\nScores: %d %d %d %d %d\nTotal Score: %d\n", s.roll, s.name, s.scores[0], s.scores[1], s.scores[2], s.scores[3], s.scores[4], total);
            break;
        }
    }

    if (!found) {
        printf("Record not found!\n");
    }

    fclose(fp);
}

void editRecord(int roll) {
    FILE *fp = fopen(FILENAME, "r+");
    if (!fp) {
        printf("Error opening file!\n");
        return;
    }

    Student s;
    int found = 0;
    long pos;

    while ((pos = ftell(fp)) != -1 && fscanf(fp, "%d %s %d %d %d %d %d", &s.roll, s.name, &s.scores[0], &s.scores[1], &s.scores[2], &s.scores[3], &s.scores[4]) != EOF) {
        if (s.roll == roll && s.isDeleted == 0) {
            found = 1;
            printf("Enter new name: ");
            scanf("%s", s.name);

            printf("Enter new scores in 5 subjects: ");
            for (int i = 0; i < 5; i++) {
                scanf("%d", &s.scores[i]);
            }

            fseek(fp, pos, SEEK_SET);  
            fprintf(fp, "%d %s %d %d %d %d %d\n", s.roll, s.name, s.scores[0], s.scores[1], s.scores[2], s.scores[3], s.scores[4]);

            printf("Record updated successfully!\n");
            break;
        }
    }

    if (!found) {
        printf("Record not found!\n");
    }

    fclose(fp);
}

void logicalDelete(int roll) {
    FILE *fp = fopen(FILENAME, "r+");
    if (!fp) {
        printf("Error opening file!\n");
        return;
    }

    Student s;
    int found = 0;
    long pos;

    while ((pos = ftell(fp)) != -1 && fscanf(fp, "%d %s %d %d %d %d %d", &s.roll, s.name, &s.scores[0], &s.scores[1], &s.scores[2], &s.scores[3], &s.scores[4]) != EOF) {
        if (s.roll == roll && s.isDeleted == 0) {
            found = 1;
            s.isDeleted = 1;  
            printf("Record logically deleted!\n");
            break;
        }
    }

    if (!found) {
        printf("Record not found!\n");
    }

    fclose(fp);
}

void physicalDelete(int roll) {
    FILE *fp = fopen(FILENAME, "r");
    FILE *temp = fopen("temp.txt", "w");
    if (!fp || !temp) {
        printf("Error opening file!\n");
        return;
    }

    Student s;
    int found = 0;

    while (fscanf(fp, "%d %s %d %d %d %d %d", &s.roll, s.name, &s.scores[0], &s.scores[1], &s.scores[2], &s.scores[3], &s.scores[4]) != EOF) {
        if (s.roll == roll) {
            found = 1;
            printf("Record physically deleted!\n");
        } else {
            fprintf(temp, "%d %s %d %d %d %d %d\n", s.roll, s.name, s.scores[0], s.scores[1], s.scores[2], s.scores[3], s.scores[4]);
        }
    }

    if (!found) {
        printf("Record not found!\n");
    }

    fclose(fp);
    fclose(temp);

    remove(FILENAME);
    rename("temp.txt", FILENAME);
}
