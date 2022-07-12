/*   Project done by Alexandre Ferreira,1st year student of COMP.SCI
    at IST (LEIC). 
    ist103307
    alexandre.bruno.ferreira@tecnico.ulisboa.pt
    MBL
*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#define MAX_AIRPORT_SIZE 41
#define MAX_IDAIR_SIZE 4
#define MAX_COUNTRY_SIZE 41
#define MAX_CITY_SIZE 51
#define MAX_IDFLIGHT_SIZE 7
#define MAX_DATE_SIZE 11
#define MAX_HOUR_SIZE 6
#define MAX_FLIGHTS_SIZE 30001
#define MAX_INPUT_SIZE 65336
/*the struct for a date, really handy to compare dates */
typedef struct 
{
   int day;
   int month;
   int year;
}date;
/* the struct for hours and minutes, for the same reason as the struct for the date */
typedef struct 
{
    int hour;
    int minutes;
} hour;
/* the struct for a airport, with its ID, country , city and number of flight 
departures from that airport */
typedef struct
{ char idair[MAX_IDAIR_SIZE];
    char country[MAX_COUNTRY_SIZE];
    char city[MAX_CITY_SIZE];
    int flights;
} airport;
/* the struct for a flight, with the flights ID, the corresponding airport ID for 
its departure and destination along with the times, the duration of the flights
and its capacity */
typedef struct {
    char idflight[MAX_IDFLIGHT_SIZE];
    char idairdep[MAX_AIRPORT_SIZE];
    char idaairar[MAX_AIRPORT_SIZE];
    date datedep;
    date datear;
    hour hourar;
    hour hourdep;
    hour duration;
    int capacity;
} flight;
/*global flight lists for the departures and arrivals, 
used in P_cmd and C_cmd respetively */
flight departures[MAX_FLIGHTS_SIZE];
flight arrivals[MAX_FLIGHTS_SIZE];
/* airport list */
airport airports[MAX_AIRPORT_SIZE];
/* flight list */
flight flights[MAX_FLIGHTS_SIZE];
/*------------------------------------------------------------------------------------------------ 
            
            A_cmd: this function adds an airport to the database. Its' arguments are the airport's
            ID, country, city, the list of current airports, and the number of current airports.
            After adding the airport to the system it prints "airport <IDAir>" in the terminal.
            Returns the current number of airports.

------------------------------------------------------------------------------------------------*/           
int A_cmd(char idairpre[], char countrypre[], char citypre[], airport airports[],int nraero){
    /* New airport structure */
    airport newAirport;
    int j,i;
    for(i=0; i < MAX_IDAIR_SIZE - 1;i++){
        /*isupper wasnt working, so instead uses ASCCI table numbers */
        if((idairpre[i] < 65 || idairpre[i] > 90) || strlen(idairpre) != 3) { 
            printf("invalid airport ID\n");
            return nraero;
        }
    }
    if(nraero == MAX_AIRPORT_SIZE  - 1){
        printf("too many airports\n");
        return nraero;
    }
    for (j = 0; j <= nraero; j++){
        if ((strcmp(airports[j].idair,idairpre)) == 0){
            printf("duplicate airport\n");
            return nraero;
        }
    }
    /*copying the given arguments into a struct */
    strcpy(newAirport.idair, idairpre);
    strcpy(newAirport.city, citypre);
    strcpy(newAirport.country, countrypre);
    newAirport.flights = 0;
    /* puts it into a list of airports */
    airports[nraero] = newAirport;
    /* adds to the counter of airports */
    nraero++;
    printf("airport %s\n", idairpre);
    return nraero;
}
/*------------------------------------------------------------------------------------------------ 
            
            Verify_date: this function verifies if a date is valid, meaning it's not before
            the current date, nor more than a year in the future. Receives the current date,
            and the year, month and day to verified. Returns 1 if valid.

------------------------------------------------------------------------------------------------*/  

