#ifndef TAULER_HPP
#define TAULER_HPP

#include "posicio.hpp"
#include "fitxa.hpp"
#include <string>
#include <vector>

static const int N_FILES = 8;
static const int N_COLUMNES = 8;

class Tauler {
private:
    // Tauler dinàmic: vector de vectors de punters a Fitxa
    std::vector<std::vector<Fitxa*>> m_tauler;

public:
    Tauler() = default;
    ~Tauler();

    // Inicialitza l’estructura 8×8 i carrega les fitxes des d’un fitxer
    void inicialitza(const std::string& nomFitxer);

    // Calcula i desa tots els moviments vàlids de cada fitxa
    void actualitzaMovimentsValids();

    // Omple 'posicions' amb les possibles destins des d'origen
    void getPosicionsPossibles(const Posicio& origen, int& n, Posicio posicions[]);

    // Mou la fitxa (i aplica captures/promocions), retorna true si és vàlid
    bool mouFitxa(const Posicio& origen, const Posicio& desti);

    // Acces directe a una casella
    Fitxa* getFitxa(const Posicio& pos) const;

    // Representació ASCII del tauler
    std::string toString() const;
};

#endif // TAULER_HPP

