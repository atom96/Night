/**
 * "Maszyna Noc"
 *
 * Drugi program zaliczeniowy na zajęcia laboratoryjne ze Wstępu
 * do programowania, będący interpreterem Języka Noc.
 *
 * Opis działania Maszyny Noc, czyli abstrakcji komputera, oraz składni
 * języka noc, a także ograniczenia realizowanej wersji, znajdują się
 * na platformie Moodle:
 * https://moodle.mimuw.edu.pl/mod/assignment/view.php?id=17464
 *
 * Autor: Arkadiusz Tomczak
 * wersja 1.1
 * data: 20 grudnia 2015
 */

#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define R_PAM 256
#define ROZM R_PAM+3
#define AC R_PAM
#define IP R_PAM+1
#define SP R_PAM+2
#define IL_ETYKIET 26
#define KOD_PODST 8
#define POZYCYJNY_PODST 10

#define ACC 0
#define IND 1
#define POP 2
#define PSH 3
#define IMM 4
#define ABS 5
#define DIS 6
#define REL 7


#define JSR 0
#define JMP 1
#define JPZ 2
#define JNZ 3
#define NND 4
#define DNN 5
#define INC 6
#define DEC 7
#define DDA 8
#define BUS 9
#define LUM 10
#define VID 11
#define DOM 12
#define SNE 13
#define SGE 14
#define SLE 15
#define ADD 16
#define SUB 17
#define MUL 18
#define DIV 19
#define MOD 20
#define SEQ 21
#define SLT 22
#define SGT 23
#define LAA 24
#define LAS 25
#define LDA 26
#define STA 27
#define ICH 28
#define OCH 29
#define INU 30
#define ONU 31

/**
 * Wypełnia tablicę o rozmiarze n wartością x.
 */
void wypelnijTablice(int t[], int n, int x) {
    int i;
    for (i = 0; i < n; ++i) {
        t[i] = x;
    }
}

/**
 * Zwraca nieujemną resztę z dzielenia liczby W przez liczbę N.
 */
int mod(int W, int N) {
    if (W % N < 0) {
        return (W % N + N);
    } else {
        return (W % N);
    }
}

/**
 * Zwraca wartość liczby, która w kodzie programu zaczyna się od pozycji i-tej.
 * Zmienna sterująca i jest przesuwana na pozycję po ostatniej cyfrze
 * wczytywanej liczby.
 */
int liczbaTlumacz(char *kod, int *i) {
    int w = 0;
    while (isdigit(kod[*i])) {
        w = POZYCYJNY_PODST * w + (kod[*i] - '0');
        ++(*i);
    }
    return w;
}

/**
 * Zwraca liczbową wartość operandu, który zapisany jest w kodzie programu
 * przy pomocy symbolu. Przesuwa zmienną sterującą i na kolejną pozycję po
 * znaku oznaczający operand.
 */
int opTlumacz(char *kod, int *i) {
    switch (kod[*i + 1]) {
        case '@':
            ++(*i);
            return ACC;
        case '^':
            ++(*i);
            return IND;
        case '}':
            ++(*i);
            return POP;
        case '{':
            ++(*i);
            return PSH;
        case '#':
            ++(*i);
            return IMM;
        case '$':
            ++(*i);
            return DIS;
        case '&':
            ++(*i);
            return REL;
        default:
            return ABS;
    }
}

/**
 * Zapisuje do pamięci Maszyny Noc odpowiednie wartości liczbowe odpowiadające
 * instrukcjom zapisanym w Języku Noc, które są poprzedzone znakiem '\\'.
 * Zmienna sterująca i jest przesuwana na kolejną pozycję po ostatnim znaku
 * rozpoznawanej instrukcji.
 */
