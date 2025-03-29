#include "database.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME_SIZE 256

// Функция для чтения строки
void readLine(char *buffer, size_t size) {
  if (fgets(buffer, size, stdin)) {
    buffer[strcspn(buffer, "\n")] = '\0';
  }
}

void printMenu() {
  printf("\n=== Menu ===\n");
  printf("1. Print database\n");
  printf("2. Add record\n");
  printf("3. Delete record\n");
  printf("4. Save database\n");
  printf("5. Load database\n");
  printf("6. Sort database\n");
  // printf("7. Display bar chart (Value)\n");
  // printf("8. Display pie chart (Value)\n");
  printf("9. Exit\n");
  printf("Choose an option: ");
}

int main(int argc, char *argv[]) {
  Database db;
  initDatabase(&db);
  char filename[FILENAME_SIZE];
  int choice;

  while (1) {
    printMenu();
    if (scanf("%d", &choice) != 1) {
      printf("Invalid input. Exiting...\n");
      break;
    }
    getchar();

    switch (choice) {
    case 1:
      printDatabase(&db);
      break;
    case 2: {
      Record rec;
      printf("Enter ID (integer): ");
      scanf("%d", &rec.id);
      getchar();
      printf("Enter Name (max %d characters): ", NAME_SIZE - 1);
      readLine(rec.name, NAME_SIZE);
      printf("Enter Value (floating point): ");
      scanf("%lf", &rec.value);
      getchar();
      addRecord(&db, &rec);
      break;
    }
    case 3: {
      size_t index;
      printf("Enter index to delete: ");
      scanf("%zu", &index);
      getchar();
      deleteRecord(&db, index);
      break;
    }
    case 4:
      printf("Enter filename to save: ");
      readLine(filename, FILENAME_SIZE);
      if (saveDatabase(&db, filename))
        printf("Database saved successfully.\n");
      break;
    case 5:
      printf("Enter filename to load: ");
      readLine(filename, FILENAME_SIZE);
      freeDatabase(&db);
      initDatabase(&db);
      if (loadDatabase(&db, filename))
        printf("Database loaded successfully.\n");
      break;
    case 6: {
      int field, direction;
      printf("Sort by field (1 - ID, 2 - Name, 3 - Value): ");
      scanf("%d", &field);
      printf("Direction (1 - Ascending, 0 - Descending): ");
      scanf("%d", &direction);
      getchar();
      sortDatabase(&db, field, direction);
      printf("Database sorted.\n");
      break;
    }
    case 9:
      freeDatabase(&db);
      return 0;
    default:
      printf("Invalid option.\n");
    }
  }

  freeDatabase(&db);
  return 0;
}
