CREATE or REPLACE FUNCTION incrementNumber(numero int) RETURNS int AS $$
BEGIN 
    RETURN numero+1;
END;
$$ LANGUAGE plpgsql; 