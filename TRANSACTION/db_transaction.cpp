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
#include <string>
#include <bits/stdc++.h>
#include <postgresql/libpq-fe.h>

using namespace std;
using std::string;

void rollback(PGconn *conn, PGresult *res){
    res = PQexec(conn, "ROLLBACK");
    exit(1);
}

void insert_data_expl(PGconn *conn){
   PGresult *res;

   fstream file;
   std::string line = "";

   file.open("data.csv", ios::in);

   if(!file.is_open()){
      cout << "Unable to open file"<< endl;
      exit(1);
   }

   int count = 0, countLine = 10005, firstLine = 0;

   res = PQexec(conn, "BEGIN");
   if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "BEGIN command failed: %s", PQerrorMessage(conn));
        PQclear(res);
        exit(1);
    }

   while (getline (file,line) )
   {         
      if(firstLine == 0){
         firstLine = 1;
      }
      else{
         vector<string> v;

         stringstream ss(line);
      
         while (ss.good()) {
            string substr;
            getline(ss, substr, ',');
            v.push_back(substr);
         }

         for (size_t i = 0; i < v.size(); i++){
            v[0].erase(std::remove(v[0].begin(),v[0].end(),'\"'),v[0].end());
            v[0].erase(std::remove(v[0].begin(),v[0].end(),'\''),v[0].end());

            int id = countLine+1;

            stringstream ss;
            ss << id;
            string idToString, first, endpart, sql;

            idToString = ss.str();

            first = "INSERT INTO public.product VALUES("+idToString+","+"\'";

            endpart = "\')";
            sql = ""+first+ ""+v[0]+""+endpart;

            cout << sql << endl;

            res = PQexec(conn,sql.c_str());

            if (PQresultStatus(res) != PGRES_COMMAND_OK)
            {
               std::cout << "Failed to insert: %s" << PQerrorMessage(conn) << std::endl;
               rollback(conn, res);
               PQclear(res);
            }
            count++;
            countLine++;
            break;
         }
      }
   }
   res = PQexec(conn, "COMMIT");
   if (PQresultStatus(res) != PGRES_COMMAND_OK){
      std::cout << "Failed to insert: %s" << PQerrorMessage(conn) << std::endl;
      rollback(conn, res);
      PQclear(res);
   }
   std::cout << count << std::endl;
   
   file.close();
}

void insert_data_impl(PGconn *conn){

   PGresult *res;
   fstream file;
   std::string line = "";
  
   file.open("data.csv", ios::in);

    if(!file.is_open()){
      cout << "Unable to open file"<< endl;
      exit(1);
   }
  
   int count = 0, countLine = 0, firstLine = 0;

   while (getline (file,line) )
   {         
      if(firstLine == 0){
         firstLine = 1;
      }
      else{
         vector<string> v;

         stringstream ss(line);
      
         while (ss.good()) {
            string substr;
            getline(ss, substr, ',');
            v.push_back(substr);
         }
      
         for (size_t i = 0; i < v.size(); i++){
            v[0].erase(std::remove(v[0].begin(),v[0].end(),'\"'),v[0].end());
            v[0].erase(std::remove(v[0].begin(),v[0].end(),'\''),v[0].end());

            int id = count+1;

            stringstream ss;
            ss << id;
            string idToString, first, endpart, sql;

            idToString = ss.str();

            first = "INSERT INTO public.product VALUES("+idToString+","+"\'";

            endpart = "\')";
            sql = ""+first+ ""+v[0]+""+endpart;

            cout << sql << endl;

            res = PQexec(conn,sql.c_str());

            if (PQresultStatus(res) != PGRES_COMMAND_OK)
            {
               std::cout << "Failed to insert: %s" << PQerrorMessage(conn) << std::endl;
               PQclear(res);
            }
            count++;
            break;
         }
      }
   }
   std::cout << count << std::endl;
   
   file.close();
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