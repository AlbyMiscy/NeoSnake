# 🐍 NeoSnake

<div align="center">

[![SFML](https://img.shields.io/badge/SFML-3.0.1-green?logo=sfml&logoColor=white&style=flat)](https://www.sfml-dev.org/)
[![Email](https://img.shields.io/badge/Email-albymiscy07%40outlook.it-blue?logo=gmail&logoColor=white&style=flat)](mailto:albymiscy07@outlook.it)
[![GitHub](https://img.shields.io/badge/GitHub-AlbyMiscy-181717?logo=github&logoColor=white&style=flat)](https://github.com/AlbyMiscy)

</div>

Un classico gioco del Serpente implementato in **C++17**, utilizzando la **libreria SFML 3.0.1** per la grafica.  Il progetto è gestito tramite **CMake 3.28** ed è pensato per l’ambiente **Linux** (compilatori GCC o Clang).

---

## ⚙️ Requisiti
- **CMake** ≥ 3.28  
- **C++17 compiler** (GCC ≥ 11 o Clang ≥ 15 consigliati)  
- **Git** (per scaricare automaticamente SFML 3.0.1 tramite `FetchContent`) 

---

## 🧠 Configurazione e compilazione
### 🔹 1. Configurazione del progetto
Posizionati nella root del progetto e genera la cartella di build con:

#### 🔧 Modalità Debug (consigliata per sviluppo)
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
```
#### 🚀 Modalità Release (per la versione finale)
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
```

### 🔹 2. Compilazione
Dopo la configurazione, costruisci il progetto con:
```bash
cmake --build build
```

L’eseguibile verrà generato in:
```bash
build/bin/NeoSnake
```

### 🔹 3. Esecuzione
Esegui il gioco da terminale:
```
./build/bin/NeoSnake
```
