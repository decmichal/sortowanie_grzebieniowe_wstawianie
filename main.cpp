#include <iostream>
#include <time.h>
#include <windows.h>
#include <psapi.h>

using namespace std;

int ile; //Zmienna sluzaca do zadeklarowania ilosci elementow w tablicy
clock_t start, stop; //Zmienne do pomiaru czasu wykonywania algorytmu
double czas1, czas2; //Utworzenie zmiennej czas do porownania czasow sortowan

static ULARGE_INTEGER lastCPU, lastSysCPU, lastUserCPU;
static int numProcessors;
static HANDLE self;

void sortowanie_grzebieniowe( int *tablica, int n ); //Utworzenie funkcji do sortowania grzebieniowego

void sortowanie_przez_wstawianie(int *tablica, int n);   //Utworzenie funkcji do sortowania poprzez wstawianie

void init();

double getCurrentValue();

int main()
{
    init();
    cout << "Porownanie czasow sortowania" << endl;
    cout<<"Ile losowych liczb w tablicy: "<<endl;
    cin>>ile;
    int *tablica1; //Utworzenie pierwszej dynamicznej tablicy
    tablica1 = new int [ile];
    int *tablica2; //Utworzonie drugiej dynamicznej tablicy
    tablica2 = new int [ile];
    srand(time(NULL));

    for(int i=0; i<ile; i++) //Wpisanie liczb pseudolosowych do tablicy 1
    {
        tablica1[i]=rand()%100000+1; //Wpisywanie liczb pseudolosowych do tablicy z zakresu od 0 do 100000
    }

    for(int i=0; i<ile; i++) //Wpisanie tych samych liczb z tablica pierwszej do tablicy drugiej, aby mozna bylo operowac na tych samych liczbach
    {
        tablica2[i]=tablica1[i];
    }

    cout<<"Sortuje teraz poprzez wstawianie. Prosze czekac"<<endl; //Pomiar czasu wykonywania sortowania poprzez wstawianie
    start=clock();
    double pomiar1=getCurrentValue();
    sortowanie_przez_wstawianie(tablica1, ile);
    pomiar1=getCurrentValue();
    cout<<"Procentowe wykorzystanie procesora podczas sortowania przez wstawianie: "<<pomiar1<<endl;
    stop=clock();
    czas2=(double)(stop-start)/CLOCKS_PER_SEC;
    cout<<"Czas trwania srotowania poprzez wstawiania: "<<czas2<<" sekund"<<endl;

     cout<<"Sortuje teraz grzebieniowo. Prosze czekac"<<endl; //Pomiar czasu wykonywania sortowania grzebieniowego
    start=clock();
    double pomiar2=getCurrentValue();
    sortowanie_grzebieniowe(tablica2, ile);
    pomiar2=getCurrentValue();
    cout<<"Procentowe wykorzystanie procesora podczas sortowania grzebieniowego: "<<pomiar2<<endl;
    stop=clock();
    czas1=(double)(stop-start)/CLOCKS_PER_SEC;
    cout<<"Czas trwania sortowania grzebieniowego: "<<czas1<<" sekund"<<endl;

    system("pause");
    return 0;

}

void sortowanie_grzebieniowe( int *tablica, int n ) /*Algorytm przyjmuje za rozpietosc wielkosc tablicy, ktora nastepnie dzieli przez 1.3 i odrzuca czesc ulamkkowa.
                                                      Nastepnie bada sie kolejno wszystkie pary obiektow odleglych o rozpietosc. Gdy rozpietosc osiagnie wartosc 1,
                                                      to srotowanie zostanie ukonczone*/

{
    int gap = n; //za rozpietosc (gap) nalezy przyjac wielkosc tablicy
    bool replace = true;

    while( gap > 1 || replace )
    {
        gap = gap * 10 / 13;
        if( gap == 0 )
             gap = 1;

        replace = false;
        for( int i = 0; i + gap < n; i++ )
        {
            if( tablica[ i + gap ] < tablica[ i ] )
            {
                swap( tablica[ i ], tablica[ i + gap ] );
                replace = true;
            }
        }
    }

}


void sortowanie_przez_wstawianie(int *tablica, int n)
{
    int i,j,x;

    for(j = n - 2; j >= 0; j--)
    {
    x = tablica[j]; //zawiera wybrany element z zbioru
    i = j + 1;
        while((i < n) && (x > tablica[i]))
        {
            tablica[i - 1] = tablica[i];
            i++;
        }
    tablica[i - 1] = x;
   }

}

void init(){
    SYSTEM_INFO sysInfo;
    FILETIME ftime, fsys, fuser;

    GetSystemInfo(&sysInfo);
    numProcessors = sysInfo.dwNumberOfProcessors;

    GetSystemTimeAsFileTime(&ftime);
    memcpy(&lastCPU, &ftime, sizeof(FILETIME));

    self = GetCurrentProcess();
    GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
    memcpy(&lastSysCPU, &fsys, sizeof(FILETIME));
    memcpy(&lastUserCPU, &fuser, sizeof(FILETIME));
}

double getCurrentValue(){
    FILETIME ftime, fsys, fuser;
    ULARGE_INTEGER now, sys, user;
    double percent;

    GetSystemTimeAsFileTime(&ftime);
    memcpy(&now, &ftime, sizeof(FILETIME));

    GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
    memcpy(&sys, &fsys, sizeof(FILETIME));
    memcpy(&user, &fuser, sizeof(FILETIME));
    percent = (sys.QuadPart - lastSysCPU.QuadPart) +
        (user.QuadPart - lastUserCPU.QuadPart);
    percent /= (now.QuadPart - lastCPU.QuadPart);
    percent /= numProcessors;
    lastCPU = now;
    lastUserCPU = user;
    lastSysCPU = sys;

    return percent * 100;
}






