#include "flagi.h"

#include <stdio.h>
#include <stdlib.h> /* dla atoi() */
#include <string.h> /* dla strcmp() */


int flag_s (char *arg) {
	if (atoi(arg) > 0) {
		printf("INFORMACJA: Liczba slow do wygenerowania: %d.\n", atoi(arg));
		return atoi(arg);
	} else {
		printf("BLAD: Po fladze -s nalezy podac liczbe calkowita wieksza od 0!\n");
		return -1;
	}
}

int flag_a (char *arg) {
	if (atoi(arg) > 0) {
		printf("INFORMACJA: Liczba akapitow do wygenerowania: %d.\n", atoi(arg));
		return atoi(arg);
	} else {
		printf("BLAD: Po fladze -a nalezy podac liczbe calkowita wieksza od 0!\n");
		return -1;
	}
}

int flag_n (char *arg) {
	if (atoi(arg) > 0) {
		printf("INFORMACJA: Wybrany rzad n-gramow: %d.\n", atoi(arg));
		return atoi(arg);
	} else {
		printf("BLAD: Po fladze -n nalezy podac liczbe calkowita wieksza od 0!\n");
		return -1;
	}
}

int error_control (int s, int a, int n, int lin, int lout, int lodtworz) {
	if (lin == -2 || lout == -1 || lodtworz == -1 || s == -2 || a == -2)
		return -1;
	if (lodtworz > 0) {
		if (n > 0) {
			if (s > 0 && a > 0)
				return 0;
			if (a < 0 || s < 0)
				return -1;
		} else {
			return -1;
		}
	} else {	
		if (lodtworz > 0 && lin == 0) {
			printf("BLAD: Nie podano plikow wyjsciowych.\n");
			return -1;
		}
		if (n > 0) {
			if (s > 0 && a > 0)
				return 0;
			if (a == -2 && s == -2)
				return -1;
			if (a == -1)
				return -1;
			if (s == -1)
				return -1;
		} else {
			return -1;
		}
		return -1;
	}
	return -1;
}	

int added_n ( int n) {
	if (n == -2) {
		n = 2;
		printf("INFORMACJA: Nie podano flagi -n: domyślnie ustawiono rzad n-gramu = 2.\n");
		return 2;
	} else {
		return n;
	}
}

int added_s (int s, int a) {
	if (s == -2) {
		s = 150;
		printf("INFORMACJA: Nie podano flagi -s: domyslnie ustawiono liczbe slow do wygenerowania = 150.\n");
		return 200;
	} else {
		return s;
	}
}

int added_a (int a) {
	if (a == -2) {
		a = 1;
		printf("INFORMACJA: Nie podano flagi -a: domyslnie ustawiono liczbe akapitow do wygenerowania = 1.\n");
		return 1;
	} else {
		return a;
	}
}

int spr_out ( char *arg ) {
	FILE *test;
	char sciezka[100] = "OUT/";
	strcat(sciezka, arg);
	if ( (test = fopen(sciezka, "w")) == NULL ) {
		printf("BLAD: Nie mozna zapisywac do pliku: %s\n", sciezka);
		return -1;
	} else {
		printf("INFORMACJA: Wygenerowany tekst bedzie zapisywany do pliku: %s\n", sciezka);
		return -3; /* udalo sie wczytac plik */
	}
	int fclose(FILE *test);
}

int spr_open ( char *arg ) {
	FILE *test;
	char sciezka[100] = "LOGS/";
	strcat(sciezka, arg);
	strcat(sciezka, ".log");
	if ( (test = fopen(sciezka, "r")) == NULL ) {
		printf("BLAD: Nie mozna wczytac pliku: %s\n", sciezka);
		int flose(FILE *test);
		return -1;
	} else {
		printf("INFORMACJA: Udalo sie wczytac plik z ustawieniami wejsciowymi: %s\n", sciezka);
		int fclose(FILE *test);
		return -3; /* udalo sie wczytac plik */
	}
}

int spr_in ( char *arg ) { /* zwraca: -2 - blad, -1 - wczytana kolejna flage,  0 - wczytano plik */
	FILE *test;
	char sciezka[100] = "IN/";
	strcat(sciezka, arg);
	if ((strcmp(arg, "-s")) != 0 && (strcmp(arg, "-a")) != 0 && (strcmp(arg, "-n")) != 0 && (strcmp(arg, "-odtworz")) != 0 && (strcmp(arg, "-out")) != 0 && (strcmp(arg, "-in")) != 0) {
		if ( (test = fopen(sciezka, "r")) == NULL ) {
			printf("BLAD: Nie mozna wczytac pliku: %s\n", sciezka);
			int fclose(FILE *test);
			return -2;
		} else {
			printf("INFORMACJA: Bede wczytywac tekst z pliku %s.\n", sciezka);
			int fclose(FILE *test);
			return 0;
		}
	} else if ((strcmp(arg, "-s")) == 0 || strcmp(arg, "-a") == 0 || strcmp(arg, "-n") == 0 || strcmp(arg, "-odtworz") == 0 || strcmp(arg, "-out") == 0 || strcmp(arg, "-in") == 0) {
			/* printf("Wykryto inna flage, funkcja spr_in konczy dzialanie\n"); */
			int fclose(FILE *test);
			return -1;
	} else {
		printf("BLAD: Podczas wczytywania plikow wejsciowych pojawil sie nieznany blad.\n");
		int fclose(FILE *test);
		return -2;
	}
}
