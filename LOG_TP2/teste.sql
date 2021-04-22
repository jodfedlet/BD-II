create table teste03(
	id int(11) AUTO_INCREMENT PRIMARY KEY,
	A int,
    B int,
    C int,
    D int,
    E int,
    F int	
);

insert into teste03 values(1,20,20,70,50,17,1);

update teste03 set A = 20, B = 20, C = 70, D = 50, E = 17, F = 1 WHERE id = 1; /*Resetar os valores depois da execução do log*/


create table teste_final(
	id int(11) AUTO_INCREMENT PRIMARY KEY,
	A int,
    B int,
    C int,
    D int,
    E int,
    F int,
    X int,
    W int	
);

insert into teste_final values(1,32,30,90,40,28,2,0,0);

update teste_final set A = 32, B = 30, C = 90, D = 40, E = 28, F = 2,X = 0, W = 0 WHERE id = 1; /*Resetar os valores depois da execução do log*/






