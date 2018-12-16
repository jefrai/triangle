#include <bits/stdc++.h>
using namespace std;

/*  PARSE STEP - TYPE ARRAY
0   ADDITION
1   SUBTRACTION
2   MULTIPLICATION
3   DIVISION
4   EXPONENTIATION
5   EQUALITY
6   LESS OR EQUAL
7   GREATER OR EQUAL
8   LESS THAN
9   GREATER THAN
10  INEQUALITY
*/

/*  PARSE STEP - DATA ARRAY
0   QUERY
1   INEQUALITY OPERATOR
2   OPERATOR
3   EXPRESSION
4   INTEGER (PARSE)/RATIONAL EXPRESSIBLE (REDUCTION)
5   BASE TRIANGLE TERM
6   LEFT PARENTHESIS
7   RIGHT PARENTHESIS
*/

/*  PARSE STEP - TRIANGLE TERMS
0   a
1   b
2   c
3   sin A
4   sin B
5   sin C
6   s - semiperimeter
7   r - radius of circumcircle
8   S - area
*/



/*
S=(s(s-a)(s-b)(s-c))^(1/2)

(1/(a/(b/c)))^(1/5)<=((a+b)^(-3/7))^(-7/5)
1=cC+ab(5s+(a^(-3/4)+b^5)^2)^(-3)
-A
375
24bC
-3c
*/

struct rat{ //rational
    long long p, q;
    long long gcd(long long a, long long b) {long long c; while (b) c = b, b = a % b, a = c; return a;}
    void red() {long long g = gcd(abs(p), abs(q)) * (q < 0 ? -1 : 1); p /= g; q /= g;}
    rat() {}
    rat(long long a, long long b) {p = a; q = b; this->red();}
    rat operator=(long long v) {p = v; q = 1; return *this;}
    rat operator+(const rat &v) const {return rat(p * v.q + v.p * q, q * v.q);}
    rat operator+(long long v) const {return rat(p + v * q, q);}
    void operator+=(const rat &v) {*this = *this + v;}
    void operator+=(long long v) {*this = *this + v;}
    rat operator-(const rat &v) const {return rat(p * v.q - v.p * q, q * v.q);}
    rat operator-(long long v) const {return rat(p - v * q, q);}
    rat operator-() const {return rat(-p, q);}
    void operator-=(const rat &v) {*this = *this - v;}
    void operator-=(long long v) {*this = *this - v;}
    rat operator*(const rat &v) const {return rat(p * v.p, q * v.q);}
    rat operator*(long long v) const {return rat(p * v, q);}
    void operator*=(const rat &v) {*this = *this * v;}
    void operator*=(long long v) {*this = *this * v;}
    rat operator/(const rat &v) const {return rat(p * v.q, q * v.p);}
    rat operator/(long long v) const {return rat(p, v * q);}
    void operator/=(const rat &v) {*this = *this / v;}
    void operator/=(long long v) {*this = *this / v;}
    bool operator<(const rat &v) const {return (*this - v).p < 0;}
    bool operator<(long long v) const {return (*this - v).p < 0;}
    bool operator>(const rat &v) const {return (*this - v).p > 0;}
    bool operator>(long long v) const {return (*this - v).p > 0;}
    bool operator<=(const rat &v) const {return (*this - v).p <= 0;}
    bool operator<=(long long v) const {return (*this - v).p <= 0;}
    bool operator>=(const rat &v) const {return (*this - v).p >= 0;}
    bool operator>=(long long v) const {return (*this - v).p >= 0;}
    bool operator==(const rat &v) const {return !(*this - v).p;}
    bool operator==(long long v) const {return !(*this - v).p;}
    bool operator!=(const rat &v) const {return (*this - v).p;}
    bool operator!=(long long v) const {return (*this - v).p;}
    string str() {return q != 1 ? (to_string(p) + "/" + to_string(q)) : to_string(p);}
};

deque<int> dq; //deque
vector<int> ty, dt, ip; //type, data, inverted powers
vector<pair<int, int> > ds; //descendants
map<char, int> om, ti; //operator-to-integer, term-to-integer map
vector<rat> rv; //rational value
priority_queue<pair<int, string> > uf; //used variables
deque<string> pl; //polynomials

