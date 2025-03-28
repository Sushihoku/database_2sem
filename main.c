#include "database.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printMenu() {
  printf("\n=== Menu ===\n");
  printf("1. Print database\n");
  printf("2. Add record\n");
  printf("3. Delete record\n");
  printf("4. Save database\n");
  printf("5. Load database\n");
  printf("6. Sort database\n");
  printf("7. Display bar chart (Value)\n");
  printf("8. Display pie chart (Value)\n");
  printf("9. Exit\n");
  printf("Choose an option: ");
}

int main(int argc, char *argv[]) {
  Database db;
  initDatabase(&db);
  char filename[256];
  int choice;

  while (1) {
    printMenu();
    if (scanf("%d", &choice) != 1)
      break;
    getchar(); // очистка символа новой строки

    if (choice == 1) {
      printDatabase(&db);
    } else if (choice == 2) {
      Record rec;
      printf("Enter ID (integer): ");
      scanf("%d", &rec.id);
      getchar();
      printf("Enter Name (max %d characters): ", NAME_SIZE - 1);
      fgets(rec.name, NAME_SIZE, stdin);
      // Удаление символа новой строки, если есть
      char *p = rec.name;
      while (*p) {
        if (*p == '\n') {
          *p = '\0';
          break;
        }
        p++;
      }
      printf("Enter Value (floating point): ");
      scanf("%lf", &rec.value);
      getchar();
      addRecord(&db, &rec);
    } else if (choice == 3) {
      size_t index;
      printf("Enter index to delete: ");
      scanf("%zu", &index);
      getchar();
      deleteRecord(&db, index);
    } else if (choice == 4) {
      printf("Enter filename to save: ");
      fgets(filename, sizeof(filename), stdin);
      filename[strcspn(filename, "\n")] = '\0';
      if (saveDatabase(&db, filename))
        printf("Database saved successfully.\n");
    } else if (choice == 5) {
      printf("Enter filename to load: ");
      fgets(filename, sizeof(filename), stdin);
      filename[strcspn(filename, "\n")] = '\0';
      freeDatabase(&db);
      initDatabase(&db);
      if (loadDatabase(&db, filename))
        printf("Database loaded successfully.\n");
    } else if (choice == 6) {
      int field, direction;
      printf("Sort by field (1 - ID, 2 - Name, 3 - Value): ");
      scanf("%d", &field);
      printf("Direction (1 - Ascending, 0 - Descending): ");
      scanf("%d", &direction);
      getchar();
      sortDatabase(&db, field, direction);
      printf("Database sorted.\n");
      /* } else if (choice == 7) {
         displayBarChart(&db, 3);
       } else if (choice == 8) {
         displayPieChart(&db, 3); */
    } else if (choice == 9) {
      break;
    } else {
      printf("Invalid option.\n");
    }
  }

  freeDatabase(&db);
  return 0;
}
