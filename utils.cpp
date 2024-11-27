#include "utils.h"
#include <fstream>

void leerBaseHechos(const string& nombreArchivo, list<Hecho>& baseHechos, string& objetivo) {
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cerr << "No se pudo abrir el archivo: " << nombreArchivo << endl;
        return;
    }

    string linea;
    int numHechos = 0;
    getline(archivo, linea);
    numHechos = stoi(linea);

    for (int i = 0; i < numHechos; i++) {
        getline(archivo, linea);
        Hecho hecho;
        size_t pos = linea.find(", FC=");
        if (pos != string::npos) {
            hecho.id = linea.substr(0, pos);
            hecho.factorCerteza = stod(linea.substr(pos + 5));
            baseHechos.push_back(hecho);
        }
    }

    if (getline(archivo, linea) && linea == "Objetivo") {
        getline(archivo, linea);
        objetivo = linea;
    }
    archivo.close();
}

void leerBaseConocimiento(const string& nombreArchivo, list<Regla>& baseConocimiento) {
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cerr << "No se pudo abrir el archivo: " << nombreArchivo << endl;
        return;
    }

    string linea;
    int numReglas = 0;
    if (getline(archivo, linea)) {
        numReglas = stoi(linea);
    }

    for (int i = 0; i < numReglas; i++) {
        if (getline(archivo, linea)) {
            Regla regla;
            size_t pos = linea.find(": ");
            if (pos != string::npos) {
                regla.id = linea.substr(0, pos);
                string resto = linea.substr(pos + 2);

                pos = resto.find(", FC=");
                if (pos != string::npos) {
                    regla.factorCerteza = stod(resto.substr(pos + 5));
                    resto = resto.substr(0, pos);
                }              
      
                 pos = resto.find(" Entonces ");
                 if (pos != string::npos) {
                    regla.consecuente = resto.substr(pos + 10);
                     string antecedentes = resto.substr(3, pos-3);

                    if (antecedentes.find(" y ") != string::npos) {
                         regla.tipo = TipoRegla::REGLA_CONJUNCION;
                        size_t start = 0;
                        size_t end = antecedentes.find(" y ");
                        while (end != string::npos) {
                            regla.antecendentes.push_back(antecedentes.substr(start, end - start));
                            start = end + 3;
                            end = antecedentes.find(" y ", start);
                        }
                        regla.antecendentes.push_back(antecedentes.substr(start));
                    } else if (antecedentes.find(" o ") != string::npos) {
                        regla.tipo = TipoRegla::REGLA_DISYUNCION;
                        size_t start = 0;
                        size_t end = antecedentes.find(" o ");
                        while (end != string::npos) {
                            regla.antecendentes.push_back(antecedentes.substr(start, end - start));
                            start = end + 3;
                            end = antecedentes.find(" o ", start);
                        }
                        regla.antecendentes.push_back(antecedentes.substr(start));
                    } else {
                        regla.tipo = TipoRegla::REGLA_UN_ANTECEDENTE;
                        regla.antecendentes.push_back(antecedentes);
                    }
                }
                
            }

            baseConocimiento.push_back(regla);
        }
    }

    archivo.close();
}

void printBaseHechos(const list<Hecho>& baseHechos, const string& objetivo) {
    cout << "Base de hechos:" << endl;
    for (const Hecho& hecho : baseHechos) {
        cout << "  " << hecho.id << ", FC=" << hecho.factorCerteza << endl;
    }
    cout << "Objetivo: " << objetivo << endl;
}

void printBaseConocimiento(const list<Regla>& baseConocimiento) {
    cout << "Base de conocimiento:" << endl;
    for (const Regla& regla : baseConocimiento) {
        cout << "  " << regla.id << ": ";
        cout << "FC=" << regla.factorCerteza << ", ";
        cout << "Tipo: ";
        cout << (regla.tipo == TipoRegla::REGLA_CONJUNCION ? "Conjuncion, " : (regla.tipo == TipoRegla::REGLA_DISYUNCION ? "Disyuncion, " : "Un antecedente, "));
        cout << "Antecedentes:";
        for (const string& antecedente : regla.antecendentes) {
            cout << " " << antecedente;
        }
        cout << ", Consecuente: " << regla.consecuente << endl;
    }
}

string generarNombreFicheroResultado(const string& archivoBC, const string& archivoBH) {
    size_t posBC = archivoBC.find_last_of("/\\");
    size_t posBH = archivoBH.find_last_of("/\\");
    string nombreBC = (posBC == string::npos) ? archivoBC : archivoBC.substr(posBC + 1);
    string nombreBH = (posBH == string::npos) ? archivoBH : archivoBH.substr(posBH + 1);
    nombreBC = nombreBC.substr(0, nombreBC.find_last_of('.'));
    nombreBH = nombreBH.substr(0, nombreBH.find_last_of('.'));
    return "salidaLog_" + nombreBC + nombreBH + ".txt";
}