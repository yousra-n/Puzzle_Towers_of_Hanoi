#include <iostream>
#include <stack>
#include <chrono>
#include <vector>
#include <csignal>  // Pour détecter les segfaults
using namespace std;
using namespace std::chrono;

// implementation de la vertion recursive des tours de hanoi
void hanoiRecursive(int n, char from, char to, char aux) {
    if (n == 0) return;
    hanoiRecursive(n - 1, from, aux, to);
    hanoiRecursive(n - 1, aux, to, from);
}

// implementation de la vertion iterative des tours de hanoi iterative avec pile simulée
void hanoiIterative(int n, char from, char to, char aux)
{
    // Rods indices : we map (from, aux, to) → (0,1,2)
    // A, B, C are just chars but not needed here
    vector<stack<int>> stacks(3);

    int src = 0, help = 1, dest = 2;

    // On pousse les disques dans la tige source
    for (int i = n; i > 0; i--)
        stacks[src].push(i);

    int totalMoves = (1 << n) - 1;

    // Si n est pair, on inverse auxiliaire et destination
    if (n % 2 == 0)
        swap(help, dest);

    // Boucle principale
    for (int i = 1; i <= totalMoves; i++)
    {
        int a, b;

        if (i % 3 == 1) { a = src; b = dest; }
        else if (i % 3 == 2) { a = src; b = help; }
        else { a = help; b = dest; }

        // simulate moveDisk(a,b)
        if (stacks[b].empty() || (!stacks[a].empty() && stacks[a].top() < stacks[b].top()))
        {
            // push/pop normally
            stacks[b].push(stacks[a].top());
            stacks[a].pop();
        }
        else
        {
            // inverse move
            stacks[a].push(stacks[b].top());
            stacks[b].pop();
        }
    }
}

// Fonction pour tester la version recursive et attraper les erreurs
bool testRecursive(int n) {
    try {
        auto start = high_resolution_clock::now();
        hanoiRecursive(n, 'A', 'C', 'B');
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start).count();
        cout << "  Recursive n = " << n << " : " << duration << " microseconde\n";
        return true;
    }
    catch (const std::exception& e) {
        cout << "  Recursive n = " << n << " : CRASHED (" << e.what() << ")\n";
        return false;
    }
    catch (...) {
        cout << "  Recursive n = " << n << " : CRASHED (stack overflow)\n";
        return false;
    }
}

// Fonction pour tester la version iterative et attraper les erreurs
bool testIterative(int n) {
    try {
        auto start = high_resolution_clock::now();
        hanoiIterative(n, 'A', 'C', 'B');
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start).count();
        cout << "  Iterative n = " << n << " : " << duration << " microseconde\n";
        return true;
    }
    catch (const std::exception& e) {
        cout << "  Iterative n = " << n << " : CRASHED (" << e.what() << ")\n";
        return false;
    }
    catch (...) {
        cout << "  Iterative n = " << n << " : CRASHED\n";
        return false;
    }
}

int main() {
    cout << "=== Tour de Hanoi ===\n";
    cout << "Augmentation progressive de n...\n\n";

    int n = 1;
    int maxRecursive = 0;
    int maxIterative = 0;

    
    while (true) {
        cout << "Test avec n = " << n << "\n";

        bool recOk = testRecursive(n);
        bool iterOk = testIterative(n);

        if (recOk) maxRecursive = n;
        if (iterOk) maxIterative = n;

        //si les deux crash on arrete
        if (!recOk && !iterOk) {
            cout << "\n=== RESULTATS FINAUX ===\n";
            cout << "Version recursive  : supporte jusqu'à n = " << maxRecursive << "\n";
            cout << "Version iterative  : supporte jusqu'à n = " << maxIterative << "\n";
            if (maxRecursive > maxIterative) { 
                // si la vertion recursive a crache avent la vertion iterative
                cout << "La vertion recursive a crash\n";
            } else { 
                // inverse on la vertion iterative crash et pas la recursive
                cout << "La vertion iterative a crash\n";
            }
            break;
        }

        n++;
        
        // en securiter 
        if (n > 50) {
            cout << "Limite de sécurité atteinte.\n";
            break;
        }
    }

    cout << "\nProgramme terminé.\n";
    return 0;
}
