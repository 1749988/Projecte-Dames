#include "tauler.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cmath>
#include <vector>
#include <functional>
#include <algorithm>
#include <array>

namespace {

    constexpr std::array<std::pair<int, int>, 4> DIRS{
        {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}}
    };

    bool dins(int f, int c) {
        return f >= 0 && f < N_FILES && c >= 0 && c < N_COLUMNES;
    }

    void copyBoard(const Fitxa src[][N_COLUMNES],
        Fitxa dst[][N_COLUMNES]) {
        for (int r = 0; r < N_FILES; ++r)
            for (int c = 0; c < N_COLUMNES; ++c)
                dst[r][c] = src[r][c];
    }

    void exploreCaptures(Fitxa board[][N_COLUMNES], const Fitxa& fit,
        ColorFitxa color, TipusFitxa tipus, Moviment seq,
        std::vector<Moviment>& out) {
        int f = seq.getSeq().back().getFila();
        int c = seq.getSeq().back().getColumna();

        for (auto [df, dc] : DIRS) {
            if (tipus == TIPUS_NORMAL) {
                int f1 = f + df, c1 = c + dc;
                int f2 = f + 2 * df, c2 = c + 2 * dc;
                if (dins(f2, c2) && !board[f1][c1].isEmpty() &&
                    board[f1][c1].getColor() != color && board[f2][c2].isEmpty()) {
                    Moviment next = seq;
                    next.afegeix(Posicio(std::string() + char('a' + c2) +
                        char('0' + (8 - f2))));
                    out.push_back(next);
                    Fitxa nb[N_FILES][N_COLUMNES];
                    copyBoard(board, nb);
                    nb[f1][c1].setEmpty();
                    nb[f][c].setEmpty();
                    nb[f2][c2] = fit;
                    exploreCaptures(nb, fit, color, tipus, next, out);
                }
            }
            else {
                int r = f + df, cc = c + dc;
                while (dins(r, cc) && board[r][cc].isEmpty()) {
                    r += df;
                    cc += dc;
                }
                if (dins(r, cc) && !board[r][cc].isEmpty() &&
                    board[r][cc].getColor() != color) {
                    int r2 = r + df, c2 = cc + dc;
                    while (dins(r2, c2)) {
                        if (!board[r2][c2].isEmpty())
                            break;
                        Moviment next = seq;
                        next.afegeix(Posicio(std::string() + char('a' + c2) +
                            char('0' + (8 - r2))));
                        out.push_back(next);
                        Fitxa nb[N_FILES][N_COLUMNES];
                        copyBoard(board, nb);
                        nb[r][cc].setEmpty();
                        nb[f][c].setEmpty();
                        nb[r2][c2] = fit;
                        exploreCaptures(nb, fit, color, tipus, next, out);
                        r2 += df;
                        c2 += dc;
                    }
                }
            }
        }
    }

} // namespace

void Tauler::inicialitza(const std::string& nomFitxer) {
    for (int i = 0; i < N_FILES; ++i)
        for (int j = 0; j < N_COLUMNES; ++j)
            m_tauler[i][j].setEmpty();

    std::ifstream in(nomFitxer);
    if (!in)
        throw std::runtime_error("No es pot obrir fitxer \"" + nomFitxer + "\"");

    char c;
    std::string pos;
    while (in >> c >> pos) {
        Posicio p(pos);
        m_tauler[p.getFila()][p.getColumna()] = Fitxa::fromChar(c);
    }
}

std::string Tauler::toString() const {
    std::stringstream ss;
    for (int i = 0; i < N_FILES; ++i) {
        ss << (8 - i) << ": ";
        for (int j = 0; j < N_COLUMNES; ++j) {
            ss << m_tauler[i][j].toChar();
            if (j < N_COLUMNES - 1) ss << ' ';
        }
        ss << '\n';
    }
    ss << "   a b c d e f g h\n";
    return ss.str();
}

