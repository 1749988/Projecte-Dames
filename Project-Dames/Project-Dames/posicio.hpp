#ifndef POSICIO_HPP
#define POSICIO_HPP

#include <string>
#include <ostream>

class Posicio {
private:
    int fila_;
    int columna_;
public:
    Posicio();
    Posicio(const std::string& pos);
    bool operator==(const Posicio& other) const;
    int  getFila()    const;
    int  getColumna() const;
    std::string toString() const;
};

std::ostream& operator<<(std::ostream& os, const Posicio& p);

#endif // POSICIO_HPP
