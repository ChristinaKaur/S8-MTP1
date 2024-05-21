#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;
const string noesiste = "EL LLIBRE NO APAREIX A L'INVENTARI";

struct Llibre
{
    string titol, ISBN, editorial, autor;
    int estoc1,estoc2;
};
const int MAX = 100;
typedef Llibre Llibres[MAX];

struct Taula_llibres {
    Llibres ll;
    int n;
};

void mostrar_menu()
{
    // Pre: --
    // Post: mostrar menu
    cout << "*********************************************" << endl;
    cout << "1. MOSTRAR UN LLIBRE" << endl;
    cout << "2. VENDRE UN LLIBRE" << endl;
    cout << "3. LLISTAR LLIBRES D'UN AUTOR I D'UNA EDITORIAL" << endl;
    cout << "4. MOSTRAR LLIBRES DE L'INVENTARI" << endl;
    cout << "5. MOSTRAR LLIBRES ORDENATS PER ISBN" << endl;
    cout << "6. MOSTRAR PRIMER LLIBRE DISPONIBLE DEL TOP10" << endl;
    cout << "7. SORTIR" << endl;
    cout << "*********************************************" << endl;
}

void mostrar_llibre(const Llibre ll)
{
    cout << "TITOL: " << ll.titol << endl;
    cout << "ISBN: " << ll.ISBN << endl;
    cout << "EDITORIAL: " << ll.editorial << endl;
    cout << "AUTOR: " << ll.autor << endl;
    cout << "ESTOC A LA LLIBRERIA 1: " << ll.estoc1 << endl;
    cout << "ESTOC A LA LLIBRERIA 2: " << ll.estoc2 << endl;
}

void mostrar_inventari(const Taula_llibres taula)
{
    for (int i = 0; i < taula.n; i++)
    {
        mostrar_llibre(taula.ll[i]);
    }
}

void llibres_autor_editorial(const string autor, const string editorial, const Taula_llibres taula)
{
    cout << "LLISTAT DE LLIBRES AMB AUTOR " << autor << " I EDITORIAL " << editorial << ":" << endl;
    for (int i = 0; i < taula.n; i++)
        if (autor == taula.ll[i].autor or editorial == taula.ll[i].editorial)
            cout << taula.ll[i].titol << endl;
}

void obrir_fitxer(const int num, ifstream& f_in)
{
    string nom_fitxer;
    cout << "ENTRA EL NOM DE FITXER DE L'INVENTARI DE LA LLIBRERIA " << num << ":" << endl;
    cin >> nom_fitxer;

    f_in.open(nom_fitxer.c_str());
    while (!f_in.is_open())
    {
        f_in.clear();
        
        cin >> nom_fitxer;
        f_in.open(nom_fitxer.c_str());
    }
}

void obrir_fitxer_top10(ifstream& f_in)
{
    string nom_fitxer;
    cout << "ENTRA EL NOM DE FITXER DEL TOP10:" << endl;
    cin >> nom_fitxer;

    f_in.open(nom_fitxer.c_str());
    while (!f_in.is_open())
    {
        f_in.clear();

        cin >> nom_fitxer;
        f_in.open(nom_fitxer.c_str());
    }
}

Llibre llegir_llibre(ifstream& f_in)
{
    Llibre ll;
    f_in >> ll.titol;
    if (!f_in.eof())
    {
        f_in >> ll.ISBN >> ll.editorial >> ll.autor >> ll.estoc1;
    }
    return ll;
}

int cerca(const string titol, const Taula_llibres taula)
{
    bool trobat = false;
    int esq = 0, dre = taula.n - 1, mig = 0;
    while (!trobat and esq <= dre)
    {
        mig = (esq + dre) / 2;

        if (titol == taula.ll[mig].titol)
            trobat = true;

        else if (titol < taula.ll[mig].titol)
            dre = mig - 1;

        else
            esq = mig + 1;
    }

    int pos = 0;
    if (trobat) pos = mig;
    else pos = -1;

    return pos;
}

void afegir(Llibre ll, Taula_llibres& taula)
{
    taula.ll[taula.n] = ll;
    taula.n++;
}

