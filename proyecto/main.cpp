#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <ctime>

using namespace std;

struct Producto {
    string nombre;
    double precio;
    int cantidad;
    string codigo_barra;
    string categoria;
    string descripcion;
};

vector<Producto*> leer_productos() {
    vector<Producto*> productos;
    ifstream archivo("productos.txt");

    if (!archivo.is_open()) {
        cerr << "Error: No se pudo abrir el archivo productos.txt" << endl;
        exit(1);
    }

    string linea;
    while (getline(archivo, linea)) {
        stringstream ss(linea);
        Producto* producto = new Producto();
        getline(ss, producto->nombre, ',');
        ss >> producto->precio;
        ss.ignore();
        ss >> producto->cantidad;
        ss.ignore();
        getline(ss, producto->codigo_barra, ',');
        getline(ss, producto->categoria, ',');
        getline(ss, producto->descripcion);
        productos.push_back(producto);
    }

    return productos;
}

void liberar_productos(vector<Producto*>& productos) {
    for (auto& producto : productos) {
        delete producto;
    }
}

void escribir_productos(const vector<Producto*>& productos) {
    ofstream archivo("productos.txt");
    for (const auto& producto : productos) {
        archivo << producto->nombre << "," << producto->precio << "," << producto->cantidad << ","
                << producto->codigo_barra << "," << producto->categoria << "," << producto->descripcion << "\n";
    }
}

vector<Producto*> buscar_producto(const string& criterio, const string& valor, const vector<Producto*>& productos) {
    vector<Producto*> resultados;
    for (const auto& producto : productos) {
        if ((criterio == "nombre" && producto->nombre == valor) ||
            (criterio == "codigo_barra" && producto->codigo_barra == valor) ||
            (criterio == "categoria" && producto->categoria == valor) ||
            (criterio == "descripcion" && producto->descripcion == valor)) {
            resultados.push_back(producto);
        }
    }
    return resultados;
}

void ver_estado_productos(const vector<Producto*>& productos) {
    for (const auto& producto : productos) {
        cout << "Nombre: " << producto->nombre << ", Precio: " << producto->precio
             << ", Cantidad: " << producto->cantidad << ", Código de barra: " << producto->codigo_barra
             << ", Categoría: " << producto->categoria << ", Descripción: " << producto->descripcion << endl;
    }
}

void generar_reporte_venta(const string& nombre_producto, int cantidad_vendida, double precio_unitario) {
    ofstream archivo("reporte_ventas.txt", ios_base::app);
    if (!archivo.is_open()) {
        archivo.open("reporte_ventas.txt", ios::out);
    }
    double total = cantidad_vendida * precio_unitario;
    time_t t = time(nullptr);
    char fecha[100];
    strftime(fecha, sizeof(fecha), "%Y-%m-%d %H:%M:%S", localtime(&t));
    archivo << fecha << " - Producto: " << nombre_producto << ", Cantidad: " << cantidad_vendida
            << ", Total: " << total << endl;
}

void ver_reporte_ventas() {
    ifstream archivo("reporte_ventas.txt");
    if (!archivo.is_open()) {
        cerr << "Error: No se pudo abrir el archivo reporte_ventas.txt" << endl;
        return;
    }

    string linea;
    while (getline(archivo, linea)) {
        cout << linea << endl;
    }
}

void realizar_venta(const string& nombre_producto, int cantidad_vendida, vector<Producto*>& productos) {
    bool venta_realizada = false;
    for (auto& producto : productos) {
        if (producto->nombre == nombre_producto && producto->cantidad >= cantidad_vendida) {
            producto->cantidad -= cantidad_vendida;
            venta_realizada = true;
            generar_reporte_venta(nombre_producto, cantidad_vendida, producto->precio);
            break;
        }
    }
    if (venta_realizada) {
        escribir_productos(productos);
        cout << "Venta realizada con éxito." << endl;
    } else {
        cout << "Venta fallida: Producto no encontrado o cantidad insuficiente." << endl;
    }
}

int main() {
    vector<Producto*> productos = leer_productos();
    int opcion;
    string criterio, valor, nombre_producto;
    int cantidad_vendida;

    do {
        cout << "1. Buscar producto\n2. Ver estado de productos\n3. Realizar venta\n4. Ver reporte de ventas\n5. Salir\n";
        cout << "Seleccione una opción: ";
        cin >> opcion;
        cin.ignore();

        switch (opcion) {
            case 1:
                cout << "Ingrese el criterio de búsqueda (nombre/codigo_barra/categoria/descripcion): ";
                getline(cin, criterio);
                cout << "Ingrese el valor a buscar: ";
                getline(cin, valor);
                {
                    vector<Producto*> resultados = buscar_producto(criterio, valor, productos);
                    if (resultados.empty()) {
                        cout << "No se encontraron productos." << endl;
                    } else {
                        for (const auto& producto : resultados) {
                            cout << "Nombre: " << producto->nombre << ", Precio: " << producto->precio
                                 << ", Cantidad: " << producto->cantidad << ", Código de barra: " << producto->codigo_barra
                                 << ", Categoría: " << producto->categoria << ", Descripción: " << producto->descripcion << endl;
                        }
                    }
                }
                break;
            case 2:
                ver_estado_productos(productos);
                break;
            case 3:
                cout << "Ingrese el nombre del producto: ";
                getline(cin, nombre_producto);
                cout << "Ingrese la cantidad vendida: ";
                cin >> cantidad_vendida;
                cin.ignore();
                realizar_venta(nombre_producto, cantidad_vendida, productos);
                break;
            case 4:
                ver_reporte_ventas();
                break;
            case 5:
                cout << "Saliendo..." << endl;
                break;
            default:
                cout << "Opción no válida. Intente de nuevo." << endl;
        }
    } while (opcion != 5);

    liberar_productos(productos);
    return 0;
}
