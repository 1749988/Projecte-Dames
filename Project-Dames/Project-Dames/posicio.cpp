#include "posicio.hpp"
#include <stdexcept>


Posicio::Posicio()
    : fila_(-1), columna_(-1)
{}


Posicio::Posicio(const std::string& pos) {
    if (pos.size() != 2
        || pos[0] < 'a' || pos[0] > 'h'
        || pos[1] < '1' || pos[1] > '8')
        throw std::invalid_argument("Posicio invalid\n");
    columna_ = pos[0] - 'a';
    fila_ = 8 - (pos[1] - '0');
}
Posicio::Posicio(int fila, int columna) : fila_(fila), columna_(columna) {}

bool Posicio::operator==(const Posicio& other) const {
    return fila_ == other.fila_ && columna_ == other.columna_;
}

int Posicio::getFila() const { return fila_; }
int Posicio::getColumna() const { return columna_; }

std::string Posicio::toString() const {
    char c = char('a' + columna_);
    char f = char('0' + (8 - fila_));
    return std::string() + c + f;
}

std::ostream& operator<<(std::ostream& os, const Posicio& p) {
    os << p.toString();
    return os;
}
