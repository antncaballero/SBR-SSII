#include <iostream>
#include <list>
#include <string>
#include <algorithm>
#include <fstream>
#include "utils.h"
using namespace std;

list<Hecho> baseHechos; //Base de hechos en forma de lista
list<Regla> baseConocimiento; //Base de conocimiento en forma de lista
string objetivo; //Objetivo a buscar

list<Regla> equiparar(string objetivo, list<Regla> reglas){                                                // Se crea una lista de reglas
    list<Regla> conjuntoReglas;
    for (Regla regla : reglas){
        if (regla.consecuente == objetivo) 
            conjuntoReglas.push_back(regla); // Se añade la regla a la lista   
    }
    return conjuntoReglas;                       // Se retorna la lista de reglas
}

bool verificarHecho(string hecho, list<Hecho> baseHechos){
    for (Hecho h : baseHechos){
        if (h.id == hecho) return true; // Si el hecho se encuentra en la base de hechos, se retorna verdadero
    }
    return false; // Si no se encuentra, se retorna falso
}

double buscarFChecho(string hecho, list<Hecho> &baseHechos){
    for (Hecho h : baseHechos){
        if (h.id == hecho) return h.factorCerteza; // Si el hecho se encuentra en la base de hechos, se retorna su factor de certeza
    }
    return 0.0; // Si no se encuentra, se retorna 0
}

double resolverReglaCaso1(Regla regla){

    auto iteradorAntecedentes = regla.antecendentes.begin();
    double factorCerteza = buscarFChecho(*iteradorAntecedentes, baseHechos);
    iteradorAntecedentes++;
    while (iteradorAntecedentes != regla.antecendentes.end()){   //si solo tiene un antecedente, no entrara en este bucle
        if (regla.tipo == TipoRegla::REGLA_CONJUNCION){
            factorCerteza = min(factorCerteza, buscarFChecho(*iteradorAntecedentes, baseHechos));
        }
        else if (regla.tipo == TipoRegla::REGLA_DISYUNCION){
            factorCerteza = max(factorCerteza, buscarFChecho(*iteradorAntecedentes, baseHechos));
        }
        iteradorAntecedentes++;
    }
    return factorCerteza;
}

double resolverReglaCaso3(Regla regla){
   return regla.factorCerteza * max(0.0, regla.factorCertezaAntecedentes);
}

double resolverReglaCaso2(list<Regla> reglas){
    if (reglas.empty()) return 0.0;

    double factorCertezaAnterior = reglas.begin()->factorCertezaConsecuente;

    for (auto it = ++reglas.begin(); it != reglas.end(); ++it) {
        double factorCertezaActual = it->factorCertezaConsecuente;

        if (factorCertezaAnterior >= 0 && factorCertezaActual >= 0) {
            factorCertezaAnterior += factorCertezaActual * (1.0 - factorCertezaAnterior);
        } else if (factorCertezaAnterior <= 0 && factorCertezaActual <= 0) {
            factorCertezaAnterior += factorCertezaActual * (1.0 + factorCertezaAnterior);
        } else {
            factorCertezaAnterior = (factorCertezaAnterior + factorCertezaActual) / (1.0 - min(abs(factorCertezaAnterior), abs(factorCertezaActual)));
        }
    }
    return factorCertezaAnterior; 
}

void añadirHecho(string idHecho, float factorCerteza, list<Hecho> &baseHechos){
    
    for (Hecho &h : baseHechos){
        if (h.id == idHecho){
            h.factorCerteza = factorCerteza;
            return;                              //si ya está, actualiza el factor de certeza y sale de la función
        }
    }
    Hecho hecho;
    hecho.id = idHecho;
    hecho.factorCerteza = factorCerteza;
    baseHechos.push_back(hecho);

}

bool verificar(string meta, list<Hecho> &baseHechos, list<Regla> baseConocimiento){ 
    
    if (verificarHecho(meta, baseHechos)) return true; // Si la meta ya está en la base de hechos, se retorna verdadero
    list<Regla> cc = equiparar(meta, baseConocimiento); // Se obtienen las reglas que tienen a la meta como consecuente
    
    for (auto it = cc.begin(); it != cc.end(); it++){     
        Regla &regla = *it;
        bool verificado = true;
        auto itAntecedentes = regla.antecendentes.begin();
        while (itAntecedentes != regla.antecendentes.end() && verificado){
            
            string nuevaMeta = (*itAntecedentes);
            verificado = verificar(nuevaMeta, baseHechos, baseConocimiento);
            itAntecedentes++;
        }
        if (verificado){
            regla.factorCertezaAntecedentes = resolverReglaCaso1(regla);
            regla.factorCertezaConsecuente = resolverReglaCaso3(regla);
        }
    }
    if (cc.size() > 1){
        double factorCertezaMeta = resolverReglaCaso2(cc);
        añadirHecho(meta, factorCertezaMeta, baseHechos);
        return true;
    }
    else{
        double factorCertezaMeta = cc.begin()->factorCertezaConsecuente;
        añadirHecho(meta, factorCertezaMeta, baseHechos);
        return true;
    }
    return false;
}

int main(int argc, char const *argv[])
{
    if (argc != 3) throw runtime_error("Error: Número de argumentos incorrecto. Se esperaban 3: SBR-FC.exe BaseConocimiento.txt BaseHechos.txt");
    
    // Leemos los argumentos
    string archivoBC = argv[1];
    string archivoBH = argv[2];
    
    leerBaseHechos(archivoBH, baseHechos, objetivo);
    leerBaseConocimiento(archivoBC, baseConocimiento);

    if (verificar(objetivo, baseHechos, baseConocimiento)){
        cout << "La meta " << objetivo << " ha sido verificada ";
        for (Hecho h : baseHechos){
            if (h.id == objetivo){
                cout << "con factor de certeza: " << h.factorCerteza << endl;
            }
        }
    }
    else{
        cout << "La meta " << objetivo << " no ha sido verificada." << endl;
    }

    return 0;
}
