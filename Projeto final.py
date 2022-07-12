"""Trabalho realizado por Alexandre Ferreira
alexandre.bruno.ferreira@tecnico.ulisboa.pt
ist1103397
MBL"""
def corrigir_palavra(new):
    """
      corrigir_palavra: cad.caracteres -> cad. caracteres
    Esta funcao corrige as palvras eliminando os surtos
    :param new: Entrada com surtos
    :return: Saida sem surtos
    MBL
    """
    new = list(new)
    count = 1
    while count != 0:
        count = 0
        for i in range(len(new)):
            if i < len(new) - 1 and abs(ord(new[i])-ord((new[i + 1]))) == 32 : #32 e a distancia entre uma letra maiscula
                del new[i:i+2]                                                 #e minuscula em ASCII
                count += 1
        cad_caracteres_final = "".join(new)
    return (cad_caracteres_final)

def eh_anagrama(str_1,str_2):
    """
     eh_anagrama: cad.caracteres x cad.caracteres -> boolean
     Esta funcao compara duas palvras e verifica se sao anagramas
    :param str_1: Palvra 1 para comparar
    :param str_2: Palavra 2 para comparar
    :return: Boolean
    MBL
    """
    str_1 = sorted(str_1.lower())
    str_2 = sorted(str_2.lower())
    for x in range(len(str_1)):
            if str_1[x] == str_2[x] and len(str_1) == len(str_2):
                boolean0 = True
            else:
                boolean0 = False
                break
    return boolean0



def corrigir_doc(arg):
    """
    corrigir doc: cad.caracteres -> cad.caracteres
    Esta funcao corrige a entrada bdb
    :param arg: Entrada bdb corrompida
    :return: Entrada bdb "limpa"
    MBL
    """
    if not isinstance(arg, str):
        raise ValueError("corrigir_doc: argumento invalido")
    arg = corrigir_palavra(arg)
    if not arg.replace(" ","").isalpha() == True:
        raise ValueError("corrigir_doc: argumento invalido")
    res = []
    arg = arg.split() # transformar o arg numa lista em que cada elemento e uma palavra, para comparar
    final = ""
    while len(arg) > 0:
        todel = []
        for i in range(len(arg)):
            if eh_anagrama(arg[0], arg[i]) and not arg[0].lower() == arg[i].lower():
                todel.append(arg[i])
        res.append(arg.pop(0))
        for j in todel:
            arg.remove(j)
        final = " ".join(res)
    return final
def obter_posicao(mov, posat):
    """
    obter_posicao: cad.caracteres x inteiro -> inteiro
    Esta funcao calcula a posicao no keypad depois do movimento inserido
    :param mov: Movimento (Cima, Baixo, Direita, Esquerda)
    :param posat: Posicao atual
    :return: Posicao final
    MBL
    """
    if mov == "C":
        if posat == 1 or posat == 2 or posat == 3:
            posat = posat
        else:
            posat = posat - 3
    if mov == "B":
        if posat == 7 or posat == 8 or posat == 9:
            posat = posat
        else:
            posat += 3
    if mov == "E":
        if posat == 1 or posat == 4 or posat == 7:
            posat = posat
        else:
            posat -= 1
    if mov == "D":
        if posat == 3 or posat == 6 or posat == 9:
            posat = posat
        else:
            posat += 1
    return posat

def obter_digito(mov,posat):
    """
    obter_digito: cad.caracters x int -> int
    Esta funcao funciona similarmente a obter_posicao mas aceita multiplos movimentos
    :param mov: Conjunto de Movimentos
    :param posat: Posicao atual
    :return: Posicao Final
    MBL
    """
    fin = 0
    if not (1 <= len(mov)):
        raise ValueError("obter_digito: argumento invalido")
    for x in mov:
       fin = obter_posicao(x,posat)
       posat = fin
    return fin


def obter_pin(tpl):
    """
    obter_pin: tuplo -> int
    Esta funcao computa os movimentos que levam ao pin final
    :param tpl: Tuplo com varias cadeias de caracteres com movimentos
    :return: Inteiro com o pin final
    MBL
    """
    if not (isinstance(tpl, tuple) and 4 <= len(tpl) <= 10):
        raise ValueError ("obter_pin: argumento invalido")
    final_tpl = ()
    inicio = 5
    for x in tpl:
        if len(x) < 1:
            raise ValueError("obter_pin: argumento invalido")
        for e in x:
            if e not in "CBDE":
                raise ValueError("obter_pin: argumento invalido")
        dig = obter_digito(x,inicio)
        mid_tpl = (dig,)
        final_tpl = final_tpl + mid_tpl #indexacao de tuplos (faz se em mais linhas do que transformar
        inicio = dig                   # em lista mas ao menos assim trablah-se sempre com tuplos)
    return final_tpl

