#include<iostream>
#include<cassert>

template <class T> class Iterator;
template <class T> class Link;

using namespace std;

// ======================================================================
//                    Klasa Punkt
// ======================================================================
class Punkt{
    string nazwa_;
    double x_ , y_;
    public:
    Punkt(){
    }
    Punkt(string nazwa, double x, double y):nazwa_(nazwa), x_(x), y_(y){
    }
    ~Punkt(){
    }
    double getSuma(){
        return x_ + y_;
    };
    string getNazwa()const{
        return nazwa_;
    }
    double getX()const{
        return x_;
    }
    double getY()const{
        return y_;
    }
};
// ======================================================================
//                    operator <<
// ======================================================================
ostream& operator<<(ostream& s, const Punkt & wyswietl){
    return s<<wyswietl.getNazwa()<<" "<<wyswietl.getX()<<" "<<wyswietl.getY()<<" ";
}
// ======================================================================
//                    Klasa Lista
// ======================================================================
template <class T>
class Lista{
    protected:
        Link <T> *first;
    public:
        Lista() : first(nullptr){}
        virtual void dodaj(T val);
        friend class Iterator <T>;
};
// ======================================================================
//                    Klasa Link
// ======================================================================
template <class T>
class Link{
    private:
        T value;
        Link <T> *next;
        Link(T val, Link *ptr): value(val), next(ptr){}
    public:
        Link <T> *insert(T val);
        friend class Lista <T>;
        friend class Iterator <T>;
};
// ======================================================================
//                    Klasa Iterator
// ======================================================================
template <class T>
class Iterator{
    public:
        Iterator(Lista <T> &aList);
        virtual bool init();
        virtual T operator()();
        virtual bool operator++(int x);
        void usunObecny();
        void dodajPotem(T newValue);
    protected:
        Lista <T> &myLista;
        Link <T> *previous;
        Link <T> *current;
};
// ======================================================================
//                    Lista -> dodaj
// ======================================================================
template <class T> void Lista<T>::dodaj(T val){
    first = new Link<T>(val, first);
}
// ======================================================================
//                    Link -> wstaw(insert)
// ======================================================================
template <class T> Link <T> * Link<T>::insert(T val){
    next =  new Link<T>(val, next);
    return next;
}
// ======================================================================
template <class T> Iterator<T>::Iterator(Lista <T> &iLista): myLista(iLista){
    init();
}
// ======================================================================
//                   Iterator -> init
// ======================================================================
template <class T> bool Iterator<T>::init(){
    previous = nullptr;
    current = myLista.first;
    return current != nullptr;
}
// ======================================================================
//                    Iterator -> dodajPotem
// ======================================================================
template <class T> void Iterator<T>::dodajPotem(T newValue){
    if(current){
        current->insert(newValue);
        return;
    }
    if(previous) current = previous->insert(newValue);
    else myLista.Lista<T>::dodaj(newValue);
}
// ======================================================================
//                    Iterator -> operator ++ -> (postfix)
// ======================================================================
template <class T> bool Iterator<T>::operator ++(int x){
    if(current != nullptr){
        previous = current;
        current = current->next;
    }
    return current != nullptr;
}
// ======================================================================
//                    Iterator -> operator()
// ======================================================================
template <class T> T Iterator<T>::operator()(){
    assert(current != nullptr);
    return current->value;
}
template <class T> void Iterator<T>::usunObecny(){
    assert(current != nullptr);
    if(previous == nullptr) myLista.first = current->next;
    else previous->next = current->next;
    delete current;
    current = nullptr;
}
// ======================================================================
//                Sprawdzanie    
// ======================================================================
bool spr(string n, string n_n){
    for(int i = 0; i < n.size(); i++){
        if(n.size() <=  n_n.size()){
            if(tolower(n[i]) < tolower(n_n[i])){
                return true;
            }
        }
    }
    if(n.size() <  n_n.size()) return true;
    return false;
}
// ======================================================================
//                Sortowanie(quick sort)   
// ======================================================================
void sortowanie(Punkt *punkty, int pierwszy, int ostatni){
    if(pierwszy >= ostatni) return;
    int lewy = pierwszy, prawy = ostatni + 1;
    Punkt pivot = punkty[lewy];
    while(true){
        do lewy++;
        while(lewy <= ostatni and (punkty[lewy].getSuma() < pivot.getSuma() or (punkty[lewy].getSuma() == pivot.getSuma() and punkty[lewy].getX() < pivot.getX()) or (punkty[lewy].getSuma() == pivot.getSuma() and punkty[lewy].getX() == pivot.getX() and spr(punkty[lewy].getNazwa(), pivot.getNazwa()))));
        do prawy--;
        while(punkty[prawy].getSuma() > pivot.getSuma() or (punkty[prawy].getSuma() == pivot.getSuma() and punkty[prawy].getX() > pivot.getX()) or (punkty[prawy].getSuma() == pivot.getSuma() and punkty[prawy].getX() == pivot.getX()) and spr(pivot.getNazwa(), punkty[prawy].getNazwa()));
        if(lewy > prawy) break;
        swap(punkty[lewy], punkty[prawy]);
    }
    swap(punkty[pierwszy], punkty[prawy]);
    sortowanie(punkty, pierwszy, prawy - 1);
    sortowanie(punkty, prawy + 1, ostatni);
}
// ======================================================================
//                Main   
// ======================================================================
int main() {
    Lista <Punkt> punktowLista;
    int n; cin>>n;
    Punkt *punkty = new Punkt[n];
    string nazwa; 
    double x, y;
    for(int i = 0; i < n; i++){
        cin>>nazwa>>x>>y;
        punkty[i] = Punkt(nazwa, x , y);
    }
    sortowanie(punkty, 0, n-1);
    Iterator <Punkt> myIterator(punktowLista);
    myIterator.dodajPotem(punkty[0]);
    myIterator.init();
    for(int i = 1; i < n; i++){
        myIterator.dodajPotem(punkty[i]);
        myIterator++;
    }
    myIterator.init();
    double suma; cin>>suma;
    do{
        if(myIterator().getSuma() == suma){
            myIterator.usunObecny();
        }
    }while(myIterator++);
    
    myIterator.init();
    do{
	cout<<myIterator();
    }while(myIterator++);    
    
    return 1;
}