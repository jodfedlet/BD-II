CREATE or REPLACE FUNCTION getText(texto varchar) RETURNS void AS  $$

BEGIN
     RAISE NOTICE texto;  
END; 
$$ LANGUAGE plpgsql;


-- select getText();