def eh_entrada(arg):
    """
    eh_entrada : Universal -> boolean
    Esta funcao verifica se a entrada BDB e valida (mesmo estando possivelmente
    corrompida)
    :param arg: Entrada BDB
    :return: Booelano
    MBL
    """
    if not (isinstance(arg,tuple) and len(arg) == 3):
        return False
    if not (isinstance(arg[0], str) and isinstance(arg[1],str)):
        return False
    if "-" not in arg[0]:
        return False
    cifra_trbld = arg[0].replace("-", "")
    if cifra_trbld.isalpha() ==  False:
        return False
    checksum1 = arg[1].replace("[","")
    checksum1 = checksum1.replace("]","")
    if not (arg[1][-1] == "]" and arg[1][0] == "[" and len(arg[1]) == 7 and checksum1.isalpha() == True):
        return False
    if not (isinstance(arg[2], tuple) and len(arg[2]) >= 2):
        return False
    for x in arg[2]:
        if not (type(x) == int and x >= 0):
            return False
    else: return True

def bubble_sort_for_validar_cifra(lt):
    """
    bubble_sort_for_validar_cifra: lista -> lista organizada
    Esta funcao organizaa letras da cifram conforme o numero de ocorrencias e ordem alfabetica
    :param lt: Lista para organizar
    :return: Lista organizada
    MBL
    """
    #so precisamos de 5 letras por isso o ciclo de for esta limtado a 5
    for _ in range(5):
        for i in range(len(lt) - 1, 0, -1):
            if lt[i][1] > lt[i - 1][1]:
                tmp = lt[i]
                lt[i] = lt[i - 1]
                lt[i - 1] = tmp
            if lt[i][1] == lt[i - 1][1]:
                if ord(lt[i][0]) < ord(lt[i - 1][0]): #em casos de ocorrencias iguais o desempate e por ordem alfabetica
                    tmp1 = lt[i]
                    lt[i] = lt[i - 1]
                    lt[i - 1] = tmp1
    res = lt[:5]
    return [res[i][0] for i in range(len(res))]
def validar_cifra(cifra,checksum):
    """
    validar_cifra: cad.caracters x cad.caracteres -> booelano
    Esta funcao verifica a coerencia da cifra com o checksum
    :param cifra: Cifra da  entrada BDB
    :param checksum: Cheksum da BDB, que contem as 5 letras com mais ocorrencias da cifra
    :return: Booleano
    MBL
    """
    dic_ocorrencias = {}
    cifra = cifra.replace("-", "")
    for x in cifra:
        if x not in dic_ocorrencias:
            dic_ocorrencias[x] = 1
        else:
            dic_ocorrencias[x] += 1
    lista_ocorrencias = dic_ocorrencias.items()
    lista_ocorrencias = bubble_sort_for_validar_cifra(list(lista_ocorrencias))
    lista_final = ""
    lista_final = lista_final.join(lista_ocorrencias)
    if lista_final == checksum[1:-1]: #indexacao que remove os parenteses retos
        return True
    else:
        return False


def filtrar_bdb(lst):
    """
    filtrar_bdb: lista -> lista
    Esta funcao filtra todas as entradas com os parametros corretos
    :param lst: Lista com multiplas entradas bdb
    :return: Entradas em que o checksum nao e coerente
    """
    result = []
    if not (isinstance(lst,list) and lst != []):
        raise ValueError("filtrar_bdb: argumento invalido")
    for x in lst:
        if eh_entrada(x) == False:
            raise ValueError("filtrar_bdb: argumento invalido")
    for i in lst:
        if not validar_cifra(i[0],i[1]):
            result.append(i)
    return result

def obter_num_seguranca(tpl):
    """
    obter_num_seguranca: tuplo -> inteiro
    Esta funcao obtem o numero de seguranca, ou seja a menor diferenca dos membros do tuplo fornecido
    :param tpl: Tuplo com os varios numeros usados para calcular o numero de seguranca
    :return: Numero seguranca
    MBL
    """
    numeros = list(tpl)
    #organizar os numeros por ordem crescente para facilitar as operacoes
    numeros = sorted(numeros)
    #termo de comparacao inicial absurdamente grande para ter a certezaa de que e menor do que qualquer diferenca entre
    #elementos do tuplo
    comp = 10**1000
    for x in range(len(numeros) - 1):
            #se a diferenca de dois termos consecutivos for menor que o termo de comparacao esse passa a ser o numero
            #de seguranca
            if x < len(numeros) and numeros[x+1] - numeros[x] < comp:
                numero_seguranca = numeros[x+1] - numeros[x]
            #o termo de comparacao e sempre a diferenca anterior, e isto so e possivel porque o numeros estao ordenados
                comp = numeros[x+1] - numeros[x]
    return numero_seguranca