void copiar_productes_1(Llibre ll, ifstream& f_in, Taula_llibres& taula)
{
    // Pre: f_in obert
    // Post: ha copiat el que queda de f_in a taula
    while (not f_in.eof()) {
        ll.estoc2 = 0;
        afegir(ll, taula);
        ll = llegir_llibre(f_in);
    }
}

void copiar_productes_2(Llibre ll, ifstream& f_in, Taula_llibres& taula)
{
    // Pre: f_in obert
    // Post: ha copiat el que queda de f_in a taula
    while (not f_in.eof()) {
        ll.estoc2 = ll.estoc1;
        ll.estoc1 = 0;
        afegir(ll, taula);
        ll = llegir_llibre(f_in);
    }
}

void vendre_estoc(Llibre& ll, const int v1, const int v2)
{
    ll.estoc1 -= v1;
    ll.estoc2 -= v2;
}

int main() 
{
    ifstream f_in1, f_in2, f_top10;

    obrir_fitxer(1, f_in1);
    obrir_fitxer(2, f_in2);
    obrir_fitxer_top10(f_top10);

    Taula_llibres llibres;
    llibres.n = 0;

    Llibre ll_1 = llegir_llibre(f_in1);
    ll_1.estoc2 = 0;
    Llibre ll_2 = llegir_llibre(f_in2);

    while (!f_in1.eof() and !f_in2.eof())
    {
        if (ll_1.titol > ll_2.titol) 
        {
            afegir(ll_1, llibres);
            ll_1 = llegir_llibre(f_in1);
            ll_1.estoc2 = 0;
        }
        else if (ll_1.titol < ll_2.titol)
        {
            afegir(ll_2, llibres);
            ll_2 = llegir_llibre(f_in2);
            ll_2.estoc1 = 0;
        }
        else
        {
            ll_1.estoc2 = ll_2.estoc1;
            afegir(ll_1, llibres);
            ll_1 = llegir_llibre(f_in1);
            ll_1.estoc2 = 0;
            ll_2 = llegir_llibre(f_in2);
            ll_2.estoc1 = 0;
        }       
    }
    if (not f_in1.eof())
        copiar_productes_1(ll_1, f_in1, llibres);
    else if (not f_in2.eof())
        copiar_productes_2(ll_2, f_in2, llibres);

    int opcio = 0;
    while (opcio != 7)
    {
        mostrar_menu();
        cout << "ENTRA UNA OPCIO:" << endl;
        cin >> opcio;

        if (opcio == 1)
        {
            string titol;
            cout << "INTRODUEIX EL TITOL DEL LLIBRE QUE VOLS MOSTRAR:" << endl;
            cin >> titol;

            int pos = cerca(titol, llibres);
            if (pos != -1)
                mostrar_llibre(llibres.ll[pos]);
            else
                cout << noesiste << endl;
        }
        else if (opcio == 2)
        {
            string titol;
            cout << "INTRODUEIX EL TITOL DEL LLIBRE QUE VOLS VENDRE:" << endl;
            cin >> titol;

            int pos = cerca(titol, llibres);
            if (pos != -1)
            {
                mostrar_llibre(llibres.ll[pos]);

                int vendre1 = 0, vendre2 = 0;
                cout << "QUANTS LLIBRES VOLS VENDRE DE LA LLIBRERIA 1? " << endl;
                cin >> vendre1;
                cout << "QUANTS LLIBRES VOLS VENDRE DE LA LLIBRERIA 2? " << endl;
                cin >> vendre2;

                vendre_estoc(llibres.ll[pos], vendre1, vendre2);
                mostrar_llibre(llibres.ll[pos]);
            }
            else
                cout << noesiste << endl;
        }
        else if (opcio == 3)
        {
            string autor, editorial;
            cout << "INTRODUEIX EL NOM DE L'AUTOR:" << endl;
            cin >> autor;
            cout << "INTRODUEIX EL NOM DE L'EDITORIAL:" << endl;
            cin >> editorial;

            llibres_autor_editorial(autor,editorial,llibres);
        }
        else if (opcio == 4)
        {
            mostrar_inventari(llibres);
        }
        else if (opcio == 5)
        {

        }
        else if (opcio == 6)
        {

        }
    }
    cout << "HAS SORTIT DE L'APLICACIO" << endl;

    return 0;
}
