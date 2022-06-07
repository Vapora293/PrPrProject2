
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 201 // definujeme si veľkosti daných polí charov
#define PREZ 51
#define RC 11
#define KPM 4
#define TYPPREZ 3
#define CAS 5

typedef struct subor { //vytvorenie štruktúry, bude obsahovať všetky riadky záznamu definované ich maximálnou veľkosťou
    char prezenter[PREZ];
    char rodnecislo[RC];
    char kpm[KPM];
    char nazov[MAX];
    char mena[MAX];
    char typprez[TYPPREZ];
    char cas[CAS];
    long datum;
    struct subor *next; // a ukazovateľ na ďalší záznam v spájanom zozname
} SUBOR;

SUBOR *novy_zaznam(char prezenter[], char rodnecislo[], char kpm[], char nazov[], char mena[], char typprez[], char cas[], long datum) { // Funkcia na kopírovanie dočasných polí do štruktúry na želané pozície
    SUBOR *vysledok = malloc(sizeof(SUBOR)); // definujeme si novú štruktúru
    strcpy(vysledok->prezenter, prezenter);
    strcpy(vysledok->rodnecislo, rodnecislo);
    strcpy(vysledok->kpm, kpm);
    strcpy(vysledok->nazov, nazov);
    strcpy(vysledok->mena, mena);
    strcpy(vysledok->typprez, typprez);
    strcpy(vysledok->cas, cas);
    (vysledok->datum) = datum;
    return vysledok;
}

SUBOR *na_koniec(SUBOR **tail, SUBOR *zaznam) { // Funkcia na pridanie záznamu na koniec zoznamu
    /*zaznam->next = (*tail)->next; */ // ? asi naviac len zbytočne
    (*tail)->next = zaznam; // do pôvodného konca zapíšeme ukazovateľ na next na náš terajší koniec (štruktúra záznam)
    *tail = zaznam; // a do konca nám zapíše aktuálny koniec zoznamu
    return zaznam; // funkcia nám returne pôvodný záznam
}

int n(FILE **p_fr, int *p_pocetzaznamov, SUBOR **head, SUBOR **tail) { // Funkcia na loadovanie štruktúr zo súboru
    SUBOR *docas; // definujeme si dočasnú premennú
    if((*head) != NULL) { // ak už boli štruktúry vytvorené, dealokujeme ich
        while(*head != NULL) { // ideme postupne od hlavičky po celom zozname (koniec zoznamu má ->next NULL
            docas = *head;
            if((*head)->next!=NULL) {
                *head = (*head)->next;
                break;
            }
            free(docas);
        }
    }
    if ((*p_fr = fopen("konferencny_zoznam.txt", "r")) == NULL) { // kontrolujeme otvorenosť súboru
        printf("Zaznamy neboli nacitane!\n");
        return -1;
    }

    int pocetriadkov = 0; // pomocné premenné na počet riadkov a riadok, ktorý budeme prechádzať vo fore pomocou fgets
    char riadok[MAX];
    while (fgets(riadok, MAX, *p_fr) != NULL) { // spočítame riadky
        pocetriadkov++;
    }
    rewind(*p_fr);

    char d_prezenter[PREZ+1]; // definujeme si dočasné polia charov, ktoré budeme kopírovať do štruktúry
    char d_rodnecislo[RC+1];
    char d_kpm[KPM+1];
    char d_nazov[MAX+1];
    char d_mena[MAX+1];
    char d_typprez[TYPPREZ+1];
    char d_cas[CAS+1];
    long d_datum;

    *p_pocetzaznamov = 0;
    int fajky = 0; // pomocná premenná na overenie korektnosti
    for (int i = 0; i <= pocetriadkov; i++) { // prejdeme postupne po riadkoch celý súbor
        fgets(riadok, MAX, *p_fr); // do pomocnej premennej riadok si skopírujeme daný riadok zo súboru
        switch (i % 9) {
            case 0: // doláre, kopírujeme obsah už načítaných riadkov do štruktúry
                if(i != 0) {
                    docas = novy_zaznam(d_prezenter, d_rodnecislo, d_kpm, d_nazov, d_mena, d_typprez, d_cas, d_datum); // do štruktúry dočas skopírujeme daný záznam. Nevykoná sa hneď na začiatku, lebo dočasné premenné ešte neobsahujú dané riadky
                    if(i == 9) {
                        *head = docas;
                        docas->next = *head;
                        *head = docas; // na začiatok nám pridá záznam, ktorý má byť v hlavičke
                        *tail = docas; // po while cykle nám zostane v premennej záznam koniec spájaného zoznamu
                    }
                    else {
                        na_koniec(tail, docas); // inak ho zapíše na koniec
                    }
                    (*p_pocetzaznamov)++; // a zvýši počet záznamov používaný naprieč ostatnými funkciami
                }
                break;
            case 1:
                strcpy(d_prezenter, riadok);
                d_prezenter[strlen(d_prezenter)-1] = 0;
                break;
            case 2:
                strcpy(d_rodnecislo, riadok);
                d_rodnecislo[strlen(d_rodnecislo)-1] = 0;
                break;
            case 3:
                strcpy(d_kpm, riadok);
                d_kpm[strlen(d_kpm)-1] = 0;
                break;
            case 4:
                strcpy(d_nazov, riadok);
                d_nazov[strlen(d_nazov)-1] = 0;
                break;
            case 5:
                strcpy(d_mena, riadok);
                d_mena[strlen(d_mena)-1] = 0;
                break;
            case 6:
                strcpy(d_typprez, riadok);
                d_typprez[strlen(d_typprez)-1] = 0;
                break;
            case 7:
                strcpy(d_cas, riadok);
                d_cas[strlen(d_cas)-1] = 0;
                break;
            case 8:
                d_datum = atol(riadok);
                break;
            default:
                break;
        }
    }
    printf("Nacitalo sa %d zaznamov\n", *p_pocetzaznamov);
    fclose(*p_fr); // Zatvoríme súbor, nebudeme už s ním pracovať
}

