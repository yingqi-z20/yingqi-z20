#include <iostream>
#include <climits>
#include <list>
#include <queue>
#include <set>

static const int mMax = 20470;
static const int nMax = 10230;
using namespace std;

int n, m, en = 0;
struct Node;
struct Edge {
    int id;
    int in, out;
    int c, f;
    bool passed;
} e[mMax];
struct Point {
    int sign = 0;
    int sign_eid = 0;
    Node *belong = nullptr;
} p[nMax];

struct InfEdge {
    int id, in, out;

    bool operator<(InfEdge ie) const {
        return id < ie.id;
    }
};

struct Node {
    set<int> S;
    set<InfEdge> InfE;
    int d = 0, rd = 0, cd = 0;
    int next[mMax] = {0};
    Node *sign = nullptr;
    bool sign_direction = false;
    int sign_i = 0;
};

list<Node> G;
Node *s, *t;
list<int> RingOfNode;
list<int> RingOfPoint;

void Initialize();

void BuildWeb();

void Edmonds_Karp();

void Euler(Node *start, list<int>::iterator iter);

void Translate();

int main() {
    Initialize();
    BuildWeb();
    Edmonds_Karp();
    Node *start = &G.front();
    auto iter = RingOfNode.begin();

    for (auto &i : G)
        if (i.d != 0)
            start = &i;

    Euler(start, iter);
    Translate();
    cout << RingOfPoint.size() << endl;

    for (auto &i : RingOfPoint) {
        cout << i << ' ';
    }

    return 0;
}

void Initialize() {
    cin >> n >> m;
    auto *InfET = new InfEdge[mMax];
    int InfETn = 0;

    for (int i = 1, a, b, c; i <= m; ++i) {
        cin >> c >> a >> b;

        if (c == 3) {
            if (a == b)
                continue;

            InfET[InfETn] = {i, a, b};
            InfETn++;
        } else {
            en++;
            e[en].id = i;
            e[en].f = 0;
            e[en].passed = false;
            e[en].in = a;
            e[en].out = b;
            e[en].c = c - 1;
        }
    }

    Node *tmp;

    for (int i = 0, x, y; i < InfETn; ++i) {
        x = InfET[i].in;
        y = InfET[i].out;

        if (p[x].belong == nullptr) {
            if (p[y].belong == nullptr) {
                G.emplace_back();
                G.back().S.insert(x);
                G.back().S.insert(y);
                p[y].belong = &G.back();
            } else {
                (*p[y].belong).S.insert(x);
            }

            p[x].belong = p[y].belong;
        } else {
            if (p[y].belong == nullptr) {
                (*p[x].belong).S.insert(y);
                p[y].belong = p[x].belong;
            } else {
                tmp = p[y].belong;

                for (auto j : tmp->S)
                    p[j].belong = p[x].belong;

                (*p[x].belong).S.merge((*tmp).S);
                (*p[x].belong).InfE.merge((*tmp).InfE);

                G.remove_if([tmp](Node x) {
                    return &x == tmp;
                });
            }
        }

        p[x].belong->InfE.insert(InfET[i]);
    }

    delete[] InfET;

    for (int i = 1; i <= n; ++i) {
        if (p[i].belong == nullptr) {
            G.emplace_back();
            G.back().S.insert(i);
            p[i].belong = &G.back();
        }
    }

    for (int i = 1; i <= en; ++i) {
        tmp = p[e[i].in].belong;
        tmp->next[tmp->d] = i;
        tmp->d++;
        tmp->cd++;
        tmp = p[e[i].out].belong;
        tmp->next[tmp->d] = -i;
        tmp->d++;
        tmp->rd++;
    }
}

void BuildWeb() {
    G.emplace_back();
    s = &G.back();
    G.emplace_back();
    t = &G.back();
    s->S.insert(n + 1);
    p[n + 1].belong = s;
    t->S.insert(n + 2);
    p[n + 2].belong = t;
    m = en + 1;

    for (auto &i : G) {
        if (&i == s || &i == t)
            continue;

        if ((i.cd - i.rd) & 1) {
            cout << "NO";
            exit(0);
        }

        if (i.cd > i.rd) {
            e[m].in = n + 1;
            e[m].out = *i.S.begin();
            e[m].c = (i.cd - i.rd) / 2;
            e[m].f = 0;
            i.next[i.d] = -m;
            i.d++;
            s->next[s->d] = m;
            s->d++;
            m++;
        }

        if (i.rd > i.cd) {
            e[m].in = *i.S.begin();
            e[m].out = n + 2;
            e[m].c = (i.rd - i.cd) / 2;
            e[m].f = 0;
            i.next[i.d] = m;
            i.d++;
            t->next[s->d] = -m;
            t->d++;
            m++;
        }
    }
}

