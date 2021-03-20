CREATE or REPLACE FUNCTION somefunc() RETURNS void AS  $$
DECLARE
    quantity int := 30;
BEGIN
    RAISE NOTICE 'Quantidade é  %', quantity;  
END; $$
LANGUAGE plpgsql;


select somefunc();
