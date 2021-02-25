drop table dados if exists;
create table dados (name varchar(50),  salary float, id int);
insert into dados values('Teste',26.5,12);


CREATE FUNCTION selecionar(p_itemno int)
RETURNS TABLE(name varchar(50),  salary float) AS $$
BEGIN
    RETURN QUERY SELECT s.name, s.salary FROM dados AS s
                 WHERE s.id = p_itemno;
END;
$$ LANGUAGE plpgsql;
