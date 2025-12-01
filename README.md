# Tours de Hanoi – Comparaison Récursif vs Itératif

![C++](https://img.shields.io/badge/C%2B%2B-17-blue.svg)
![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey)

Projet simple comparer **deux implémentations** de l’algorithme classique des **Tours de Hanoi** :

- Version récursive
- Version itérative
Le programme augmente progressivement le nombre de disques `n` jusqu’à ce que **les deux versions plantent**, mesure le temps d’exécution.

## le but de ce projet

- Voir concrètement la **limite de la pile d’appel** (stack overflow) en récursif
- Comparer les performances réelles des deux approches

La version itérative accepte facilement ** 30 disques**, alors que la récursive s’effondre vers 40 selon la taille du stack du système.

## Compilation

# Windows (MinGW ou MSVC)
g++ -std=c++17 -O2 hanoi.cpp -o hanoi.exe
