//Jakub_Grudziński_197973_EiT_BlackBoxGame
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

const int wiersze = 5;
const int kolumny = 5;
const char granicaGoraDol = 205;
const char granicaLewoPrawo = 186;
const char rogGornyLewy = 201;
const char rogGornyPrawy = 187;
const char rogDolnyLewy = 200;
const char rogDolnyPrawy = 188;
int obecnyPoziom;
int atomyLiczba;
int atomyZaznaczone;
int pozycjaGraczaWiersz;
int pozycjaGraczaKolumna;
char kierunek;
int poprzedniWiersz;
int poprzedniaKolumna;

class BlackBoxGame {
private:
    char tablica[wiersze][kolumny];
    pair<int, int> ruchyUndo[5];
    pair<int, int> ruchyRedo[5];
    int rozmiarStosuUndo;
    int rozmiarStosuRedo;
    int laserPozycjaWiersz;
    int laserPozycjaKolumna;
    int laserKierunekWiersz;
    int laserKierunekKolumna;

public:
    BlackBoxGame() {
        obecnyPoziom = 1;
        atomyLiczba = 3;
        atomyZaznaczone = 0;
        pozycjaGraczaWiersz = 0;
        pozycjaGraczaKolumna = 0;

        utworzPlansze();

        rozmiarStosuUndo = 0;
        rozmiarStosuRedo = 0;
    }

    void printWitaj()
    {
        cout << "Witaj w grze BlackBox! - Jakub_Grudzinski_197973" << endl << endl << "Znajdz ukryte atomy na planszy" << endl;
    }

    void utworzPlansze() {
        for (int i = 0; i < wiersze; i++) {
            for (int j = 0; j < kolumny; j++) {
                tablica[i][j] = ' ';
            }
        }

        srand(static_cast<unsigned>(time(nullptr)));
        for (int i = 0; i < atomyLiczba; i++) {
            int wiersz = rand() % wiersze;
            int kolumna = rand() % kolumny;
            if (tablica[wiersz][kolumna] != 'A') {
                tablica[wiersz][kolumna] = 'A';
            }
            else {
                i--;
            }
        }
        laserKierunekWiersz = 0;
        laserKierunekKolumna = 1;
    }

    void pokazPlansze() {
        cout << rogGornyLewy;
        for (int j = 0; j < kolumny * 3; j++) {
            cout << granicaGoraDol;
        }
        cout << rogGornyPrawy << endl;

        for (int i = 0; i < wiersze; i++) {
            cout << granicaLewoPrawo;
            for (int j = 0; j < kolumny; j++) {
                if (i == pozycjaGraczaWiersz && j == pozycjaGraczaKolumna) {
                    cout << "[X]";
                }
                else {
                    cout << "[" << (tablica[i][j] == 'A' ? ' ' : tablica[i][j]) << "]";
                }
            }
            cout << granicaLewoPrawo << endl;
        }
        cout << rogDolnyLewy;
        for (int j = 0; j < kolumny * 3; j++) {
            cout << granicaGoraDol;
        }
        cout << rogDolnyPrawy << endl;
    }

    void wystrzelLaser() {
        int startWiersz = pozycjaGraczaWiersz;
        int startKolumna = pozycjaGraczaKolumna;

        laserPozycjaWiersz = startWiersz;
        laserPozycjaKolumna = startKolumna;

        while (laserPozycjaWiersz >= 0 && laserPozycjaWiersz < wiersze && laserPozycjaKolumna >= 0 && laserPozycjaKolumna < kolumny) {
            if (tablica[laserPozycjaWiersz][laserPozycjaKolumna] == 'A') {
                cout << "Trafiles w atom (H)!" << endl;
                tablica[laserPozycjaWiersz][laserPozycjaKolumna] = 'H';
                ++atomyZaznaczone;
                break;
            }
            else {
                laserPozycjaWiersz += laserKierunekWiersz;
                laserPozycjaKolumna += laserKierunekKolumna;
                if (tablica[laserPozycjaWiersz][laserPozycjaKolumna] == ' ') {
                    cout << "Laser sie odbil (R)!" << endl;
                    tablica[laserPozycjaWiersz][laserPozycjaKolumna] = 'R';
                    break;
                }
            }

        }
        dodajRuchUndo(startWiersz, startKolumna);
        wyczyscRuchyRedo();
    }

