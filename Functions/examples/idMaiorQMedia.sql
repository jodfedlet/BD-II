drop table users if exists;
create table users (id int, nome varchar(50));
insert into users values(1,'Teste1');
insert into users values(2,'Teste2');
insert into users values(3,'Teste3');
insert into users values(4,'Teste4');
insert into users values(5,'Teste5');


CREATE or REPLACE FUNCTION idMaiorQMedia() RETURNS TABLE(nome varchar(50)) AS $$
DECLARE
    media float;
BEGIN 
    SELECT AVG(id) INTO media from users;
    RAISE NOTICE 'id %',media;
    RETURN QUERY SELECT users.nome FROM users WHERE id > media;
END;
$$ LANGUAGE plpgsql;        