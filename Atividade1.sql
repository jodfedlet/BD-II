a)

CREATE OR REPLACE FUNCTION 	aumento() RETURN VOID AS $$

BEGIN
	alter table employee SET salary = salary + salary*0.1 where id IN (select id from employee)

END;
$$ LANGUAGE plpgsql;

b)

CREATE OR REPLACE FUNCTION 	aumentoX(X float, N int) RETURN VOID AS $$

BEGIN
	alter table employee SET salary = salary + salary*X/100 where id IN (select id from employee where id > N)

END;
$$ LANGUAGE plpgsql;

c)

CREATE OR REPLACE FUNCTION 	remocao() RETURN VOID AS $$

BEGIN
	delete from employee  where salary > (select avg(salary) from employee)

END;
$$ LANGUAGE plpgsql;

d)
alter table employee add current_user varchar(50);
alter table employee add current_date date;

CREATE OR REPLACE FUNCTION 	insereDados() RETURN VOID AS $$

BEGIN
	insert into employee(id, name, birthDate, salary, CURRENT_USER,CURRENT_DATE);
END;
$$ LANGUAGE plpgsql;