def decifrar_texto(cad,shift):
    """
    decifrar_texto: cadeia de caracteres e inteiro -> cadeia de caracters
    Esta funcao decifra o texto, similarmente a uma cifra de Cesar, com o "shift de letras no abecedario inserida
    :param cad: Cadeia de caracteres a ser decifrada
    :param shift: Numero de posicoes a deslocar
    :return: Cadeia de caracteres decifrada
    """
    final = ""
    for i in range(len(cad)):
        if cad[i] != "-":
            #
            if i % 2 == 0:
                new = (chr((ord(cad[i]) - 97 + shift + 1) % 26 + 97)) # comprimir a deslocacao entre 25 e somar isso a
                final += new                                          # a 97
            else:
                new = (chr((ord(cad[i]) - 97 + shift + -1) % 26 + 97))
                final += new
        else:
            final += " "
    return final

def decifrar_bdb(lst):
    """
    decifrar_bdb: lista -> lista  Esta funcao identifica as entradas corrompidas da bdb
    Esta funcao decifra as frases da bdd corrompidas
    :param lst: Lista com entradas bdb corrompidas
    :return: Lista com entradas decifradas
    MBL
    """
    final = []
    if not isinstance(lst,list):
        raise ValueError("decifrar_bdb: argumento invalido")
    for i in lst:
        if eh_entrada(i) == False:
            raise ValueError("decifrar_bdb: argumento invalido")
        num_seg = obter_num_seguranca(i[2])
        decifrado = decifrar_texto(i[0],num_seg)
        final.append(decifrado)
    return final
def eh_utilizador(dic):
    """
    eh_utilizador: Universal -> booleano
    Esta funcao verifica a validade do argumento contendo as informacoes do utilizador
    :param dic: Dicionario contedno as informacoes do utilizador
    :return: Booleano
    MBL
    """
    if not isinstance(dic, dict):
        return False
    lst = list(dic.items())
    if not ("name" in dic and "pass" in dic and "rule" in dic and len(dic) == 3 and dic["name"] != "" and dic["pass"] != ""):
        return False
    if not (type(dic["name"]) == str and type(dic["pass"] == str and type(lst[2][1]) == tuple and lst[2] != {})):
        return False
    dic_rule = dic.get("rule")
    if not (type(dic_rule.get("vals")) == tuple and type(dic_rule.get("char")) == str and type(dic["pass"]) == str):
        return False
    for x in dic_rule.get("vals"):
        if x < 0:
            return False
        if dic_rule.get("vals")[0] > dic_rule.get("vals")[1]:
            return False
    if len(dic["rule"]) == 0 or len(dic_rule["char"]) != 1:
        return False
    return True

def eh_senha_valida(password,dic_rule):
    """
    eh_senha_valida: password,regra -> booleano
    Esta funcao verifica a validade da senha segundo as regras individuais e gerais
    :param password: password
    :param dic_rule: dicionario com as regras individuais
    :return: Booleano
    MBL
    """
    vogais = "aeiou"
    #regras gerais
    count1 = 0
    count2 = 0
    for i in range(len(password)):
        if i < len(password) -1 and password[i] == password[i+1]:
            count1 += 1
        if password[i] in vogais:
             count2 += 1
    if not (count1 >= 1 and count2 >= 3):
        return False
    #regras individuais
    count3 = 0
    letra = dic_rule.get("char")
    intervalo = dic_rule.get("vals")
    for x in password:
        if x == letra:
            count3 += 1
    if not (intervalo[0] <= count3 <= intervalo[1]):
        return False
    return True
def filtrar_senhas(lst_dic):
    """
    filtrar_senhas: lista -> lista
    Esta funcao filtra as listas e apenas retorna as que estao erradas
    :param lst_dic: Lista com os utilizadores e as suas informacoes
    :return: Lista filtrada
    """
    final = []
    if not (isinstance(lst_dic, list) == True and lst_dic != []):
        raise ValueError("filtrar_senhas: argumento invalido")
    for i in range(len(lst_dic)):
        if eh_utilizador(lst_dic[i]) == False:
            raise ValueError("filtrar_senhas: argumento invalido")
        password = lst_dic[i]["pass"]
        regra = lst_dic[i]["rule"]
        if eh_senha_valida(password,regra) == False:
            final.append(lst_dic[i][("name")])
    return sorted(final)