CREATE or REPLACE FUNCTION getText(texto varchar) RETURNS varchar AS  $$

BEGIN
     RETURN texto;  
END; 
$$ LANGUAGE plpgsql;


-- select getText();