inline void syr() {cout << "SYNTAX ERROR " << endl;/* exit(0);*/}
inline string rf(string s, string a, string b) {for (int i = 0; (i = s.find(a, i)) < s.size(); i += b.size()) s.replace(i, a.size(), b); return s;}
inline long long pw(long long i, long long p) {return !p ? 1 : ((p == 1) ? i : (pw(i * i, p / 2) * (p % 2 ? i : 1)));}
inline rat pw(rat i, long long p) {return rat(pw(i.p, p), pw(i.q, p));}
inline int mp(int i, int j) {ty.push_back(3); dt.push_back(2); ds.push_back(make_pair(i, j)); ip.push_back(-1); rv.push_back(rat(0, 1)); return ty.size() - 1;}

inline bool inr(bool z) { //chooses interpretation of preceding items as integers or multiplication | performed before appending digits, terms or open parentheses
    if (dq.empty() || ty[dq.back()] < 3 || 5 < ty[dq.back()]) return 0;
    if (ty[dq.back()] == 4 && z) return 1;
    ty.push_back(2);
    dt.push_back(2);
    ds.push_back(make_pair(-1, -1));
    dq.push_back(ty.size() - 1);
    return 0;
}

inline bool cnd(int p) { //condense deque terms with precedence at least i | performed before appending operators or closing parentheses | open parenthesis - inequality - arithmetic - geometric - exponentiation
    int q, t, i;
    while (dq.size() > 2) {
        if (ty[dq[dq.size() - 2]] != 1 && ty[dq[dq.size() - 2]] != 2 && ty[dq[dq.size() - 2]] != 6) syr();
        if (ty[dq[dq.size() - 2]] == 6) break;
        else if (ty[dq[dq.size() - 2]] == 1) q = 0;
        else {
            if (dt[dq[dq.size() - 2]] < 2) q = 1;
            else if (dt[dq[dq.size() - 2]] < 4) q = 2;
            else q = 3;
        }
        if (q < p) break;
        if (ty[dq[dq.size() - 3]] < 3 || 5 < ty[dq[dq.size() - 3]]) break;
        if (ty[dq[dq.size() - 1]] < 3 || 5 < ty[dq[dq.size() - 1]]) break;
        ty.push_back(q ? 3 : 0);
        dt.push_back(dt[dq[dq.size() - 2]]);
        ds.push_back(make_pair(dq[dq.size() - 3], dq[dq.size() - 1]));
        for (i = 0; i < 3; ++i) dq.pop_back();
        dq.push_back(ty.size() - 1);
    }
    return !dq.empty() && 3 <= ty[dq.back()] && ty[dq.back()] <= 5;
}

void rd0(int i) { //simplification of rational expressions
    if (ds[i].first > -1 && ds[i].second > -1) {rd0(ds[i].first); rd0(ds[i].second);}
    if (ty[i] == 4) {rv[i] = dt[i]; return;}
    if (ty[i] == 3 && ty[ds[i].first] == 4 && ty[ds[i].second] == 4) {
        if (dt[i] == 0) rv[i] = rv[ds[i].first] + rv[ds[i].second];
        if (dt[i] == 1) rv[i] = rv[ds[i].first] - rv[ds[i].second];
        if (dt[i] == 2) rv[i] = rv[ds[i].first] * rv[ds[i].second];
        if (dt[i] == 3) rv[i] = rv[ds[i].first] / rv[ds[i].second];
        if (dt[i] == 4 && rv[ds[i].second].q == 1) rv[i] = pw(rv[ds[i].first], rv[ds[i].second].p);
        ty[i] = (dt[i] < 4 || rv[ds[i].second].q == 1) ? 4 : 3;
    }
}

