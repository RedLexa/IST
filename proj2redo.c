
/* Variaveis Globais */
/* primeiro projeto de IAED
 * autor: vmm
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_NUM_AEROPORTOS 40	/* número máximo de areoportos */
#define MAX_NUM_VOOS 30000	/* número máximo de voos */
#define HASH_SIZE 20011		/*size of the hash table */
#define hashindex(A) (A*27)%HASH_SIZE /*hash algorithm */
#define MAX_INPUT_SIZE 65536

#define MAX_CODIGO_AEROPORTO 4	/* dimensão do código do aeroporto */
#define MAX_NOME_PAIS 31	/* dimensão do nome do pais */
#define MAX_NOME_CIDADE 51	/* dimensão do nome da cidade */

#define MAX_CODIGO_VOO 7	/* dimensão do código do voo */
#define MAX_DATA 11		/* dimensão da data */
#define MAX_HORA 6		/* dimensão da hora */

#define NAO_EXISTE -1		/* código de erro */

#define ANO_INICIO 2022		/* ano inicial do sistema */
#define DIAS_ANO 365		/* número de dias no ano */
#define HORAS_DIA 24		/* número de horas por dia */
#define MINUTOS_HORA 60		/* número de minutos numa hora */
#define MINUTOS_DIA 1440	/* número de minutos do dia */

#define TRUE 1			/* verdadeiro */
#define FALSE 0			/* falso */

/* Tipos de Dados */

typedef struct {
	char id[MAX_CODIGO_AEROPORTO];
	char pais[MAX_NOME_PAIS];
	char cidade[MAX_NOME_CIDADE];
	int numVoos;
} Aeroporto;

typedef struct {
	int dia;
	int mes;
	int ano;
} Data;

typedef struct {
	int hora;
	int minuto;
} Hora;

typedef struct {
	char id[MAX_CODIGO_VOO];
	char partida[MAX_CODIGO_AEROPORTO];
	char chegada[MAX_CODIGO_AEROPORTO];
	Data data;
	Hora hora;
	Hora duracao;
	int capacidade;
	int horaPartida;
	int horaChegada;
	int currpass;
	int print;
} Voo;

int _numAeroportos = 0;		/* número de aeroportos introduzidos */
Aeroporto _aeroportos[MAX_NUM_AEROPORTOS];	/* vetor de aeroportos */

int _numVoos = 0;		/* número de voos introduzidos */
Voo _voos[MAX_NUM_VOOS];	/* vetor de voos */

Data _hoje = { 1, 1, 2022 };	/* data atual do sistema */

const int _diasMesAc[] =	/* dias acumulados por mês (jan=1) */
{ 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 };
/* struct for a reservation */
typedef struct {
	char flightid[MAX_CODIGO_VOO];
	int passengers;
	Data date;
	char * code;
	int print;
} reservation;
/* struct for an item of the linked list of reservations */
typedef struct noderes {
	reservation res;
	struct noderes *nexthash;
	struct noderes *nextall;
} *linkres;

/* Funcoes Leitura */

Hora leHora() {
	Hora h;
	scanf("%d:%d", &h.hora, &h.minuto);
	return h;
}


Data leData() {
	Data d;
	scanf("%d-%d-%d", &d.dia, &d.mes, &d.ano);
	return d;
}


int leProximaPalavra(char str[]) {
	char c = getchar();
	int i = 0;
	while (c == ' ' || c == '\t')
		c = getchar();
	while (c != ' ' && c != '\t' && c != '\n') {
		str[i++] = c;
		c = getchar();
	}
	str[i] = '\0';
	return (c == '\n');
}


void lePalavraAteFimDeLinha(char str[]) {
	char c = getchar();
	int i = 0;
	while (c == ' ' || c == '\t')
		c = getchar();
	while (c != '\n') {
		str[i++] = c;
		c = getchar();
	}
	str[i] = '\0';
}


