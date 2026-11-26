//ajuda do claude

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <climits>
#include <iomanip>

using namespace std;

const int N = 8;
const double INF = 1e18;

string cidades[N] = {
    "Sao Paulo",
    "Lisboa",
    "Madrid",
    "Paris",
    "Roma",
    "Atenas",
    "Amsterda",
    "Berlim"
};

struct Aresta {
    int destino;
    double custo;
    double distancia;
    double tempo;
};

vector<Aresta> grafo[N];

struct ArestaKruskal {
    int origem, destino;
    double custo, distancia, tempo;
};

void adicionarAresta(int u, int v, double custo, double dist, double tempo) {
    grafo[u].push_back({v, custo, dist, tempo});
    grafo[v].push_back({u, custo, dist, tempo});
}

void construirGrafo() {
    adicionarAresta(0, 1, 4500, 9400,  11.0);
    adicionarAresta(0, 2, 4800, 9900,  12.0);
    adicionarAresta(0, 3, 5200, 9200,  12.5);

    adicionarAresta(1, 2,  350,  624,   6.5);

    adicionarAresta(2, 3,  600, 1054,   9.5);

    adicionarAresta(3, 4,  700,  687,   7.5);

    adicionarAresta(3, 6,  450,  430,   3.5);

    adicionarAresta(3, 7,  500,  878,   7.0);

    adicionarAresta(4, 5,  800,  211,   1.5);

    adicionarAresta(4, 7,  650,  960,   8.5);

    adicionarAresta(6, 7,  300,  577,   5.5);

    adicionarAresta(5, 7, 1100, 1800,   3.0);

    adicionarAresta(2, 4, 1200, 1952,  13.5);

    adicionarAresta(1, 3, 1000, 1453,  10.0);

    adicionarAresta(3, 5,  900, 2100,   3.5);

    adicionarAresta(4, 6,  600, 1300,   2.5);
}

pair<vector<double>, vector<int>> dijkstra(int inicio, int tipoPeso) {
    vector<double> dist(N, INF);
    vector<int> prev(N, -1);
    priority_queue<pair<double,int>, vector<pair<double,int>>, greater<>> pq;

    dist[inicio] = 0;
    pq.push({0, inicio});

    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();

        if (d > dist[u]) continue;

        for (auto& a : grafo[u]) {
            double peso = 0;
            if (tipoPeso == 0) peso = a.custo;
            else if (tipoPeso == 1) peso = a.distancia;
            else peso = a.tempo;

            if (dist[u] + peso < dist[a.destino]) {
                dist[a.destino] = dist[u] + peso;
                prev[a.destino] = u;
                pq.push({dist[a.destino], a.destino});
            }
        }
    }
    return {dist, prev};
}

void somarCaminho(const vector<int>& prev, int destino, double& custo, double& distancia, double& tempo) {
    custo = distancia = tempo = 0;
    for (int v = destino; prev[v] != -1; v = prev[v]) {
        for (auto& a : grafo[prev[v]]) {
            if (a.destino == v) {
                custo += a.custo;
                distancia += a.distancia;
                tempo += a.tempo;
                break;
            }
        }
    }
}

vector<int> caminhoAte(const vector<int>& prev, int destino) {
    vector<int> caminho;
    for (int v = destino; v != -1; v = prev[v])
        caminho.push_back(v);
    reverse(caminho.begin(), caminho.end());
    return caminho;
}

void imprimirCaminho(vector<int>& prev, int destino) {
    if (prev[destino] == -1 && destino != 0) {
        cout << "Sem caminho";
        return;
    }
    vector<int> caminho;
    for (int v = destino; v != -1; v = prev[v])
        caminho.push_back(v);
    reverse(caminho.begin(), caminho.end());
    for (int i = 0; i < (int)caminho.size(); i++) {
        if (i) cout << " -> ";
        cout << cidades[caminho[i]];
    }
}

int pai[N], rank_[N];

int encontrar(int x) {
    if (pai[x] != x) pai[x] = encontrar(pai[x]);
    return pai[x];
}

