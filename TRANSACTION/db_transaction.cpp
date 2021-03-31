/*
TO compile  g++ db_transaction.cpp -o test -lpq
./test 1 or 0  //0 for explicit and 1 for implicit 

*/

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <chrono>
#include <fstream>
#include <ctime>
#include <postgresql/libpq-fe.h>

using namespace std;

void rollback(PGconn *conn, PGresult *res){
    res = PQexec(conn, "ROLLBACK");
    exit(1);
}

void insert_data_expl(PGconn *conn){
   PGresult *res;

   fstream file;
   std::string line = "";

   file.open("data.csv", ios::in);
  

   if (file.is_open()){
      res = PQexec(conn, "BEGIN");
      while (getline (file,line) )
      {
         string sql = "INSERT INTO public.product VALUES("+line+")";
         res = PQexec(conn,sql.c_str());
      }
      res = PQexec(conn, "COMMIT");
   
      if (PQresultStatus(res) != PGRES_COMMAND_OK)
      {
         std::cout << "Failed to insert: %s" << PQerrorMessage(conn) << std::endl;
         rollback(conn, res);
         PQclear(res);
      }
      file.close();
   }

   else cout << "Unable to open file";
}

void insert_data_impl(PGconn *conn){

   PGresult *res;
   fstream file;
   std::string line = "";

   file.open("data.csv", ios::in);
  
   if (file.is_open()){
      while (getline (file,line) )
      {
         string sql = "INSERT INTO public.product VALUES("+line+")";
         res = PQexec(conn,sql.c_str());
      }
    
      if (PQresultStatus(res) != PGRES_COMMAND_OK)
      {
         std::cout << "Failed to insert: %s" << PQerrorMessage(conn) << std::endl;
         PQclear(res);
      }
      file.close();
   }

   else cout << "Unable to open file";
}

int main(int argc, char* argv[]) {

   PGconn *conn;

   int option;
   
   char *sql;

   if(argc != 2){
      std::cout << "Escolhe uma opcao" << std::endl;
      exit(1);

   }else{

      option = atoi(argv[1]);
      if(option != 1 && option != 0){
         std::cout << "Opcao invahlida" << std::endl;
         exit(1);
      }

      conn = PQconnectdb("dbname=hw2 host=localhost user=postgres password=postgres");

      if (PQstatus(conn) != CONNECTION_OK){
         std::cout << "Connection to database failed: %s" << PQerrorMessage(conn) << std::endl;
         exit(1);
      }else{
         std::cout << "Connected successfully: %s" << PQerrorMessage(conn) << std::endl;
      }

      auto start = std::chrono::system_clock::now();
      if(option == 1){
         insert_data_impl(conn);
      }else{
         insert_data_expl(conn);
      }
      
      auto end = std::chrono::system_clock::now();
      std::chrono::duration<double> elapsed_seconds = end-start;
      std::cout << "Tempo de execução: " << elapsed_seconds.count() << "s\n";
   }
}