#include <iostream>
#include <iomanip>
#include <new>

using namespace std;

template<typename T>
class MatrizBase {
protected:
    int _filas;
    int _columnas;
public:
    MatrizBase(): _filas(0), _columnas(0) {}
    MatrizBase(int f,int c): _filas(f), _columnas(c) {}
    virtual ~MatrizBase() {}
    virtual void cargarValores() = 0;
    virtual MatrizBase<T>* sumar(const MatrizBase<T>& otra) const = 0;
    virtual void imprimir() const = 0;
    virtual T get(int r,int c) const = 0;
    virtual void set(int r,int c,const T &val) = 0;
    int filas() const { return _filas; }
    int columnas() const { return _columnas; }
};

template<typename T>
class MatrizDinamica : public MatrizBase<T> {
    T **_datos;
public:
    MatrizDinamica(int f=0,int c=0) : MatrizBase<T>(f,c), _datos(nullptr) {
        if (f>0 && c>0) {
            _datos = new T*[f];
            for (int i=0;i<f;i++){
                _datos[i] = new T[c];
                for(int j=0;j<c;j++) _datos[i][j] = T();
            }
        }
    }
    ~MatrizDinamica(){
        if (_datos) {
            for(int i=0;i<this->_filas;i++) delete [] _datos[i];
            delete [] _datos;
            _datos = nullptr;
        }
    }
    MatrizDinamica(const MatrizDinamica<T>& o) : MatrizBase<T>(o._filas,o._columnas), _datos(nullptr) {
        int f = this->_filas, c = this->_columnas;
        if (f>0 && c>0) {
            _datos = new T*[f];
            for (int i=0;i<f;i++){
                _datos[i] = new T[c];
                for(int j=0;j<c;j++) _datos[i][j] = o._datos[i][j];
            }
        }
    }
    MatrizDinamica<T>& operator=(const MatrizDinamica<T>& o){
        if (this == &o) return *this;
        if (_datos) {
            for(int i=0;i<this->_filas;i++) delete [] _datos[i];
            delete [] _datos;
            _datos = nullptr;
        }
        this->_filas = o._filas; this->_columnas = o._columnas;
        int f = this->_filas, c = this->_columnas;
        if (f>0 && c>0) {
            _datos = new T*[f];
            for (int i=0;i<f;i++){
                _datos[i] = new T[c];
                for(int j=0;j<c;j++) _datos[i][j] = o._datos[i][j];
            }
        }
        return *this;
    }
    MatrizDinamica(MatrizDinamica<T>&& o) noexcept : MatrizBase<T>(o._filas,o._columnas), _datos(o._datos) {
        o._datos = nullptr;
        o._filas = 0; o._columnas = 0;
    }
    MatrizDinamica<T>& operator=(MatrizDinamica<T>&& o) noexcept {
        if (this==&o) return *this;
        if (_datos) {
            for(int i=0;i<this->_filas;i++) delete [] _datos[i];
            delete [] _datos;
        }
        this->_filas = o._filas; this->_columnas = o._columnas;
        _datos = o._datos;
        o._datos = nullptr; o._filas = 0; o._columnas = 0;
        return *this;
    }
    T get(int r,int c) const override {
        if (!_datos || r<0 || r>=this->_filas || c<0 || c>=this->_columnas) return T();
        return _datos[r][c];
    }
    void set(int r,int c,const T &v) override {
        if (!_datos || r<0 || r>=this->_filas || c<0 || c>=this->_columnas) return;
        _datos[r][c] = v;
    }
    void cargarValores() override {
        for(int i=0;i<this->_filas;i++){
            for(int j=0;j<this->_columnas;j++){
                T valor;
                cout << "valor ["<<i<<"]["<<j<<"] = ";
                cin >> valor;
                _datos[i][j] = valor;
            }
        }
    }
    MatrizBase<T>* sumar(const MatrizBase<T>& otra) const override {
        if (this->_filas != otra.filas() || this->_columnas != otra.columnas()) {
            cout << "dimensiones incompatibles\n";
            return nullptr;
        }
        MatrizDinamica<T>* r = new MatrizDinamica<T>(this->_filas,this->_columnas);
        for(int i=0;i<this->_filas;i++){
            for(int j=0;j<this->_columnas;j++){
                T a = this->get(i,j);
                T b = otra.get(i,j);
                r->set(i,j,a + b);
            }
        }
        return r;
    }
    void imprimir() const override {
        for(int i=0;i<this->_filas;i++){
            cout << "| ";
            for(int j=0;j<this->_columnas;j++){
                cout << setw(8) << this->get(i,j) << " ";
            }
            cout << "|\n";
        }
    }
};

