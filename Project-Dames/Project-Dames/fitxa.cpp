#include "fitxa.hpp"

Fitxa::Fitxa() {
    setEmpty();
}

void Fitxa::setEmpty() {
    tipus_ = TIPUS_EMPTY;
    color_ = COLOR_BLANC;
    moviments_.clear();
}

void Fitxa::set(TipusFitxa tipus, ColorFitxa color) {
    tipus_ = tipus;
    color_ = color;
    moviments_.clear();
}

bool Fitxa::isEmpty() const {
    return tipus_ == TIPUS_EMPTY;
}

TipusFitxa Fitxa::getTipus() const {
    return tipus_;
}

ColorFitxa Fitxa::getColor() const {
    return color_;
}

void Fitxa::afegeixMoviment(const Moviment& m) {
    moviments_.push_back(m);
}

void Fitxa::netejaMoviments() {
    moviments_.clear();
}

const std::vector<Moviment>& Fitxa::getMoviments() const {
    return moviments_;
}

void Fitxa::converteixDama() {
    if (tipus_ == TIPUS_NORMAL)
        tipus_ = TIPUS_DAMA;
}

char Fitxa::toChar() const {
    if (tipus_ == TIPUS_EMPTY)  return '_';
    if (tipus_ == TIPUS_NORMAL) return (color_ == COLOR_BLANC ? 'O' : 'X');
    // dama
    return (color_ == COLOR_BLANC ? 'D' : 'R');
}

Fitxa Fitxa::fromChar(char c) {
    Fitxa f;
    switch (c) {
    case 'O': f.set(TIPUS_NORMAL, COLOR_BLANC); break;
    case 'X': f.set(TIPUS_NORMAL, COLOR_NEGRE); break;
    case 'D': f.set(TIPUS_DAMA, COLOR_BLANC); break;
    case 'R': f.set(TIPUS_DAMA, COLOR_NEGRE); break;
    default:  f.setEmpty();                   break;
    }
    return f;
}