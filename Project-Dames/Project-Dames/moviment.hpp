
#ifndef MOVIMENT_HPP
#define MOVIMENT_HPP

#include "posicio.hpp"
#include <vector>

class Moviment {
private:
    std::vector<Posicio> seq_;
public:
    Moviment() = default;
    Moviment(const Posicio& start) { seq_.push_back(start); }
    void afegeix(const Posicio& p) { seq_.push_back(p); }
    int  longitud() const { return int(seq_.size()); }
    const std::vector<Posicio>& getSeq() const { return seq_; }
};

#endif // MOVIMENT_HPP