void Edmonds_Karp() {
    queue<Node *> Q;
    startEK:
    Node *u = s;
    Node *NX;

    do {

        for (int i = 0; i < u->d; ++i) {
            if (u->next[i] > 0 && e[u->next[i]].c > e[u->next[i]].f
                && (NX = p[e[u->next[i]].out].belong)->sign == nullptr) {
                NX->sign = u;
                NX->sign_direction = true;
                Q.push(NX);
                NX->sign_i = u->next[i];
                continue;
            }

            if (u->next[i] < 0 && e[-u->next[i]].f > 0
                && (NX = p[e[-u->next[i]].in].belong)->sign == nullptr) {
                NX->sign = u;
                NX->sign_direction = false;
                Q.push(NX);
                NX->sign_i = -u->next[i];
            }
        }

        u = nullptr;

        if (!Q.empty()) {
            u = Q.front();
            Q.pop();
        }
    } while (u != t && u != nullptr);

    if (u == t) {
        do {
            if (u->sign_direction) {
                e[u->sign_i].f++;
            } else {
                e[u->sign_i].f--;
            }

            u = u->sign;
        } while (u != s);

        for (auto &i : G)
            i.sign = nullptr;

        Q = std::queue<Node *>();
        goto startEK;
    }

    for (int i = 0; i < s->d; ++i) {
        if (e[s->next[i]].f != e[s->next[i]].c) {
            cout << "NO";
            exit(0);
        }
    }

    cout << "YES" << endl;
    G.pop_back();
    G.pop_back();

    for (auto &i : G) {
        i.d = i.rd + i.cd;
    }

    m = en;
}

void Euler(Node *start, list<int>::iterator iter) {
    for (int i = 0; i < start->rd + start->cd;) {
        if (start->next[i] > 0) {
            if (e[start->next[i]].f == 1) {
                i++;
                continue;
            }
        } else {
            if (e[-start->next[i]].f == 0) {
                i++;
                continue;
            }
        }

        if (!e[abs(start->next[i])].passed) {
            e[abs(start->next[i])].passed = true;
            RingOfNode.insert(iter, start->next[i]);
            start->d--;

            if (start->next[i] > 0)
                start = p[e[start->next[i]].out].belong;
            else
                start = p[e[-start->next[i]].in].belong;

            i = 0;
            m--;
            start->d--;
        } else
            i++;
    }

    if (m != 0) {
        for (iter = RingOfNode.begin(); iter != RingOfNode.end(); iter++) {
            if (*iter > 0 && p[e[*iter].in].belong->d != 0) {
                Euler(p[e[*iter].in].belong, iter);
                continue;
            }

            if (*iter < 0 && p[e[-*iter].out].belong->d != 0) {
                Euler(p[e[-*iter].out].belong, iter);
                continue;
            }
        }
    }
}

void Way(const int &a, const int &b);

void Translate() {
    if (RingOfNode.empty())
        return;

    int a, b;

    for (auto i = RingOfNode.begin(), end = --RingOfNode.end(); i != end; ++i) {
        if (*i > 0) {
            a = e[*i].out;

            if (*(++i) > 0)
                b = e[*i].in;
            else
                b = e[-*i].out;

            --i;
            RingOfPoint.push_back(e[*i].id);

            if (a != b)
                Way(a, b);
        } else {
            a = e[-*i].in;

            if (*(++i) > 0)
                b = e[*i].in;
            else
                b = e[-*i].out;

            --i;
            RingOfPoint.push_back(-e[-*i].id);

            if (a != b)
                Way(a, b);
        }
    }

    auto i = --RingOfNode.end();
    auto j = RingOfNode.begin();

    if (*i > 0) {
        a = e[*i].out;

        if (*j > 0)
            b = e[*j].in;
        else
            b = e[-*j].out;

        RingOfPoint.push_back(e[*i].id);

        if (a != b)
            Way(a, b);
    } else {
        a = e[-*i].in;

        if (*j > 0)
            b = e[*j].in;
        else
            b = e[-*j].out;

        RingOfPoint.push_back(-e[-*i].id);

        if (a != b)
            Way(a, b);
    }
}

void Way(const int &a, const int &b) {
    const Node &g = *p[a].belong;
    p[b].sign = INT_MAX;

    do {
        for (auto i : g.InfE) {
            if (p[i.in].sign == 0) {
                if (p[i.out].sign == 0)
                    continue;
                else {
                    p[i.in].sign = i.out;
                    p[i.in].sign_eid = i.id;

                    if (i.in == a)
                        goto found;
                }
            } else {
                if (p[i.out].sign == 0) {
                    p[i.out].sign = i.in;
                    p[i.out].sign_eid = -i.id;

                    if (i.out == a)
                        goto found;
                } else
                    continue;
            }
        }
    } while (true);

    found:
    int x = a;

    do {
        RingOfPoint.push_back(p[x].sign_eid);
        x = p[x].sign;
    } while (x != b);

    for (auto i : g.S) {
        p[i].sign = 0;
        p[i].sign_eid = 0;
    }
}