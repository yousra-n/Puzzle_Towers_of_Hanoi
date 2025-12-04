#include <iostream>
#include <stack>
#include <chrono>
#include <vector>
#include <fstream> 
using namespace std;
using namespace std::chrono;


// implementation du code iterative 
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


// implementation du code recursive
void hanoiRecursive(int n, char from, char to, char aux) {
    if (n == 0) return;
    hanoiRecursive(n - 1, from, aux, to);
    hanoiRecursive(n - 1, aux, to, from);
}

// creation des ficher csv
ofstream csvRecursive("hanoi_recursive.csv");
ofstream csvIterative("hanoi_iterative.csv");
ofstream csvResultats("hanoi_resultats.csv");

// inisialisation des fichiers csv
void initCSV() {
    csvRecursive << "n,temps_microsecondes\n"; //pour les resultat du code recursive
    csvIterative << "n,temps_microsecondes\n"; //pour les resultat du code iterative
    csvResultats << "n;recursive_microsecondes;iterative_microsecondes\n"; //pour les resultat du code recursive et iterative 
}

// fonction ecriture sur les fichers
void ecrireCSV(ofstream& file, int n, long long temps) {
    file << n << "," << temps << "\n";
    file.flush();
}

void ecrireResultat(int n, long long tempsRec, long long tempsIter) {
    csvResultats << n << ";" << tempsRec << ";" << tempsIter << "\n";
    csvResultats.flush();
}

// on teste la fonction recursive 
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

// on teste la fonction iterative 
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

// main
int main() {

    cout << "=== Tours de Hanoi - Comparaison Recursive vs Iterative ===\n\n";
    initCSV();

    int n = 1;
    int maxRecursive = 0;
    int maxIterative = 0;

    cout << fixed ;

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

        // Limite pour pas faire des itteration indefiniment
        if (n >= 40) {
            cout << "\nLimite de test atteinte.\n";
            break;
        }

        // stop si recursive a crash depuis longtemps
        if (maxRecursive > 0 && n > maxRecursive + 10) {
            cout << "\nVersion recursive bloquee depuis n=" << maxRecursive << ", on arrete.\n";
            break;
        }
        // stop si itterative a crash depuis longtemps
        if (maxIterative > 0 && n > maxIterative + 5) {
            cout << "\nLa version iterative a crash depuis n=" << maxIterative << " on arrete les tests.\n";
            break;
        }

        n++; //incremantaion de n 
    }

    // resumer des resultats obtenu
    cout << "\n" << string(60, '=') << "\n";
    cout << "           RESULTATS FINAUX\n";
    cout << string(60, '=') << "\n";
    cout << "Recursive  : supporte jusqu'a n = " << maxRecursive << "\n";
    cout << "Iterative  : supporte jusqu'a n = " << maxIterative << "\n";
    cout << string(60, '=') << "\n\n";

    //fermer les fichiers
    csvRecursive.close();
    csvIterative.close();
    csvResultats.close();

    return 0;
}