void leAteFimDeLinha() {
	char c = getchar();
	while (c != '\n')
		c = getchar();
}


/* Funcoes Datas e Horas */

void mostraData(Data d) {
	if (d.dia < 10)
		printf("0");
	printf("%d-", d.dia);
	if (d.mes < 10)
		printf("0");
	printf("%d-%d", d.mes, d.ano);
}


void mostraHora(Hora h) {
	if (h.hora < 10)
		printf("0");
	printf("%d:", h.hora);
	if (h.minuto < 10)
		printf("0");
	printf("%d", h.minuto);
}


int converteDataNum(Data d) {
	return (d.ano - ANO_INICIO) * DIAS_ANO + _diasMesAc[d.mes - 1] +
		d.dia - 1;
}


int converteHoraNum(Hora h) {
	return ((h.hora * MINUTOS_HORA) + h.minuto);
}


int converteDataHoraNum(Data d, Hora h) {
	return converteDataNum(d) * MINUTOS_DIA + converteHoraNum(h);
}


Hora converteNumHora(int num) {
	Hora h;
	h.minuto = num % MINUTOS_HORA;
	h.hora = ((num - h.minuto) / MINUTOS_HORA) % HORAS_DIA;
	return h;
}


Data converteNumData(int num) {
	Data d;
	int i = 0;
	num = (num - (num % MINUTOS_DIA)) / MINUTOS_DIA;
	d.ano = (num / DIAS_ANO) + ANO_INICIO;
	num = num - ((d.ano - ANO_INICIO) * DIAS_ANO);
	while (i <= 11 && num >= _diasMesAc[i])
		i++;
	d.mes = i;
	d.dia = num - _diasMesAc[i - 1] + 1;
	return d;
}


int validaData(Data d) {
	int numData = converteDataNum(d);
	Data proximoAno = _hoje;
	proximoAno.ano++;
	return !(numData < converteDataNum(_hoje)
		 || numData > converteDataNum(proximoAno));
}


int validaHora(Hora h) {
	return !(h.hora > 12 || (h.hora == 12 && h.minuto > 0));
}


/* Algoritmo de ordenação BubbleSort */

void bubbleSort(int indexes[], int size, int (*cmpFunc)(int a, int b)) {
	int i, j, done;

	for (i = 0; i < size - 1; i++) {
		done = 1;
		for (j = size - 1; j > i; j--)
			if ((*cmpFunc) (indexes[j - 1], indexes[j])) {
				int aux = indexes[j];
				indexes[j] = indexes[j - 1];
				indexes[j - 1] = aux;
				done = 0;
			}
		if (done)
			break;
	}
}


/* Funcoes Aeroportos */


int aeroportoInvalido(char id[]) {
	int i;
	for (i = 0; id[i] != '\0'; i++)
		if (!(id[i] >= 'A' && id[i] <= 'Z'))
			return TRUE;
	return FALSE;
}


int encontraAeroporto(char id[]) {
	int i;
	for (i = 0; i < _numAeroportos; i++)
		if (!strcmp(id, _aeroportos[i].id))
			return i;
	return NAO_EXISTE;
}


void adicionaAeroporto() {
	Aeroporto a;

	leProximaPalavra(a.id);
	leProximaPalavra(a.pais);
	lePalavraAteFimDeLinha(a.cidade);

	if (aeroportoInvalido(a.id))
		printf("invalid airport ID\n");
	else if (_numAeroportos == MAX_NUM_AEROPORTOS)
		printf("too many airports\n");
	else if (encontraAeroporto(a.id) != NAO_EXISTE)
		printf("duplicate airport\n");
	else {
		strcpy(_aeroportos[_numAeroportos].id, a.id);
		strcpy(_aeroportos[_numAeroportos].pais, a.pais);
		strcpy(_aeroportos[_numAeroportos].cidade, a.cidade);
		_aeroportos[_numAeroportos].numVoos = 0;
		_numAeroportos++;
		printf("airport %s\n", a.id);
	}
}


