#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <postgresql/libpq-fe.h>

using namespace std;

static void
rollback(PGconn *conn, PGresult *res)
{
    res = PQexec(conn, "ROLLBACK");
    exit(1);
}

void insert_data_expl(PGconn *conn){
   PGresult *res;

   res = PQexec(conn, "BEGIN");
   
   res = PQexec(conn,"INSERT INTO public.product VALUES(22,'Test Piphane 22')");
   res = PQexec(conn,"INSERT INTO public.product VALUES(16,'Test Piphane 16')");

   res = PQexec(conn, "COMMIT");
  
   if (PQresultStatus(res) != PGRES_COMMAND_OK)
   {
       std::cout << "Failed to insert: %s" << PQerrorMessage(conn) << std::endl;
      rollback(conn, res);
      PQclear(res);
   }
}

void insert_data_impl(PGconn *conn){
   PGresult *res;
   
   res = PQexec(conn,"INSERT INTO public.product VALUES(20,'Test Piphane 20 impl')");
   res = PQexec(conn,"INSERT INTO public.product VALUES(16,'Test Piphane 16 impl')");

   if (PQresultStatus(res) != PGRES_COMMAND_OK)
   {
       std::cout << "Failed to insert: %s" << PQerrorMessage(conn) << std::endl;
      rollback(conn, res);
      PQclear(res);
   }
}

int main(int argc, char* argv[]) {

   PGconn *conn;
   

   char *sql;


   conn = PQconnectdb("dbname=hw2 host=localhost user=postgres password=postgres");

   /* Check to see that the backend connection was successfully made */
   if (PQstatus(conn) != CONNECTION_OK)
   {
      std::cout << "Connection to database failed: %s" << PQerrorMessage(conn) << std::endl;
      exit(1);
   }

   insert_data_impl(conn);
   insert_data_expl(conn);
}