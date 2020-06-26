#include <functional>
#include <vector>
#include <iostream>
#include <list>
using namespace std;
enum Directon { Up, Down, Left, Right, Origin };

struct node {
    vector<int> board;
    int f, g, h;
    int parent;
    vector<int> childs;
    Directon from;
    void setboard(Directon x, int a[]) {
        for (int i = 0; i < 9; ++i) {
            this->board.push_back(a[i]);
        }
        this->from = x;
    }
    void setboard(vector<int> b) {
        this->board = b;
    }
    void showBoard() {
        cout << "***************" << endl;
        for (int i = 0; i < 9; ++i)
            cout << this->board[i] << (i % 3 == 2 ? "\n" : " ");
        cout << "***************" << endl;
    }
    void setf(int g, int h) {
        this->g = g;
        this->h = h;
        this->f = g + h;
    }
    bool sameBoard(node& n, node& cmp) {
        for (int i = 0; i < 9; ++i)
            if (n.board[i] != cmp.board[i])
                return false;
        return true;
    }
};

struct QueNode {
    int gidx, f;
};

struct Openset {
    list<QueNode> set;
    bool cmp(QueNode& a, QueNode& b) {
        return a.f > b.f;
    }
    void print_queue(vector<node>& G) {
        cout << "openset:" << endl;
        for (auto i : this->set) {
            cout  << "Gidx :"<< i.gidx << ", f:"<< G[i.gidx].f << ", g:"<< G[i.gidx].g << ", h:" << G[i.gidx].h << endl;
            G[i.gidx].showBoard();
            cout << endl;
        }
    }
    void insert(node n, int idx, vector<node> &G) {
        bool in = false;
        for (auto &i : this->set) {
            if (G[i.gidx].sameBoard(G[i.gidx], n) && i.f >= n.f) {
                i.f = n.f;
                i.gidx = idx;
                in = true;
                break;
            }
        }
        if (!in) {
            QueNode a = {idx, n.f};
            set.push_back(a);
            set.sort([](QueNode& a, QueNode& b) { return (a.f < b.f); });
        }
    }
    bool empty() {
        return this->set.size() == 0;
    }
};

struct AStarAlgorithm {
    node start, target;
    vector<node> G;
    vector<int> closeset;
    Openset openset;
    void printSolution(int idx, int sum) {
        if (idx == (G.size() - 1))
            cout << "最佳路径为：";
        if (idx != 0) {
            printSolution(G[idx].parent, sum+1);
        }
        if (idx == 0)
            cout << sum << endl << "起点：" << endl;
        if (idx == (G.size() - 1)) {
            cout << "终点：" << endl;
        }
        G[idx].showBoard();
    }
    //获取逆序数, 0不能算一个数, 0只是一个没有棋子的空格
    int getInverseNum(vector<int> board) {
        int InverseNum = 0;
        for (auto i = 0; i < board.size(); i++) {
            if (board[i] == 0) continue;
            for (auto j = i + 1; j < board.size(); j++) {
                if (board[j] == 0) continue;
                if (board[i] > board[j]) InverseNum++;
            }
        }
        return InverseNum;
    }

    //判断是否可解，逆序数奇偶性相同则可解
    bool isSolveable(node& start, node& end) {
        int sn = getInverseNum(start.board);
        int en = getInverseNum(end.board);
        //cout << sn << ", " << en << endl;
        if ((sn % 2) == (en % 2))
            return true;
        return false;
    }

    bool done(node& n) {
        return n.sameBoard(n, this->target);
    }

    int inGraph(node& n) {
        for (int i = 0; i < G.size(); ++i)
            if (n.sameBoard(n, G[i])) return i;
        return -1;
    }
    int inCloseset(int idx) {
        return -1;
    }

    int calcG(node& f) {
        //g(x) = d(x),x节点的深度
        return f.g + 1;
    }
    int calcH(node& n) {
        // h(x) = w(x), 不在位棋子的个数
        int h = 0;
        for (int i = 0; i < 9; ++i)
            if (n.board[i] != 0 && n.board[i] != this->target.board[i])
                h++;
        return h;
    }
    int calcF(node& n, node& f) {
        n.setf(this->calcG(f), this->calcH(n));
        return n.f;
    }

    void input() {
        cout << "input the start chess board value" << endl;
        int s[] = { 4, 3, 6, 5, 7, 2, 8, 1, 0 };
        //int s[] = { 2, 8, 3, 1, 6, 4, 7, 0, 5 };
        int e[] = { 1, 2, 3, 4, 5, 6, 7, 8, 0 };
        //int e[] = { 1, 2, 3, 8, 0, 4, 7, 6, 5 };
        this->start.setboard(Origin, s);
        cout << "input the target chess board value" << endl;
        this->target.setboard(Origin, e);
        this->start.setf(0, calcH(this->start));
        this->target.setf(-1, 0);
        G.push_back(this->start);
        this->openset.insert(this->start, 0, G);
    }