void mostraAeroporto(int index) {
	printf("%s %s %s %d\n", _aeroportos[index].id,
		_aeroportos[index].cidade, _aeroportos[index].pais,
		_aeroportos[index].numVoos);
}


int cmpAeroportos(int a, int b) {
	return (strcmp(_aeroportos[a].id, _aeroportos[b].id) > 0);
}


void listaTodosAeroportos() {
	int i;
	int indexAeroportos[MAX_NUM_AEROPORTOS];

	for (i = 0; i < _numAeroportos; i++)
		indexAeroportos[i] = i;

	bubbleSort(indexAeroportos, _numAeroportos, cmpAeroportos);

	for (i = 0; i < _numAeroportos; i++)
		mostraAeroporto(indexAeroportos[i]);
}


void listaAeroportos() {
	char id[MAX_CODIGO_AEROPORTO];
	int indexAeroporto, ultima = 0;

	ultima = leProximaPalavra(id);
	if (strlen(id) == 0)
		listaTodosAeroportos();
	else {
		while (strlen(id) != 0) {
			indexAeroporto = encontraAeroporto(id);
			if (indexAeroporto == NAO_EXISTE)
				printf("%s: no such airport ID\n", id);
			else
				mostraAeroporto(indexAeroporto);
			if (!ultima)
				ultima = leProximaPalavra(id);
			else
				break;
		}
	}
}



/* Funcoes Voos */

void mostraVoo(int index) {
	if(_voos[index].print == 1){
		printf("%s %s %s ", _voos[index].id, _voos[index].partida,
			_voos[index].chegada);
		mostraData(_voos[index].data);
		printf(" ");
		mostraHora(_voos[index].hora);
		printf("\n");
	}
}

void mostraVooPartida(int index) {
	printf("%s %s ", _voos[index].id, _voos[index].chegada);
	mostraData(_voos[index].data);
	printf(" ");
	mostraHora(_voos[index].hora);
	printf("\n");
}

void mostraVooChegada(int index) {
	Hora h = converteNumHora(_voos[index].horaChegada);
	printf("%s %s ", _voos[index].id, _voos[index].partida);
	mostraData(converteNumData(_voos[index].horaChegada));
	printf(" ");
	mostraHora(h);
	printf("\n");
}



int encontraVoo(char id[], Data d) {
	int numData = converteDataNum(d);
	int i;
	for (i = 0; i < _numVoos; i++){
		if (!strcmp(id, _voos[i].id)
			&& numData == converteDataNum(_voos[i].data) && _voos[i].print == 1)
				return i;
	}
	return NAO_EXISTE;
}


int validaIDVoo(char id[]) {
	int i = 0, l = strlen(id);
	if (l < 3)
		return FALSE;
	for (i = 0; i < 2; i++)
		if (!(id[i] >= 'A' && id[i] <= 'Z'))
			return FALSE;

	while (id[i] != '\0') {
		if (!(id[i] >= '0' && id[i] <= '9'))
			return FALSE;
		i++;
	}
	return TRUE;
}

int validaVoo(Voo v) {
	if (validaIDVoo(v.id) == FALSE)
		printf("invalid flight code\n");
	else if (encontraVoo(v.id, v.data) != NAO_EXISTE)
		printf("flight already exists\n");
	else if (encontraAeroporto(v.partida) == NAO_EXISTE)
		printf("%s: no such airport ID\n", v.partida);
	else if (encontraAeroporto(v.chegada) == NAO_EXISTE)
		printf("%s: no such airport ID\n", v.chegada);
	else if (_numVoos == MAX_NUM_VOOS)
		printf("too many flights\n");
	else if (validaData(v.data) == FALSE)
		printf("invalid date\n");
	else if (validaHora(v.duracao) == FALSE)
		printf("invalid duration\n");
	else if (v.capacidade < 10)
		printf("invalid capacity\n");
	else
		return TRUE;
	return FALSE;
}

