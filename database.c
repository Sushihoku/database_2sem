#include "database.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 10

// Инициализация базы данных
void initDatabase(Database *db) {
  db->records = (Record *)malloc(INITIAL_CAPACITY * sizeof(Record));
  db->size = 0;
  db->capacity = INITIAL_CAPACITY;
}

// Освобождение ресурсов базы данных
void freeDatabase(Database *db) {
  free(db->records);
  db->records = NULL;
  db->size = 0;
  db->capacity = 0;
}

// Вывод таблицы базы данных
void printDatabase(const Database *db) {
  printf("Index | ID   | Name                         | Value\n");
  printf("--------------------------------------------------------\n");
  for (size_t i = 0; i < db->size; i++) {
    Record *r = db->records + i; // арифметика указателей
    printf("%5zu | %4d | %-28s | %lf\n", i, r->id, r->name, r->value);
  }
}

// Добавление нового элемента в базу
void addRecord(Database *db, const Record *rec) {
  if (db->size == db->capacity) {
    db->capacity *= 2;
    db->records = (Record *)realloc(db->records, db->capacity * sizeof(Record));
  }
  *(db->records + db->size) = *rec;
  db->size++;
}

// Удаление элемента по индексу
void deleteRecord(Database *db, size_t index) {
  if (index >= db->size) {
    printf("Invalid index.\n");
    return;
  }
  for (size_t i = index; i < db->size - 1; i++) {
    *(db->records + i) = *(db->records + i + 1);
  }
  db->size--;
}

// Сохранение базы в текстовый файл (каждая запись на отдельной строке:
// id,name,value)
int saveDatabase(const Database *db, const char *filename) {
  FILE *fp = fopen(filename, "w");
  if (!fp) {
    perror("Cannot open file for writing");
    return 0;
  }
  for (size_t i = 0; i < db->size; i++) {
    Record *r = db->records + i;
    fprintf(fp, "%d,%s,%lf\n", r->id, r->name, r->value);
  }
  fclose(fp);
  return 1;
}

// Загрузка базы из текстового файла
int loadDatabase(Database *db, const char *filename) {
  FILE *fp = fopen(filename, "r");
  if (!fp) {
    perror("Cannot open file for reading");
    return 0;
  }
  // Перед загрузкой очищаем базу
  db->size = 0;
  char line[256];
  while (fgets(line, sizeof(line), fp)) {
    Record rec;
    if (sscanf(line, "%d,%49[^,],%lf", &rec.id, rec.name, &rec.value) == 3) {
      addRecord(db, &rec);
    }
  }
  fclose(fp);
  return 1;
}

// Функции сравнения для сортировки
static int compareByIDAsc(const void *a, const void *b) {
  const Record *r1 = (const Record *)a;
  const Record *r2 = (const Record *)b;
  return r1->id - r2->id;
}
static int compareByIDDesc(const void *a, const void *b) {
  return compareByIDAsc(b, a);
}

static int compareByNameAsc(const void *a, const void *b) {
  const Record *r1 = (const Record *)a;
  const Record *r2 = (const Record *)b;
  return strcmp(r1->name, r2->name);
}
static int compareByNameDesc(const void *a, const void *b) {
  return compareByNameAsc(b, a);
}

static int compareByValueAsc(const void *a, const void *b) {
  const Record *r1 = (const Record *)a;
  const Record *r2 = (const Record *)b;
  if (r1->value < r2->value)
    return -1;
  else if (r1->value > r2->value)
    return 1;
  return 0;
}
static int compareByValueDesc(const void *a, const void *b) {
  return compareByValueAsc(b, a);
}

/*
  field: 1 - id, 2 - name, 3 - value.
  ascending: 1 для по возрастанию, 0 для убывания.
*/
void sortDatabase(Database *db, int field, int ascending) {
  int (*cmp)(const void *, const void *) = NULL;
  if (field == 1) {
    cmp = ascending ? compareByIDAsc : compareByIDDesc;
  } else if (field == 2) {
    cmp = ascending ? compareByNameAsc : compareByNameDesc;
  } else if (field == 3) {
    cmp = ascending ? compareByValueAsc : compareByValueDesc;
  } else {
    printf("Invalid field for sorting.\n");
    return;
  }
  qsort(db->records, db->size, sizeof(Record), cmp);
}
