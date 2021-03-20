/*
2.1

A partir do banco de dados hw2 (que deve ser carregado do arquivo “trigger AT1” presente no moodle), construa gatilhos para as seguintes funcionalidades:
2.1- se o produto em estoque chegar ao valor 0 (não tem mais o produto) então o mesmo deve ser removido das tabelas Product e Stock. A aplicação faz o controle para o caso de quantity<0. 


*/


CREATE OR REPLACE FUNCtION process_remove_data() RETURNS TRIGGER AS $$
DECLARE 
    quantidade int;
BEGIN

    SELECT quantity 
    INTO quantidade 
    FROM public.stock AS st 
    LEFT JOIN public.product AS pd 
    ON st.eid = pd.eid;

    IF ((quantidade < 0 or quantidade IS NULL)  AND (TG_OP = 'UPDATE' OR TG_OP = 'DELETE')) THEN
        IF (TG_OP = 'UPDATE') THEN 
            DELETE FROM public.stock WHERE eid = NEW.eid;
            DELETE FROM public.product WHERE eid = NEW.eid;
            RETURN NEW;  
        ELSE
            DELETE FROM public.stock WHERE eid = OLD.eid;
            DELETE FROM public.product WHERE eid = OLD.eid;
            RETURN OLD;
        END IF;    
    ELSE
        RETURN NULL;
    END IF;    

    RETURN NULL;  

END;
$$ LANGUAGE plpgsql; 

CREATE TRIGGER remove_data AFTER UPDATE OR DELETE ON public.stock
FOR EACH ROW EXECUTE PROCEDURE process_remove_data();

/**
Exemplos de testes
*/

a)
INSERT INTO public.product VALUES(1,'Teste update');
INSERT INTO public.stock VALUES(1,1);

UPDATE public.stock SET quantity = -1 WHERE eid = 1; 

b) 

INSERT INTO public.product VALUES(2,'Teste delete');
INSERT INTO public.stock VALUES(2,1);

DELETE FROM public.stock WHERE eid = 2;



