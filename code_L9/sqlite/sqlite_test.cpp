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
         "VALUES (4, 'OCBC', 'FINANCE', 15.6, 16.2 );";

   /* Execute SQL statement */
   int rc = sqlite3_exec(db, sql.c_str(), callback, 0, &errMsg);
   
   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", errMsg);
      sqlite3_free(errMsg);
   } else {
      fprintf(stdout, "Records created successfully\n");
   }
}

void selectFromTable(sqlite3* db) {
   char *errMsg = 0;
   const char* data = "Callback function called";
   /* Create SQL statement */
   string sql = "SELECT * FROM STOCK";

   /* Execute SQL statement */
   int rc = sqlite3_exec(db, sql.c_str(), callback, (void *)data, &errMsg);
   
   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", errMsg);
      sqlite3_free(errMsg);
   } else {
      fprintf(stdout, "Records selected successfully\n");
   }
}

void updateTable(sqlite3* db) {
   char *errMsg = 0;
   int rc;
   const char* data = "Callback function called";
   
   /* Create merged SQL statement */
   string sql = "UPDATE STOCK set OPEN = 500.00 where ID=1; " \
         "SELECT * from STOCK";

   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql.c_str(), callback, (void*)data, &errMsg);
   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", errMsg);
      sqlite3_free(errMsg);
   } else {
      fprintf(stdout, "Records udpated successfully\n");
   }
}

void deleteRecords(sqlite3* db) {
   char *errMsg = 0;
   int rc;
   const char* data = "Callback function called";
   
   /* Create merged SQL statement */
   string sql = "DELETE from STOCK where ID=2; " \
         "SELECT * from STOCK";

   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql.c_str(), callback, (void*)data, &errMsg);
   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", errMsg);
      sqlite3_free(errMsg);
   } else {
      fprintf(stdout, "Records deleted successfully\n");
   }
}

int main(int argc, char* argv[]) {

   // example 1: create, open a db (file)
   sqlite3 *db;
   char *errMsg = 0;
   int rc = sqlite3_open("test.db", &db);
   if( rc ) {
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      return(0);
   } else {
      fprintf(stderr, "Opened database successfully\n");
   }
   
   // example 2: create table
   if (true) 
      createTable(db);
   
   // example 3: insert into table
   if (true)
      insertIntoTable(db);

   // example 4: select * from table
   if(true)
      selectFromTable(db);

   // example 5: update operation
   if(true)
      updateTable(db);

   // example 6: delete records
   if(true)
      deleteRecords(db);

   sqlite3_close(db);
}