    void przemiescGracza(char kierunek) {
        switch (kierunek) {
        case 'w':
        case 'W':
            if (pozycjaGraczaWiersz > 0) {
                --pozycjaGraczaWiersz;
            }
            break;
        case 's':
        case 'S':
            if (pozycjaGraczaWiersz < wiersze - 1) {
                ++pozycjaGraczaWiersz;
            }
            break;
        case 'a':
        case 'A':
            if (pozycjaGraczaKolumna > 0) {
                --pozycjaGraczaKolumna;
            }
            break;
        case 'd':
        case 'D':
            if (pozycjaGraczaKolumna < kolumny - 1) {
                ++pozycjaGraczaKolumna;
            }
            break;
        }
        laserKierunekWiersz = pozycjaGraczaWiersz - poprzedniWiersz;
        laserKierunekKolumna = pozycjaGraczaKolumna - poprzedniaKolumna;

        dodajRuchUndo(poprzedniWiersz, poprzedniaKolumna);
        wyczyscRuchyRedo();
    }

    void dodajRuchUndo(int wiersz, int kolumna) {
        if (rozmiarStosuUndo < 5) {
            ruchyUndo[rozmiarStosuUndo++] = make_pair(wiersz, kolumna);
        }
        else {
            for (int i = 0; i < 4; i++) {
                ruchyUndo[i] = ruchyUndo[i + 1];
            }
            ruchyUndo[4] = make_pair(wiersz, kolumna);
        }
    }

    void wyczyscRuchyUndo() {
        rozmiarStosuUndo = 0;
    }

    void wyczyscRuchyRedo() {
        rozmiarStosuRedo = 0;
    }

    void cofnijRuch() {
        if (rozmiarStosuUndo > 0) {
            ruchyRedo[rozmiarStosuRedo++] = ruchyUndo[rozmiarStosuUndo - 1];
            for (int i = rozmiarStosuUndo - 1; i > 0; i--) {
                ruchyUndo[i] = ruchyUndo[i - 1];
            }
            --rozmiarStosuUndo;

            pozycjaGraczaWiersz = ruchyUndo[rozmiarStosuUndo].first;
            pozycjaGraczaKolumna = ruchyUndo[rozmiarStosuUndo].second;

            cout << "Cofnieto ruch" << endl;
        }
        else {
            cout << "Brak ruchow do cofniecia" << endl;
        }
    }

    void przywrocRuch() {
        if (rozmiarStosuRedo > 0) {
            ruchyUndo[rozmiarStosuUndo++] = ruchyRedo[rozmiarStosuRedo - 1];
            for (int i = rozmiarStosuRedo - 1; i > 0; i--) {
                ruchyRedo[i] = ruchyRedo[i - 1];
            }
            --rozmiarStosuRedo;

            pozycjaGraczaWiersz = ruchyUndo[rozmiarStosuUndo - 1].first;
            pozycjaGraczaKolumna = ruchyUndo[rozmiarStosuUndo - 1].second;

            cout << "Przywrocono ruch" << endl;
        }
        else {
            cout << "Brak ruchow do przywrocenia" << endl;
        }
    }


    void pokazUkryteAtomy() {
        cout << "\nUmiejscowienie atomow na planszy:" << endl;
        for (int i = 0; i < wiersze; i++) {
            for (int j = 0; j < kolumny; j++) {
                cout << "[" << (tablica[i][j] == 'A' ? 'A' : ' ') << "]";
            }
            cout << endl;
        }
        cout << endl;
    }

    void restartPoziom() {
        utworzPlansze();
        atomyZaznaczone = 0;
        pozycjaGraczaWiersz = wiersze / 5;
        pozycjaGraczaKolumna = kolumny / 5;
        cout << "Poziom " << obecnyPoziom << " zrestartowany" << endl;
    }