void rd1(int i) { //propagation/elimination of negative powers
    //printf("?%d %d %d %d\n", i, ty[i], ds[i].first, ds[i].second);
    if (ty[i] == 4 || ty[i] == 5) return;
    rd1(ds[i].first);
    rd1(ds[i].second);
    int dl = ds[i].first, dr = ds[i].second; //original left and right descendants
    if (0 <= dt[i] && dt[i] <= 1) {
        if (ip[dr] > -1) {ds[i].first = mp(dl, ip[dr]); ip[i] = ip[dr];}
        if (ip[dl] > -1) {ds[i].second = mp(ip[dl], dr); ip[i] = ip[dl];}
        if (ip[dl] > -1 && ip[dr] > -1) ip[i] = mp(ip[dl], ip[dr]);
    }
    if (dt[i] == 3) {swap(ds[i].second, ip[ds[i].second]); dt[i] = 2;}
    if (dt[i] == 2) {
        if (ip[dl] > -1) ip[i] = ip[dl];
        if (ip[dr] > -1) ip[i] = ip[dr];
        if (ip[dl] > -1 && ip[dr] > -1) ip[i] = mp(ip[dl], ip[dr]);
    }
    if (dt[i] == 4 && ty[ds[i].second] != 4) syr();
    if (dt[i] == 4) ip[i] = ip[ds[i].first];
    if (dt[i] == 4 && rv[ds[i].second] < 0) {swap(ds[i].first, ip[i]); rv[ds[i].second] = -rv[ds[i].second];}
    if (dt[i] == 4) {
        ty.push_back(3);
        dt.push_back(4);
        ds.push_back(make_pair(ip[i], ds[i].second));
        ip.push_back(0);
        rv.push_back(rat(0, 1));
        ip[i] = ty.size() - 1;
    }
    if (5 <= dt[i]) {
        if (ip[dr] > -1) {ds[i].first = mp(dl, ip[dr]); ip[i] = ip[dr];}
        if (ip[dl] > -1) {ds[i].second = mp(ip[dl], dr); ip[i] = ip[dl];}
        if (ip[dl] > -1 && ip[dr] > -1) ip[i] = mp(ip[dl], ip[dr]);
    }
}

void rd2(int i, int o, bool s) { //cleaning up of identity operations
    if (ty[i] == 4 || ty[i] == 5) return;
    rd2(ds[i].first, i, 0);
    rd2(ds[i].second, i, 1);
    if (o < 0) return;
    int dl = ds[i].first, dr = ds[i].second;
    if (dt[i] == 0 && ty[dl] == 4 && rv[dl] == 0) {(s ? ds[o].second : ds[o].first) = dr; return;}
    if (dt[i] <= 1 && ty[dr] == 4 && rv[dr] == 0) {(s ? ds[o].second : ds[o].first) = dl; return;}
    if (dt[i] == 2 && ty[dl] == 4 && rv[dl] == 1) {(s ? ds[o].second : ds[o].first) = dr; return;}
    if (dt[i] == 2 && ty[dr] == 4 && rv[dr] == 1) {(s ? ds[o].second : ds[o].first) = dl; return;}
    if (dt[i] == 4 && ty[dl] == 4 && (rv[dl] == 0 || rv[dl] == 1 || rv[dr] == 1)) {(s ? ds[o].second : ds[o].first) = dl; return;}
}

void cmp(int i) { //compile tree into polynomials
    string zp[11] = {"+", "-", "*", "/", "^", "=", "<=", ">=", "<", ">", "!="}, zq[9] = {"a", "b", "c", "A", "B", "C", "s", "r", "S"};
    if (ty[i] == 4 || ty[i] == 5) return;
    cmp(ds[i].first);
    cmp(ds[i].second);
    int dl = ds[i].first, dr = ds[i].second;
    string l, r;
    l = (ty[dl] < 4 || 5 < ty[dl]) ? ("z(" + to_string(dl) + ")") : (ty[dl] == 4 ? rv[dl].str() : zq[dt[dl]]);
    r = (ty[dr] < 4 || 5 < ty[dr]) ? ("z(" + to_string(dr) + ")") : (ty[dr] == 4 ? rv[dr].str() : zq[dt[dr]]);
    if (ty[i] == 0) {
        if (dt[i] == 5) pl.push_front(l + "-" + r);
        if (dt[i] == 6) {
            uf.push(make_pair(50, "l"));
            pl.push_front(l+"-"+r+"+l^2");
        }
        if (dt[i] == 7) {
            uf.push(make_pair(50, "l"));
            pl.push_front(l+"-"+r+"-l^2");
        }
        if (dt[i] == 8) {
            uf.push(make_pair(50, "l"));
            uf.push(make_pair(50, "m"));
            pl.push_front("l*m-1");
            pl.push_front(l+"-"+r+"+l^2");
        }
        if (dt[i] == 9) {
            uf.push(make_pair(50, "l"));
            uf.push(make_pair(50, "m"));
            pl.push_front("l*m-1");
            pl.push_front(l+"-"+r+"-l^2");
        }
        if (dt[i] == 10) {
            uf.push(make_pair(50, "l"));
            uf.push(make_pair(50, "m"));
            pl.push_front("l*m-1");
            pl.push_front(l+"-"+r+"-l");
        }
        return;
    }
    if (dt[i] == 0) {
        uf.push(make_pair(100, "z(" + to_string(i) + ")"));
        pl.push_back("z(" + to_string(i) + ")-(" + l + "+" + r + ")");
    }
    if (dt[i] == 1) {
        uf.push(make_pair(100, "z(" + to_string(i) + ")"));
        pl.push_back("z(" + to_string(i) + ")-(" + l + "-" + r + ")");
    }
    if (dt[i] == 2) {
        uf.push(make_pair(100, "z(" + to_string(i) + ")"));
        pl.push_back("z(" + to_string(i) + ")-(" + l + "*" + r + ")");
    }
    if (dt[i] == 3) {
        uf.push(make_pair(80, "z(" + to_string(i) + ")"));
        pl.push_back("z(" + to_string(i) + ")*" + r + "-" + l);
    }
    if (dt[i] == 4) {
        uf.push(make_pair(0, "z(" + to_string(i) + ")"));
        if (rv[dr] > 0) {
            if (rv[dr].p == 1) pl.push_back("z(" + to_string(i) + ")^" + to_string(rv[dr].q) + "-" + l);
            else if (rv[dr].q == 1) pl.push_back("z(" + to_string(i) + ")-" + l + "^" + to_string(rv[dr].p));
            else pl.push_back("z(" + to_string(i) + ")^" + to_string(rv[dr].q) + "-" + l + "^" + to_string(rv[dr].p));
        } else {
            if (rv[dr].p == -1) pl.push_back("z(" + to_string(i) + ")^" + to_string(rv[dr].q) + "*" + l + "-1");
            else if (rv[dr].q == 1) pl.push_back("z(" + to_string(i) + ")*" + l + "^" + to_string(rv[dr].p) + "-1");
            else pl.push_back("z(" + to_string(i) + ")^" + to_string(rv[dr].q) + "*" + l + "^" + to_string(rv[dr].p) + "-1");
        }
    }
}

