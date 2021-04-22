import re;
import mysql.connector as mc
from mysql.connector import Error

arquivo = open('teste03', 'r')
arquivolist = list(arquivo)  # cria uma lista com o .txt
REDO = []  # salva quem vai ser feito REDO
redoList = []  # salva possível transação a aplicar redo.
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
            return connection;

    except Error as e:
        print('Failed to connect to the database',e, "\n")
                                   

connection = db_connection()
cursor = connection.cursor()

cursor.execute("select * from teste03")
res = cursor.fetchone();
#print(res)    

valores = words.findall(arquivolist[0])
variaveis = {}
for i in range(0,len(valores),2): #Iniciar primeiros valores das variáveis (A B C...)
    variaveis[valores[i]]= valores[i+1]
del valores
print("Variáveis iniciais", variaveis, "\n")
end = 0

startedTransaction = []

for linha in reversed(arquivolist): #Verificar os casos e criar as listas de REDO
    # if commit.search(linha):  #Procura commit
    #     REDO.append(extracT.findall(linha)[0])

    if end_checkpoint.search(linha):
        end = 1
    elif end and start_checkpoint.search(linha): #procura start checkpoint para 
        end = 0
        break   
    elif commit.search(linha):  #Procura commit
        REDO.append(extracT.findall(linha)[0]) 
    elif start.search(linha): #verificar transacao que iniciaram
        startedTransaction.append(extracT.findall(linha)[0])  

uncommited = list(set(startedTransaction) - set(REDO))
    
print("Aplicado REDO:", REDO, "\n")
print("Não aplicado REDO:", uncommited, "\n")

for j in range(1,len(arquivolist)-1,1):
    linha = arquivolist[j]  
    if (checkvalue.search(linha)) and (extracT.findall(linha)[0] in REDO) and not start_checkpoint.search(linha):          
        variaveis[words.findall(linha)[2]] = words.findall(linha)[3]
   

print("Resultado:", variaveis)
arquivo.close()

if connection.is_connected():
    cursor.close()
    connection.close()
