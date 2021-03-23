/*
To compile the code, use the following command
g++ db_transaction.cpp -o output -L/usr/include/mysql/mysql  -lmysqlclient && ./output
*/


#include <iostream>
#include <mysql/mysql.h> 

using namespace std;

struct connection_details{
    const char *server, *pass, *database, *user;
};

MYSQL* mysql_connection_setup(struct connection_details mysql_details){
    MYSQL *connection = mysql_init(NULL);

    if (!mysql_real_connect(connection, mysql_details.server, mysql_details.user,
     mysql_details.pass, mysql_details.database, 0,NULL,0)
     ){
        std::cout << "Connection failed: " << mysql_error(connection) << std::endl;
        exit(1);
    }
    return connection;
    
}

MYSQL_RES* mysql_execute_query(MYSQL *connection, const char *sql_query){
    if(mysql_query(connection, sql_query)){
        std::cout << "Mysql Query Error: "<< mysql_error(connection) << std::endl;
    }

    return mysql_use_result(connection);
}

int main(int argc, char* argv[]) {
  
  MYSQL *conn;
  MYSQL_RES *res;
  MYSQL_ROW row;


  struct connection_details mysql_data;
  mysql_data.server = "localhost";
  mysql_data.user = "root";
  mysql_data.pass = "root";
  mysql_data.database = "Nuvann";

  conn = mysql_connection_setup(mysql_data);
  res = mysql_execute_query(conn, "Select * from products");

  std::cout << "Display data: \n" << std::endl;

  while ((row = mysql_fetch_row(res)) != NULL)
  {
      std::cout << row[0] << " | "<< row[1] << " | "<< row[2] << " | "<< row[3] << " | "<< row[4] << 
      std::endl << std::endl;
  }

  mysql_free_result(res);
  mysql_close(conn);
  
  return 0;
}