bool unir(int x, int y) {
    int px = encontrar(x), py = encontrar(y);
    if (px == py) return false;
    if (rank_[px] < rank_[py]) swap(px, py);
    pai[py] = px;
    if (rank_[px] == rank_[py]) rank_[px]++;
    return true;
}

void kruskal() {
    vector<ArestaKruskal> arestas;
    for (int u = 0; u < N; u++) {
        for (auto& a : grafo[u]) {
            if (u < a.destino) {
                arestas.push_back({u, a.destino, a., a.distancia, a.tempo});
            }
        }
    }

    sort(arestas.begin(), arestas.end(), [](const ArestaKruskal& a, const ArestaKruskal& b) {
        return a. < b.;
    });

    for (int i = 0; i < N; i++) { pai[i] = i; rank_[i] = 0; }

    cout << "\n+======================================================+\n";
    cout << "|         ARVORE GERADORA MINIMA (Kruskal)             |\n";
    cout << "+======================================================+\n";
    cout << left << setw(20) << "Origem" << setw(20) << "Destino"
         << setw(12) << "(R$)" << setw(12) << "Dist(km)" << "Tempo(h)\n";
    cout << string(76, '-') << "\n";

    double total = 0, totalDist = 0, totalTempo = 0;
    int arestasAGM = 0;

    for (auto& a : arestas) {
        if (unir(a.origem, a.destino)) {
            cout << left << setw(20) << cidades[a.origem]
                 << setw(20) << cidades[a.destino]
                 << setw(12) << a.
                 << setw(12) << a.distancia
                 << a.tempo << "\n";
            total  += a.;
            totalDist   += a.distancia;
            totalTempo  += a.tempo;
            arestasAGM++;
            if (arestasAGM == N - 1) break;
        }
    }

    cout << string(76, '-') << "\n";
    cout << left << setw(40) << "TOTAIS DA AGM:"
         << setw(12) << total << setw(12) << totalDist << totalTempo << "\n";
}

void imprimirMatriz(int tipoPeso) {
    string nomes[] = {" (R$)", "DISTANCIA (km)", "TEMPO (horas)"};
    cout << "\n--- Matriz de " << nomes[tipoPeso] << " ---\n";
    cout << setw(12) << " ";
    for (int i = 0; i < N; i++) cout << setw(10) << cidades[i].substr(0,9);
    cout << "\n";

    double mat[N][N];
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            mat[i][j] = (i == j) ? 0 : -1;

    for (int u = 0; u < N; u++)
        for (auto& a : grafo[u]) {
            double val = (tipoPeso==0) ? a.custo : (tipoPeso==1) ? a.distancia : a.tempo;
            mat[u][a.destino] = val;
        }

    for (int i = 0; i < N; i++) {
        cout << setw(12) << cidades[i].substr(0,11);
        for (int j = 0; j < N; j++) {
            if (mat[i][j] < 0) cout << setw(10) << "---";
            else cout << setw(10) << mat[i][j];
        }
        cout << "\n";
    }
}

