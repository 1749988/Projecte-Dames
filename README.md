# Project-Dames

## Descripción del proyecto

**Project-Dames** es una implementación completa del juego de Damas en C++, desarrollada para la asignatura de Metodologia de Programación. Se ha diseñado un **motor de juego** basado en:

  - **Gestión dinámica de memoria**: la clase `Tauler` utiliza `std::vector<Fitxa*>` y `new`/`delete` para manejar piezas en tiempo de ejecución.
  - **Lógica de movimientos**: cálculo de movimientos simples y capturas recursivas para fichas normales y damas.
  - **Modo Replay**: almacenamiento y reproducción de secuencias de jugadas mediante la clase `CuaMoviments`, estructurada como una lista enlazada.



