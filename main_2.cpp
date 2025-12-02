#include <iostream>
#include <stack>
#include <chrono>
#include <vector>
#include <fstream>
using namespace std;
using namespace std::chrono;

// implementation de l'algorithme recursive
void hanoiRecursive(int n, char from, char to, char aux) {
    if (n == 0) return;
    hanoiRecursive(n - 1, from, aux, to);
    hanoiRecursive(n - 1, aux, to, from);
}

// implementation de l'algorithme itterative
void hanoiIterative(int n, char from, char to, char aux) {
    vector<stack<int>> stacks(3);
    int src = 0, help = 1, dest = 2;

    for (int i = n; i > 0; i--) stacks[src].push(i);

    int totalMoves = (1 << n) - 1;
    if (n % 2 == 0) swap(help, dest);

    for (int i = 1; i <= totalMoves; i++) {
        int a, b;
        if (i % 3 == 1)      { a = src;  b = dest; }
        else if (i % 3 == 2) { a = src;  b = help; }
        else                 { a = help; b = dest; }

        if (stacks[b].empty() || (!stacks[a].empty() && stacks[a].top() < stacks[b].top())) {
            stacks[b].push(stacks[a].top());
            stacks[a].pop();
        } else {
            stacks[a].push(stacks[b].top());
            stacks[b].pop();
        }
    }
}

// creation des fichiers csv
ofstream csvRecursive("hanoi_recursive.csv");
ofstream csvIterative("hanoi_iterative.csv");
ofstream csvResultats("hanoi_resultats.csv");

// initialisation
void initCSV() {
    csvRecursive << "n,temps_microsecondes\n";
    csvIterative << "n,temps_microsecondes\n";
    csvResultats << "n,recursive_micros,iterative_micros\n";
}

// ecriture sur les fichiers recursive et iterative
void ecrireCSV(ofstream& file, int n, long long temps) {
    file << n << "," << temps << "\n";
    file.flush();
}

// ecriture sur le fichier resultats 
void ecrireResultat(int n, long long tempsRec, long long tempsIter) {
    csvResultats << n << ";" << tempsRec << ";" << tempsIter << "\n";
    csvResultats.flush();
}

// fonction pour teste et attraper les erreurs pour la fonction recursive
long long testRecursive(int n) {
    try {
        auto start = high_resolution_clock::now();
        hanoiRecursive(n, 'A', 'C', 'B');
        auto end = high_resolution_clock::now();
        long long duration = duration_cast<microseconds>(end - start).count();
        ecrireCSV(csvRecursive, n, duration);
        return duration;
    }
    catch (...) {
        ecrireCSV(csvRecursive, n, 0);
        return 0;
    }
}

// fonction pour teste et attraper les erreurs pour la fonction iterative
long long testIterative(int n) {
    try {
        auto start = high_resolution_clock::now();
        hanoiIterative(n, 'A', 'C', 'B');
        auto end = high_resolution_clock::now();
        long long duration = duration_cast<microseconds>(end - start).count();
        //cout << "  Iterative n = " << n << " : " << duration << " microseconde\n";
        ecrireCSV(csvIterative, n, duration);
        return duration;
    }
    catch (...) {
        //cout << "  Iterative n = " << n << " : CRASHED\n";
        ecrireCSV(csvIterative, n, 0);
        return 0;
    }
}

int main() {
    cout << "=== Tours de Hanoi - Comparaison Recursive vs Iterative ===\n\n";
    initCSV();

    int n = 1;
    int maxRecursive = 0;
    int maxIterative = 0;

    cout << fixed;

    while (true) {
        cout << "\n--- Test n = " << n << " ---\n";

        long long tempsRec = testRecursive(n);
        long long tempsIter = testIterative(n);

        // remplirer le fichier resultat
        ecrireResultat(n, tempsRec, tempsIter);

        if (tempsRec > 0) maxRecursive = n;
        if (tempsIter > 0) maxIterative = n;

        cout << "Recursive  : ";
        if (tempsRec > 0) cout << tempsRec << " microseconde\n";
        else cout << "CRASH (stack overflow)\n";

        cout << "Iterative  : ";
        if (tempsIter > 0) cout << tempsIter << " microseconde\n";
        else cout << "CRASH\n";

        //  on met une limite
        if (n > 45) {
            cout << "\nLimite atteinte.\n";
            break;
        }

        // si la vertion recursive a crash depuis un moment on arrete (on ajoute 5 iteration de plus)
        if (maxRecursive > 0 && n > maxRecursive + 5) {
            cout << "\nLa version recursive a crash depuis n=" << maxRecursive << " on arrete les tests.\n";
            break;
        }

        // si la vertion iterative a crash depuis un moment on arrete (on ajoute 5 iteration de plus)
        if (maxIterative > 0 && n > maxIterative + 5) {
            cout << "\nLa version iterative a crash depuis n=" << maxIterative << " on arrete les tests.\n";
            break;
        }

        n++;  // on augmente le n 
    }

    // un resumer des resultats finaux obtenu
    cout << "\n" << string(50, '=') << "\n";
    cout << "           RESULTATS FINAUX\n";
    cout << string(50, '=') << "\n";
    cout << "Recursive supporte jusqu'à n = " << maxRecursive << "\n";
    cout << "Iterative supporte jusqu'à n = " << maxIterative << "\n";
    cout << string(50, '=') << "\n\n";

    // fermeture des fichiers
    csvRecursive.close();
    csvIterative.close();
    csvResultats.close();
    system("pause");
    return 0;
}
