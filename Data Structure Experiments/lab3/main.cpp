#include <iostream>
#include <vector>
#include <queue>
#include <climits>

struct ENode {
    int adjVex;
    int weight;
    ENode* next;
};

struct LGraph {
    int n;
    int e;
    std::vector<ENode*> a;
};

void initGraph(LGraph &lg, int nSize) {
    lg.n = nSize;
    lg.e = 0;
    lg.a.resize(nSize, nullptr);
}

void insertEdge(LGraph &lg, int u, int v, int w) {
    ENode* newNode = new ENode{v, w, lg.a[u]};
    lg.a[u] = newNode;
}

std::vector<int> dijkstra(const LGraph &lg, int start) {
    std::vector<int> distances(lg.n, INT_MAX);
    distances[start] = 0;

    using Node = std::pair<int, int>;
    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> pq;
    pq.push({0, start});

    while (!pq.empty()) {
        int dist = pq.top().first;
        int current = pq.top().second;
        pq.pop();

        if (dist > distances[current]) continue;

        for (ENode* p = lg.a[current]; p != nullptr; p = p->next) {
            int next = p->adjVex;
            int weight = p->weight;
            if (distances[current] + weight < distances[next]) {
                distances[next] = distances[current] + weight;
                pq.push({distances[next], next});
            }
        }
    }

    return distances;
}

int main() {
    int n, m;
    std::cout << "输入顶点数(n)和边数(m): ";
    std::cin >> n >> m;

    LGraph graph;
    initGraph(graph, n);

    std::cout << "输入每条边 (起点 终点 权重):" << std::endl;
    for (int i = 0; i < m; ++i) {
        int u, v, w;
        std::cin >> u >> v >> w;
        insertEdge(graph, u, v, w);
        insertEdge(graph, v, u, w);
    }

    int start, end;
    std::cout << "输入起点和终点: ";
    std::cin >> start >> end;

    std::vector<int> distances = dijkstra(graph, start);

    if (distances[end] == INT_MAX) {
        std::cout << "从 " << start << " 到 " << end << " 没有路径。" << std::endl;
    } else {
        std::cout << "从 " << start << " 到 " << end << " 的最短路径长度是 " << distances[end] << "." << std::endl;
    }

    for (int i = 0; i < n; ++i) {
        ENode* p = graph.a[i];
        while (p) {
            ENode* temp = p;
            p = p->next;
            delete temp;
        }
    }

    return 0;
}