void criaVoo(Voo v) {
	strcpy(_voos[_numVoos].id, v.id);
	strcpy(_voos[_numVoos].partida, v.partida);
	strcpy(_voos[_numVoos].chegada, v.chegada);
	_voos[_numVoos].data = v.data;
	_voos[_numVoos].hora = v.hora;
	_voos[_numVoos].duracao = v.duracao;
	_voos[_numVoos].capacidade = v.capacidade;
	_voos[_numVoos].horaPartida =
		converteDataHoraNum(_voos[_numVoos].data,
					_voos[_numVoos].hora);
	_voos[_numVoos].horaChegada =
		_voos[_numVoos].horaPartida +
		converteHoraNum(_voos[_numVoos].duracao);
	_voos[_numVoos].currpass = 0;
	_voos[_numVoos].print = 1;
	_aeroportos[encontraAeroporto(v.partida)].numVoos++;
	_numVoos++;
}

void adicionaListaVoos() {
	Voo v;
	int i;

	if (leProximaPalavra(v.id)) {
		for (i = 0; i < _numVoos; i++)
			mostraVoo(i);
		return;
	} else {
		leProximaPalavra(v.partida);
		leProximaPalavra(v.chegada);
		v.data = leData();
		v.hora = leHora();
		v.duracao = leHora();
		scanf("%d", &v.capacidade);
		leAteFimDeLinha();
	}

	if (validaVoo(v))
		criaVoo(v);
}


int cmpVoosPartida(int a, int b) {
	return (_voos[a].horaPartida > _voos[b].horaPartida);
}


int cmpVoosChegada(int a, int b) {
	return (_voos[a].horaChegada > _voos[b].horaChegada);
}


void listaVoosPartida() {
	int indexVoos[MAX_NUM_VOOS], i, n = 0;
	char partida[MAX_CODIGO_AEROPORTO];

	lePalavraAteFimDeLinha(partida);

	if (encontraAeroporto(partida) == NAO_EXISTE)
		printf("%s: no such airport ID\n", partida);

	for (i = 0; i < _numVoos; i++) {
		if (!strcmp(_voos[i].partida, partida))
			indexVoos[n++] = i;
	}

	bubbleSort(indexVoos, n, cmpVoosPartida);

	for (i = 0; i < n; i++)
		mostraVooPartida(indexVoos[i]);
}


void listaVoosChegada() {
	int indexVoos[MAX_NUM_VOOS], i, n = 0;
	char chegada[MAX_CODIGO_AEROPORTO];

	lePalavraAteFimDeLinha(chegada);

	if (encontraAeroporto(chegada) == NAO_EXISTE)
		printf("%s: no such airport ID\n", chegada);

	for (i = 0; i < _numVoos; i++) {
		if (!strcmp(_voos[i].chegada, chegada))
			indexVoos[n++] = i;
	}

	bubbleSort(indexVoos, n, cmpVoosChegada);

	for (i = 0; i < n; i++)
		mostraVooChegada(indexVoos[i]);
}


void alteraData() {
	Data d;

	d = leData();
	leAteFimDeLinha();
	if (validaData(d) == FALSE)
		printf("invalid date\n");
	else {
		_hoje = d;
		mostraData(_hoje);
		printf("\n");
	}
}

/*----------------------------------------------------------------------------------


			hashSpot: This fucntion receives a flight ID and its size and returns
			corresponding index in the Hash Table;



------------------------------------------------------------------------------------*/