    void wyswietlPostep() {
        cout << "Obecny poziom: " << obecnyPoziom << endl;
        cout << "Zaznaczone atomy " << atomyZaznaczone << "/" << atomyLiczba << endl;
    }

    void wyjdzDoMenu() {
        cout << "Powrot do menu" << endl;
    }

    void przejdzDoNastepnegoPoziomu() {
        ++obecnyPoziom;
        utworzPlansze();
        atomyZaznaczone = 0;
        pozycjaGraczaWiersz = wiersze / 5;
        pozycjaGraczaKolumna = kolumny / 5;
        cout << "Przechodzisz do nastepnego poziomu: " << obecnyPoziom << endl;
    }
};

int main() {
    BlackBoxGame game;

    game.printWitaj();

    char opcje;
    do {
        cout << "\nMenu glowne:" << endl;
        cout << "1. Wybierz poziom (1)" << endl;
        cout << "2. Wyjdz z gry (k)" << endl;

        cout << "Wybierz opcje: ";
        cin >> opcje;

        switch (opcje) {
        case '1':
            cout << "Podaj numer poziomu : ";
            cin >> obecnyPoziom;
            obecnyPoziom = max(1, obecnyPoziom);
            game.utworzPlansze();
            while (atomyZaznaczone < atomyLiczba) {
                game.pokazPlansze();
                game.wyswietlPostep();

                char kierunek;
                cout << "Podaj kierunek ruchu W/S/A/D, nacisnij 'X/x' aby strzelic, 'Q/q' aby wyswietlic menu, 'H' aby na chwile pokazac umiejscowienie atomow, 'U/u' aby cofnac ostatni wykonany ruch, 'R/r' aby przywrocic ostatni wykonany ruch, lub 'k' aby zakonczyc i zobaczyc rozwiazanie: ";
                cin >> kierunek;

                switch (kierunek) {
                case 'W':
                case 'w':
                case 'S':
                case 's':
                case 'A':
                case 'a':
                case 'D':
                case 'd':
                    game.przemiescGracza(kierunek);
                    break;

                case 'X':
                case 'x':
                    game.wystrzelLaser();
                    break;

                case 'Q':
                case 'q':
                    cout << "\nMenu rozgrywki:" << endl;
                    cout << "1. Kontynuuj rozgrywke (1)" << endl;
                    cout << "2. Wyswietl plansze od nowa (2)" << endl;
                    cout << "3. Zakoncz i zobacz rozwiazanie (p)" << endl;
                    cout << "4. Przejdz do nastepnego poziomu -> (4)" << endl;

                    cout << "Wybierz opcje: ";
                    cin >> opcje;

                    switch (opcje) {
                    case '1':
                        break;

                    case '2':
                        game.restartPoziom();
                        break;

                    case 'p':
                        cout << "Koniec rozgrywki..." << endl;
                        game.pokazUkryteAtomy();
                        exit(0);
                        break;

                    case'4':
                        game.przejdzDoNastepnegoPoziomu();
                        break;

                    default:
                        cout << "Niepoprawna opcja. Sprobuj ponownie" << endl;
                    }
                    break;

                case 'H':
                    game.pokazUkryteAtomy();
                    break;

                case 'U':
                case 'u':
                    game.cofnijRuch();
                    break;

                case 'R':
                case 'r':
                    game.przywrocRuch();
                    break;

                case 'k':
                    game.pokazUkryteAtomy();
                    exit(0);
                    break;

                default:
                    cout << "Niepoprawna opcja. Sprobuj ponownie" << endl;
                }
            }

            cout << "\nGratulacje! Znalazles wszystkie atomy" << endl;
            game.pokazPlansze();
            game.wyswietlPostep();

            cout << "Nacisnik k, aby zakonczyc rozgrywke lub inny klawisz, aby kontynuowac: ";
            cin >> opcje;

            if (opcje == 'k') {
                cout << "Koniec rozgrywki..." << endl;
                exit(0);
                break;
            }
            else {
                game.restartPoziom();
                break;
            }
            break;

        case 'k':
            exit(0);
            break;

        default:
            cout << "Niepoprawna opcja. Sprobuj ponownie" << endl;
        }

    } while (opcje != '4');

    return 0;
}