template<typename T, int M, int N>
class MatrizEstatica : public MatrizBase<T> {
    T _datos[M][N];
public:
    MatrizEstatica(): MatrizBase<T>(M,N) {
        for(int i=0;i<M;i++) for(int j=0;j<N;j++) _datos[i][j] = T();
    }
    T get(int r,int c) const override {
        if (r<0 || r>=M || c<0 || c>=N) return T();
        return _datos[r][c];
    }
    void set(int r,int c,const T &v) override {
        if (r<0 || r>=M || c<0 || c>=N) return;
        _datos[r][c] = v;
    }
    void cargarValores() override {
        for(int i=0;i<M;i++){
            for(int j=0;j<N;j++){
                T val;
                cout << "valor ["<<i<<"]["<<j<<"] = ";
                cin >> val;
                _datos[i][j] = val;
            }
        }
    }
    MatrizBase<T>* sumar(const MatrizBase<T>& otra) const override {
        if (M != otra.filas() || N != otra.columnas()) {
            cout << "dimensiones incompatibles\n";
            return nullptr;
        }
        MatrizEstatica<T,M,N>* r = new MatrizEstatica<T,M,N>();
        for(int i=0;i<M;i++){
            for(int j=0;j<N;j++){
                r->_datos[i][j] = this->_datos[i][j] + otra.get(i,j);
            }
        }
        return r;
    }
    void imprimir() const override {
        for(int i=0;i<M;i++){
            cout << "| ";
            for(int j=0;j<N;j++){
                cout << setw(8) << _datos[i][j] << " ";
            }
            cout << "|\n";
        }
    }
};

template<typename T>
MatrizBase<T>* operator+(const MatrizBase<T>& a, const MatrizBase<T>& b) {
    return a.sumar(b);
}

