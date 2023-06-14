#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <ctime>

using namespace std;

// Klasa pola planszy
class Pole 
{
public:
    enum Status { PUSTE, STATEK, TRAFIONE, PUDLO };

    Pole() : status(PUSTE) {}

    void ustawStatus(Status nowyStatus) 
    {
        status = nowyStatus;
    }

    Status getStatus() const 
    {
        return status;
    }

    friend ostream& operator<<(ostream& os, const Pole& pole);

private:
    Status status;
};

ostream& operator<<(ostream& os, const Pole& pole) 
{
    switch (pole.status) 
    {
    case Pole::PUSTE:
        os << '.';
        break;
    case Pole::STATEK:
        os << 'S';
        break;
    case Pole::TRAFIONE:
        os << 'X';
        break;
    case Pole::PUDLO:
        os << 'O';
        break;
    }
    return os;
}

// Klasa planszy
class Plansza 
{
public:
    vector<vector<Pole>> plansza;
    int rozmiar;

    Plansza(int rozmiar) : rozmiar(rozmiar) 
    {
        for (int i = 0; i < rozmiar; ++i) 
        {
            plansza.push_back(vector<Pole>(rozmiar));
        }
    }

    void ustawStatek(int x, int y) 
    {
        plansza[x][y].ustawStatus(Pole::STATEK);
    }

    bool sprawdzTrafienie(int x, int y) 
    {
        if (plansza[x][y].getStatus() == Pole::STATEK) 
        {
            plansza[x][y].ustawStatus(Pole::TRAFIONE);
            return true;
        }
        else 
        {
            plansza[x][y].ustawStatus(Pole::PUDLO);
            return false;
        }
    }

    bool czyKoniecGry() const 
    {
        for (int i = 0; i < rozmiar; ++i) 
        {
            for (int j = 0; j < rozmiar; ++j) 
            {
                if (plansza[i][j].getStatus() == Pole::STATEK) 
                {
                    return false;
                }
            }
        }
        return true;
    }
    friend ostream& operator<<(ostream& os, const Plansza& plansza); 
};

ostream& operator<<(ostream& os, const Plansza& plansza) 
{
    for (int i = 0; i < plansza.rozmiar; ++i) 
    {
        for (int j = 0; j < plansza.rozmiar; ++j) 
        {
            os << plansza.plansza[i][j] << ' ';
        }
        os << std::endl;
    }
    return os;
}

// Klasa abstrakcyjna statku
class Statek 
{
public:
    Statek(int rozmiar) : rozmiar(rozmiar) {}

    int getRozmiar() const 
    {
        return rozmiar;
    }

    virtual string getNazwa() const = 0;
    virtual void umiescNaPlanszy(Plansza& plansza) = 0;

protected:
    int rozmiar;
};

// Klasa czo³g
class Czolg : public Statek 
{
public:
    Czolg() : Statek(4) {}

    string getNazwa() const override 
    {
        return "Czolg";
    }

    void umiescNaPlanszy(Plansza& plansza) override 
    {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<int> dis(0, plansza.rozmiar - rozmiar);

        int x = dis(gen);
        int y = dis(gen);

        for (int i = x; i < x + rozmiar; ++i) 
        {
            plansza.ustawStatek(i, y);
        }
    }
};

// Klasa okrêt podwodny
class OkretPodwodny : public Statek 
{
public:
    OkretPodwodny() : Statek(3) {}

    string getNazwa() const override 
    {
        return "Okret podwodny";
    }

    void umiescNaPlanszy(Plansza& plansza) override 
    {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<int> dis(0, plansza.rozmiar - rozmiar);

        int x = dis(gen);
        int y = dis(gen);

        for (int i = y; i < y + rozmiar; ++i) 
        {
            plansza.ustawStatek(x, i);
        }
    }
};

// Klasa ³ódka
class lodka : public Statek
{
public:
    lodka() : Statek(2) {}

    string getNazwa() const override
    {
        return "£ódka";
    }

    void umiescNaPlanszy(Plansza& plansza) override
    {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<int> dis(0, plansza.rozmiar - rozmiar);

        int x = dis(gen);
        int y = dis(gen);

        for (int i = y; i < y + rozmiar; ++i)
        {
            plansza.ustawStatek(x, i);
        }
    }
};