int main() {
    construirGrafo();

    cout << "+======================================================+\n";
    cout << "|           EUROTRIP - GRAFO DE DESLOCAMENTOS          |\n";
    cout << "|  Roteiro: Sao Paulo > Lisboa > Madrid > Paris        |\n";
    cout << "|           > Atenas > Roma > Amsterda > Berlim        |\n";
    cout << "+======================================================+\n";

    cout << "\n================ MATRIZES DE ADJACENCIA ================\n";
    imprimirMatriz(0);
    imprimirMatriz(1);
    imprimirMatriz(2);

    string labels[] = {"CUSTO (R$)", "DISTANCIA (km)", "TEMPO (horas)"};

    for (int tipo = 0; tipo < 3; tipo++) {
        auto [dist, prev] = dijkstra(0, tipo);

        cout << "\n============ DIJKSTRA - MENOR " << labels[tipo] << " ============\n";
        cout << "Origem: " << cidades[0] << "\n\n";
        cout << left << setw(12) << "Destino" << setw(12) << "Custo(R$)"
             << setw(12) << "Dist(km)" << setw(12) << "Tempo(h)" << "Caminho\n";
        cout << string(90, '-') << "\n";

        for (int i = 1; i < N; i++) {
            double c, d, h;
            somarCaminho(prev, i, c, d, h);
            cout << setw(12) << cidades[i] << setw(12) << c
                 << setw(12) << d << setw(12) << h;
            imprimirCaminho(prev, i);
            cout << "\n";
        }
    }

    kruskal();

    cout << "\n==================== CONCLUSAO =========================\n";
    cout << "Criterios analisados entre " << cidades[0] << " e " << cidades[N - 1] << " (destino final):\n\n";

    string nomes[] = {"CUSTO", "DISTANCIA", "TEMPO"};
    vector<vector<int>> prevs(3);
    double totais[3][3];
    for (int tipo = 0; tipo < 3; tipo++) {
        prevs[tipo] = dijkstra(0, tipo).second;
        somarCaminho(prevs[tipo], N - 1, totais[tipo][0], totais[tipo][1], totais[tipo][2]);
        cout << left << setw(20) << ("  Menor " + nomes[tipo] + ":")
             << "R$ " << totais[tipo][0] << " | " << totais[tipo][1] << " km | "
             << totais[tipo][2] << " h  ->  ";
        imprimirCaminho(prevs[tipo], N - 1);
        cout << "\n";
    }

    vector<int> c0 = caminhoAte(prevs[0], N - 1);
    vector<int> c1 = caminhoAte(prevs[1], N - 1);
    vector<int> c2 = caminhoAte(prevs[2], N - 1);

    if (c0 == c1 && c1 == c2) {
        cout << "\nOs tres criterios resultam no mesmo caminho.\n";
        cout << "Criterio mais vantajoso: qualquer um (rota unica).\n";
    } else if (c0 == c2) {
        cout << "\nO caminho mais barato tambem e o mais rapido.\n";
        cout << "Criterio mais vantajoso: CUSTO e TEMPO (mesma rota)\n";
    } else {
        double extraCusto = totais[2][0] - totais[0][0];
        double extraDist = totais[2][1] - totais[0][1];
        double horasPoupadas = totais[0][2] - totais[2][2];

        cout << "\nO caminho mais rapido difere do mais barato:\n";
        cout << "  Tempo poupado:   " << horasPoupadas << " h\n";
        cout << "  Custo adicional: R$ " << extraCusto << "\n";
        cout << "  Distancia extra: " << extraDist << " km\n";

        const double LIMITE_CUSTO_POR_HORA = 200;
        bool vantagemTempo = horasPoupadas > 0 && extraCusto / horasPoupadas <= LIMITE_CUSTO_POR_HORA;

        if (vantagemTempo) {
            cout << "\nCriterio mais vantajoso: TEMPO\n";
            cout << "Justificativa: cada hora poupada custa R$ " << extraCusto / horasPoupadas
                 << ", abaixo do limite de R$ " << LIMITE_CUSTO_POR_HORA << " por hora.\n";
        } else {
            cout << "\nCriterio mais vantajoso: CUSTO"
                 << (c0 == c1 ? " (mesmo caminho da menor DISTANCIA)" : "") << "\n";
            if (horasPoupadas > 0) {
                cout << "Justificativa: ganhar " << horasPoupadas << " h custaria R$ " << extraCusto
                     << " a mais (R$ " << extraCusto / horasPoupadas << " por hora poupada),\n";
                cout << "acima do limite de R$ " << LIMITE_CUSTO_POR_HORA << " por hora";
                if (extraDist > 0) cout << ", alem de " << extraDist << " km a mais";
                cout << ".\n";
            } else {
                cout << "Justificativa: o caminho mais barato ja e tao rapido quanto os demais.\n";
            }
        }
    }
    cout << "========================================================\n";

    return 0;
}
