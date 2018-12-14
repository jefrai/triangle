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
*/



/*
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
};

deque<int> dq; //deque
vector<int> ty, dt; //type, data
vector<pair<int, int> > ds; //descendants
map<char, int> om, ti; //operator-to-integer, term-to-integer map
vector<rat> rv; //rational value
vector<vector<pair<int, rat> > > ip; //inverted powers

inline void syr() {cout << "SYNTAX ERROR " << endl;/* exit(0);*/}

/*inline vector<pair<int, int> > prf(int x) { //prime factorization
    vector<pair<int, int> > v;
    if (x < 0) {v.push_back(make_pair(-1, 1)); x *= -1;}
    return v;
}*/

inline string rf(string s, string a, string b) {for (int i = 0; (i = s.find(a, i)) < s.size(); i += b.size()) s.replace(i, a.size(), b); return s;}

inline long long pw(long long i, long long p) {return !p ? 1 : ((p == 1) ? i : (pw(i * i, p / 2) * (p % 2 ? i : 1)));}
inline rat pw(rat i, long long p) {return rat(pw(i.p, p), pw(i.q, p));}

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

}

/*  TODO
    convert negative/?rational? powers
    output equation tree to singular
    ?implement user shorthand definitions
*/
int main() {
    ios_base::sync_with_stdio(0);
    int N, f, x, i, j;
    char zp[5] = {'+', '-', '*', '/', '^'}, zq[8] = {'a', 'b', 'c', 'A', 'B', 'C', 's', 'r'}; //operator-to-integer, term-to-integer
    int cp[5] = {1, 1, 2, 2, 4}; //condensation priority
    for (i = 0; i < 5; ++i) om[zp[i]] = i;
    for (i = 0; i < 8; ++i) ti[zq[i]] = i;
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
            if (i + 1 < N && s[i + 1] == '=') {dt.push_back(8); ++i;}
            else dt.push_back(6);
            ds.push_back(make_pair(-1, -1));
            dq.push_back(ty.size() - 1);
            continue;
        }
        if (s[i] == '>') {
            if (!cnd(0)) syr();
            ty.push_back(1);
            if (i + 1 < N && s[i + 1] == '=') {dt.push_back(9); ++i;}
            else dt.push_back(7);
            ds.push_back(make_pair(-1, -1));
            dq.push_back(ty.size() - 1);
            continue;
        }
        if (s[i] == '!') {
            if (!cnd(0)) syr();
            if (i + 1 < N && s[i + 1] == '=') {
                ty.push_back(1);
                dq.push_back(10);
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
    for (i = 0; i < ty.size(); ++i) printf("%d: TY %d DT %d DS %d %d\n", i, ty[i], dt[i], ds[i].first, ds[i].second);
    for (i = 0; i < dq.size(); ++i) printf("!!%d: %d\n", i, dq[i]);
    printf("\n");
    rv.assign(ty.size(), rat(0, 1));
    rd0(dq[0]);
    for (i = 0; i < ty.size(); ++i) printf("%d: TY %d DT %d DS %d %d RV %lld %lld\n", i, ty[i], dt[i], ds[i].first, ds[i].second, rv[i].p, rv[i].q);
    for (i = 0; i < dq.size(); ++i) printf("!!%d: %d\n", i, dq[i]);
    rd1(dq[0]);
    return 0;
}
