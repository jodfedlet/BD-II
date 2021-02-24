create table employee(
	id int,
	name varchar(50),
	bithDate int,
	salary float
);

insert into employee values(1,'fulano 1','23',1000.05),
			values(2,'fulano 2','33',5200.55),
			values(3,'fulano 3','24',4000.55),
			values(4,'fulano 4','26',1200.55);
	

a)

CREATE OR REPLACE FUNCTION aumento() RETURN VOID AS $$

BEGIN
	alter table employee SET salary = salary + salary*0.1 where id IN (select id from employee)

END;
$$ LANGUAGE plpgsql;

select aumento();

b)

CREATE OR REPLACE FUNCTION aumentoX(X float, N int) RETURN VOID AS $$

BEGIN
	alter table employee SET salary = salary + salary*X/100 where id IN (select id from employee where id > N)

END;
$$ LANGUAGE plpgsql;

select aumentoX(20.2,2);

c)

CREATE OR REPLACE FUNCTION remocao() RETURN VOID AS $$

BEGIN
	delete from employee  where salary > (select avg(salary) from employee)

END;
$$ LANGUAGE plpgsql;

select remocao();

d)
alter table employee add current_user varchar(50);
alter table employee add current_date date;

CREATE OR REPLACE FUNCTION insereDados(id int, name varchar, birthDate int, salary float) RETURN VOID AS $$

BEGIN
	insert into employee(id, name, birthDate, salary, CURRENT_USER,CURRENT_DATE);
END;
$$ LANGUAGE plpgsql;

select insereDados(5,'fulano de tal','26',1200.55);