void v(int pocetzaznamov, SUBOR *head) { // Funkcia na vypísanie spájaného zoznamu
    if(head != NULL) { // kvôli ošetreniu, ak štruktúry ešte nie sú vytvorené
        int poradie = 1; // kvôli vypísaniu poradia od jednotky
        SUBOR *docas = head;
        for(int i = 0; i < pocetzaznamov; i++) { // najprv vypíšeme údaje zo štruktúry
            printf("%d.\nPrezenter: %s\nRodne cislo: %s\nKod prezentacnej miestnosti: %s\nNazov prispevku: %s\nMena autorov: %s\nTyp prezentovania: %s\nCas prezentovania: %s\nDatum: %li\n", poradie, docas->prezenter, docas->rodnecislo, docas->kpm, docas->nazov, docas->mena, docas->typprez, docas->cas, docas->datum);
            docas = docas->next; // a potom sa posunieme v zozname o štruktúru ďalej
            poradie++;
        }
    }
    else{ // Ak ešte neexistujú, nespraví nič

    }
}

int p(SUBOR **head, SUBOR **tail, int *pocetzaznamov) { // Pridanie záznamu do spájaného zoznamu
    // pridať na prvé miesto, ak ešte neboli vytvorené

    int miesto; // vytvoríme si pomocné premenné na načítanie vstupu z konzoly
    char vstup_prezenter[PREZ+1];
    char vstup_rodnecislo[RC+1];
    char vstup_kpm[KPM+1];
    char vstup_nazov[MAX+1];
    char vstup_mena[MAX+1];
    char vstup_typprez[TYPPREZ+1];
    char vstup_cas[CAS+1];
    long vstup_datum;
    SUBOR *ojednomenej = *head; // Potrebujeme si ošetriť, ak sa má jednať o začiatok súboru
    scanf("%d", &miesto);
    getchar(); // ošetrujeme si prechod zo scanf na fgets a postupne naplníme všetky vstupy a skrátime ich o znak konca riadku
    fgets(vstup_prezenter, MAX, stdin);
    vstup_prezenter[strlen(vstup_prezenter)-1] = 0;
    fgets(vstup_rodnecislo, MAX, stdin);
    vstup_rodnecislo[strlen(vstup_rodnecislo)-1] = 0;
    fgets(vstup_kpm, MAX, stdin);
    vstup_kpm[strlen(vstup_kpm)-1] = 0;
    fgets(vstup_nazov, MAX, stdin);
    vstup_nazov[strlen(vstup_nazov)-1] = 0;
    fgets(vstup_mena, MAX, stdin);
    vstup_mena[strlen(vstup_mena)-1] = 0;
    fgets(vstup_typprez, MAX, stdin);
    vstup_typprez[strlen(vstup_typprez)-1] = 0;
    fgets(vstup_cas, MAX, stdin);
    vstup_cas[strlen(vstup_cas)-1] = 0;
    scanf("%li", &vstup_datum);

    SUBOR *docas = novy_zaznam(vstup_prezenter, vstup_rodnecislo, vstup_kpm, vstup_nazov, vstup_mena, vstup_typprez, vstup_cas, vstup_datum); // naplníme dočasnú štruktúru danými vstupmi
    if (*head == NULL) {
        (*head) = docas;
        (*tail) = docas;
        (*pocetzaznamov)++;
    }
    else if (miesto == 1) { // ošetrujeme head, ak sa jedná o prvý záznam
        docas->next = ojednomenej; // Linkneme nový záznam k predošlému prvému
        *head = docas; // prepíšeme hlavičku na nový záznam
        (*pocetzaznamov)++;
    }
    else { // inak si prejdeme celý zoznam, ak je číslo menšie ako počet záznamov, tak ho hodí na danú pozíciu a zvyšok poposúva
        if(miesto <= (*pocetzaznamov)) {
            for(int i = 1; i < miesto-1; i++) {
                ojednomenej = ojednomenej->next; // poposúvane si všetky záznamy pred želaným miestom
            }
            docas->next = ojednomenej->next; // potom si linkneme nový záznam s ďalším v poradí
            ojednomenej->next = docas; // a prepíšeme pôvodné poradie na nový záznam v zázname pred ním
        } else {
            na_koniec(tail, docas); // a ak číslo nie je menšie ako počet záznamov tak ho šupneme na koniec
        }
        (*pocetzaznamov)++; // a zvýšime celkový počet záznamov
    }
}

