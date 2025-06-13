#include "tauler.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cmath>
#include <vector>
#include <functional>
#include <algorithm>
#include <array>
#include "moviment.hpp"

using namespace std;

namespace {

    // Direccions diagonals
    constexpr array<pair<int, int>, 4> DIRS{ {
        {1,1},{1,-1},{-1,1},{-1,-1}
    } };

    // Comprova si (f,c) és a dins del tauler
    bool dins(int f, int c) {
        return f >= 0 && f < N_FILES && c >= 0 && c < N_COLUMNES;
    }

    // Copia un tauler estàtic a un altre (per a l’exploració de captures)
    void copyBoard(const Fitxa src[][N_COLUMNES],
        Fitxa dst[][N_COLUMNES]) {
        for (int r = 0; r < N_FILES; ++r)
            for (int c = 0; c < N_COLUMNES; ++c)
                dst[r][c] = src[r][c];
    }

    // Recursiu: explora totes les seqüències de captures
    void exploreCaptures(Fitxa board[][N_COLUMNES],
        const Fitxa& fit,
        ColorFitxa color,
        TipusFitxa tipus,
        Moviment seq,
        vector<Moviment>& out) {
        int f = seq.getSeq().back().getFila();
        int c = seq.getSeq().back().getColumna();

        for (const pair<int, int>& dir : DIRS) {
            int df = dir.first;
            int dc = dir.second;
            if (tipus == TIPUS_NORMAL) {
                int f1 = f + df, c1 = c + dc;
                int f2 = f + 2 * df, c2 = c + 2 * dc;
                if (dins(f2, c2)
                    && !board[f1][c1].isEmpty()
                    && board[f1][c1].getColor() != color
                    && board[f2][c2].isEmpty()) {
                    Moviment next = seq;
                    next.afegeix(Posicio(
                        string() + char('a' + c2) + char('0' + (8 - f2))
                    ));
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
                    r += df; cc += dc;
                }
                if (dins(r, cc)
                    && !board[r][cc].isEmpty()
                    && board[r][cc].getColor() != color) {
                    int r2 = r + df, c2 = cc + dc;
                    while (dins(r2, c2)) {
                        if (!board[r2][c2].isEmpty()) break;
                        Moviment next = seq;
                        next.afegeix(Posicio(
                            string() + char('a' + c2) + char('0' + (8 - r2))
                        ));
                        out.push_back(next);
                        Fitxa nb[N_FILES][N_COLUMNES];
                        copyBoard(board, nb);
                        nb[r][cc].setEmpty();
                        nb[f][c].setEmpty();
                        nb[r2][c2] = fit;
                        exploreCaptures(nb, fit, color, tipus, next, out);
                        r2 += df; c2 += dc;
                    }
                }
            }
        }
    }

} // namespace

// ------------------------------
// Destructor: allibera totes les Fitxa*
Tauler::~Tauler() {
    for (vector<Fitxa*>& fila : m_tauler)
        for (Fitxa* ptr : fila)
            delete ptr;  // ignora nullptr
}

// ------------------------------
// Inicialitza amb vector dinàmic i crea Fitxa* per cada peça
void Tauler::inicialitza(const string& nomFitxer) {
    // 1) Redimensiona i inicialitza a nullptr
    m_tauler.assign(
        N_FILES,
        vector<Fitxa*>(N_COLUMNES, nullptr)
    );

    // 2) Llegeix fitxer i col·loca fitxes
    ifstream in(nomFitxer);
    if (!in)
        throw runtime_error(
            "No es pot obrir fitxer \"" + nomFitxer + "\""
        );

    char c; string pos;
    while (in >> c >> pos) {
        Posicio p(pos);
        int f = p.getFila(), col = p.getColumna();
        // Si ja hi havia una fitxa, la borrem
        delete m_tauler[f][col];
        // Creem nova fitxa dinàmica
        Fitxa* fp = new Fitxa(Fitxa::fromChar(c));
        m_tauler[f][col] = fp;
    }
}

