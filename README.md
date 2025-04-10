# Projecte-Dames

Este repositorio contiene una implementación básica del juego de Damas (Checkers) en C++, siguiendo la estructura de clases propuesta en la asignatura.

## Estructura del proyecto

- **Ftixa.h / Fitxa.cpp**  
  Declara e implementa la clase `Fitxa`, que representa una pieza en el tablero (puede ser normal, dama o vacía; color blanco o negro).

- **Moviment.h / Moviment.cpp**  
  Declara e implementa la clase `Moviment`, que encapsula la información sobre un movimiento específico (posiciones recorridas, capturas, etc.).

- **Posicio.h / Posicio.cpp**  
  Representa una posición en el tablero (ejemplo: “b2”). Permite convertir de string a coordenadas internas y viceversa, además de comparar dos posiciones.

- **Tauler.h / Tauler.cpp**  
  Contiene la lógica principal del tablero: inicialización a partir de un fichero de texto, actualización de movimientos válidos, movimiento de piezas, así como un método `toString()` para mostrar el estado del tablero.