int z(SUBOR **head, SUBOR **tail, int *pocetzaznamov) { // Funkcia na vymazanie záznamov z listu
    if(*head == NULL){ // ošetrenie neexistujúcich záznamov
        printf("Zaznamy neboli este nacitane\n");
        return -1;
    }
    char vstup_rodnecislo[RC+1];
    scanf("%s", &vstup_rodnecislo); // definujeme si rodné číslo zo vstupu
    SUBOR *docas = (*head)->next; // a dočasnú štruktúru a štruktúru o jedna pred ňou
    SUBOR *docas_pred = *head;
    int najv = (*pocetzaznamov);
    for(int i = 0; i < najv-1; i++) {
        if(strcmp(docas_pred->rodnecislo, vstup_rodnecislo) == 0 && i == 0) { // ošetrujeme si, keď sa jedná o prvý záznam v liste
            *head = (*head)->next; //posunieme si hlavičku, zvyšok sa posunie cez else na konci for cyklu
            printf("Prezenter s menom %s bol vymazany\n", docas_pred->prezenter);
            SUBOR *vymaz = docas_pred; // a freeujeme daný záznam
            free(vymaz);
            (*pocetzaznamov)--;
        }
        if(strcmp(docas->rodnecislo, vstup_rodnecislo) == 0) {
            printf("Prezenter s menom %s bol vymazany\n", docas->prezenter);
            SUBOR *vymaz = docas; // definujeme si dočasnú premennú vymaz na freeovanie danej štruktúry
            docas = docas->next; // poposúvame si záznamy tak, aby sme vyhodili požadovaný záznam z listu
            docas_pred->next = docas;
            (*pocetzaznamov)--;
            free(vymaz);
            } else {
            docas_pred = docas; // ak nesedí rodné číslo, posunieme sa ďalej. Vykonáva sa aj po prvom ife
            docas = docas->next;
        }
    }
}

int h(SUBOR *head, int pocetzaznamov) { // Funkcia na vypísanie záznamov z danej prezentačnej miestnosti
    if(head == NULL){ // ošetrujeme si nenačítané záznamy do štruktúr
        printf("Zaznamy neboli este nacitane\n");
        return -1;
    }
    char vstup_kpm[KPM+1];
    scanf("%s", &vstup_kpm); // naskenujeme si kód prezentačnej miestnosti
    SUBOR *docas = head;
    int poradie = 0;
    for(int i = 0; i < pocetzaznamov; i++) {
        if(strcmp(docas->kpm, vstup_kpm)==0) { // ak sedí KPM na vstupe s KPM v štruktúre, vypíšeme danú štruktúru aj s poradím
            poradie++;
            printf("%d.\nPrezenter: %s\nRodne cislo: %s\nKod prezentacnej miestnosti: %s\nNazov prispevku: %s\nMena autorov: %s\nTyp prezentovania: %s\nCas prezentovania: %s\nDatum: %li\n", poradie, docas->prezenter, docas->rodnecislo, docas->kpm, docas->nazov, docas->mena, docas->typprez, docas->cas, docas->datum);
        }
        docas = docas->next; // a posunieme sa na ďalšiu
    }
    if(poradie==0) {
        printf("Pre prezentacnu miestnost %s nie su prezentujuce osoby\n", vstup_kpm); // ošetrujeme si pomocnou premennou zlý KPM
    }
}

