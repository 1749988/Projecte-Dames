#include "cuamoviments.hpp"
#include <fstream>
#include <stdexcept>

// Constructor: comença amb cua buida
CuaMoviments::CuaMoviments()
    : cap(nullptr), cua(nullptr) {}

// Destructor: allibera tots els nodes pendents
CuaMoviments::~CuaMoviments() {
    vaciar();
}

void CuaMoviments::encola(const Moviment& m) {
    Nodo* nou = new Nodo(m);
    if (cua) {
        cua->seguent = nou;
    }
    else {
        cap = nou;
    }
    cua = nou;
}

Moviment CuaMoviments::desencola() {
    if (!cap) {
        throw std::runtime_error("Cua buida: no hi ha moviments per reproducir");
    }
    Nodo* actual = cap;
    Moviment m = actual->moviment;
    cap = cap->seguent;
    if (!cap) {
        cua = nullptr;
    }
    delete actual;
    return m;
}

bool CuaMoviments::estaVacia() const {
    return cap == nullptr;
}

void CuaMoviments::vaciar() {
    while (cap) {
        Nodo* següent = cap->seguent;
        delete cap;
        cap = següent;
    }
    cua = nullptr;
}

void CuaMoviments::guardaEnFichero(const std::string& nomFitxer) {
    std::ofstream out(nomFitxer);
    if (!out) {
        throw std::runtime_error("No es pot obrir fitxer per escriure: " + nomFitxer);
    }
    for (Nodo* it = cap; it; it = it->seguent) {
        const auto& seq = it->moviment.getSeq();
        // Escriu posició inicial i final del moviment
        out << seq.front().toString()
            << ' '
            << seq.back().toString()
            << '\n';
    }
}

void CuaMoviments::cargaDeFichero(const std::string& nomFitxer) {
    std::ifstream in(nomFitxer);
    if (!in) {
        throw std::runtime_error("No es pot obrir fitxer per llegir: " + nomFitxer);
    }
    std::string origen, desti;
    while (in >> origen >> desti) {
        Moviment m(Posicio(origen));
        m.afegeix(Posicio(desti));
        encola(m);
    }
}