void insBackslashTlumacz(int nazwane[], char *kod, int pam[], int *i,
                         int *j, int kwadratowe[], int okragle[], int *kw,
                         int *okr) {
    ++(*i);
    switch (kod[*i]) {
        case '(':
            okragle[*okr] = *j;
            ++(*okr);
            ++(*i);
            break;
        case ')':
            pam[okragle[--*(okr)]] = *j;
            ++(*i);
            break;
        case '[':
            kwadratowe[*kw] = *j;
            ++(*kw);
            ++(*i);
            break;
        case ']':
            pam[kwadratowe[--*(kw)]] = *j;
            ++(*i);
            break;
        case '"':
            ++(*i);
            (*j) += liczbaTlumacz(kod, i);
            break;
        case '_':
            pam[*j] = KOD_PODST * JSR + opTlumacz(kod, i);
            ++(*i);
            ++(*j);
            break;
        case '~':
            pam[*j] = KOD_PODST * JNZ + opTlumacz(kod, i);
            ++(*i);
            ++(*j);
            break;
        case '|':
            pam[*j] = KOD_PODST * JMP + opTlumacz(kod, i);
            ++(*i);
            ++(*j);
            break;
        case '}':
            pam[*j] = KOD_PODST * INC + opTlumacz(kod, i);
            ++(*i);
            ++(*j);
            break;
        case '{':
            pam[*j] = KOD_PODST * DEC + opTlumacz(kod, i);
            ++(*i);
            ++(*j);
            break;
        case '+':
            pam[*j] = KOD_PODST * DDA + opTlumacz(kod, i);
            ++(*i);
            ++(*j);
            break;
        case '-':
            pam[*j] = KOD_PODST * BUS + opTlumacz(kod, i);
            ++(*i);
            ++(*j);
            break;
        case '*':
            pam[*j] = KOD_PODST * LUM + opTlumacz(kod, i);
            ++(*i);
            ++(*j);
            break;
        case '/':
            pam[*j] = KOD_PODST * VID + opTlumacz(kod, i);
            ++(*i);
            ++(*j);
            break;
        case '%':
            pam[*j] = KOD_PODST * DOM + opTlumacz(kod, i);
            ++(*i);
            ++(*j);
            break;
        case '=':
            pam[*j] = KOD_PODST * SNE + opTlumacz(kod, i);
            ++(*i);
            ++(*j);
            break;
        case '<':
            pam[*j] = KOD_PODST * SGE + opTlumacz(kod, i);
            ++(*i);
            ++(*j);
            break;
        case '>':
            pam[*j] = KOD_PODST * SLE + opTlumacz(kod, i);
            ++(*i);
            ++(*j);
            break;
        case '?':
            pam[*j] = KOD_PODST * INU + opTlumacz(kod, i);
            ++(*i);
            ++(*j);
            break;
        case '!':
            pam[*j] = KOD_PODST * ONU + opTlumacz(kod, i);
            ++(*i);
            ++(*j);
            break;
        default:
            if (isdigit(kod[*i])) {
                pam[*j] = mod(-liczbaTlumacz(kod, i), R_PAM);
                ++(*j);
            } else if (isalpha(kod[*i])) {
                nazwane[kod[*i] - 'a'] = *j;
                ++(*i);
            } else {
                assert(false);
            }
            break;
    }

}

/**
 * Zapisuje do pamięci Maszyny Noc odpowiednie wartości liczbowe odpowiadające
 * instrukcjom zapisanym w Języku Noc, które nie są poprzedzone znakiem '\\'.
 * Zmienna sterująca i jest przesuwana na kolejną pozycję po ostatnim znaku
 * rozpoznawanej instrukcji.
 */
