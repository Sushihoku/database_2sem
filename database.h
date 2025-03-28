#ifndef DATABASE_H
#define DATABASE_H
#include <stdio.h>

#define NAME_SIZE 50

typedef struct {
  char name[NAME_SIZE];
  double value;
  int id;
} Record;

typedef struct {
  Record *records;
  size_t size;
  size_t capacity;
} Database;

void initDatabase(Database *db);
void freeDatabase(Database *db);
void printDatabase(const Database *db);
void addRecord(Database *db, const Record *rec);
void deleteRecord(Database *db, size_t index);
int saveDatabase(const Database *db, const char *filename);
int loadDatabase(Database *db, const char *filename);
void sortDatabase(Database *db, int field, int ascending);
void displayBarChart(const Database *db);
void displayPieChart(const Database *db);

#endif // DATABASE_H