// ------------------------------
// Calcula moviments vàlids usant lògica migrada
void Tauler::actualitzaMovimentsValids() {
    // Neteja moviments de cada fitxa
    for (int i = 0; i < N_FILES; ++i)
        for (int j = 0; j < N_COLUMNES; ++j)
            if (m_tauler[i][j])
                m_tauler[i][j]->netejaMoviments();

    // Per cada fitxa, calcula captures i passos simples
    for (int i = 0; i < N_FILES; ++i) {
        for (int j = 0; j < N_COLUMNES; ++j) {
            Fitxa* ptr = m_tauler[i][j];
            if (!ptr) continue;

            // 1) Creem un tauler local de Fitxa per explorar
            Fitxa board[N_FILES][N_COLUMNES];
            for (int r = 0; r < N_FILES; ++r) {
                for (int c = 0; c < N_COLUMNES; ++c) {
                    if (m_tauler[r][c])
                        board[r][c] = *m_tauler[r][c];
                    else
                        board[r][c].setEmpty();
                }
            }

            Posicio origen(
                string() + char('a' + j) + char('0' + (8 - i))
            );
            ColorFitxa color = ptr->getColor();
            TipusFitxa tipus = ptr->getTipus();

            // 2) Captures recursives
            vector<Moviment> captureMoves;
            exploreCaptures(board, *ptr, color, tipus,
                Moviment(origen), captureMoves);

            // 3) Moviments simples
            vector<Moviment> simpleMoves;
            for (const pair<int, int>& dir : DIRS) {
                int df = dir.first, dc = dir.second;
                if (tipus == TIPUS_NORMAL) {
                    // només cap amunt o cap avall segons color
                    if ((color == COLOR_BLANC && df != -1) ||
                        (color == COLOR_NEGRE && df != 1))
                        continue;
                    int f1 = i + df, c1 = j + dc;
                    if (dins(f1, c1) && !m_tauler[f1][c1]) {
                        Moviment m(origen);
                        m.afegeix(Posicio(
                            string() + char('a' + c1) +
                            char('0' + (8 - f1))
                        ));
                        simpleMoves.push_back(m);
                    }
                }
                else {
                    int r = i + df, c = j + dc;
                    while (dins(r, c) && !m_tauler[r][c]) {
                        Moviment m(origen);
                        m.afegeix(Posicio(
                            string() + char('a' + c) +
                            char('0' + (8 - r))
                        ));
                        simpleMoves.push_back(m);
                        r += df; c += dc;
                    }
                }
            }

            // 4) Ordenem i assignem moviments a ptr
            if (tipus == TIPUS_NORMAL) {
                for (const Moviment& m : captureMoves) ptr->afegeixMoviment(m);
                for (const Moviment& m : simpleMoves)  ptr->afegeixMoviment(m);
            }
            else {
                for (const Moviment& m : simpleMoves)  ptr->afegeixMoviment(m);
                for (const Moviment& m : captureMoves) ptr->afegeixMoviment(m);
            }
        }
    }
}

// ------------------------------
// Llista les posicions destí possibles des d'origen
void Tauler::getPosicionsPossibles(
    const Posicio& origen,
    int& n,
    Posicio posicions[]
) {
    n = 0;
    int fo = origen.getFila(), co = origen.getColumna();
    if (!m_tauler[fo][co]) return;
    actualitzaMovimentsValids();
    vector<Moviment> movs = m_tauler[fo][co]->getMoviments();
    for (const Moviment& m : movs) {
        if (m.longitud() < 2) continue;
        Posicio dest = m.getSeq().back();
        bool exists = false;
        for (int k = 0; k < n; ++k)
            if (posicions[k] == dest) { exists = true; break; }
        if (!exists) posicions[n++] = dest;
    }
}