int a(SUBOR **head, int pocetzaznamov) { // Funkcia na aktualizáciu typu prezentovania
    char vstup_rodnecislo[RC];
    char vstup_typprez[TYPPREZ];
    scanf("%s %s", &vstup_rodnecislo, &vstup_typprez); // načítame rodné číslo a KPM do pomocných premenných

    if ((strlen(vstup_rodnecislo) != 10) || (atoll(vstup_rodnecislo) % 11 != 0)) { // cez najbližšie dva ify si overujeme korektnosť
        printf("Zadane udaje nie su korektne, zadaj novy retazec:\n");
        return -1;
    }

    if (strcmp("PD", vstup_typprez) == 0 || strcmp("UD", vstup_typprez) == 0 || strcmp("PP", vstup_typprez) == 0 || strcmp("UP", vstup_typprez) == 0) { // typ prezentovania musí byť jeden z uvedených

    } else {
        printf("Zadane udaje nie su korektne, zadaj novy retazec:\n");
        return -1;
    }

    SUBOR *docas = *head;
    for(int i = 0; i < pocetzaznamov; i++) {
        if(strcmp(docas->rodnecislo, vstup_rodnecislo)==0) { // ak je rodné číslo v štruktúre zhodné v zázname tak zmení typ prezentovania v danej štruktúre
            strcpy(docas->typprez, vstup_typprez);
            printf("Prezenter s menom %s bude prezentovat prispevok %s : %s\n", docas->prezenter, docas->nazov, docas->typprez);
        }
        if(docas->next !=NULL) { // ošetrenie konca záznamu, kedže nepoužívame tail
            docas = docas->next;
        }
    }
    return 0;
}

int r(SUBOR **head, SUBOR **tail, int pocetzaznamov) { // Funkcia na prehodenie dvoch záznamov v liste
    if(*head == NULL){ // Ošetrenie nenačítaných záznamov
        printf("Zaznamy neboli este nacitane\n");
        return -1;
    }
    int cprvyzaznam;
    int cdruhyzaznam;
    int prvycheck=0;
    scanf("%d %d", &cprvyzaznam, &cdruhyzaznam); // načítame čísla záznamov na prehodenie

    if(cprvyzaznam > pocetzaznamov || cdruhyzaznam > pocetzaznamov || cprvyzaznam == 0 || cdruhyzaznam == 0 || (cprvyzaznam==cdruhyzaznam)) { // ak sú mimo listu, funkcia sa zastaví
        return -1;
    }
    if(cdruhyzaznam < cprvyzaznam) { // ošetrujeme si, keď napíšeme na druhé miesto menšie číslo ako na prvom
        int swap = cprvyzaznam;
        cprvyzaznam = cdruhyzaznam;
        cdruhyzaznam = swap;
    }

    SUBOR *docas = (*head); // definujeme si začiatok zoznamu, prvý želaný záznam, o jeden pred ním a analogicky pre druhý
    SUBOR *preddocas;
    SUBOR *predprvy;
    SUBOR *prvyzaznam;
    SUBOR *preddruhy;
    SUBOR *druhyzaznam;
    for(int i = 0; i < pocetzaznamov; i++) {
        if(i == (cprvyzaznam-1)) { // zistíme si číslo prvého požadovaného záznamu. Ošetrujeme, ak sa jedná o head
            if(i == 0) { // vtedy pracujeme iným spôsobom. Sem sa dostaneme aj vtedy, keď je druhý záznam 1
                prvycheck = 1;
            }
            prvyzaznam = docas; // uložíme si požadovaný záznam a záznam predtým, analogicky pre druhý
            predprvy = preddocas;
        }
        if(i == (cdruhyzaznam-1)) {
            druhyzaznam = docas;
            preddruhy = preddocas;
        }
        else {
            preddocas = docas; // v inom prípade postupujeme ďalej v zozname
            docas = docas->next;
        }
    }

    SUBOR *docas1 = prvyzaznam; // použijeme ďalšie pomocné štruktúry
    SUBOR *docas2 = druhyzaznam;
    if(prvycheck == 1) { // Ak je jedno z čísel prvý záznam
        if(cdruhyzaznam == 2) { // ošetrujeme, ak meníme 1 a 2
            druhyzaznam->next = prvyzaznam->next; // najprv vymeníme ich ďalšie záznamy v poradí
            prvyzaznam = druhyzaznam; // potom samotné záznamy
            preddruhy->next = docas1; // pred druhým dáme prvý
            prvyzaznam->next = docas1->next;
            (*head) = prvyzaznam; // a vymeníme hlavičku
        }
        else {
            druhyzaznam->next = prvyzaznam->next; // najprv vymeníme ich ďalšie záznamy v poradí
            prvyzaznam = druhyzaznam; // potom samotné záznamy
            preddruhy->next = docas1; // pred druhým dáme prvý
            docas1->next = docas2->next; // a za prvým bude pôvodný z druhého (v prípade posledného prvku sa dá null)
            (*head) = prvyzaznam; // a vymeníme hlavičku
        }
    }
    if(prvycheck == 0) { // v prípade, že nie je jedno z čísel 1
        predprvy->next = druhyzaznam; // namiesto prvého záznamu dáme na jeho miesto druhý
        druhyzaznam->next = prvyzaznam->next; // na jeho nové miesto next dáme pôvodný ďalší záznam z prvého
        preddruhy->next = docas1; // na druhé požadované miesto dáme prvý želaný záznam
        if (cdruhyzaznam != pocetzaznamov) { // ak sa jedná o posledný záznam, next neriešime
            docas1->next = docas2->next; // a linkneme ho o prvok ďalej
        }
    }
}

