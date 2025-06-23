#include <stdio.h>
#include <sqlite3.h> 
#include <string>

using namespace std;

static int callback(void *data, int argc, char **argv, char **azColName) {
   int i;
   for(i = 0; i<argc; i++) {
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

void createTable(sqlite3* db) { 
   std::string sql = "CREATE TABLE STOCK("  \
      "ID INT PRIMARY KEY     NOT NULL," \
      "NAME           TEXT    NOT NULL," \
      "INDUSTRY       CHAR(50)," \
      "OPEN           REAL," \
      "CLOSE          REAL );";

   /* Execute SQL statement */
   char *errMsg = 0;
   int rc = sqlite3_exec(db, sql.c_str(), callback, 0, &errMsg);
   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", errMsg);
      sqlite3_free(errMsg);
   } else {
      fprintf(stdout, "Table created successfully\n");
   }
}

void insertIntoTable(sqlite3* db) {
    char *errMsg = 0;
   /* Create SQL statement */
   string sql = "INSERT INTO STOCK (ID,NAME,INDUSTRY,OPEN,CLOSE) "  \
         "VALUES (1, 'APPL', 'TECH', 511.5, 512.0 ); " \
         "INSERT INTO STOCK (ID,NAME,INDUSTRY,OPEN,CLOSE) "  \
         "VALUES (2, 'NVIDA', 'TECH', 2000.5, 1995.2 ); " \
         "INSERT INTO STOCK (ID,NAME,INDUSTRY,OPEN,CLOSE)" \
         "VALUES (3, 'DBS', 'FINANCE', 25.6, 25.4 );" \
         "INSERT INTO STOCK (ID,NAME,INDUSTRY,OPEN,CLOSE)" \
         "VALUES (4, 'OCBC', 'FINANCE', 15.6, 16.2 );" \
         "SELECT * FROM STOCK;";

   /* Execute SQL statement */
   int rc = sqlite3_exec(db, sql.c_str(), callback, 0, &errMsg);
   
   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", errMsg);
      sqlite3_free(errMsg);
   } else {
      fprintf(stdout, "Records created successfully\n");
   }
}

void alterTable(sqlite3* db) {
    char *errMsg = 0;
   /* Create SQL statement */
   string sql = "ALTER TABLE STOCK ADD COLUMN HIGH REAL;" \ 
      "SELECT * FROM STOCK;";
   /* Execute SQL statement */
   int rc = sqlite3_exec(db, sql.c_str(), callback, 0, &errMsg);
   
   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", errMsg);
      sqlite3_free(errMsg);
   } else {
      fprintf(stdout, "SQL executed successfully\n");
   }
}

void dropTable(sqlite3* db) {
   char *errMsg = 0;
   const char* data = "Callback function called";
   /* Create SQL statement */
   string sql = "DROP TABLE STOCK";

   /* Execute SQL statement */
   int rc = sqlite3_exec(db, sql.c_str(), callback, (void *)data, &errMsg);
   
   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", errMsg);
      sqlite3_free(errMsg);
   } else {
      fprintf(stdout, "SQL executed successfully\n");
   }
}

void countRecords(sqlite3* db) {
   char *errMsg = 0;
   int rc;
   const char* data = "Callback function called";
   
   /* Create merged SQL statement */
   string sql = "SELECT COUNT(*) FROM STOCK WHERE INDUSTRY='FINANCE';";

   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql.c_str(), callback, (void*)data, &errMsg);
   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", errMsg);
      sqlite3_free(errMsg);
   } else {
      fprintf(stdout, "SQL executed successfully\n");
   }
}

void groupBy(sqlite3* db) {
   char *errMsg = 0;
   int rc;
   const char* data = "Callback function called";
   
   /* Create merged SQL statement */
   string sql = "SELECT sum(OPEN) from STOCK WHERE INDUSTRY='TECH' " \
   "GROUP BY INDUSTRY;";

   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql.c_str(), callback, (void*)data, &errMsg);
   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", errMsg);
      sqlite3_free(errMsg);
   } else {
      fprintf(stdout, "SQL executed successfully\n");
   }
}

void createIndex(sqlite3* db) {
   char *errMsg = 0;
   int rc;
   const char* data = "Callback function called";
   
   /* Create merged SQL statement */
   string sql = "CREATE INDEX index_industry on STOCK (INDUSTRY COLLATE NOCASE); ";

   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql.c_str(), callback, (void*)data, &errMsg);
   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", errMsg);
      sqlite3_free(errMsg);
   } else {
      fprintf(stdout, "SQL executed successfully\n");
   }
}

int main(int argc, char* argv[]) {

   // example 1: create, open a db (file)
   sqlite3 *db;
   char *errMsg = 0;
   int rc = sqlite3_open("test2.db", &db);
   if( rc ) {
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      return(0);
   } else {
      fprintf(stderr, "Opened database successfully\n");
   }
   
   // example 2: create table
   if (true) 
      createTable(db);
   
   // example 3: count records
   if (true)
      insertIntoTable(db);

   // example 3: count records
   if (true)
      countRecords(db);

   // example 4: group by
   if(true)
      groupBy(db);

   // example 5: create index operation
   if(true)
      createIndex(db);

   // example 6: alter table
   if(true)
      alterTable(db);

   // example 7: drop table
   if(true)
      dropTable(db);

   sqlite3_close(db);
}