// ------------------------------
// Mou la fitxa, aplica captures obligatòries i promocions
bool Tauler::mouFitxa(
    const Posicio& origen,
    const Posicio& desti
) {
    int fo = origen.getFila(), co = origen.getColumna();
    Fitxa* origPtr = m_tauler[fo][co];
    if (!origPtr || origPtr->isEmpty()) return false;

    actualitzaMovimentsValids();
    vector<Moviment> movs = origPtr->getMoviments();

    // Comptador de captures per moviment
    function<int(const Moviment&)> countCaptures = [](const Moviment& mv) {
        int cap = 0;
        const vector<Posicio>& seq = mv.getSeq();
        for (size_t k = 0; k + 1 < seq.size(); ++k) {
            if (abs(seq[k + 1].getFila() - seq[k].getFila()) > 1 ||
                abs(seq[k + 1].getColumna() - seq[k].getColumna()) > 1)
                ++cap;
        }
        return cap;
        };

    // Global: màxim captures i peces que en poden fer
    int globalMaxCaptures = 0;
    vector<Posicio> piecesCapturing;
    for (int i = 0; i < N_FILES; ++i)
        for (int j = 0; j < N_COLUMNES; ++j) {
            Fitxa* p = m_tauler[i][j];
            if (!p || p->getColor() != origPtr->getColor()) continue;
            int localMax = 0;
            for (const Moviment& mv : p->getMoviments())
                localMax = max(localMax, countCaptures(mv));
            if (localMax > 0) {
                piecesCapturing.push_back(
                    Posicio(string() + char('a' + j) +
                        char('0' + (8 - i)))
                );
                globalMaxCaptures = max(globalMaxCaptures, localMax);
            }
        }

    // Troba el moviment escollit
    bool found = false;
    Moviment chosen;
    for (const Moviment& m : movs) {
        if (m.getSeq().back() == desti) {
            chosen = m; found = true; break;
        }
    }
    if (!found) return false;
    int chosenCaps = countCaptures(chosen);
    const vector<Posicio>& seq = chosen.getSeq();

    // Si hi ha captures obligatòries però no en fem
    if (globalMaxCaptures > 0 && chosenCaps == 0) {
        m_tauler[desti.getFila()][desti.getColumna()] = origPtr;
        m_tauler[fo][co] = nullptr;
        for (const Posicio& p : piecesCapturing)
            m_tauler[p.getFila()][p.getColumna()]->setEmpty();
        actualitzaMovimentsValids();
        return true;
    }

    // Aplica captures intermedies (inclou bufada)
    for (size_t k = 0; k + 1 < seq.size(); ++k) {
        int r1 = seq[k].getFila(), c1 = seq[k].getColumna();
        int r2 = seq[k + 1].getFila(), c2 = seq[k + 1].getColumna();
        int dr = (r2 > r1 ? 1 : (r2 < r1 ? -1 : 0));
        int dc = (c2 > c1 ? 1 : (c2 < c1 ? -1 : 0));
        int rr = r1 + dr, cc = c1 + dc;
        while (rr != r2 || cc != c2) {
            if (m_tauler[rr][cc] && m_tauler[rr][cc]->getColor() != origPtr->getColor()) {
                delete m_tauler[rr][cc];
                m_tauler[rr][cc] = nullptr;
                break;
            }
            rr += dr; cc += dc;
        }
    }

    // Mou la fitxa i possible promoció
    m_tauler[fo][co] = nullptr;
    Posicio last = seq.back();
    if (origPtr->getTipus() == TIPUS_NORMAL) {
        int fd = last.getFila();
        if ((origPtr->getColor() == COLOR_BLANC && fd == 0) ||
            (origPtr->getColor() == COLOR_NEGRE && fd == N_FILES - 1))
            origPtr->converteixDama();
    }
    m_tauler[last.getFila()][last.getColumna()] = origPtr;
    actualitzaMovimentsValids();
    return true;
    // Retorna el punter a la fitxa d'una posicio donada
    Fitxa* Tauler::getFitxa(const Posicio & pos) const {
        int f = pos.getFila();
        int c = pos.getColumna();
        if (f < 0 || f >= N_FILES || c < 0 || c >= N_COLUMNES) return nullptr;
        return m_tauler[f][c];
}

// ------------------------------
// ASCII art del tauler amb punters
string Tauler::toString() const {
    ostringstream ss;
    for (int i = 0; i < N_FILES; ++i) {
        ss << (8 - i) << ": ";
        for (int j = 0; j < N_COLUMNES; ++j) {
            Fitxa* f = m_tauler[i][j];
            ss << (f ? f->toChar() : '_');
            if (j < N_COLUMNES - 1) ss << ' ';
        }
        ss << '\n';
    }
    ss << "   a b c d e f g h\n";
    return ss.str();
}