void insTlumacz(char *kod, int pam[], int *i, int *j) {
    switch (kod[*i]) {
        case '_':
            pam[*j] = KOD_PODST * JMP + opTlumacz(kod, i);
            ++(*i);
            ++(*j);
            break;
        case '~':
            pam[*j] = KOD_PODST * JPZ + opTlumacz(kod, i);
            ++(*i);
            ++(*j);
            break;
        case '|':
            pam[*j] = KOD_PODST * NND + opTlumacz(kod, i);
            ++(*i);
            ++(*j);
            break;
        case '+':
            pam[*j] = KOD_PODST * ADD + opTlumacz(kod, i);
            ++(*i);
            ++(*j);
            break;
        case '-':
            pam[*j] = KOD_PODST * SUB + opTlumacz(kod, i);
            ++(*i);
            ++(*j);
            break;
        case '*':
            pam[*j] = KOD_PODST * MUL + opTlumacz(kod, i);
            ++(*i);
            ++(*j);
            break;
        case '/':
            pam[*j] = KOD_PODST * DIV + opTlumacz(kod, i);
            ++(*i);
            ++(*j);
            break;
        case '%':
            pam[*j] = KOD_PODST * MOD + opTlumacz(kod, i);
            ++(*i);
            ++(*j);
            break;
        case '=':
            pam[*j] = KOD_PODST * SEQ + opTlumacz(kod, i);
            ++(*i);
            ++(*j);
            break;
        case '<':
            pam[*j] = KOD_PODST * SLT + opTlumacz(kod, i);
            ++(*i);
            ++(*j);
            break;
        case '>':
            pam[*j] = KOD_PODST * SGT + opTlumacz(kod, i);
            ++(*i);
            ++(*j);
            break;
        case ',':
            pam[*j] = KOD_PODST * LAA + opTlumacz(kod, i);
            ++(*i);
            ++(*j);
            break;
        case '`':
            pam[*j] = KOD_PODST * LAS + opTlumacz(kod, i);
            ++(*i);
            ++(*j);
            break;
        case '.':
            pam[*j] = KOD_PODST * LDA + opTlumacz(kod, i);
            ++(*i);
            ++(*j);
            break;
        case ':':
            pam[*j] = KOD_PODST * STA + opTlumacz(kod, i);
            ++(*i);
            ++(*j);
            break;
        case '?':
            pam[*j] = KOD_PODST * ICH + opTlumacz(kod, i);
            ++(*i);
            ++(*j);
            break;
        case '!':
            pam[*j] = KOD_PODST * OCH + opTlumacz(kod, i);
            ++(*i);
            ++(*j);
            break;
        default:
            assert(false);
            break;
    }
}

/**
 * Wpisuje do pamięci Maszyny Noc kody znaków których ciąg w napisie zakończony
 * jest przez znak '"'. Przesuwa wartość zmiennej i do pierwszego znaku za '"'.
 */
void napisTlumacz(char *kod, int pam[], int *i, int *j) {
    ++(*i);
    while (kod[*i] != '"') {
        if (kod[*i] != '\\') {
            pam[*j] = kod[*i];
        } else {
            ++(*i);
            switch (kod[*i]) {
                case '\\':
                    pam[*j] = '\\';
                    break;
                case 'a':
                    pam[*j] = '\a';
                    break;
                case 'b':
                    pam[*j] = '\b';
                    break;
                case 'f':
                    pam[*j] = '\f';
                    break;
                case 'n':
                    pam[*j] = '\n';
                    break;
                case 'r':
                    pam[*j] = '\r';
                    break;
                case 't':
                    pam[*j] = '\t';
                    break;
                case 'v':
                    pam[*j] = '\v';
                    break;
                default:
                    pam[(*j)++] = '\\';
                    pam[*j] = kod[*i];
                    break;
            }
        }
        ++(*i);
        ++(*j);
    }

    ++(*i);
}

/**
 * Uzupelnia brakujące adresy na które wskazują etykiety nazwane.
 */
void uzupelnijAdresy(int nazwane[], int pam[], int n) {
    int i;
    for (i = 0; i < n; ++i) {
        if (pam[i] < 0) {
            pam[i] = nazwane[-pam[i] - 1];
        }
    }
}

/**
 * Wpisuje do pamięci Maszyny Noc wartości odpowiadające kolejnym instrukcjom
 * zapisanym w Języku Noc.
 */
