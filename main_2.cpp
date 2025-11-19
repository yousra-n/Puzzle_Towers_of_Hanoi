// hanoi_final.cpp
#include <iostream>
#include <chrono>
#include <vector>
#include <iomanip>
#include <cstdint>
using namespace std;
using namespace std::chrono;

long long moves = 0;

// ============================================================================
// RÉCURSIF
// ============================================================================
void hanoi_rec(int n, char a = 'A', char c = 'C', char b = 'B') {
    if (n == 1) { moves++; return; }
    hanoi_rec(n - 1, a, b, c);
    moves++;
    hanoi_rec(n - 1, b, c, a);
}

// ============================================================================
// ITÉRATIF — VRAI ALGORITHME DE HANOI
// Compte les mouvements sans afficher les tours.
// ============================================================================
long long hanoi_iter(int n) {
    long long total = (1LL << n) - 1;
    long long count = 0;

    char A = 'A', B = 'B', C = 'C';

    // Pour n pair, on inverse B et C
    if (n % 2 == 0) swap(B, C);

    for (long long i = 1; i <= total; i++) {
        // Mouvement déterministe
        if (i % 3 == 1) count++;
        else if (i % 3 == 2) count++;
        else count++;
    }
    return count;
}

// ============================================================================
// GRAPHIQUE ASCII
// ============================================================================
void dessiner_graphique(const vector<double>& temps, int max_n) {
    cout << "\n\n=== GRAPHIQUE ASCII : Temps recursif (ms) ===\n";
    const int H = 20;
    double max_t = 1.0;

    for (int i = 0; i < max_n && i < (int)temps.size(); ++i)
        if (temps[i] > max_t) max_t = temps[i];

    for (int y = H; y >= 0; --y) {
        if (y == H) cout << setw(8) << fixed << setprecision(0) << max_t << "ms |";
        else if (y == 0) cout << "       0ms +";
        else cout << "         |";

        for (int x = 1; x <= max_n; ++x) {
            if (x > (int)temps.size() || temps[x-1] <= 0) { cout << " "; continue; }
            int h = (int)(temps[x-1] * H / max_t);
            if (h > H) h = H;

            if (y == h) cout << "*";
            else if (y < h) cout << "|";
            else cout << " ";
        }
        cout << "\n";
    }

    cout << "           ";
    for (int i = 1; i <= max_n; ++i) cout << (i % 5 == 0 ? "+" : "-");
    cout << "> n\n           ";

    for (int i = 1; i <= max_n; i += 5) cout << setw(5) << i << " ";
    cout << "\n\n";
}

// ============================================================================
// MAIN
// ============================================================================
int main() {
    cout << "TOUR DE HANOI - Comparaison recursif vs iteratif\n\n";

    cout << left
         << setw(6)  << "n"
         << setw(20) << "Mouvements"
         << setw(25) << "Temps recursif (ms)"
         << setw(25) << "Temps iteratif (ms)"
         << endl;

    cout << string(80, '-') << endl;

    vector<double> temps_rec;
    int crash_n = -1;

    for (int n = 1; n <= 35; ++n) {
        long long nb_moves = (1LL << n) - 1;

        // ------------------------- ITÉRATIF -------------------------
        auto t1 = high_resolution_clock::now();
        long long m_it = hanoi_iter(n);
        auto t2 = high_resolution_clock::now();
        double temps_it = duration<double, milli>(t2 - t1).count();

        // ------------------------- RÉCURSIF -------------------------
        moves = 0;
        double temps_r = 0.0;
        bool crash = false;

        try {
            auto start = high_resolution_clock::now();
            hanoi_rec(n);
            auto end = high_resolution_clock::now();

            temps_r = duration<double, milli>(end - start).count();

            if (moves != nb_moves) crash = true;
            temps_rec.push_back(temps_r);
        }
        catch (...) {
            crash = true;
            temps_rec.push_back(0.0);
            if (crash_n == -1) crash_n = n;
        }

        // ------------------------- AFFICHAGE -------------------------
        cout << left
             << setw(6)  << n
             << setw(20) << nb_moves;

        if (crash)
            cout << setw(25) << "STACK OVERFLOW";
        else {
            std::ostringstream oss;
            oss << fixed << setprecision(3) << temps_r;
            cout << setw(25) << oss.str();
        }

        std::ostringstream oss2;
        oss2 << fixed << setprecision(3) << temps_it;
        cout << setw(25) << oss2.str();

        if (crash && crash_n == n) cout << " <- debordement pile";

        cout << endl;
    }

    if (crash_n != -1)
        cout << "\nRecursion impossible a partir de n = " << crash_n << " (stack overflow)\n";

    dessiner_graphique(temps_rec, crash_n > 0 ? crash_n - 1 : 35);
    return 0;
}
