#include <stdio.h>
#include <stdlib.h> /* dla atoi() */
#include <string.h> /* dla strcmp() */

#include "flagi.h"
#include "generator.h"

int main(int argc, char * argv[]) {

	/* -2 -> nie podano wartosci */
	int s = -2; /* liczba slow */
	int a = -2; /* liczba akapitow */
	int n = -2; /* rzad n-gramu */
	FILE *in[100]; /* pliki wejsciowe */
	FILE *out; /* plik wyjsciowy */
	FILE *open; /* wczytywanie ustawien */

	char sciezka[100]; /* dodawanie sciezki (folderu) i rozszerzenia do nazyw pliku */
	char nazwa_wyj[100];
	char nazwa_wej[100];

	/* zmienne do sprawdzania, czy juz uzyto flagi */
	/* jesli == 1 -> flaga uzyta dwa razy, zignoruj ponowne uzycie*/
	int ls = 0;
	int la = 0;
	int ln = 0;
	int lin = 0; /* lin == -1 -> blad we wczytywaniu plikow, bedzie przechowywac liczbe plikow wejsciowych */
	int lout = -4; /* lout == -1 -> blad, lout == -4 -> dopisz stdout */
	int lopen = 0; /* blad */
	int index = 0; /* numer argumentu, od ktorego zaczynaja sie nazwy plikow wejsciowych */
	int status_in = 0; /* kontrola statusu wczytywania plikow wejsciowych */

	int i; /* iteracja po tablicy argumentow w celu wyszukania flag i pobrania danych z wejscia*/
	int j; /* do iteracji przy wczytywaniu plików wejściowych */

	/* BUFORY do odczytu z pliku przy wczytywaniu ustawien */
	char sbufor[100];
	
	nazwa_wyj[0] = 0;
	printf("\n");
	for(i = 1; i < argc; i++) {
		if(strcmp(argv[i], "-s") == 0 && ls != 1) {
			ls = 1;
			s = flag_s(argv[i+1]);
		} else if(strcmp(argv[i], "-a") == 0 && la != 1) {
			la = 1;
			a = flag_a(argv[i+1]);
		} else if(strcmp(argv[i], "-n") == 0 && ln != 1 && lopen == 0) {
			ln = 1;
			n = flag_n(argv[i+1]);
		} else if(strcmp(argv[i], "-out") == 0 && lout != 1) {
			lout = spr_out(argv[i+1]);
			if (lout == -3)
				lout = i+1;
				sciezka[0] = 0;
				strcat(sciezka, "OUT/");
				strcat(sciezka, argv[i+1]);
				out = fopen(sciezka, "w");
				strcat(nazwa_wyj, argv[i+1]);
		} else if(strcmp(argv[i], "-open") == 0 && lopen != 1 && ln == 0 && lin == 0) {
			lopen = spr_open(argv[i+1]);
			if (lopen == -3)
				lopen = i+1;
				sciezka[0] = 0;
				strcat(sciezka, "LOGS/");
				strcat(sciezka, argv[i+1]);
				strcat(sciezka, ".log");
				open = fopen(sciezka, "r");
				strcat(nazwa_wej, argv[i+1]);
				nazwa_wej[0] = 0;
				strcat(nazwa_wej, argv[i+1]);
		} else if(strcmp(argv[i], "-in") == 0 && lin > -1) {
			index = i + 1;
			status_in = spr_in(argv[index]);
			for(j = index; j < argc && lin > -1 && status_in == 0; j++) {
				sciezka[0] = 0;
				strcat(sciezka, "IN/");
				strcat(sciezka, argv[j]);
				in[j - index] = fopen(sciezka, "r");
				lin++;
				if( j < argc - 1 )
					status_in = spr_in(argv[j+1]);
			}
			if (status_in == -2)
				lin = -2;
		}
	}

	/* !!! INFORMACJA O ROZPOCZECIU PRACY !!! */

	if (lopen > 0) {
		if (s == -2 && a == -2) {
			printf("\nINFORMACJA: Nie podano liczby generowanych slow ani akapitow. Teraz Program pobierze ostatnio uzywane ustawienia wejsciowe z pliku: %s\n", argv[lopen]);
			sciezka[0] = 0;
			if (lout != -4) {
				if ((lout = spr_out(nazwa_wyj)) == -3) {
					sciezka[0] = 0;
					strcat(sciezka, "OUT/");
					strcat(sciezka, nazwa_wyj);
					out = fopen(sciezka, "w");
				}
			}
			if (fscanf(open, "%s", sbufor) && strcmp(sbufor, "-n") == 0) {
				if (fscanf(open, "%s", sbufor)) {
					n = flag_n(sbufor);
					sbufor[0] = 0;
					if (fscanf(open, "%s", sbufor) && strcmp(sbufor, "-s") == 0) {
						sbufor[0] = 0;
						if (fscanf(open, "%s", sbufor)) {
							s = flag_s(sbufor);
							a = -2;
						} else {
							printf("BLAD: Nieprawidlowy format pliku z ustawieniami wejsciowymi.\n");
						}
					} else if (strcmp(sbufor, "-a") == 0) {
						sbufor[0] = 0;
						if (fscanf(open, "%s", sbufor)) {
							a = flag_a(sbufor);
							s = -2;
						} else {
							printf("BLAD: Nieprawidlowy format pliku z ustawieniami wejsciowymi.\n");
						}
					}
				} else {
					printf("BLAD: Nieprawidlowy format pliku z ustawieniami wejsciowymi.\n");
				}
			} else { 
				printf("BLAD: Nieprawidlowy format pliku z ustawieniami wejsciowymi.\n");
			}
		} else {
			rewind(open);
			if (fscanf(open, "%s", sbufor) && strcmp(sbufor, "-n") == 0) {
				fscanf(open, "%s", sbufor);
				n = flag_n(sbufor);
				printf("INFORMACJA: Wczytany rzad n-gramu: %d.\n", n);
			}
		}
	}

	/* ustawianie wartosci domyslnych, kontrola bledow */
	n = added_n( n );
	s = added_s( s, a );
	a = added_a( a );
	if ( lout == -4 ) {
		printf("INFORMACJA: Nie podano flagi -out, program bedzie generowac tekst na wyjscie standardowe.\n");
	/*	out = stdout;*/
		nazwa_wyj[0] = 0;
		strcat(nazwa_wyj, "OUT/");
		strcat(nazwa_wyj, "stdout");
		if ((out = fopen(nazwa_wyj, "w")) == NULL)
			printf("BLAD: Nie mozna utworzyc tymczasowego pliku %s.\n", nazwa_wyj);
		nazwa_wyj[0] = 0;
		strcat(nazwa_wyj, "stdout");
		nazwa_wej[0] = 0;
		strcat(nazwa_wej, "stdout");
		lout = -3;
		
	}

/*	printf("kontrola bledow: s = %d, a = %d, n = %d, lin = %d, lout = %d, lopen = %d\n",s,a,n,lin,lout,lopen);*/
	if (error_control( s, a, n, lin, lout, lopen) == -1) { /* -1 jezeli jest blad - zakoncz program */
		if (lopen == 0) {
			printf("\nINFORMACJA: Program zakonczyl dzialanie w fazie wczytywania flag z powodu napotkanych bledow.\n\n");
		} else {
			printf("\nINFORMACJA: Program zakonczyl dzialanie w fazie wczytywania ustawien wejsciowych z pliku z powodu napotkanych bledow.\n\n");
		}
		return -1;
	}

/*	if (a == -2 && lopen == 0) {
		printf("\nINFORMACJA: Generator rozpoczyna prace z nastepujacymi parametrami:\n  rzad n-gramu = %d\n  maksymalna liczba slow do wygenerowania = %d\n  plik wyjsciowy: %s\n  pliki wejsciowe:", n, s, lout == -4 ? "wyjscie standardowe" : argv[lout]);
		for(j = 0; j < lin; j++)
			printf(" %s", argv[j+index]);
		printf("\n\n");
	} else if (s == -2 && lopen == 0) {
		printf("\nINFORMACJA: Generator rozpoczyna prace z nastepujacymi parametrami:\n  rzad n-gramu = %d\n  maksymalna liczba akapitow do wygenerowania = %d\n  plik wyjsciowy: %s\n  pliki wejsciowe:", n, a, lout == -4 ? "wyjscie standardowe" : argv[lout]);
		for(j = 0; j < lin; j++)
			printf(" %s", argv[j+index]);
		printf("\n\n");
	} */

	if (generator( n, s, a, in, out, open, lin, lopen, nazwa_wyj, nazwa_wej) != 0) {
		printf("\nINFORMACJA: Program zakonczyl dzialanie z powodu napotkanych bledow.\n\n");
		return -1;
	} else {
		printf("\nINFORMACJA: Program prawidlowo zakonczyl prace.\n\n");
		return 0;
	}
}