void Tauler::actualitzaMovimentsValids() {

    for (int i = 0; i < N_FILES; ++i)
        for (int j = 0; j < N_COLUMNES; ++j)
            m_tauler[i][j].netejaMoviments();

    for (int i = 0; i < N_FILES; ++i) {
        for (int j = 0; j < N_COLUMNES; ++j) {
            Fitxa fit = m_tauler[i][j];
            if (fit.isEmpty())
                continue;

            Posicio origen(std::string() + char('a' + j) + char('0' + (8 - i)));

            Fitxa board[N_FILES][N_COLUMNES];
            copyBoard(m_tauler, board);

            TipusFitxa tipus = fit.getTipus();
            ColorFitxa color = fit.getColor();

            std::vector<Moviment> captureMoves;
            Moviment start(origen);
            exploreCaptures(board, fit, color, tipus, start, captureMoves);

            std::vector<Moviment> simpleMoves;
            for (auto [df, dc] : DIRS) {
                if (tipus == TIPUS_NORMAL) {
                    if ((color == COLOR_BLANC && df != -1) ||
                        (color == COLOR_NEGRE && df != 1))
                        continue;
                    int f1 = i + df, c1 = j + dc;
                    if (dins(f1, c1) && m_tauler[f1][c1].isEmpty()) {
                        Moviment m(origen);
                        m.afegeix(Posicio(std::string() + char('a' + c1) +
                            char('0' + (8 - f1))));
                        simpleMoves.push_back(m);
                    }
                }
                else {
                    int r = i + df, c = j + dc;
                    while (dins(r, c) && m_tauler[r][c].isEmpty()) {
                        Moviment m(origen);
                        m.afegeix(Posicio(std::string() + char('a' + c) +
                            char('0' + (8 - r))));
                        simpleMoves.push_back(m);
                        r += df;
                        c += dc;
                    }
                }
            }

            if (tipus == TIPUS_NORMAL) {
                for (auto& m : captureMoves)
                    m_tauler[i][j].afegeixMoviment(m);
                for (auto& m : simpleMoves)
                    m_tauler[i][j].afegeixMoviment(m);
            }
            else {
                for (auto& m : simpleMoves)
                    m_tauler[i][j].afegeixMoviment(m);
                for (auto& m : captureMoves)
                    m_tauler[i][j].afegeixMoviment(m);
            }
        }
    }
}

void Tauler::getPosicionsPossibles(const Posicio& origen, int& n, Posicio pos[]) {
    n = 0;
    int fo = origen.getFila(), co = origen.getColumna();
    if (m_tauler[fo][co].isEmpty()) return;
    actualitzaMovimentsValids();
    auto movs = m_tauler[fo][co].getMoviments();
    for (const auto& m : movs) {
        if (m.longitud() < 2) continue;
        Posicio dest = m.getSeq().back();
        if (dest == origen) continue;
        bool exists = false;
        for (int k = 0; k < n; ++k) {
            if (pos[k] == dest) { exists = true; break; }
        }
        if (!exists) pos[n++] = dest;
    }
}

