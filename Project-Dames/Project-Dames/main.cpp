//
//  main.cpp
//

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) || defined(_WIN64)

#include <iostream>
#define SDL_MAIN_HANDLED
#include <windows.h>
// Llibreria gràfica
#include "../Graphic Lib/libreria.h"
#include "../Graphic Lib/NFont/NFont.h"
#include <conio.h>

#elif __APPLE__

#include "../Graphic Lib/libreria.h"
#include "../Graphic Lib/NFont/NFont.h"
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#include <SDL2/SDL.h>
#pragma clang diagnostic pop

#else // Linux

#include <iostream>
#define SDL_MAIN_HANDLED
#include "../Graphic Lib/libreria.h"
#include "../Graphic Lib/NFont/NFont.h"

#endif

#include "./joc.hpp"
#include "./info_joc.hpp"

int main(int argc, const char* argv[])
{
    // Instruccions per SDL
    SDL_SetMainReady();
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    // Crea i mostra la finestra
    Screen pantalla(TAMANY_PANTALLA_X, TAMANY_PANTALLA_Y);
    pantalla.show();

    // ─── Inicialitza la lògica del joc ──────────────────────
    Joc joc;
    joc.inicialitza(
        MODE_JOC_NORMAL,               // Mode de joc: NORMAL o REPLAY
        "Games/tauler.txt",            // Fitxer amb posició inicial
        "Games/moviments.txt"          // Fitxer per desar/carregar moviments
    ); :contentReference[oaicite:0]{ index = 0 }

        // ─── Bucle principal ────────────────────────────────────
        do {
            pantalla.processEvents();

            bool mouseStatus = Mouse_getBtnLeft();
            int mousePosX = Mouse_getX();
            int mousePosY = Mouse_getY();

            // Processa clics, dibuixa el tauler, peces, highlights i text
            joc.actualitza(mousePosX, mousePosY, mouseStatus);

            pantalla.update();
        } while (!Keyboard_GetKeyTrg(KEYBOARD_ESCAPE));

    // ─── Finalitza ──────────────────────────────────────────
    // Desem moviments (només en MODE_JOC_NORMAL) i tanquem SDL
    joc.finalitza();
    SDL_Quit();

    return 0;
}