string crt(int i) {
    string zp[11] = {"+", "-", "*", "/", "^", "=", "<=", ">=", "<", ">", "!="}, zq[9] = {"a", "b", "c", "A", "B", "C", "s", "r", "S"};
    if (ty[i] == 4) return "(" + to_string(rv[i].p) + "/" + to_string(rv[i].q) + ")";
    if (ty[i] == 5) return zq[dt[i]];
    if (ty[i] == 0 || ty[i] == 3) return "(" + crt(ds[i].first) + zp[dt[i]] + crt(ds[i].second) + ")";
}

/*  TODO
    debugging
    ?implement user shorthand definitions
    ?decimal point input support
*/
int main() {
    //freopen("out.txt", "w", stdout);
    ios_base::sync_with_stdio(0);
    int N, f, x, i, j;
    char zp[5] = {'+', '-', '*', '/', '^'}, zq[9] = {'a', 'b', 'c', 'A', 'B', 'C', 's', 'r', 'S'}; //operator-to-integer, term-to-integer
    int cp[5] = {1, 1, 2, 2, 4}; //condensation priority
    for (i = 0; i < 5; ++i) om[zp[i]] = i;
    for (i = 0; i < 9; ++i) ti[zq[i]] = i;
    ty.push_back(4);
    dt.push_back(1);
    ds.push_back(make_pair(-1, -1));
    string s;
    getline(cin, s);
    N = s.size();
    for (i = 0; i < N; ++i) {
        if (s[i] == '=') {
            if (!cnd(0)) syr();
            ty.push_back(1);
            dt.push_back(5);
            ds.push_back(make_pair(-1, -1));
            dq.push_back(ty.size() - 1);
        }
        if (s[i] == '<') {
            if (!cnd(0)) syr();
            ty.push_back(1);
            if (i + 1 < N && s[i + 1] == '=') {dt.push_back(6); ++i;}
            else dt.push_back(8);
            ds.push_back(make_pair(-1, -1));
            dq.push_back(ty.size() - 1);
            continue;
        }
        if (s[i] == '>') {
            if (!cnd(0)) syr();
            ty.push_back(1);
            if (i + 1 < N && s[i + 1] == '=') {dt.push_back(7); ++i;}
            else dt.push_back(9);
            ds.push_back(make_pair(-1, -1));
            dq.push_back(ty.size() - 1);
            continue;
        }
        if (s[i] == '!') {
            if (!cnd(0)) syr();
            if (i + 1 < N && s[i + 1] == '=') {
                ty.push_back(1);
                dt.push_back(10);
                ds.push_back(make_pair(-1, -1));
                dq.push_back(ty.size() - 1);
                ++i;
                continue;
            } else syr();
        }
        if (s[i] == '(') {
            inr(0);
            ty.push_back(6);
            dt.push_back(0);
            ds.push_back(make_pair(-1, -1));
            dq.push_back(ty.size() - 1);
        }
        if (s[i] == ')') {
            if (!cnd(0)) syr();
            if (dq.size() < 2 || ty[dq[dq.size() - 2]] != 6) syr();
            x = dq.back();
            for (j = 0; j < 2; ++j) dq.pop_back();
            dq.push_back(x);
            ty.push_back(7);
            dt.push_back(0);
            ds.push_back(make_pair(-1, -1));
        }
        if ('0' <= s[i] && s[i] <= '9') {
            if (inr(1)) {dt.back() *= 10; dt.back() += s[i] - '0';}
            else {
                ty.push_back(4);
                dt.push_back(s[i] - '0');
                ds.push_back(make_pair(-1, -1));
                dq.push_back(ty.size() - 1);
            }
        }
        if (om.find(s[i]) != om.end()) {
            if (cnd(cp[om[s[i]]])) {
                ty.push_back(2);
                dt.push_back(om[s[i]]);
                ds.push_back(make_pair(-1, -1));
                dq.push_back(ty.size() - 1);
            } else if (s[i] == '-') {
                ty.push_back(4);
                dt.push_back(-1);
                ds.push_back(make_pair(-1, -1));
                dq.push_back(ty.size() - 1);
                ty.push_back(2);
                dt.push_back(2);
                ds.push_back(make_pair(-1, -1));
                dq.push_back(ty.size() - 1);
            } else syr();
        }
        if (ti.find(s[i]) != ti.end()) {
            inr(0);
            ty.push_back(5);
            dt.push_back(ti[s[i]]);
            ds.push_back(make_pair(-1, -1));
            dq.push_back(ty.size() - 1);
        }
    }
    cnd(0);
    if (dq.size() > 1) syr();
    rv.assign(ty.size(), rat(0, 1));
    rv[0] = 1;
    rd0(dq[0]);
    ip.assign(ty.size(), 0);
    //rd1(dq[0]);
    rd2(dq[0], -1, 0);
    /*for (i = 0; i < ty.size(); ++i) printf("%d: TY %d DT %d DS %d %d RV %lld %lld\n", i, ty[i], dt[i], ds[i].first, ds[i].second, rv[i].p, rv[i].q);
    for (i = 0; i < dq.size(); ++i) printf("!!%d: %d\n", i, dq[i]);
    printf("\n");
    cout << crt(dq[0]) << endl;
    cout << endl;*/
    cmp(dq[0]);
    cout << "ring r = 0, (s, S, ";
    while (!uf.empty()) {cout << uf.top().second + ", "; uf.pop();}
    cout << "r, a, b, c, A, B, C, v, w, x, y, i, j, k), dp;" << endl;
    cout << endl;
    cout << "poly t0 = a^2-((i-j)^2+k^2);" << endl;
    cout << "poly t1 = b^2-(i^2+k^2);" << endl;
    cout << "poly t2 = c^2-i^2;" << endl;
    cout << "poly t3 = s-(a+b+c)/2;" << endl;
    cout << "poly t4 = S-ik/2;" << endl;
    cout << "poly t5 = 4rS-abc;" << endl;
    cout << "poly t6 = 2rA-a;" << endl;
    cout << "poly t7 = 2rB-b;" << endl;
    cout << "poly t8 = 2rC-c;" << endl;
    cout << "poly c0 = a-w^2;" << endl;
    cout << "poly c1 = b-x^2;" << endl;
    cout << "poly c2 = c-y^2;" << endl;
    cout << "poly c3 = S-v^2;" << endl;
    for (i = pl.size() - 1; i; --i) cout << "poly p" + to_string(pl.size() - i - 1) + " = " + pl[i] << ";" << endl;
    cout << "poly rs = " + pl[0] + ";" << endl;
    cout << endl;
    cout << "ideal I = t0, t1, t2, t3, t4, t5, t6, t7, t8, c0, c1, c2, c3";
    for (i = 0; i < pl.size() - 1; ++i) cout << ", p" + to_string(i);
    cout << ";" << endl;
    cout << "I = groebner(I);" << endl;
    cout << "reduce(rs, I);" << endl;
    return 0;
}
