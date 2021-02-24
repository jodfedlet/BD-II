CREATE or REPLACE FUNCTION getText(texto varchar) RETURNS void AS  $$

BEGIN
     RETURN texto;  
END; 
$$ LANGUAGE plpgsql;


-- select getText();