bool Tauler::mouFitxa(const Posicio& origen, const Posicio& desti) {
    int fo = origen.getFila(), co = origen.getColumna();
    Fitxa orig = m_tauler[fo][co];
    if (orig.isEmpty()) return false;

    actualitzaMovimentsValids();
    auto movs = m_tauler[fo][co].getMoviments();

    auto countCaptures = [](const Moviment& mv) {
        int cap = 0;
        const auto& seq = mv.getSeq();
        for (size_t k = 0; k + 1 < seq.size(); ++k) {
            int r1 = seq[k].getFila(), c1 = seq[k].getColumna();
            int r2 = seq[k + 1].getFila(), c2 = seq[k + 1].getColumna();
            if (std::abs(r2 - r1) > 1 || std::abs(c2 - c1) > 1) ++cap;
        }
        return cap;
        };

    int globalMaxCaptures = 0;
    std::vector<Posicio> piecesCapturing;
    for (int i = 0; i < N_FILES; ++i) {
        for (int j = 0; j < N_COLUMNES; ++j) {
            if (m_tauler[i][j].isEmpty() || m_tauler[i][j].getColor() != orig.getColor())
                continue;
            const auto& mvs = m_tauler[i][j].getMoviments();
            bool recorded = false;
            for (const auto& mv : mvs) {
                int captures = countCaptures(mv);
                if (captures > 0 && !recorded) {
                    piecesCapturing.push_back(Posicio(std::string() + char('a' + j) +
                        char('0' + (8 - i))));
                    recorded = true;
                }
                if (captures > globalMaxCaptures)
                    globalMaxCaptures = captures;
            }
        }
    }

    bool trobat = false;
    Moviment chosen;
    for (const auto& m : movs) {
        if (m.longitud() >= 2 && m.getSeq().back() == desti) {
            chosen = m;
            trobat = true;
            break;
        }
    }
    if (!trobat) return false;

    int chosenCaptures = countCaptures(chosen);

    const auto& seq = chosen.getSeq();

    if (globalMaxCaptures > 0 && chosenCaptures == 0) {

        m_tauler[fo][co].setEmpty();
        m_tauler[desti.getFila()][desti.getColumna()] = orig;

        for (const auto& p : piecesCapturing) {
            m_tauler[p.getFila()][p.getColumna()].setEmpty();
        }
        actualitzaMovimentsValids();
        return true;
    }

    if (globalMaxCaptures > 0 && chosenCaptures < globalMaxCaptures) {
        for (size_t k = 0; k + 1 < seq.size(); ++k) {
            int r1 = seq[k].getFila(), c1 = seq[k].getColumna();
            int r2 = seq[k + 1].getFila(), c2 = seq[k + 1].getColumna();
            int dr = (r2 > r1) ? 1 : (r2 < r1) ? -1 : 0;
            int dc = (c2 > c1) ? 1 : (c2 < c1) ? -1 : 0;
            int rr = r1 + dr, cc = c1 + dc;
            while (rr != r2 || cc != c2) {
                if (!m_tauler[rr][cc].isEmpty() && m_tauler[rr][cc].getColor() != orig.getColor()) {
                    m_tauler[rr][cc].setEmpty();
                    break;
                }
                rr += dr; cc += dc;
            }
        }

        m_tauler[fo][co].setEmpty();

        actualitzaMovimentsValids();
        return true;
    }

    for (size_t k = 0; k + 1 < seq.size(); ++k) {
        int r1 = seq[k].getFila(), c1 = seq[k].getColumna();
        int r2 = seq[k + 1].getFila(), c2 = seq[k + 1].getColumna();
        int dr = (r2 > r1) ? 1 : (r2 < r1) ? -1 : 0;
        int dc = (c2 > c1) ? 1 : (c2 < c1) ? -1 : 0;
        int rr = r1 + dr, cc = c1 + dc;
        while (rr != r2 || cc != c2) {
            if (!m_tauler[rr][cc].isEmpty() && m_tauler[rr][cc].getColor() != orig.getColor()) {
                m_tauler[rr][cc].setEmpty();
                break;
            }
            rr += dr; cc += dc;
        }
    }
    m_tauler[fo][co].setEmpty();
    Posicio last = seq.back();

    if (orig.getTipus() == TIPUS_NORMAL) {
        int fd = last.getFila();
        if ((orig.getColor() == COLOR_BLANC && fd == 0) ||
            (orig.getColor() == COLOR_NEGRE && fd == N_FILES - 1)) {
            orig.converteixDama();
        }
    }
    m_tauler[last.getFila()][last.getColumna()] = orig;
    actualitzaMovimentsValids();
    return true;
}