void zaladuj(int pam[], char *kod) {
    int nazwane[IL_ETYKIET];
    int okragle[R_PAM];
    int kwadratowe[R_PAM];
    int i = 0;
    int j = 0;
    int okr = 0;
    int kw = 0;

    wypelnijTablice(nazwane, IL_ETYKIET, -1);

    while (kod[i] != '\0') {
        switch (kod[i]) {
            case '\0':
                break;
            case ';':
                while (kod[i] != '\n' && kod[i] != '\0') {
                    ++i;
                }
                break;
            case ' ':
                ++i;
                break;
            case '\n':
                ++i;
                break;
            case '\t':
                ++i;
                break;
            case '"':
                napisTlumacz(kod, pam, &i, &j);
                break;
            case '(':
                okragle[okr] = j;
                ++j;
                ++okr;
                ++i;
                break;
            case '[':
                kwadratowe[kw] = j;
                ++j;
                ++kw;
                ++i;
                break;
            case ')':
                pam[j] = okragle[--okr];
                ++j;
                ++i;
                break;
            case ']':
                pam[j] = kwadratowe[--kw];
                ++j;
                ++i;
                break;
            case '\\':
                insBackslashTlumacz(nazwane, kod, pam, &i, &j, kwadratowe,
                                    okragle,
                                    &kw, &okr);
                break;
            default:
                if (isdigit(kod[i])) {
                    pam[j] = mod(liczbaTlumacz(kod, &i), R_PAM);
                    ++j;
                } else if (isalpha(kod[i])) {
                    if (nazwane[kod[i] - 'a'] > -1) {
                        pam[j] = nazwane[kod[i] - 'a'];
                        ++j;
                        ++i;
                    } else {
                        pam[j] = -kod[i] - 1 + 'a';
                        ++j;
                        ++i;
                    }
                } else {
                    insTlumacz(kod, pam, &i, &j);
                }
                break;
        }
    }

    uzupelnijAdresy(nazwane, pam, R_PAM);
}

/**
 * Zwraca adres operandu w pamięci Maszyny Noc.
 */
int adr_operand(int pam[], int op) {
    int adr = -1;
    switch (op) {
        case ACC:
            adr = AC;
            break;
        case IND:
            adr = pam[AC];
            break;
        case POP:
            adr = pam[SP];
            pam[SP] = mod(pam[SP] + 1, R_PAM);
            break;
        case PSH:
            pam[SP] = mod(pam[SP] - 1, R_PAM);
            adr = pam[SP];
            break;
        case IMM:
            adr = mod(pam[IP] + 1, R_PAM);
            break;
        case ABS:
            adr = pam[mod(pam[IP] + 1, R_PAM)];
            break;
        case DIS:
            adr = mod(pam[mod(pam[IP] + 1, R_PAM)] + pam[SP], R_PAM);
            break;
        case REL:
            adr = mod(pam[mod(pam[IP] + 1, R_PAM)] + pam[IP] + 2, R_PAM);
            break;
        default:
            break;
    }
    return adr;
}

/**
 * Odpowiada za przejscie do następnej instrukcji podczas wykonywania programu
 * przez Maszynę Noc.
 */
void przejdz(int pam[], int op) {
    if (op < IMM) {
        pam[IP] = mod(pam[IP] + 1, R_PAM);
    } else {
        pam[IP] = mod(pam[IP] + 2, R_PAM);
    }
}

/**
 * Wykonuje odpowiednią instrukcje zapisaną w pamięci Maszyny Moc.
 */
