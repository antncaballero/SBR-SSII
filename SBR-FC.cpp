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
ofstream ficheroResultado; //Fichero de salida del algoritmo donde se mostrara el proceso de inferencia asi como el resultado final

list<Regla> equiparar(string objetivo, list<Regla> reglas){                                                // Se crea una lista de reglas
    list<Regla> cc;
    for (Regla regla : reglas){
        if (regla.consecuente == objetivo) 
            cc.push_back(regla); // Se añade la regla a la lista   
    }
    
    ficheroResultado << "Conjunto conflicto para la meta " << objetivo  << ": { ";
    for (Regla r : cc) ficheroResultado << r.id << " ";
    ficheroResultado << "}" << endl;
    
    return cc;                       // Se retorna la lista de reglas
}

bool verificarHecho(string hecho, list<Hecho> baseHechos){
    for (Hecho h : baseHechos){
        if (h.id == hecho) {
            ficheroResultado << "La meta " << h.id << " ha sido verificada con factor de certeza: " << h.factorCerteza << endl;
            return true; // Si el hecho se encuentra en la base de hechos, se retorna verdadero
        }
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
    while (iteradorAntecedentes != regla.antecendentes.end()){   
        if (regla.tipo == TipoRegla::REGLA_CONJUNCION){
            factorCerteza = min(factorCerteza, buscarFChecho(*iteradorAntecedentes, baseHechos));
        }
        else if (regla.tipo == TipoRegla::REGLA_DISYUNCION){
            factorCerteza = max(factorCerteza, buscarFChecho(*iteradorAntecedentes, baseHechos));
        }
        iteradorAntecedentes++;
    }
    ficheroResultado << "\tCaso 1: Factor de certeza de los antecedentes combinados para la regla " << regla.id << " = " << factorCerteza << endl;
    return factorCerteza;
}

double resolverReglaCaso3(Regla regla){
   double fc= regla.factorCerteza * max(0.0, regla.factorCertezaAntecedentes);
   ficheroResultado <<"\tCaso 3: Factor de certeza para el consecuente de la regla " << regla.id <<", " << regla.consecuente << " = " << fc << endl;
   return fc;
}

double resolverReglaCaso2(string meta, list<Regla> reglas){
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
    ficheroResultado << "Caso 2: Factor de certeza para la meta " << meta << " = " << factorCertezaAnterior << endl;
    return factorCertezaAnterior; 
}

void addHecho(string idHecho, float factorCerteza, list<Hecho> &baseHechos){
    
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

double inferirFc(string meta, list<Hecho> &baseHechos, list<Regla> baseConocimiento){ 
    
    ficheroResultado << "Verificando la meta " << meta << endl;

    if (verificarHecho(meta, baseHechos)) return buscarFChecho(meta, baseHechos); // Si la meta ya está en la base de hechos, no hace falta buscar más, se devuelve su fc
    list<Regla> cc = equiparar(meta, baseConocimiento); // Se obtienen las reglas que tienen a la meta como consecuente
     
    for (auto it = cc.begin(); it != cc.end(); it++){     
        Regla &regla = *it;
        
        ficheroResultado << "Seleccionamos la regla " << regla.id << " con atecedentes: { ";;
        for (string antecedente : regla.antecendentes) ficheroResultado << antecedente << " ";
        ficheroResultado << "}" << endl;

        auto itAntecedentes = regla.antecendentes.begin();
        while (itAntecedentes != regla.antecendentes.end()){      
            string nuevaMeta = (*itAntecedentes);
            inferirFc(nuevaMeta, baseHechos, baseConocimiento);
            itAntecedentes++;
        } 
        ficheroResultado << "Regla " << regla.id << " activada" << endl;
        if (regla.antecendentes.size() > 1) regla.factorCertezaAntecedentes = resolverReglaCaso1(regla);
        else regla.factorCertezaAntecedentes = buscarFChecho(regla.antecendentes.front(), baseHechos);
        
        regla.factorCertezaConsecuente = resolverReglaCaso3(regla);   
    }
    double factorCertezaMeta;
    if (cc.size() > 1){
        ficheroResultado << "Se resuelve el conjunto conflicto para la meta "<< meta << " con el caso 2" << endl;
        factorCertezaMeta = resolverReglaCaso2(meta, cc);
        addHecho(meta, factorCertezaMeta, baseHechos);      
    }
    else {
        factorCertezaMeta = cc.begin()->factorCertezaConsecuente; // es 0 si no hay reglas en el CC, se podría añadir una comparación para ver si CC.isEmpty(), pero a efectos prácticos da el mismo resultado
        addHecho(meta, factorCertezaMeta, baseHechos);
    }
    ficheroResultado << "La meta " << meta << " ha sido verificada con factor de certeza: " << factorCertezaMeta << endl;
    return factorCertezaMeta;
}

int main(int argc, char const *argv[])
{
    if (argc != 3) throw runtime_error("Error: Número de argumentos incorrecto. Se esperaban 3: SBR-FC.exe BaseConocimiento.txt BaseHechos.txt");
    
    // Leemos los argumentos
    string archivoBC = argv[1];
    string archivoBH = argv[2];  
    leerBaseHechos(archivoBH, baseHechos, objetivo);
    leerBaseConocimiento(archivoBC, baseConocimiento);

    string nombreArchivoSalida = generarNombreFicheroResultado(archivoBC, archivoBH);
    ficheroResultado.open(nombreArchivoSalida);// Se abre el archivo de salida
    if (ficheroResultado.fail()){
        throw runtime_error("No se pudo abrir el archivo de salida");
    }

    ficheroResultado << "Base de hechos: " << archivoBH << endl;
    ficheroResultado << "Base de conocimiento: " << archivoBC << endl;
    ficheroResultado << "Objetivo: " << objetivo << endl;
    ficheroResultado << "\nProceso de inferencia:\n" << endl;

    inferirFc(objetivo, baseHechos, baseConocimiento);
    cout << "La meta " << objetivo << " ha sido verificada con factor de certeza: " << buscarFChecho(objetivo, baseHechos) << endl;

    ficheroResultado.close();
    return 0;
}
