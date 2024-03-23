#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STUDENTS 100

struct Student {
    int studentNumber;
    char name[50];
    char surname[50];
    char department[50];
};

int numStudents = 0;

struct Student students[MAX_STUDENTS];

void addStudentToFile(struct Student newStudent) {
    FILE *file = fopen("students.txt", "a");
    if (file == NULL) {
        printf("Dosya açilamadi!\n");
        exit(1);
    }
    fprintf(file, "%d %s %s %s\n", newStudent.studentNumber, newStudent.name, newStudent.surname, newStudent.department);
    fclose(file);
}
void readStudentsFromFile() {
    FILE *file = fopen("students.txt", "r");
    if (file == NULL) {
        printf("Dosya açilamadi!\n");
        exit(1);
    }

    while (fscanf(file, "%d %s %s %s", &students[numStudents].studentNumber, students[numStudents].name, students[numStudents].surname, students[numStudents].department) != EOF) {
        numStudents++;
    }

    fclose(file);
}
void searchStudentByNumber(int studentNumber) {
    readStudentsFromFile();
    int i;
    for (i = 0; i < numStudents; i++) {
        if (students[i].studentNumber == studentNumber) {
            printf("Ogrenci Bilgileri:\n");
            printf("Ogrenci No: %d\n", students[i].studentNumber);
            printf("Adi: %s\n", students[i].name);
            printf("Soyadi: %s\n", students[i].surname);
            printf("Bolumu: %s\n", students[i].department);
            return;
        }
    }
    printf("Ogrenci bulunamadi.\n");
}

void updateStudent(int studentNumber, const char *name, const char *surname, const char *department) {
    FILE *file = fopen("students.txt", "r");
    FILE *tempFile = fopen("temp.txt", "w");
    if (file == NULL || tempFile == NULL) {
        printf("Dosya açılamadı!\n");
        exit(1);
    }
    int found = 0;
    char line[150];
    while (fgets(line, sizeof(line), file)) {
        int currentStudentNumber;
        sscanf(line, "%d", &currentStudentNumber);

        if (currentStudentNumber == studentNumber) {
            fprintf(tempFile, "%d %s %s %s\n", studentNumber, name, surname, department);
            found = 1;
        } else {
            fprintf(tempFile, "%s", line);
        }
    }
    fclose(file);
    fclose(tempFile);
    if (!found) {
        printf("Öğrenci bulunamadı.\n");
        remove("temp.txt");
        return;
    }
    remove("students.txt");
    rename("temp.txt", "students.txt");

    printf("Öğrenci bilgileri güncellendi.\n");
}
void deleteStudent(int studentNumber) {
    FILE *file = fopen("students.txt", "r");
    FILE *tempFile = fopen("temp.txt", "w");
    if (file == NULL || tempFile == NULL) {
        printf("Dosya açılamadı!\n");
        exit(1);
    }

    int found = 0;
    char line[150];
    while (fgets(line, sizeof(line), file)) {
        int currentStudentNumber;
        sscanf(line, "%d", &currentStudentNumber);

        if (currentStudentNumber != studentNumber) {
            fprintf(tempFile, "%s", line);
        } else {
            found = 1;
        }
    }
    fclose(file);
    fclose(tempFile);
    if (!found) {
        printf("Öğrenci bulunamadı.\n");
        remove("temp.txt");
        return;
    }
    remove("students.txt");
    rename("temp.txt", "students.txt");
    printf("Öğrenci başarıyla silindi.\n");
}

int main() {
    int choice, studentNumber;
    char name[50], surname[50], department[50];
    struct Student newStudent;

    while (1) {
        printf("\n1. Yeni ogrenci ekle\n");
        printf("2. Ogrenci sorgula (dogrudan erisim)\n");
        printf("3. Ogrenci guncelle\n");
        printf("4. Ogrenci sil\n");
        printf("5. Cikis\n");
        printf("Seciminizi yapiniz: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Ogrenci numarasi: ");
                scanf("%d", &newStudent.studentNumber);
                printf("Adi: ");
                scanf("%s", newStudent.name);
                printf("Soyadi: ");
                scanf("%s", newStudent.surname);
                printf("Bolumu: ");
                scanf("%s", newStudent.department);
                addStudentToFile(newStudent);
                printf("Ogrenci basariyla eklendi.\n");
                break;
            case 2:
                printf("Ogrenci numarasini giriniz: ");
                scanf("%d", &studentNumber);
                searchStudentByNumber(studentNumber);
                break;
            case 3:
                printf("Guncellenecek ogrenci numarasi: ");
                scanf("%d", &studentNumber);
                printf("Yeni adi: ");
                scanf("%s", name);
                printf("Yeni soyadi: ");
                scanf("%s", surname);
                printf("Yeni bolumu: ");
                scanf("%s", department);

                updateStudent(studentNumber, name, surname, department);
                break;
            case 4:
                printf("Silinecek ogrenci numarasi: ");
                scanf("%d", &studentNumber);
                deleteStudent(studentNumber);
                break;        
            case 5:
                exit(0);
            default:
                printf("Gecersiz secim!\n");
        }
    }

    return 0;
}