int m (SUBOR *head, int pocetzaznamov) { // doplnok, nefunguje
    SUBOR *pomocna = head;
    int *pocet_samohlasok = (int*) malloc(pocetzaznamov*sizeof(int));
    int *indexy = (int*) malloc(pocetzaznamov*(sizeof(int)));
    for (int i = 0; i < pocetzaznamov; i++) {
        int pocitadlo = 0;
        for(int j = 0; j < strlen(pomocna->prezenter); j++) {
            if((pomocna->prezenter)[j] == 'a' || (pomocna->prezenter)[j] == 'e' || (pomocna->prezenter)[j] == 'i' || (pomocna->prezenter)[j] == 'o' || (pomocna->prezenter)[j] == 'u' || (pomocna->prezenter)[j] == 'y') {
                pocitadlo++;
            }
        }
        pocet_samohlasok[i] = pocitadlo;
        indexy[i] = i;
        pomocna = pomocna->next;
    }
    for(int i = 0; i < pocetzaznamov; i++) {
        printf("%d ", pocet_samohlasok[i]);
    }
    printf("\n");
    for(int i = 0; i < pocetzaznamov; i++) {
        printf("%d ", indexy[i]);
    }
    for (int i = 0; i < pocetzaznamov - 1; i++) {
        // loop to compare array elements
        for (int j = 0; j < pocetzaznamov - i - 1; j++) {
            // compare two adjacent elements
            // change > to < to sort in descending order
            if (pocet_samohlasok[indexy[j]] > pocet_samohlasok[indexy[j] + 1]) {
                // swapping occurs if elements
                // are not in the intended order
                int temp = pocet_samohlasok[indexy[j]];
                pocet_samohlasok[indexy[j]] = pocet_samohlasok[indexy[j]+1];
                pocet_samohlasok[indexy[j]+1] = temp;
                int temp2 = indexy[j];
                indexy[j] = indexy[j+1];
                indexy[j+1] = temp2;
            }
        }
    }
    for(int i = 0; i < pocetzaznamov; i++) {
        printf("%d ", indexy[i]);
    }
        // Last i elements are already in place
    int breaking = 0;
    for (int i = 0; i < pocetzaznamov; i++) {
        if (breaking == 3) {
            break;
        }
        pomocna = head;
        int min;
        for (int j = 0; j < pocetzaznamov; j++) {
            int pomoc = 0;
            if(pocet_samohlasok[i] < pocet_samohlasok[j]) {
                min = j;
                pocet_samohlasok[j] = 0;
                pomoc++;
            }
            if (j == pocetzaznamov-1 && pomoc == 0) {
                    min = i;
                    pocet_samohlasok[i] = 1;
            }
        }
        for(int k = 0; k < min; k++) {
            pomocna = pomocna->next;
        }
        printf("%s, ", pomocna->prezenter);
        breaking++;
    }
}

