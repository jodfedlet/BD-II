import re;
import mysql.connector as mc
from mysql.connector import Error

finalTeste = False
tabelaArquivo = 'teste03'
if finalTeste:
    tabelaArquivo = 'teste_final'
arquivo = open(tabelaArquivo, 'r')
arquivolist = list(arquivo)  # cria uma lista com o .txt
REDO = []  # salva quem vai ser feito REDO
# Variaveis p/ identificar se existe no .txt

checkvalue = re.compile(r'T[0-9]*,', re.IGNORECASE) # re.IGNORECASE -> ignorar se maiuscula ou minuscula
commit = re.compile(r'commit', re.IGNORECASE) # re.IGNORECASE -> ignorar se maiuscula ou minuscula
extracT = re.compile(r'(?!commit\b)(?!CKPT\b)(?!Start\b)\b\w+', re.IGNORECASE) # Ignora as palavras descritas e coloca as demais em uma lista com .findall
words = re.compile(r'\w+', re.IGNORECASE) # Utilizado p/ pegar o valor das variaveis

start_checkpoint = re.compile(r'Start CKPT', re.IGNORECASE) # re.IGNORECASE -> ignorar se maiuscula ou minuscula
end_checkpoint = re.compile(r'End CKPT', re.IGNORECASE) # re.IGNORECASE -> ignorar se maiuscula ou minuscula
start = re.compile(r'Start', re.IGNORECASE) # re.IGNORECASE -> ignorar se maiuscula ou minuscula

def db_connection():
    try:
       connection = mc.connect(host='localhost',
                                database='log_tp2',
                                user='root',
                                password='')
       if connection.is_connected():
            db_info = connection.get_server_info()
            print('Connected to mysql server: ', db_info, "\n")
            return connection

    except Error as e:
        print('Failed to connect to the database',e, "\n")
                                   

connection = db_connection()
cursor = connection.cursor()


valores = words.findall(arquivolist[0])
variaveis = {}
for i in range(0,len(valores),2): #Iniciar primeiros valores das variáveis (A B C...)
    variaveis[valores[i]]= valores[i+1]
del valores
print("Variáveis iniciais", variaveis, "\n")
end = False

startedTransaction = []
checkPoinValue = []

for linha in reversed(arquivolist): #Verificar os casos e criar as listas de REDO+
    if end_checkpoint.search(linha):
        end = True
    elif end and start_checkpoint.search(linha): #procura start checkpoint para e se o mesmo é válido
        transacoes = extracT.findall(linha)
        for transacao in transacoes:
            checkPoinValue.append(transacao)
        end = False
        break   
    elif commit.search(linha):  #Procura commit
        REDO.append(extracT.findall(linha)[0]) 
    elif start.search(linha): #verificar transacao que iniciaram
        startedTransaction.append(extracT.findall(linha)[0])  

uncommited = list(set(startedTransaction) - set(REDO))
    
print("Transacoes com checkpoint:", checkPoinValue, "\n")    
print("Aplicado REDO:", REDO, "\n")
print("Não aplicado REDO:", uncommited, "\n")

updatedValue = []

for j in range(1,len(arquivolist)-1,1):
    linha = arquivolist[j]  
    if (checkvalue.search(linha)) and (extracT.findall(linha)[0] in REDO) and not start_checkpoint.search(linha):          
        id = words.findall(linha)[1]
        column = words.findall(linha)[2]
        value = words.findall(linha)[3]

        sqlSelect = "select "+column+" from "+tabelaArquivo+" WHERE id = "+id
        print("-------------------------------------")
        print(sqlSelect)
        cursor.execute(sqlSelect)
        dbValue = list(cursor.fetchone())[0]

        #if dbValue != int(value):
            
        print("Valores de {0}: antigo -> {1} atual -> {2}".format(column, dbValue, value))
        sqlUpdate = "update "+tabelaArquivo+" SET "+column+" = "+value+" WHERE id = "+id
        print(sqlUpdate)
        print("-------------------------------------")
        print()

        cursor.execute(sqlUpdate)
        connection.commit()

        updatedValue.append(column)

        variaveis[column] = value
   

updatedValue = list(set(updatedValue)) #remover valores duplicados

print("Dados Atualizados:", updatedValue)
print("Resultado:", variaveis)
arquivo.close()

if connection.is_connected():
    cursor.close()
    connection.close()