void instrukcja_wykonaj(int pam[]) {
    int op = pam[pam[IP]] % KOD_PODST;
    int ins = pam[pam[IP]] / KOD_PODST;
    int adr = adr_operand(pam, op);
    char znak;
    int liczba;
    przejdz(pam, op);
    switch (ins) {
        case JSR:
            if (op == ACC) {
                exit(EXIT_SUCCESS);
            }
            pam[AC] = pam[IP];
            pam[IP] = adr;
            break;
        case JMP:
            if (op == ACC) {
                exit(EXIT_SUCCESS);
            }
            pam[IP] = adr;
            break;
        case JPZ:
            if (op == ACC) {
                exit(EXIT_SUCCESS);
            }
            if (pam[AC] == 0) {
                pam[IP] = adr;
            }
            break;
        case JNZ:
            if (op == ACC) {
                exit(EXIT_SUCCESS);
            }
            if (pam[AC] != 0) {
                pam[IP] = adr;
            }
            break;
        case NND:
            pam[AC] = mod(~(pam[AC] & pam[adr]), R_PAM);
            break;
        case DNN:
            pam[adr] = mod(~(pam[adr] & pam[AC]), R_PAM);
            break;
        case INC:
            pam[adr] = mod(pam[adr] + 1, R_PAM);
            break;
        case DEC:
            pam[adr] = mod(pam[adr] - 1, R_PAM);
            break;
        case DDA:
            pam[adr] = mod(pam[adr] + pam[AC], R_PAM);
            break;
        case BUS:
            pam[adr] = mod(pam[adr] - pam[AC], R_PAM);
            break;
        case LUM:
            pam[adr] = mod(pam[adr] * pam[AC], R_PAM);
            break;
        case VID:
            if (pam[AC] == 0) {
                exit(EXIT_FAILURE);
            }
            pam[adr] = mod(pam[adr] / pam[AC], R_PAM);
            break;
        case DOM:
            if (pam[AC] == 0) {
                exit(EXIT_FAILURE);
            }
            pam[adr] = mod(pam[adr] % pam[AC], R_PAM);
            break;
        case SNE:
            if (pam[AC] != pam[adr]) {
                pam[IP] = mod(pam[IP] + 2, R_PAM);
            }
            break;
        case SGE:
            if (pam[AC] >= pam[adr]) {
                pam[IP] = mod(pam[IP] + 2, R_PAM);
            }
            break;
        case SLE:
            if (pam[AC] <= pam[adr]) {
                pam[IP] = mod(pam[IP] + 2, R_PAM);
            }
            break;
        case ADD:
            pam[AC] = mod(pam[adr] + pam[AC], R_PAM);
            break;
        case SUB:
            pam[AC] = mod(pam[AC] - pam[adr], R_PAM);
            break;
        case MUL:
            pam[AC] = mod(pam[AC] * pam[adr], R_PAM);
            break;
        case DIV:
            if (pam[adr] == 0) {
                exit(EXIT_FAILURE);
            }
            pam[AC] = mod(pam[AC] / pam[adr], R_PAM);
            break;
        case MOD:
            if (pam[adr] == 0) {
                exit(EXIT_FAILURE);
            }
            pam[AC] = mod(pam[AC] % pam[adr], R_PAM);
            break;
        case SEQ:
            if (pam[AC] == pam[adr]) {
                pam[IP] = mod(pam[IP] + 2, R_PAM);
            }
            break;
        case SLT:
            if (pam[AC] < pam[adr]) {
                pam[IP] = mod(pam[IP] + 2, R_PAM);
            }
            break;
        case SGT:
            if (pam[AC] > pam[adr]) {
                pam[IP] = mod(pam[IP] + 2, R_PAM);
            }
            break;
        case LAA:
            if (op == ACC) {
                exit(EXIT_SUCCESS);
            }
            pam[AC] = adr;
            break;
        case LAS:
            if (op == ACC) {
                exit(EXIT_SUCCESS);
            }
            pam[SP] = adr;
            break;
        case LDA:
            pam[AC] = pam[adr];
            break;
        case STA:
            pam[adr] = pam[AC];
            break;
        case ICH:
            if (scanf("%c", &znak) > 0) {
                pam[adr] = znak;
            } else {
                pam[adr] = R_PAM - 1;
            }
            break;
        case OCH:
            printf("%c", pam[adr]);
            break;
        case INU:
            if (scanf("%d", &liczba) > 0) {
                pam[adr] = mod(liczba, R_PAM);
            } else {
                pam[adr] = R_PAM - 1;
            }
            break;
        case ONU:
            printf("%d", pam[adr]);
            break;
        default:
            break;
    }
}

/**
 * Wykonuje program, zapisany w pamięci Maszyny Noc
 */
void wykonaj(int pam[]) {
    for (;;) {
        instrukcja_wykonaj(pam);
    }
}

int main(int argc, char *argv[]) {
    int pam[ROZM];
    assert(argc == 2);
    wypelnijTablice(pam, ROZM, 0);
    zaladuj(pam, argv[1]);
    wykonaj(pam);
    return 0;
}

