#ifndef FITXA_HPP
#define FITXA_HPP

#include "moviment.hpp"
#include <vector>

enum TipusFitxa { TIPUS_NORMAL, TIPUS_DAMA, TIPUS_EMPTY };
enum ColorFitxa { COLOR_NEGRE, COLOR_BLANC };

class Fitxa {
private:
    TipusFitxa tipus_;
    ColorFitxa color_;
    std::vector<Moviment> moviments_;
public:
    Fitxa();
    void setEmpty();
    void set(TipusFitxa tipus, ColorFitxa color);
    bool isEmpty() const;
    TipusFitxa getTipus() const;
    ColorFitxa getColor() const;
    void afegeixMoviment(const Moviment& m);
    void netejaMoviments();
    const std::vector<Moviment>& getMoviments() const;
    void converteixDama();
    char toChar() const;
    static Fitxa fromChar(char c);
};

#endif // FITXA_HPP
