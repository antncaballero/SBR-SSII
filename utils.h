#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <list>
#include <string>

using namespace std;

enum class TipoRegla {
    REGLA_CONJUNCION,    // Operador y
    REGLA_DISYUNCION,    // Operador o
    REGLA_UN_ANTECEDENTE // Solo un antecedente
};

struct Hecho {
    double factorCerteza;
    string id;
};

struct Regla {
    TipoRegla tipo;
    string consecuente;
    list<string> antecendentes;
    double factorCerteza;
    double factorCertezaAntecedentes;
    double factorCertezaConsecuente;
    string id;
};

void leerBaseHechos(const string& nombreArchivo, list<Hecho>& baseHechos, string& objetivo);
void leerBaseConocimiento(const string& nombreArchivo, list<Regla>& baseConocimiento);
void printBaseHechos(const list<Hecho>& baseHechos, const string& objetivo);
void printBaseConocimiento(const list<Regla>& baseConocimiento);
string generarNombreFicheroResultado(const string& archivoBC, const string& archivoBH);

#endif