// Klasa gry
class Gra {
public:
    Gra(int rozmiar) : planszaGracza(rozmiar), planszaKomputera(rozmiar) 
    {
        czolgGracza = make_unique<Czolg>();
        okretPodwodnyGracza = make_unique<OkretPodwodny>();
        lodkaGracza = make_unique<lodka>();
        lodkaGracza = make_unique<lodka>();

        czolgKomputera = make_unique<Czolg>();
        okretPodwodnyKomputera = make_unique<OkretPodwodny>();
        lodkaKomputera = make_unique<lodka>();
        lodkaKomputera = make_unique<lodka>();

        czolgGracza->umiescNaPlanszy(planszaGracza);
        okretPodwodnyGracza->umiescNaPlanszy(planszaGracza);
        lodkaGracza->umiescNaPlanszy(planszaGracza);
        lodkaGracza->umiescNaPlanszy(planszaGracza);

        czolgKomputera->umiescNaPlanszy(planszaKomputera);
        okretPodwodnyKomputera->umiescNaPlanszy(planszaKomputera);
        lodkaGracza->umiescNaPlanszy(planszaKomputera);
        lodkaGracza->umiescNaPlanszy(planszaKomputera);
    }

    void rozpocznijGre() 
    {
        while (!planszaGracza.czyKoniecGry() && !planszaKomputera.czyKoniecGry()) 
        {
            cout << "Twoja plansza:" << endl;
            cout << planszaGracza << endl;

            cout << "Plansza komputera:" << endl;
            cout << planszaKomputera << endl;

            int x, y;
            cout << "Podaj wspolrzedne strzalu (x y) oddzielone spacja: ";
            cin >> x >> y;

            bool trafienie = planszaKomputera.sprawdzTrafienie(x, y);
            if (trafienie) 
            {
                cout << "Trafiony!" << std::endl;
            }
            else {
                cout << "Pudlo!" << std::endl;
            }

            if (!planszaKomputera.czyKoniecGry()) 
            {
                wykonajRuchKomputera();
            }
        }

        if (planszaGracza.czyKoniecGry()) 
        {
            cout << "Koniec gry! Przegrales." << endl;
        }
        else {
            cout << "Koniec gry! Wygrales!" << endl;
        }

        zapiszDoPliku("plansza_gracza.txt", planszaGracza);
        zapiszDoPliku("plansza_komputera.txt", planszaKomputera);
    }

private:
    void wykonajRuchKomputera() 
    {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<int> dis(0, planszaGracza.rozmiar - 1);

        int x = dis(gen);
        int y = dis(gen);

        bool trafienie = planszaGracza.sprawdzTrafienie(x, y);
        if (trafienie) 
        {
            cout << "Komputer trafil w Twoj statek!" << endl;
        }
        else 
        {
            cout << "Komputer spudlowal!" << endl;
        }
    }

    void zapiszDoPliku(const string& nazwaPliku, const Plansza& plansza) 
    {
        ofstream plik(nazwaPliku);
        if (plik.is_open()) 
        {
            plik << plansza;
            plik.close();
            cout << "Zapisano plansze do pliku: " << nazwaPliku << endl;
        }
        else 
        {
            cout << "Nie udalo sie zapisac planszy do pliku." << endl;
        }
    }

    Plansza planszaGracza;
    Plansza planszaKomputera;

    unique_ptr<Czolg> czolgGracza;
    unique_ptr<OkretPodwodny> okretPodwodnyGracza;
    unique_ptr<lodka> lodkaGracza;

    unique_ptr<Czolg> czolgKomputera;
    unique_ptr<OkretPodwodny> okretPodwodnyKomputera;
    unique_ptr<lodka> lodkaKomputera;
};

int main() 
{
    cout << "*** GRA W STATKI ***" << endl;

    int rozmiar;
    cout << "Podaj rozmiar planszy: ";
    cin >> rozmiar;

    Gra gra(rozmiar);
    gra.rozpocznijGre();

    return 0;
}