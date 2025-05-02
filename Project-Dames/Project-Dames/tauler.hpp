#ifndef TAULER_HPP
#define TAULER_HPP

#include "posicio.hpp"
#include "fitxa.hpp"
#include <string>

static const int N_FILES = 8;
static const int N_COLUMNES = 8;

class Tauler {
private:
    Fitxa m_tauler[N_FILES][N_COLUMNES];
public:
    void inicialitza(const std::string& nomFitxer);
    void actualitzaMovimentsValids();
    void getPosicionsPossibles(const Posicio& origen, int& n, Posicio posicions[]);
    bool mouFitxa(const Posicio& origen, const Posicio& desti);
    std::string toString() const;
};

#endif // TAULER_HPP
