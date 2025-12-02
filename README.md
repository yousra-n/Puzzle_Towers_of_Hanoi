# Tours de Hanoi – Comparaison Récursif vs Itératif

Projet simple comparer **deux implémentations** de l’algorithme classique des **Tours de Hanoi** en c++:

- Version récursive
- Version itérative
Le programme augmente progressivement le nombre de disques `n` jusqu’à ce que **les deux versions plantent**, mesure le temps d’exécution.

## le but de ce projet

- Voir concrètement la **limite de la pile d’appel** (stack overflow) en récursif
- Comparer les performances réelles des deux approches

## Compilation

# Windows (MinGW)
g++ -std=c++17 -O2 hanoi.cpp -o hanoi.exe
