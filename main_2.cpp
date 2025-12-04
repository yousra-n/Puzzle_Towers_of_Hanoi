#include <iostream>
#include <stack>
#include <chrono>
#include <vector>
#include <fstream>
#include <iomanip>
using namespace std;
using namespace std::chrono;

struct Frame {
    int n;
    char from, to, aux;
    int state;
};

void hanoiIterative(int n, char from, char to, char aux) {
    stack<Frame> st;
    st.push({n, from, to, aux, 0});

    while (!st.empty()) {
        Frame &f = st.top();
        if (f.n == 1) {
            st.pop();
            continue;
        }
        if (f.state == 0) {
            f.state = 1;
            st.push({f.n - 1, f.from, f.aux, f.to, 0});
        }
        else if (f.state == 1) {
            f.state = 2;
        }
        else {
            st.pop();
            st.push({f.n - 1, f.aux, f.to, f.from, 0});
        }
    }
}


// ======================== VERSION RÉCURSIVE (inchangée) ========================
void hanoiRecursive(int n, char from, char to, char aux) {
    if (n == 0) return;
    hanoiRecursive(n - 1, from, aux, to);
    // cout << "Déplace disque " << n << " de " << from << " vers " << to << endl;
    hanoiRecursive(n - 1, aux, to, from);
}

// ======================== FICHIERS CSV ========================
ofstream csvRecursive("hanoi_recursive.csv");
ofstream csvIterative("hanoi_iterative.csv");
ofstream csvResultats("hanoi_resultats.csv");

void initCSV() {
    csvRecursive << "n,temps_microsecondes\n";
    csvIterative << "n,temps_microsecondes\n";
    csvResultats << "n;recursive_micros;iterative_micros\n";
}

void ecrireCSV(ofstream& file, int n, long long temps) {
    file << n << "," << temps << "\n";
    file.flush();
}

void ecrireResultat(int n, long long tempsRec, long long tempsIter) {
    csvResultats << n << ";" << tempsRec << ";" << tempsIter << "\n";
    csvResultats.flush();
}

// ======================== TESTS ========================
long long testRecursive(int n) {
    try {
        auto start = high_resolution_clock::now();
        hanoiRecursive(n, 'A', 'C', 'B');
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start).count();
        ecrireCSV(csvRecursive, n, duration);
        return duration;
    }
    catch (...) {
        ecrireCSV(csvRecursive, n, 0);
        return 0;
    }
}

long long testIterative(int n) {
    try {
        auto start = high_resolution_clock::now();
        hanoiIterative(n, 'A', 'C', 'B');
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start).count();
        ecrireCSV(csvIterative, n, duration);
        return duration;
    }
    catch (...) {
        ecrireCSV(csvIterative, n, 0);
        return 0;
    }
}

// ======================== MAIN ========================
int main() {
    cout << "=== Tours de Hanoi - Comparaison Recursive vs Iterative ===\n\n";
    initCSV();

    int n = 1;
    int maxRecursive = 0;
    int maxIterative = 0;

    cout << fixed << setprecision(3);

    while (true) {
        cout << "\n--- Test n = " << n << " ---\n";

        long long tempsRec = testRecursive(n);
        long long tempsIter = testIterative(n);

        ecrireResultat(n, tempsRec, tempsIter);

        if (tempsRec > 0) maxRecursive = n;
        if (tempsIter > 0) maxIterative = n;

        cout << "Recursive  : ";
        if (tempsRec > 0) cout << tempsRec << " microseconde\n";
        else cout << "CRASH (stack overflow)\n";

        cout << "Iterative  : ";
        if (tempsIter > 0) cout << tempsIter << " microseconde";
        else cout << "CRASH";
        cout << endl;

        // Limite raisonnable (l'itératif peut aller beaucoup plus loin si tu veux)
        if (n >= 40) {
            cout << "\nLimite de test atteinte.\n";
            break;
        }

        // Arrêt si récursif a crashé depuis longtemps
        if (maxRecursive > 0 && n > maxRecursive + 10) {
            cout << "\nVersion recursive bloquee depuis n=" << maxRecursive << ", on arrete.\n";
            break;
        }
        if (maxIterative > 0 && n > maxIterative + 5) {
            cout << "\nLa version iterative a crash depuis n=" << maxIterative << " on arrete les tests.\n";
            break;
        }

        n++;
    }

    cout << "\n" << string(60, '=') << "\n";
    cout << "           RESULTATS FINAUX\n";
    cout << string(60, '=') << "\n";
    cout << "Recursive  : supporte jusqu'a n = " << maxRecursive << "\n";
    cout << "Iterative  : supporte jusqu'a n = " << maxIterative << " (et bien plus !)\n";
    cout << string(60, '=') << "\n\n";

    csvRecursive.close();
    csvIterative.close();
    csvResultats.close();

    return 0;
}