int hashSpot(char id[], int size){
    int i,temp = 0;
    
    for(i = 0; i <= size - 1; i++){
        temp = temp + id[i];
    }
return hashindex(temp);
}
/*----------------------------------------------------------------------------------


			addReservation: This fucntion receives the hashtable, a flight ID, a 
			string buffer containing the reservation code and the number of 
			people in that reservation, the date, and the linked list of all
			reservations. It adds a reservation to the database, and returns the
			new head of the linked list.



------------------------------------------------------------------------------------*/
linkres addReservation(linkres hashtable[],char idflight[], char buffer[], int day, int month, int year,linkres allreserv){
	int nrflight,size,size2,hashindex,i,newpassengers; 
	char *code;
	Data data;
	linkres New  = NULL,comp = NULL,temp = NULL,compall = NULL,previous = NULL;
	size2 = strlen(idflight);
	hashindex =hashSpot(idflight,size2);
	data.dia = day;
	data.mes = month;
	data.ano = year;
	code = malloc(sizeof(char)*(strlen(buffer) + 1));
	if(code == NULL){
		printf("No memory\n");
		exit(0);
	}
	sscanf(buffer,"%s %d", code, &newpassengers);
	size = strlen(code);
	if(size < 10){
		printf("invalid reservation code\n");
            return allreserv; 
	} 
	for (i = 0; i <= size - 1 ; i++){
		if(isdigit(code[i]) == 0 && isupper(code[i]) == 0){
				printf("invalid reservation code\n");
            	return allreserv;
				
					
			}
		}
	nrflight = encontraVoo(idflight,data);
	if (nrflight == NAO_EXISTE){
		 printf("%s: flight does not exist\n",idflight);
			return allreserv;
	}
	/* erro reservation already exists */
	if(allreserv != NULL){
		compall = allreserv;
		while(compall){ /*looks through the linkeed list of all reservations */
          if(compall->nextall == NULL){ /* condition for last element */
			  if((strcmp(code,compall->res.code) == 0) && (compall->res.print == 1)){ 
					printf("%s: flight reservation already used\n",code);
					return allreserv;

				}
			}
			if(strcmp(code,compall->res.code) == 0 && (compall->res.print == 1)){ 
				printf("%s: flight reservation already used\n",code);
				return allreserv;
			}
		compall = compall->nextall;
		}
	}
	if((_voos[nrflight].currpass + newpassengers) > _voos[nrflight].capacidade){
		printf("too many reservations\n");
        return allreserv;
	}
	if(validaData(data) == FALSE){
        printf("invalid date\n");
        return allreserv;
	}

	if (newpassengers <= 0){
        printf("invalid passenger number\n");
        return allreserv;
	}
	/* copies the information of the reservation into a node */
	New = (linkres)malloc(sizeof(struct noderes));
	if(New == NULL){
		printf("No memory\n");
		exit(0);
	}
    strcpy(New->res.flightid, idflight);
    New->res.passengers = newpassengers;
    New->res.date.dia = day;
    New->res.date.mes= month;
    New->res.date.ano = year;
	New ->res.print = 1;
    New->nexthash = NULL;
	New->nextall = NULL;
    New->res.code = malloc(sizeof(char)*(size+1));
	if(New->res.code == 0){
		printf("No memory\n");
	}
	strcpy(New->res.code,code);
	free(code);
	_voos[nrflight].currpass = _voos[nrflight].currpass + newpassengers;
    if(allreserv == NULL){ /*conditon for empty linked list */
	   allreserv = New;

	}
	else{
	    temp = allreserv;
	    allreserv = New;
	    New->nextall = temp;
	}
	/* when adding a reservation to the corresponding hashtable index, already puts the 
	list in lexicographical order */
	if(hashtable[hashindex] == NULL){ /*condition for empty index in linked list */
        hashtable[hashindex] = New;
		return allreserv;
    }
	else{
		comp = hashtable[hashindex];
		if(strcmp(New->res.code,hashtable[hashindex]->res.code) <= 0){ /*if the new node needs to be put behind the current head */
			hashtable[hashindex] = New;
			New->nexthash = comp;
			return allreserv;
	
		}
		while((strcmp(New->res.code,comp->res.code ) > 0) && comp->nexthash != NULL){ /* iteration through linked list */
			previous = comp; 
			comp = comp->nexthash;

		}
		if(strcmp(New->res.code,comp->res.code) == 0){ /* if two reservations are the same (this is possible due to */
			hashtable[hashindex] = New;					/* not actually deleting reservations from the list */
			New->nexthash = comp;
			return allreserv;
		}
		if(comp->nexthash == NULL && (strcmp(New->res.code,comp->res.code ) > 0)){ 
			New->nexthash = NULL;										/*if the new node needs to be pit in front of the last */			
			comp->nexthash = New;										/* node of the linked list */
			return allreserv;

		}
		else{
			New -> nexthash = previous->nexthash; /*if the new node needs to be pit in behind of the last */
			previous->nexthash = New;			/* node of the linked list */
			return allreserv;
			
		} 
		New->nexthash = comp->nexthash; /*if the node needs to be put between two nodes */
		comp->nexthash = New;
		return allreserv;
	}
}
/*----------------------------------------------------------------------------------


			listsReservations: This fucntion receives the hashtable, a flight ID, a 
			the date. It prints the reservations of the given flight ID to the
			terminal, in lexicographic order.


------------------------------------------------------------------------------------*/
 void listsReservations(linkres hashtable[], char idflight[],int day, int month, int year){
	int hashindex,size;
	linkres current;
	Data data;
	data.dia = day;
	data.mes = month;
	data.ano = year;
	if(validaData(data) == FALSE){
        printf("invalid date\n");
        return;
	}
	if(encontraVoo(idflight,data) == NAO_EXISTE){
		 printf("%s: flight does not exist\n",idflight);
		 return;
	}
	current = NULL;
    size = strlen(idflight);
    hashindex = hashSpot(idflight,size);
    current = hashtable[hashindex];
	if(current == NULL){
		return;
	}
    while(current != NULL){
        if (current->res.date.dia == day && current->res.date.mes == month && current->res.date.ano == year
        && (strcmp(idflight,current->res.flightid) == 0) && (current->res.print == 1)){ /*condition that verifies that the function only prints */
            if (current->nexthash == NULL){			/* condition for last element 		reservations of the given flight and that haven't been */		
                printf("%s %d\n",current->res.code, current->res.passengers);			/* deleted */	
				return;
            }
        printf("%s %d\n",current->res.code, current->res.passengers);
		}  
    current = current->nexthash;
	} 
}
/*----------------------------------------------------------------------------------


			addsListsReservations: This function receives the hashtable, a string buffer
			with the input and the linked list of all reservations.
			It either adds a reservation to the database or prints the reservations 
			for the given flight, depending on user input and returns the current
			head of the linked list of all reservations.


------------------------------------------------------------------------------------*/
linkres addsListsReservations(linkres hashtable[],char buffer[],linkres allreserv){
	char idflight[MAX_CODIGO_VOO]; 
	 int day, month, year;
	 leProximaPalavra(idflight);
	 scanf("%d-%d-%d",&day,&month,&year);
	 lePalavraAteFimDeLinha(buffer);
	 if(isupper(buffer[0]) || isdigit(buffer[0])){ /*if there is still information after the flight id */
		allreserv = addReservation(hashtable,idflight,buffer,day,month,year,allreserv);
	 }
	 else{
		listsReservations(hashtable,idflight,day,month,year); 
	 }
	return allreserv;
}
/*----------------------------------------------------------------------------------


			deletesReservation: This function receives the linked list of all reservations
			and the reservation code.It deletes the reservation from the database, by
			marking it as non-existent.


------------------------------------------------------------------------------------*/
void deletesReservation(linkres allreserv,char *code){
	int found = 0,nrvoo;
	linkres search;
	search = allreserv;
	while(search->nextall){ /*iterates through linked list */
		if(strcmp(search->res.code,code) == 0){ /*condition for finding code */
				search->res.print = 0;
				nrvoo = encontraVoo(search->res.flightid,search->res.date);
				_voos[nrvoo].currpass = _voos[nrvoo].currpass - search->res.passengers;
				found++;
			}	
		search = search->nextall;
	}
	if (search->nextall == NULL){ /*condition for last element */
			if (strcmp(code,search->res.code) == 0 && search->res.print ==1){
				search->res.print = 0;
				nrvoo = encontraVoo(search->res.flightid,search->res.date);
				_voos[nrvoo].currpass = _voos[nrvoo].currpass - search->res.passengers;
				found++;
			}
			if(found == 0){
				printf("not found\n");
			}
			fflush(stdout);
			return;
		}
}
/*----------------------------------------------------------------------------------


			deletesFlight: This function receives the hashtable and the 
			flight code.It deletes the flight from the database, by
			marking it as non-existent, along with the flights reservations.


------------------------------------------------------------------------------------*/
void deletesFlight(char *code,linkres hashtable[]){
	int i,j = 0,size,hashindex;
	linkres search;
	search = NULL;
	
	for(i = 0; i <= _numVoos - 1; i++){
		if(strcmp(code,_voos[i].id) == 0){ /*deletes flight */
			_voos[i].print = 0;	
			j++;	
		}   
	}
	if(j == 0){			/*condition to verify if didnt find the corresponding flight */
		printf("not found\n");
	}
	size = strlen(code);
	hashindex = hashSpot(code,size);
	search = hashtable[hashindex];
	if(hashtable[hashindex] != NULL){ /*iteration through corresponding index in the hashtable */
		while(search != NULL){
			if(search->nexthash == NULL){ /*condition for last element */
				if(strcmp(search->res.flightid,code) == 0){
					search->res.print = 0;
					return;
				}
			}
			if(strcmp(search->res.flightid,code) == 0){
					search->res.print = 0;
			}	
				search = search->nexthash;
		}
	}
}
/*----------------------------------------------------------------------------------


			deletesFlightReservation: This function receives the hashtable, the 
			linked list of all reservations and the input in a string buffer.
			It either deletes either the reservation from the database or the flight, by
			marking it as non-existent, depending on user input.


------------------------------------------------------------------------------------*/
void deletesFlightReservation(linkres hashtable[],char buffer [],linkres allreserv){
	char *code;
	int size;
	leProximaPalavra(buffer);
	size = strlen(buffer);
	code = malloc(sizeof(char) * (size + 1));
	if(code == NULL){
		printf("No memory\n");
		exit(0);
	}
	strcpy(code,buffer);
	if( size <= 6){
		deletesFlight(code,hashtable);
		
	}
	else{
		deletesReservation(allreserv,code);
	}
	free(code);
}


int main() {
	int c,i;
	/*hashtable with pointers to a struct of a node */
	linkres hashtable[HASH_SIZE];
	/* head of linked list of all reservations */
	linkres allreserv,walk1;
	char buffer [MAX_INPUT_SIZE];
	allreserv = NULL;
	for(i = 0; i <= HASH_SIZE -1; i++){
		hashtable[i] = NULL;
	}
	while ((c = getchar()) != EOF) {
		memset(buffer,0,MAX_INPUT_SIZE);
		switch (c) {
		case 'q': 
		while(allreserv != NULL){
			walk1 = allreserv->nextall;
			free(allreserv->res.code);
			free(allreserv);
			allreserv = walk1;
		}
		free(walk1);
		return 0;
		case 'a': adicionaAeroporto();
			break;
		case 'l': listaAeroportos();
			break;
		case 'v': adicionaListaVoos();
			break;
		case 'p': listaVoosPartida();
			break;
		case 'c': listaVoosChegada();
			break;
		case 't': alteraData();
			break;
		case 'r': allreserv = addsListsReservations(hashtable,buffer,allreserv);
			break;
		case 'e': deletesFlightReservation(hashtable,buffer,allreserv);
			break; 
		}
	}

	return 0;
}