int main(){
    cout << "--- Sistema generico de Algebra Lineal ---\n";
    cout << "elige tipo: 1) int  2) float\n";
    int tipo=0;
    cin >> tipo;
    if (tipo!=1 && tipo!=2) {
        cout << "tipo no soportado\n";
        return 0;
    }

    if (tipo==1) {
        MatrizBase<int>* arr[20];
        for(int i=0;i<20;i++) arr[i]=nullptr;
        int cnt=0;
        while(true){
            cout << "\n1 Crear Matriz Dinamica\n2 Crear Matriz Estatica (2x2,3x2,3x3,4x4)\n3 Sumar Matrices\n4 Imprimir Matrices\n5 Salir\n";
            int op; cin >> op;
            if (op==1){
                int r,c; cout << "filas: "; cin >> r; cout << "cols: "; cin >> c;
                MatrizBase<int>* m = new MatrizDinamica<int>(r,c);
                m->cargarValores();
                if (cnt<20) arr[cnt++]=m; else { delete m; cout << "capacidad llena\n"; }
            } else if (op==2){
                cout << "elige: 1)2x2 2)3x2 3)3x3 4)4x4\n";
                int s; cin >> s;
                MatrizBase<int>* m = nullptr;
                if (s==1) m = new MatrizEstatica<int,2,2>();
                else if (s==2) m = new MatrizEstatica<int,3,2>();
                else if (s==3) m = new MatrizEstatica<int,3,3>();
                else if (s==4) m = new MatrizEstatica<int,4,4>();
                else { cout << "opcion invalida\n"; continue; }
                m->cargarValores();
                if (cnt<20) arr[cnt++]=m; else { delete m; cout << "capacidad llena\n"; }
            } else if (op==3){
                if (cnt<2) { cout << "no hay matrices suficientes\n"; continue; }
                cout << "elige indice a (0.."<<cnt-1<<"): "; int a; cin >> a;
                cout << "elige indice b (0.."<<cnt-1<<"): "; int b; cin >> b;
                if (a<0||a>=cnt||b<0||b>=cnt) { cout << "indices invalidos\n"; continue; }
                MatrizBase<int>* r = *arr[a] + *arr[b];
                if (r) {
                    if (cnt<20) arr[cnt++]=r; else { delete r; cout << "capacidad llena, resultado descartado\n"; }
                }
            } else if (op==4){
                if (cnt==0) { cout << "sin matrices\n"; continue; }
                for(int i=0;i<cnt;i++){
                    cout << "idx " << i << " (" << arr[i]->filas() << "x" << arr[i]->columnas() << ")\n";
                    arr[i]->imprimir();
                }
            } else if (op==5){
                for(int i=0;i<cnt;i++) delete arr[i];
                cout << "salir\n";
                break;
            } else cout << "opcion invalida\n";
        }
    } else {
        MatrizBase<float>* arr[20];
        for(int i=0;i<20;i++) arr[i]=nullptr;
        int cnt=0;
        while(true){
            cout << "\n1 Crear Matriz Dinamica\n2 Crear Matriz Estatica (2x2,3x2,3x3,4x4)\n3 Sumar Matrices\n4 Imprimir Matrices\n5 Salir\n";
            int op; cin >> op;
            if (op==1){
                int r,c; cout << "filas: "; cin >> r; cout << "cols: "; cin >> c;
                MatrizBase<float>* m = new MatrizDinamica<float>(r,c);
                m->cargarValores();
                if (cnt<20) arr[cnt++]=m; else { delete m; cout << "capacidad llena\n"; }
            } else if (op==2){
                cout << "elige: 1)2x2 2)3x2 3)3x3 4)4x4\n";
                int s; cin >> s;
                MatrizBase<float>* m = nullptr;
                if (s==1) m = new MatrizEstatica<float,2,2>();
                else if (s==2) m = new MatrizEstatica<float,3,2>();
                else if (s==3) m = new MatrizEstatica<float,3,3>();
                else if (s==4) m = new MatrizEstatica<float,4,4>();
                else { cout << "opcion invalida\n"; continue; }
                m->cargarValores();
                if (cnt<20) arr[cnt++]=m; else { delete m; cout << "capacidad llena\n"; }
            } else if (op==3){
                if (cnt<2) { cout << "no hay matrices suficientes\n"; continue; }
                cout << "elige indice a (0.."<<cnt-1<<"): "; int a; cin >> a;
                cout << "elige indice b (0.."<<cnt-1<<"): "; int b; cin >> b;
                if (a<0||a>=cnt||b<0||b>=cnt) { cout << "indices invalidos\n"; continue; }
                MatrizBase<float>* r = *arr[a] + *arr[b];
                if (r) {
                    if (cnt<20) arr[cnt++]=r; else { delete r; cout << "capacidad llena, resultado descartado\n"; }
                }
            } else if (op==4){
                if (cnt==0) { cout << "sin matrices\n"; continue; }
                for(int i=0;i<cnt;i++){
                    cout << "idx " << i << " (" << arr[i]->filas() << "x" << arr[i]->columnas() << ")\n";
                    arr[i]->imprimir();
                }
            } else if (op==5){
                for(int i=0;i<cnt;i++) delete arr[i];
                cout << "salir\n";
                break;
            } else cout << "opcion invalida\n";
        }
    }
    return 0;
}