int otoc_skupinu(SUBOR **head, int k) {
    SUBOR *smernik = *head;
    SUBOR *docas_predprve = NULL;
    SUBOR *docas_prve = *head;
    SUBOR *docas_preddruhe = NULL;
    SUBOR *docas_druhe = NULL;
    int pocitadlo = 0;
    int parnost = 0;
    int zaciatok = 0;
    while(smernik->next!= NULL) {
        if (pocitadlo == k-1) { // definujeme si prvykrat druhe prvky na otacanie
            docas_preddruhe = smernik;
            docas_druhe = smernik->next;
        }
        if (((pocitadlo+1) % k == 0) && (parnost % 2 == 0)) { // ak sme o 1 prvok nizsie ako k, ideme zapisovat premenne
            docas_predprve = smernik; // prvykrat sa udeje pri 3 prvku (0, k a 2k)
            docas_prve = smernik->next;
            parnost++;
        }
        if (((pocitadlo+1) % k == 0) && (parnost % 2 == 1)) { // ak sme o prvok nizsie ako k a mame uz naplnene prve struktury
            docas_preddruhe = smernik;
            docas_druhe = smernik->next;
            parnost++;
            if(zaciatok == 0) { // definujeme si iny pripad, ak ide o zaciatok zoznamu
                SUBOR *swap1 = docas_prve;
                SUBOR *swap2 = docas_druhe;
                docas_prve = docas_druhe;
                docas_prve->next = swap1->next;
                docas_druhe = swap1;
                docas_druhe->next = swap2->next;
                docas_preddruhe->next = docas_druhe;
                zaciatok++;
                *head = docas_prve;

            }
            if (zaciatok != 0) { // pre ostatne pripady
                SUBOR *swap1 = docas_prve;
                SUBOR *swap2 = docas_druhe;
                docas_predprve->next = docas_druhe;
                docas_druhe->next = swap1->next;
                docas_preddruhe->next = swap1;
                swap1->next = swap2->next;
            }
        }
        smernik = smernik->next;
    }
}
int otoc (SUBOR **head, SUBOR **tail) {
    SUBOR* prev = NULL;
    SUBOR* current = *head;
    SUBOR* next = NULL;
    while (current != NULL) {
            // Store next
            next = current->next;
            // Reverse current node's pointer
            current->next = prev;
            // Move pointers one position ahead.
            prev = current;
            current = next;
        }
        *head = prev;
    }
    //SUBOR *poradie = *head;
    //while(poradie->next != NULL) {
    //    SUBOR *temp = poradie;
    //    SUBOR *temp2 = poradie->next;
    //    poradie = poradie->next;
    //    poradie->next = temp;
    //    (poradie->next)->next = temp2;
    //    poradie = temp2;
    //}
    //SUBOR *temp = *head;
    //*head = *tail;
    //*tail = temp;

int main() {
    FILE *fr; // definujeme si súbor, pomocnú premennú na koniec while cyklu, vstup, hlavičku, koniec a počet záznamov
    int koniec = 0;
    char vstup;
    SUBOR *head = NULL;
    SUBOR *tail = NULL;
    int pocetzaznamov = 0;
    while(koniec == 0) {
        scanf("%s", &vstup);
        if (vstup == 'n') {
            n(&fr, &pocetzaznamov, &head, &tail);
        }
        if (vstup == 'v') {
            v(pocetzaznamov, head);
        }
        if (vstup == 'p') {
            p(&head, &tail, &pocetzaznamov);
        }
        if (vstup == 'z') {
            z(&head, &tail, &pocetzaznamov);
        }
        if (vstup == 'h') {
            h(head, pocetzaznamov);
        }
        if (vstup == 'a') {
            int check = 0;
            check = a(&head, pocetzaznamov);
            while(check == -1) {
                check = a(&head, pocetzaznamov);
            }
        }
        if (vstup == 'r') {
            r(&head, &tail, pocetzaznamov);
        }
        if (vstup == 'o') {
            otoc_skupinu(&head, 4);
        }
        if (vstup == 'm') {
            m(head, pocetzaznamov);
        }
        if (vstup == 'k') {
            if(head == NULL) { // ak ešte nie sú načítané štruktúry, funkcia len skončí program

            } else { // ak sú, tak ich vynulujeme
                SUBOR *docas = head;
                while(head != NULL) {
                    docas = head;
                    if (head->next == NULL){
                        head = NULL;
                    }
                    else {
                        head = head->next;
                    }
                    free(docas);
                    docas = NULL;
                }
            }
            koniec++;
        }
    }
}

