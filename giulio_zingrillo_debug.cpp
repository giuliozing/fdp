#include <iostream>
using namespace std;

class MatQuad{
    int **m;
    int s;
public:
    MatQuad(int);
    void aggiorna(const int*, const int);//mi sembra opportuno, anche in analogia con le cstringhe, passare il vettore come argomento costante
    void stampa();//bug: non è necessario passare l'istanza come argomento perché la funzione è dichiarata dentro una classe
    bool trova();
    void raddoppia();
    ~MatQuad();
};
MatQuad::MatQuad(int size){
    if(s<1) s =1; //gestione del processo in caso di dimensione negativa
    s = size;
    m = new int*[s];

    for(int j = 0; j < s; j++){

        m[j] = new int[s];

        for(int i = 0; i < s; i++)
            m[j][i] = 0;
    }
}

void MatQuad::aggiorna(const int* vett, const int dim){
    if(vett==nullptr||dim<1) return;//controllo sulla consistenza del vettore
    int i=0, h = 0, lim = dim;
    if(lim>s*s) lim = s*s;//evita di far girare il ciclo for più a lungo del necessario
    for(int j = 0; j < lim; j++){
        if(j!=0&&j%s==0){i++; h+=s;}
        m[i][j-h] = vett[j];
    }
    /* A causa dell'allocazione in memoria dinamica, la fine della prima riga non è contigua,
    in memoria, all'inizio della successiva. Implementate misure correttive in questo senso con la variabile h.*/
    return;
}


void MatQuad::stampa(){
    for(int i = 0; i < s; i++){
        for(int j = 0; j < s; j++){
            cout << m[i][j] << " ";
        }

        cout << endl;
    }
    return;
}

bool MatQuad::trova(){
    if(s<3) return false;//controllo preliminare sulla coerenza dell'operazione in essere
    //int c = 0; inutile
    for(int i = 0; i < s; i++){
        for(int j = 0; j < s-2; j++){//pongo il massimo a s-3: non ha senso controllare gli ultimi due elementi, e peraltro vado in buffer overflow
            if(m[i][j] == m[i][j+1]&&m[i][j]==m[i][j+2]) return true;
            //c++; inutile
        }
        /*if(c >= 3)
         return true;
         oltre al fatto che c dovrebbe essere due, perché indica un'uguaglianza,
         il codice non pone alcuna condizione sulla consecutività degli elementi*/

    }

    return false;
}

void MatQuad::raddoppia(){
    int** vecchiam = m;
    int k = s;
    s = s*2;
    m = new int*[s];/*Pericoloso: in questo modo stiamo perdendo l'unico
                    punto di accesso alla matrice originaria, che peraltro
                    neanche eliminiamo. Salvo l'indirizzo della vecchia matrice*/

    for(int i = 0; i < s; i++){
        m[i] = new int[s];
        for(int j = 0; j < s; j++){
            if(i < k && j < k)
                m[i][j] = 8;
            else if(i >= k && j >= k)//correzione dei k+1 in k
                m[i][j] = 9;
            else if(i<k&& j>=k)
                m[i][j] = vecchiam[i][j-k];
            else
                m[i][j] = vecchiam[i-k][j];//il programma non ricopiava la vecchia matrice negli altri quadranti
        }
    }
    for(int i = 0; i<k; i++){
        delete[] vecchiam[i];//eliminazione delle varie righe in memoria dinamica
    }
    delete[] vecchiam; //eliminazione della vecchia matrice
    return;
}

MatQuad::~MatQuad(){
    for(int i = 0; i<s; ++i){
        delete[] m[i];//dealloco tutti i singoli vettori riga dalla memoria dinamica
    }
    delete[] m; //dealloco la matrice
}

int main(){
    cout<<"--- PRIMA PARTE ---" << endl;
    cout << "\nTest del costruttore e dell'operatore di uscita" << endl;
    MatQuad m(4);
    m.stampa();
    cout << endl;

    cout << "Test della 'aggiorna'" << endl;
    const int dim = 7;
    int vett[dim] = {5, 5, 5, 6, 4, 4, 3};
    m.aggiorna(vett, dim);
    m.stampa();
    cout << endl;

    cout << "Test della 'trova':" << endl;
    if ( m.trova() )
        cout << "trovata una riga con almeno tre valori consecutivi uguali fra loro"<<endl;
    else
        cout << "nessuna riga con almeno tre valori consecutivi uguali fra loro"<<endl;


    cout << "\n--- SECONDA PARTE ---" << endl;

    cout << "\nTest della 'raddoppia'"<<endl;
    m.raddoppia();
    m.stampa();
    cout << endl;

    {
        MatQuad m2(3);
    }
    cout << "Test del distruttore (m2 e' stata appena distrutta)" << endl;

    return 0;
}
