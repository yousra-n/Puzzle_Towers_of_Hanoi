//prototype Tour de Hanoï avec interface graphique Win32 (sur code blocks)
#include <windows.h>
#include <commctrl.h>
#include <sstream>
#include <iomanip>
#include <vector>
#include <chrono>
using namespace std;
using namespace std::chrono;

#pragma comment(lib, "comctl32.lib")

#define IDC_CALCULER 1001
#define IDC_EDIT_N   1002
#define IDC_RESULTAT 1003

HWND hEditN, hResultat;
long long moves = 0;

// fonction recursive de hanoi
void hanoi_rec(int n, char a = 'A', char c = 'C', char b = 'B') {
    if (n == 1) { moves++; return; }
    hanoi_rec(n - 1, a, b, c);
    moves++;
    hanoi_rec(n - 1, b, c, a);
}

long long hanoi_iter(int n) {
    return (1LL << n) - 1; 
}

// graphe ascii temporaire
string generer_graphique(const vector<double>& temps, int max_n) {
    stringstream ss;
    ss << "\r\n\r\n=== GRAPHIQUE ASCII : Temps recursif (ms) ===\r\n";
    const int H = 20;
    double max_t = 1.0;
    for (double t : temps) if (t > max_t) max_t = t;

    for (int y = H; y >= 0; --y) {
        if (y == H) ss << fixed << setprecision(0) << setw(8) << max_t << "ms |";
        else if (y == 0) ss << " 0ms +";
        else ss << "       |";

        for (int x = 0; x < max_n; ++x) {
            if (x >= (int)temps.size() || temps[x] <= 0) { ss << " "; continue; }
            int h = (int)(temps[x] * H / max_t);
            if (h > H) h = H;
            if (y == h) ss << "*";
            else if (y < h) ss << "|";
            else ss << " ";
        }
        ss << "\r\n";
    }
    ss << "        ";
    for (int i = 1; i <= max_n; ++i) ss << (i % 5 == 0 ? "+" : "-");
    ss << "> n\r\n        ";
    for (int i = 1; i <= max_n; i += 5) ss << setw(5) << i << " ";
    ss << "\r\n\r\n";
    return ss.str();
}

void CalculerHanoi() {
    char buffer[10];
    GetWindowText(hEditN, buffer, 10);
    int max_n = atoi(buffer);
    if (max_n < 1 || max_n > 35) max_n = 30;

    stringstream resultat;
    resultat << "TOUR DE HANOI - Comparaison recursif vs iteratif\r\n\r\n";
    resultat << left << setw(6) << "n" << setw(20) << "Mouvements" 
             << setw(25) << "Temps recursif (ms)" << setw(25) << "Temps iteratif (ms)\r\n";
    resultat << string(80, '-') << "\r\n";

    vector<double> temps_rec;
    int crash_n = -1;

    for (int n = 1; n <= max_n; ++n) {
        long long nb_moves = (1LL << n) - 1;

        auto t1 = high_resolution_clock::now();
        long long m_it = hanoi_iter(n);
        auto t2 = high_resolution_clock::now();
        double temps_it = duration<double, milli>(t2 - t1).count();

        moves = 0;
        double temps_r = 0.0;
        bool crash = false;
        try {
            auto start = high_resolution_clock::now();
            hanoi_rec(n);
            auto end = high_resolution_clock::now();
            temps_r = duration<double, milli>(end - start).count();
            temps_rec.push_back(temps_r);
        }
        catch (...) {
            crash = true;
            temps_rec.push_back(0.0);
            if (crash_n == -1) crash_n = n;
        }

        resultat << left << setw(6) << n << setw(20) << nb_moves;
        if (crash)
            resultat << setw(25) << "STACK OVERFLOW";
        else
            resultat << fixed << setprecision(3) << setw(25) << temps_r;
        resultat << fixed << setprecision(3) << setw(25) << temps_it;
        if (crash && crash_n == n) resultat << " <- debordement pile";
        resultat << "\r\n";
    }

    if (crash_n != -1)
        resultat << "\r\nRecursion impossible a partir de n = " << crash_n << " (stack overflow)\r\n";

    resultat << generer_graphique(temps_rec, crash_n > 0 ? crash_n - 1 : max_n);

    SetWindowText(hResultat, resultat.str().c_str());
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CREATE:
        CreateWindow("STATIC", "Entrez n (max 35) :", WS_VISIBLE | WS_CHILD, 20, 20, 200, 20, hwnd, NULL, NULL, NULL);
        hEditN = CreateWindow("EDIT", "30", WS_VISIBLE | WS_CHILD | WS_BORDER, 220, 20, 60, 25, hwnd, (HMENU)IDC_EDIT_N, NULL, NULL);
        CreateWindow("BUTTON", "Lancer le test", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 300, 20, 140, 30, hwnd, (HMENU)IDC_CALCULER, NULL, NULL);

        hResultat = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL, 
                                 20, 70, 760, 480, hwnd, (HMENU)IDC_RESULTAT, NULL, NULL);
        SendMessage(hResultat, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), TRUE);
        return 0;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDC_CALCULER)
            CalculerHanoi();
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// main
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
    InitCommonControls();

    const char CLASS_NAME[] = "HanoiGUIClass";

    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(0, CLASS_NAME, "Tour de Hanoï - Récursif vs Itératif", 
                               WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 820, 620, 
                               NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, nCmdShow);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}