    node move(int nodeidx, int nullidx, Directon nextplace) {
        node next;
        next.setboard(G[nodeidx].board);
        switch (nextplace)
        {
        case Up:
            next.board[nullidx] = next.board[nullidx + 3];
            next.board[nullidx + 3] = 0;
            next.from = Up;
            //cout << "move Up" << endl;
            break;
        case Down:
            next.board[nullidx] = next.board[nullidx - 3];
            next.board[nullidx - 3] = 0;
            next.from = Down;
            //cout << "move Down" << endl;
            break;
        case Left:
            next.board[nullidx] = next.board[nullidx + 1];
            next.board[nullidx + 1] = 0;
            next.from = Left;
            //cout << "move Left" << endl;
            break;
        case Right:
            next.board[nullidx] = next.board[nullidx - 1];
            next.board[nullidx - 1] = 0;
            next.from = Right;
            //cout << "move Right" << endl;
            break;
        default:
            break;
        }
        calcF(next, G[nodeidx]);
        next.parent = nodeidx;
        return next;
    }

    bool expandNode(int x) {
        int place = -1;
        int nexts[4] = { 0 };
        for (int i = 0; i < 9; ++i)
            if (G[x].board[i] == 0) {
                place = i;
                break;
            }
        if (place > 5) nexts[1] = 1;
        else if (place < 3) nexts[0] = 1;
        else {
            nexts[0] = 1;
            nexts[1] = 1;
        }
        if (place % 3 == 0) nexts[2] = 1;
        if ((place + 1) % 3 == 0) nexts[3] = 1;
        if ((place + 2) % 3 == 0) {
            nexts[2] = 1;
            nexts[3] = 1;
        }
        //from的反方向不能走，走就回头了
        switch (G[x].from)
        {
        case Up:
            nexts[1] = 0;
            break;
        case Down:
            nexts[0] = 0;
            break;
        case Left:
            nexts[3] = 0;
            break;
        case Right:
            nexts[2] = 0;
            break;
        default:
            break;
        }
        int i = 0;
        for (Directon d = Up; d < Origin; d = (Directon)(d + 1), ++i) {
            if (nexts[i] == 1) {
                node n = move(x, place, d);
                //如果拓展出的节点不是目标节点
                if (!done(n)) {
                    //n.showBoard();
                    int idx = inGraph(n);
                    //如果图G中没有这个节点，加入图G，加入openset
                    if (idx == -1) {
                        G.push_back(n);
                        G[x].childs.push_back(G.size() - 1);
                        openset.insert(n,int(G.size() - 1), G);
                    }
                    //如果已经在图G中,且新值小于旧值
                    else if (G[idx].f > n.f) {
                        //父节点不相同，调整相关指针
                        if (n.parent != G[idx].parent) {
                            //从原来此节点的父节点的子节点集合中，删掉原来这一个子节点
                            int found = -1;
                            for (int z = 0; z < G[G[idx].parent].childs.size(); ++z) {
                                if (G[G[idx].parent].childs[z] == idx) {
                                    found = z;
                                    break;
                                }
                            }
                            if (found > 0 && found < G[G[idx].parent].childs.size())
                                G[G[idx].parent].childs.erase(G[G[idx].parent].childs.begin() + found);
                            //在拓展源点的子节点集合中加入此索引
                            G[x].childs.push_back(idx);
                        }
                        G[idx] = n;         //用f值小的替换f值大的
                        //如果节点已经在closeset中，将其从closeset中删除
                        if (inCloseset(idx) >= 0) {
                            for (int z = 0; z < closeset.size(); ++z)
                                if (closeset[z] == idx) {
                                    closeset.erase(closeset.begin() + z);
                                    break;
                                }
                        }
                        //加入openset中
                        openset.insert(n, idx, G);
                    }
                }
                //如果拓展出的节点是目标节点，加入图G
                else {
                    G.push_back(n);
                    return true;
                }
            }
        }
        return false;
    }

    bool solve() {
        input();
        if (!isSolveable(this->start, this->target))
            return false;
        G.push_back(this->start);
        bool succ = false;
        while (!openset.empty() && !succ) {
            //从openset第一个节点开始拓展节点
            succ = expandNode(openset.set.begin()->gidx);
            //将openset第一个节点加入closeset
            closeset.push_back(openset.set.begin()->gidx);
            //将openset的第一个节点移除
            openset.set.pop_front();
        }
        //openset.print_queue(G);
        cout << endl;
        printSolution(G.size()-1, 0);
    }
};

int main() {
    AStarAlgorithm a;
    a.solve();
    return 0;
}