int Verify_date(date currdate, int year, int month, int day){
    if(year < currdate.year){
        return 0;
    }
    if (year == currdate.year && month < currdate.month){
        return 0;
    }
    if(year == currdate.year && month == currdate.month && day < currdate.day){
        return 0;
    }
    if(day > currdate.day && month == currdate.month && year > currdate.year){
        return 0;
    }
    if(day == currdate.day && month > currdate.month && year > currdate.year){
        return 0;
    }

    return 1;
}
/*------------------------------------------------------------------------------------------------ 
            
            Compare_Data_for_P: Auxiliar function that helps sort flights by date. 
            Receives two flights,and compares their departure times and dates.
            Returns >0 if flight 1 is happening further in the future than flight 2;

------------------------------------------------------------------------------------------------*/  
int Compare_data_for_P(flight voo1, flight voo2){ 
    if(voo1.datedep.year == voo2.datedep.year){
        if (voo1.datedep.month == voo2.datedep.month){
            if (voo1.datedep.day == voo2.datedep.day){
                if (voo1.hourdep.hour == voo2.hourdep.hour){
                    if (voo1.hourdep.minutes == voo2.hourdep.minutes){
                        return 0;
                    }
                    /* Arithmetic operations like these help to verify which
                    flight is the oldest */
                    return voo1.hourdep.minutes - voo2.hourdep.minutes;
                }
                return voo1.hourdep.hour - voo2.hourdep.hour;
            }
            return voo1.datedep.day - voo2.datedep.day;
        }
        return voo1.datedep.month - voo2.datedep.month;
    }
    return voo1.datedep.year - voo2.datedep.year;

}
/*------------------------------------------------------------------------------------------------ 
            
            Compare_Data_for_C: Auxiliar function that helps sort flights by date. 
            Receives two flights, and compares their arrivals times and dates.
            Returns >0 if flight 1 is happening further in the future than flight 2;
            Really similar to Compare_Data_for_P, but for arrivals.

------------------------------------------------------------------------------------------------*/  
int Compare_data_for_C(flight voo1, flight voo2){ 
    if(voo1.datear.year == voo2.datear.year){
        if (voo1.datear.month == voo2.datear.month){
            if (voo1.datear.day == voo2.datear.day){
                if (voo1.hourar.hour == voo2.hourar.hour){
                    if (voo1.hourar.minutes == voo2.hourar.minutes){
                        return 0;
                    }
                    return voo1.hourar.minutes - voo2.hourar.minutes;
                }
                return voo1.hourar.hour - voo2.hourar.hour;
            }
            return voo1.datear.day - voo2.datear.day;
        }
        return voo1.datear.month - voo2.datear.month;
    }
    return voo1.datear.year - voo2.datear.year;

}
/*------------------------------------------------------------------------------------------------ 
            
            L_without_cmd: Lists all airports by alphabetic order of their IDs.
            Receives the current list of aiports, and the current number of aiports.
            Each airport is printed in a different line under the following output:
            "<ID> <city> <country> #flights", where flights is the number of flights
            that depart from that airport.

------------------------------------------------------------------------------------------------*/  
void L_without_cmd(airport airports[], int nraero){
    int j,i;
    /* temporary list for sorting and a throwayay airport for bubble sort */
    airport temp[MAX_AIRPORT_SIZE], trader;
    /*creating auxiliar list to be sorted*/
    for (i = 0; i <= nraero - 1; i++){
        temp[i] = airports[i];
    }
    /*alphabetic bubble sort */
    for (i =0;i < nraero -1 ; i++){                       
        for( j = 0; j < nraero - i - 1; j++){
            /* strcmp() returns >0 if the first non matching letter of the 
            first argument is larger in ASCII than the second argument */
            if(strcmp(temp[j].idair,temp[j+1].idair) > 0){
                trader = temp[j];
                temp[j] = temp[j+1];
                temp[j+1] = trader;
            }
        }
    }
    for(j=0; j <= nraero - 1; j++){
        printf("%s %s %s %d\n",temp[j].idair, temp[j].city, temp[j].country, temp[j].flights);
    }
}
/*------------------------------------------------------------------------------------------------ 
            
            L_with_cmd: Lists all  given airports by order the of ID's given by the user.
            Receives the string with the ids, the current list of aiports, and the current number of aiports.
            Each airport is printed in a different line under the following output:
            "<ID> <city> <country> #flights", where flights is the number of flights
            that depart from that airport.

------------------------------------------------------------------------------------------------*/  
void L_with_cmd(char linput[],int size,airport airports[], int nraero){
    int i,j, index = 0;
    char id[MAX_IDAIR_SIZE];
    /* noid is a variable to verify if the given ID exists */
    int noid;
    /*this for loop is a parser that looks for ids , by looking for a whitespace in the input.
    every character that isnt a whitespace gets added to the id variable. when one character is
    added, the index counter gets one more unit. after finding a whitespace, index gets
    reset to zero.*/
    for(j = 2; j <= size; j++){
        if (j == size || linput[j] == ' '){
            index = 0;
            noid = 0;
            /* after finding an id, checks if its in the system, if it is then prints that flight*/
            for(i = 0;i <= nraero - 1 ; i++){
                if (strcmp(airports[i].idair,id) == 0){
                    printf("%s %s %s %d\n",airports[i].idair,airports[i].city,airports[i].country,airports[i].flights);
                    noid = 1;
                    break;
                }

            }
            if (noid == 0){
                printf("%s: no such airport ID\n",id);
            }


        }
        /*adding the new character to id and addig a unit to index */
        else{
            id[index] = linput[j];
            index++;
        }
    }
}
/*------------------------------------------------------------------------------------------------ 
            
            V_without_cmd: Lists all flights by order of creation.
            Receives the list of flights, and the current number of flights.
            Each flight is printed in a different line under the following output:
            <FlightCode> <IDAirportDeparture> <IDAeroportArrival> <DateDeparture> <TimeDeparture>.


------------------------------------------------------------------------------------------------*/  
void V_without_cmd(flight flights[], int nrflights){
    int i;
    for(i = 0; i <= nrflights - 1; i++){
        printf("%s %s %s %02d-%02d-%d %02d:%02d\n",flights[i].idflight,flights[i].idairdep,
               flights[i].idaairar,flights[i].datedep.day,flights[i].datedep.month,flights[i].datedep.year,
               flights[i].hourdep.hour,flights[i].hourdep.minutes);

    }
}
/*------------------------------------------------------------------------------------------------ 
            
            V_with_cmd: Adds a flight to the database.
            Receives the list of flights,list of airports, current number of flights
            the code of the flight, the ID of the airport it departs from, the ID
            of the airport it arrives in, the date and the time of arrival, and the 
            current date.
    
------------------------------------------------------------------------------------------------*/ 
int V_with_cmd(flight flights[], airport airports[], int nrflights,char idflightpre[],
               char idairDEPpre[],char idairARpre[], int day, int month ,int  year, int hourdep,
               int minutedep,int  hourdura, int minutedura ,int capacitypre, int nraero, date currdate){
    /* New flight structure to be added */
    flight newFlight;
    int i,noid,size3;
    /* checks if the first two letters of the flight code are upper case */
    for(i = 0; i <=1; i++){
        if ((idflightpre[0] < 65 || idflightpre[0] > 90) || strlen(idflightpre) > 6
            || (idflightpre[1] < 65 || idflightpre[1] > 90)){ /* same situation as in A_cmd */
            printf("invalid flight code\n");
            return nrflights;
        }
    }
    /* checks if the remaining characters are digits */
    size3 = strlen(idflightpre);
    for(i = 2; i <= size3 - 1 ; i++){
        if(isdigit(idflightpre[i]) == 0){
            printf("invalid flight code\n");
            return nrflights;
        }
    }
    /* verifies if flight already exists */
    for(i = 0;i <= nrflights - 1; i++){
        if (strcmp(idflightpre,flights[i].idflight) == 0 && day == flights[i].datedep.day &&
            month == flights[i].datedep.month
            && year == flights[i].datedep.year){
            printf("flight already exists\n");
            return nrflights;
        }

    }
    /* looks for existence of given ID */
    noid = 0;
    for(i = 0;i <= nraero - 1; i++){
        if(strcmp(idairDEPpre,airports[i].idair) == 0){
            noid = 1;
        }
    }
    if(noid ==0){
        printf("%s: no such airport ID\n",idairDEPpre);
        return nrflights;
    }
    noid = 0;
    for(i = 0;i <= nraero - 1; i++){
        if(strcmp(idairARpre,airports[i].idair) == 0){
            noid = 1;
        }
    }
    if(noid==0){
        printf("%s: no such airport ID\n",idairARpre);
        return nrflights;
    }
    if(nrflights == MAX_FLIGHTS_SIZE - 1){
        printf("too many flights\n");
        return nrflights;

    }
    /* uses verify date to make sure that given date is valid */
    if(Verify_date(currdate,year,month,day) == 0){
        printf("invalid date\n");
        return nrflights;
    }
    /*checks the duration */
    if(hourdura == 12 && minutedura > 0){
        printf("invalid duration\n");
        return nrflights;
    }
    if (hourdura > 12){
        printf("invalid duration\n");
        return nrflights;
    }
    if(capacitypre < 10 || capacitypre > 100){
        printf("invalid capacity\n");
        return nrflights;
    }
    /* copies the given arguments into a struct */
    strcpy(newFlight.idflight,idflightpre);
    strcpy(newFlight.idairdep,idairDEPpre);
    strcpy(newFlight.idaairar,idairARpre);
    newFlight.datedep.day = day;
    newFlight.datedep.month = month;
    newFlight.datedep.year = year;
    newFlight.hourdep.hour = hourdep;
    newFlight.hourdep.minutes = minutedep;
    newFlight.duration.hour = hourdura;
    newFlight.duration.minutes = minutedura;
    newFlight.capacity = capacitypre;
    /* adds it into a list */
    flights[nrflights] = newFlight;
    /* adds to the corresponding airports number of flights */
    for(i = 0; i <= nraero - 1; i++){
        if(strcmp(idairDEPpre,airports[i].idair) == 0){
            airports[i].flights++;
        }
    }
    /* adds to the counter of flights */
    nrflights++;
    return nrflights;
}
/*------------------------------------------------------------------------------------------------ 
            
            P_cmd: Lists all flights that depart from given ID, by oldest to most recent.
            Receives the Aiport's ID,list of flights,list of airports, current number of flights,
            and current number of airports.
            Each flight is printed in a different line under the following output:
            <FlightCode> <IDAeroportArrival> <DateDeparture> <TimeDeparture>.

------------------------------------------------------------------------------------------------*/ 
void P_cmd(char idair[], flight flights[],int nrflights,airport airports[], int nraero){
    /* throwaway flight for bubble sort */
    flight trader;
    int i,j;
    /* number of flights counter */
    int nrdepartures = 0;
    int id = 0;
    for(i = 0; i <= nraero - 1; i++){
       if(strcmp(airports[i].idair,idair) == 0){
           id = 1;
       }
    }
    /* checks for ID */
    if(id == 0){
        printf("%s: no such airport ID\n",idair);
        return;
    }
    /* creates list of all flights that depart from given ID */
    for(i = 0; i <= nrflights - 1; i++){
       if(strcmp(flights[i].idairdep,idair) == 0){
            departures[nrdepartures] = flights[i];
            nrdepartures++;

        }
    }
    /* sorts the departures list (bubble sort) */
    for (i = 0;i < nrdepartures - 1 ; i++){  
        for( j = 0; j < nrdepartures - i - 1; j++){
                if(Compare_data_for_P(departures[j],departures[j+1]) > 0){
                trader = departures[j];
                departures[j] = departures[j+1];
                departures[j+1] = trader;
            }
        }
    }
     for(i = 0; i <= nrdepartures -1 ; i++){
        printf("%s %s %02d-%02d-%d %02d:%02d\n", departures[i].idflight, departures[i].idaairar, 
        departures[i].datedep.day,departures[i].datedep.month,departures[i].datedep.year,
        departures[i].hourdep.hour,departures[i].hourdep.minutes);

    }
        
}
/*------------------------------------------------------------------------------------------------ 
            
            compute_arrival: Computes a flights time of arrival.
            Receives a flight.
            Returns that flight structure with its' date and time of arrival variables updated.

------------------------------------------------------------------------------------------------*/ 
flight compute_arrival(flight arrival){
    /* Creating a temporary variable with all of the flights departure
    info for easier computing */
    int hourtemp,minutetemp,daytemp,yeartemp,monthtemp;
        daytemp = arrival.datedep.day;
        monthtemp = arrival.datedep.month;
        yeartemp = arrival.datedep.year;
        /* adding the duration to the time of departure */
        hourtemp = arrival.duration.hour + arrival.hourdep.hour;
        minutetemp = arrival.duration.minutes + arrival.hourdep.minutes;
        /*add an hour if minutes surpass 59 */
        if(minutetemp > 59){
           hourtemp++;
           minutetemp = (minutetemp - 60);
           if(minutetemp < 0){
               minutetemp = minutetemp * -1;
           }
        } 
        /* add a day if hours surpass 23 */   
        if (hourtemp > 23){
                daytemp++;
                hourtemp = (hourtemp-24);
                if(hourtemp < 0){
                    hourtemp = -1 * hourtemp;
                }
            }
                /* if the month is february and the current day is bigger than 28,
                add a month and reset day to 1  */
                if(daytemp > 28 && monthtemp == 2){
                    monthtemp++;

                    daytemp = 1;
                    
                }  
                /* if it is 31 day month and current day is bigger than 32, add a month and reset day to 1  */
                if (daytemp > 31 && (monthtemp == 1|| monthtemp == 3 || monthtemp == 5 || monthtemp == 7 
                || monthtemp==8 || monthtemp== 10)){
                    monthtemp++;
                    daytemp = 1;
                    
                }
                /* if it is 30 day month and current day is bigger than 31, add a month,and reset day to 1 */
                if (daytemp > 30 && (monthtemp == 2|| monthtemp == 4 || monthtemp == 6 || monthtemp == 9 
                || monthtemp== 11)){
                    monthtemp++;
                    daytemp = 1;
                    
                }
                /* if it is december and current day is bigger than 32, add a year and reset month to 1 */
                if (daytemp > 31 && monthtemp == 12){
                    monthtemp = 1;
                    daytemp = 1;
                    yeartemp++;
                }


        /* updates arrival times */
        arrival.hourar.minutes = minutetemp;
        arrival.hourar.hour = hourtemp;
        arrival.datear.day = daytemp;
        arrival.datear.month = monthtemp;
        arrival.datear.year = yeartemp;
        return arrival;

}
/*------------------------------------------------------------------------------------------------ 
            
            C_cmd: Lists all flights that arrive in a given ID, by oldest to most recent.
            Receives the Aiport's ID,list of flights,list of airports, current number of flights,
            and current number of airports.
            Each flight is printed in a different line under the following output:
            <FlightCode> <IDAeroportDeparture> <DateArrival> <TimeAarrival>.

------------------------------------------------------------------------------------------------*/ 
/* check P_cmd for more comments */
void C_cmd(char idair[], flight flights[],int nrflights,airport airports[],int nraero){
    flight trader;
    int i, j;
    int nrarrivals = 0;
    int id = 0;
    for(i = 0; i <= nraero - 1; i++){
       if(strcmp(airports[i].idair,idair) == 0){
           id = 1;
       }
    }
    if( id == 0 ){
        printf("%s: no such airport ID\n",idair);
        return;
    }
    for(i = 0; i <= nrflights - 1; i++){
        if(strcmp(flights[i].idaairar,idair) == 0){
            arrivals[nrarrivals] = flights[i];
            /* computes the arrival times for the flights */
            arrivals[nrarrivals] = compute_arrival(arrivals[nrarrivals]);

            nrarrivals++;
        }
    }
     /* sorts the arrivals list (bubble sort) */
    for (i = 0;i < nrarrivals - 1 ; i++){                      
        for( j = 0; j < nrarrivals - i - 1; j++){
            if(Compare_data_for_C(arrivals[j],arrivals[j+1]) > 0){
                trader = arrivals[j];
                arrivals[j] = arrivals[j+1];
                arrivals[j+1] = trader;
            }
        }
       
    }
    for(i = 0; i <= nrarrivals - 1 ; i++){
        printf("%s %s %02d-%02d-%d %02d:%02d\n", arrivals[i].idflight, arrivals[i].idairdep, 
        arrivals[i].datear.day,arrivals[i].datear.month,arrivals[i].datear.year,
        arrivals[i].hourar.hour,arrivals[i].hourar.minutes);

    }

}
int main() {
    int nraero = 0, size1, nrflights = 0,length = 1 ,count = 0;
    /* every variable that ends with pre is used to store the values taken from the input to then
    give to the corresponding functions. For example, idairpre is the an airport ID,
    countrypre is a contry, etc. */
    int capacitypre, day, month, year, hourdep, minutedep, hourdura, minutedura,
    newday,newmonth,newyear;
    char *command, commandtest, idairpre1[MAX_IDAIR_SIZE],
            idairpre2[MAX_IDAIR_SIZE],idairpre3[MAX_IDAIR_SIZE],
            countrypre[MAX_COUNTRY_SIZE], citypre[MAX_CITY_SIZE],
            idflightpre[MAX_IDFLIGHT_SIZE],
            idairDEPpre[MAX_IDAIR_SIZE], idairARpre[MAX_IDAIR_SIZE],c;
    /* current date */
    date currdate;
    currdate.day = 1;
    currdate.month = 1;
    currdate.year = 2022;
    command = malloc(length * sizeof(char)); 
    while (1) {
        command = malloc(length * sizeof(char)); 
        while((c = getchar()) != '\n'){ 
            if(count >= length)
                command = realloc(command, (length += 1) * sizeof(char));
            command[count++] = c;
        }
        command[count] = '\0';
        printf("%s\n",command);
        /*condition for q command */
        if (command[0] == 'q') {    
            exit(0);
        }
        /* condition for a command */
        if (command[0] == 'a') {
            /* separating input */
            sscanf(command,"%c %s %s %[^\n]", &commandtest, idairpre1, countrypre, citypre);
            nraero = A_cmd(idairpre1, countrypre, citypre, airports, nraero);
        }
        /* condition for l command */
        if (command[0] == 'l') {
            size1 = strlen(command);
             /* verifies if command is given without arguments */
            if (size1 == 1) {
                L_without_cmd(airports, nraero);

            } else {
                L_with_cmd(command, size1, airports, nraero);

            }
        }
        /* condition for v command */
        if (command[0] == 'v') {
            /* verifies if command is given without arguments */
            if (strlen(command) == 1) {
                V_without_cmd(flights, nrflights);
            } else {
                /* if given with arguments, separates arguments */
                sscanf(command, "%c %s %s %s %d-%d-%d %d:%d %d:%d %d",&commandtest, idflightpre, idairDEPpre, idairARpre, &day, &month,
                       &year, &hourdep, &minutedep, &hourdura, &minutedura, &capacitypre);
                nrflights = V_with_cmd(flights, airports, nrflights, idflightpre, idairDEPpre, idairARpre, day, month,
                                       year, hourdep, minutedep, hourdura, minutedura, capacitypre, nraero, currdate);
            }
        }
        /* condition for p command */
        if (command[0] == 'p'){
                /* separates the input */
            sscanf(command,"%c %[^\n]",&commandtest,idairpre2);
            P_cmd(idairpre2,flights,nrflights,airports,nraero);
        }
        /* condition for c command */
        if (command[0] == 'c'){
            /* separates the input */
            sscanf(command,"%c %s",&commandtest,idairpre3);
            C_cmd(idairpre3,flights,nrflights,airports,nraero);
        }
        /* condition for t command */
        if (command[0] == 't'){
            /* separates the input */
            sscanf(command,"%c %d-%d-%d",&commandtest,&newday,&newmonth,&newyear);
            /* verifies if date is valid */
            if(Verify_date(currdate,newyear,newmonth,newday) == 0){
                printf("invalid date\n");
            }
            else{
                /*if it its valid, uptdates the current date */
                currdate.day = newday;
                currdate.month = newmonth;
                currdate.year = newyear;
                printf("%02d-%02d-%d\n",newday,newmonth,newyear);
            }
        }
    }


    return 0;
}