#include <bits/stdc++.h>
using namespace std;

/*  INIT
0   PT      NEW POINT
1   FT      FINITE LOCUS CONSTRAINT
2   LN      LINE LOCUS CONSTRAINT
3   RD      CIRCLE/RADIUS LOCUS CONSTRAINT
4   MS      MISC.
5   NEG     NEGATED CONSTRAINT
6   QR      QUERY
*/

/*  0 - NEW POINT
0   NEW     CREATE POINT (IDENTIFIER)
1   NEC     CREATE POINT (IDENTIFIER, COORDINATE IDENTIFIERS)
*/

/*  1 - FINITE LOCUS CONSTRAINT
0   ILL     INTERSECTION LINE-LINE (2, 2)
1   ILC     INTERSECTION LINE-CIRCLE (2, 2)
2   ILI     INTERSECTION LINE-CIRCLE IMMEDIATE (2, 1, 1I)
3   ICC     INTERSECTION CIRCLE-CIRCLE (2, 2)
4   ICI     INTERSECTION CIRCLE-CIRCLE IMMEDIATE (2, 1, 1I)
5   ICD     INTERSECTION CIRCLE-CIRCLE DIIMMEIDATE (1, 1I, 1, 1I)
6   MID     MIDPOINT (2)
7   PROJ    PROJECTION ONTO LINE (1, 2)
8   MIR     MIRROR OVER LINE (1, 2)
9   TAN     TANGENT (3) SOURCE-CENTRE-RADIUS
10  TANI    TANGENT IMMEDIATE (2, 1I)
?   (CENTERS)
*/

/*  2 - LINE LOCUS CONSTRAINT
0   COL     COLLINEAR (2)
1   COLI    COLLINEAR IMMEDIATE (3I)
2   PARA    PARALLEL (1, 2)
3   PER     PERPENDICULAR (1, 2)
4   PBIS    PERPENDICULAR BISECTOR (2)
5   ABIS    ANGLE BISECTOR (1, 2)
*/

/*  3 - CIRCLE/RADIUS LOCUS CONSTRAINT
0   RAD     RADIUS (1, 2) CENTRE, RADIUS LENGTH
1   RADI    RADIUS IMMEDIATE (1, 1I)
2   CONC    CONCYCLIC (3)
*/

/*  4 - OTHER
0   EQ     EQUAL POINTS (1)
1   NEQ     NONEQUAL POINTS (1)
2   ORI     POINT AS ORIGIN (0)
3   AXI     POINT ON X-AXIS (0)
*/

struct rat{ //rational
    long long p, q;
    long long gcd(long long a, long long b) {long long c; while (b) c = b, b = a % b, a = c; return a;}
    void red() {long long g = gcd(abs(p), abs(q)) * (q < 0 ? -1 : 1); p /= g; q /= g;}
    rat() {}
    rat(long long i) {p = i; q = 1;}
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

int nfc, npm, ori = -1, axi = -1; //number of coordinates, number of internal negation points, (0, 0) point, (x, 0) point
set<char> apn; //alphanumeric characters
vector<string> v; //vector
vector<int> pv; //priority vector
map<string, int> mpi, mci; //map from point identifier to index
vector<pair<int, int> > pcd; //coordinate indices of point
deque<string> pl; //polynomials

inline string rs(int i, bool d) {return "w(" + to_string(d ? pcd[i].second : pcd[i].first) + ")";}

void create(vector<string> f) {
    mpi[f[1]] = pcd.size();
    if (f[0] == "new") {
        pcd.push_back(make_pair(nfc, nfc + 1));
        nfc += 2;
    }
    if (f[0] == "nec") {
        if (mci.find(f[2]) != mci.end() || mci.find(f[3]) != mci.end()) {cout << "Invalid" << endl; return;}
        mci[f[2]] = nfc;
        mci[f[3]] = nfc + 1;
        pcd.push_back(make_pair(nfc, nfc + 1));
        nfc += 2;
    }
}

void finite(vector<string> f) {
    int r[5], i, j, k, l;
    if (f[0] == "ill") {
        for (i = 0; i < 5; ++i) {if (mpi.find(f[i + 1]) == mpi.end()) {cout << "Invalid" << endl; return;} r[i] = mpi[f[i + 1]];}
        pl.push_back(rs(r[0], 0) + " * ((" + rs(r[1], 0) + " - " + rs(r[2], 0) + ") * (" + rs(r[3], 1) + " - " + rs(r[4], 1) + ") - (" + rs(r[1], 1) + " - " + rs(r[2], 1) + ") * (" + rs(r[3], 0) + " - " + rs(r[4], 0) + ")) - (");
        pl.back() += "(" + rs(r[1], 0) + " * " + rs(r[2], 2) + " - " + rs(r[1], 1) + " * " + rs(r[2], 0) + ") * (" + rs(r[3], 0) + " - " + rs(r[4], 0) + ") - ";
        pl.back() += "(" + rs(r[1], 0) + " - " + rs(r[2], 0) + ") * (" + rs(r[3], 0) + " * " + rs(r[4], 2) + " - " + rs(r[3], 1) + " * " + rs(r[4], 0) + "))";
        pl.push_back(rs(r[0], 1) + " * ((" + rs(r[1], 0) + " - " + rs(r[2], 0) + ") * (" + rs(r[3], 1) + " - " + rs(r[4], 1) + ") - (" + rs(r[1], 1) + " - " + rs(r[2], 1) + ") * (" + rs(r[3], 0) + " - " + rs(r[4], 0) + ")) - (");
        pl.back() += "(" + rs(r[1], 0) + " * " + rs(r[2], 2) + " - " + rs(r[1], 1) + " * " + rs(r[2], 0) + ") * (" + rs(r[3], 1) + " - " + rs(r[4], 1) + ") - ";
        pl.back() += "(" + rs(r[1], 1) + " - " + rs(r[2], 1) + ") * (" + rs(r[3], 0) + " * " + rs(r[4], 2) + " - " + rs(r[3], 1) + " * " + rs(r[4], 0) + "))";
    }
    if (f[0] == "ilc") {
        for (i = 0; i < 5; ++i) {if (mpi.find(f[i + 1]) == mpi.end()) {cout << "Invalid" << endl; return;} r[i] = mpi[f[i + 1]];}
        pl.push_back("(" + rs(r[0], 1) + " - " + rs(r[1], 1) + ") * (" + rs(r[2], 0) + " - " + rs(r[1], 0) + ") - (" + rs(r[2], 1) + " - " + rs(r[1], 1) + ") * (" + rs(r[0], 0) + " - " + rs(r[1], 0) + ")");
        pl.push_back("(" + rs(r[0], 0) + " - " + rs(r[3], 0) + ")^2 + (" + rs(r[0], 1) + " - " + rs(r[3], 1) + ")^2 - (" + rs(r[4], 0) + " - " + rs(r[3], 0) + ")^2 - (" + rs(r[4], 1) + " - " + rs(r[3], 1) + ")^2");
    }
    if (f[0] == "ili") {
        for (i = 0; i < 4; ++i) {if (mpi.find(f[i + 1]) == mpi.end()) {cout << "Invalid" << endl; return;} r[i] = mpi[f[i + 1]];}
        rat cf;
        for (i = j = k = 0; i < f[5].size(); ++i) {
            if (!i && f[5][i] == '-') k = 1;
            else if ('0' <= f[5][i] && f[5][i] <= '9') cf *= 10, cf += f[5][i] - '0';
            else if (i && i < f[5].size() - 1 && f[5][i] == '/') {cf = rat(1) / cf; j = 1;}
            else {cout << "Invalid" << endl; return;}
        }
        if (j) cf = rat(1) / cf;
        if (k) cf = -cf;
        pl.push_back("(" + rs(r[0], 1) + " - " + rs(r[1], 1) + ") * (" + rs(r[2], 0) + " - " + rs(r[1], 0) + ") - (" + rs(r[2], 1) + " - " + rs(r[1], 1) + ") * (" + rs(r[0], 0) + " - " + rs(r[1], 0) + ")");
        pl.push_back("(" + rs(r[0], 0) + " - " + rs(r[3], 0) + ")^2 + (" + rs(r[0], 1) + " - " + rs(r[3], 1) + ")^2 - (" + cf.str() + ")^2");
    }
    if (f[0] == "icc") {
        for (i = 0; i < 5; ++i) {if (mpi.find(f[i + 1]) == mpi.end()) {cout << "Invalid" << endl; return;} r[i] = mpi[f[i + 1]];}
        pl.push_back("(" + rs(r[0], 0) + " - " + rs(r[1], 0) + ")^2 + (" + rs(r[0], 1) + " - " + rs(r[1], 1) + ")^2 - (" + rs(r[2], 0) + " - " + rs(r[1], 0) + ")^2 - (" + rs(r[2], 1) + " - " + rs(r[1], 1) + ")^2");
        pl.push_back("(" + rs(r[0], 0) + " - " + rs(r[3], 0) + ")^2 + (" + rs(r[0], 1) + " - " + rs(r[3], 1) + ")^2 - (" + rs(r[4], 0) + " - " + rs(r[3], 0) + ")^2 - (" + rs(r[4], 1) + " - " + rs(r[3], 1) + ")^2");
    }
    if (f[0] == "ici") {
        for (i = 0; i < 4; ++i) {if (mpi.find(f[i + 1]) == mpi.end()) {cout << "Invalid" << endl; return;} r[i] = mpi[f[i + 1]];}
        rat cf;
        for (i = j = k = 0; i < f[5].size(); ++i) {
            if (!i && f[5][i] == '-') k = 1;
            else if ('0' <= f[5][i] && f[5][i] <= '9') cf *= 10, cf += f[5][i] - '0';
            else if (i && i < f[5].size() - 1 && f[5][i] == '/') {cf = rat(1) / cf; j = 1;}
            else {cout << "Invalid" << endl; return;}
        }
        if (j) cf = rat(1) / cf;
        if (k) cf = -cf;
        pl.push_back("(" + rs(r[0], 0) + " - " + rs(r[1], 0) + ")^2 + (" + rs(r[0], 1) + " - " + rs(r[1], 1) + ")^2 - (" + rs(r[2], 0) + " - " + rs(r[1], 0) + ")^2 - (" + rs(r[2], 1) + " - " + rs(r[1], 1) + ")^2");
        pl.push_back("(" + rs(r[0], 0) + " - " + rs(r[3], 0) + ")^2 + (" + rs(r[0], 1) + " - " + rs(r[3], 1) + ")^2 - (" + cf.str() + ")^2");
    }
    if (f[0] == "icd") {
        for (i = 0; i < 3; ++i) {if (mpi.find(f[i + 1]) == mpi.end()) {cout << "Invalid" << endl; return;} r[i] = mpi[f[i + (i > 1) + 1]];}
        rat cf[2];
        for (i = 0; i < 3; ++i) {
            for (j = k = l = 0; j < f[i - !i + 4].size(); ++j) {
                if (!j && f[i - !i + 4][j] == '-') l = 1;
                else if ('0' <= f[i - !i + 4][j] && f[i - !i + 4][j] <= '9') cf[i] *= 10, cf[i] += f[i - !i + 4][j] - '0';
                else if (j && j < f[i - !i + 4].size() - 1 && f[i - !i + 4][j] == '/') {cf[i] = rat(1) / cf[i]; k = 1;}
                else {cout << "Invalid" << endl; return;}
            }
            if (k) cf[i] = rat(1) / cf[i];
            if (l) cf[i] = -cf[i];
        }
        pl.push_back("(" + rs(r[0], 0) + " - " + rs(r[1], 0) + ")^2 + (" + rs(r[0], 1) + " - " + rs(r[1], 1) + ")^2 - (" + cf[0].str() + ")^2");
        pl.push_back("(" + rs(r[0], 0) + " - " + rs(r[3], 0) + ")^2 + (" + rs(r[0], 1) + " - " + rs(r[3], 1) + ")^2 - (" + cf[1].str() + ")^2");
    }
    if (f[0] == "mid") {
        for (i = 0; i < 3; ++i) {if (mpi.find(f[i + 1]) == mpi.end()) {cout << "Invalid" << endl; return;} r[i] = mpi[f[i + 1]];}
        pl.push_back("2 * " + rs(r[0], 0) + " - " + rs(r[1], 0) + " - " + rs(r[2], 0));
        pl.push_back("2 * " + rs(r[0], 1) + " - " + rs(r[1], 1) + " - " + rs(r[2], 1));
    }
    if (f[0] == "proj") {
        for (i = 0; i < 4; ++i) {if (mpi.find(f[i + 1]) == mpi.end()) {cout << "Invalid" << endl; return;} r[i] = mpi[f[i + 1]];}
        pl.push_back("(" + rs(r[0], 0) + " - " + rs(r[2], 0) + ") * ((" + rs(r[3], 0) + " - " + rs(r[2], 0) + ")^2 + (" + rs(r[3], 1) + " - " + rs(r[2], 1) + ")^2) - ");
        pl.back() += "(" + rs(r[3], 0) + " - " + rs(r[2], 0) + ") * ((" + rs(r[1], 0) + " - " + rs(r[2], 0) + ") * (" + rs(r[3], 0) + " - " + rs(r[2], 0) + ") + (" + rs(r[1], 1) + " - " + rs(r[2], 1) + ") * (" + rs(r[3], 1) + " - " + rs(r[2], 1) + "))";
        pl.push_back("(" + rs(r[0], 1) + " - " + rs(r[2], 1) + ") * ((" + rs(r[3], 1) + " - " + rs(r[2], 1) + ")^2 + (" + rs(r[3], 0) + " - " + rs(r[2], 0) + ")^2) - ");
        pl.back() += "(" + rs(r[3], 1) + " - " + rs(r[2], 1) + ") * ((" + rs(r[1], 1) + " - " + rs(r[2], 1) + ") * (" + rs(r[3], 1) + " - " + rs(r[2], 1) + ") + (" + rs(r[1], 0) + " - " + rs(r[2], 0) + ") * (" + rs(r[3], 0) + " - " + rs(r[2], 0) + "))";
    }
    if (f[0] == "mir") {
        for (i = 0; i < 4; ++i) {if (mpi.find(f[i + 1]) == mpi.end()) {cout << "Invalid" << endl; return;} r[i] = mpi[f[i + 1]];}
        pl.push_back("((" + rs(r[0], 0) + " + " + rs(r[1], 0) + ") / 2 - " + rs(r[2], 0) + ") * ((" + rs(r[3], 0) + " - " + rs(r[2], 0) + ")^2 + (" + rs(r[3], 1) + " - " + rs(r[2], 1) + ")^2) - ");
        pl.back() += "(" + rs(r[3], 0) + " - " + rs(r[2], 0) + ") * ((" + rs(r[1], 0) + " - " + rs(r[2], 0) + ") * (" + rs(r[3], 0) + " - " + rs(r[2], 0) + ") + (" + rs(r[1], 1) + " - " + rs(r[2], 1) + ") * (" + rs(r[3], 1) + " - " + rs(r[2], 1) + "))";
        pl.push_back("((" + rs(r[0], 1) + " + " + rs(r[1], 1) + ") / 2 - " + rs(r[2], 1) + ") * ((" + rs(r[3], 1) + " - " + rs(r[2], 1) + ")^2 + (" + rs(r[3], 0) + " - " + rs(r[2], 0) + ")^2) - ");
        pl.back() += "(" + rs(r[3], 1) + " - " + rs(r[2], 1) + ") * ((" + rs(r[1], 1) + " - " + rs(r[2], 1) + ") * (" + rs(r[3], 1) + " - " + rs(r[2], 1) + ") + (" + rs(r[1], 0) + " - " + rs(r[2], 0) + ") * (" + rs(r[3], 0) + " - " + rs(r[2], 0) + "))";
    }
    if (f[0] == "tan") {
        for (i = 0; i < 4; ++i) {if (mpi.find(f[i + 1]) == mpi.end()) {cout << "Invalid" << endl; return;} r[i] = mpi[f[i + 1]];}
        pl.push_back("(" + rs(r[0], 0) + " - " + rs(r[2], 0) + ")^2 + (" + rs(r[0], 1) + " - " + rs(r[2], 1) + ")^2 - (" + rs(r[3], 0) + " - " + rs(r[2], 0) + ")^2 + (" + rs(r[3], 1) + " - " + rs(r[2], 1) + ")^2");
        pl.push_back("(" + rs(r[0], 1) + " - " + rs(r[1], 1) + ") * (" + rs(r[0], 1) + " - " + rs(r[2], 1) + ") + (" + rs(r[0], 0) + " - " + rs(r[1], 0) + ") * (" + rs(r[0], 0) + " - " + rs(r[2], 0) + ")");
    }
    if (f[0] == "tani") {
        for (i = 0; i < 3; ++i) {if (mpi.find(f[i + 1]) == mpi.end()) {cout << "Invalid" << endl; return;} r[i] = mpi[f[i + 1]];}
        rat cf;
        for (i = j = k = 0; i < f[4].size(); ++i) {
            if (!i && f[4][i] == '-') k = 1;
            else if ('0' <= f[4][i] && f[4][i] <= '9') cf *= 10, cf += f[4][i] - '0';
            else if (i && i < f[4].size() - 1 && f[4][i] == '/') {cf = rat(1) / cf; j = 1;}
            else {cout << "Invalid" << endl; return;}
        }
        pl.push_back("(" + rs(r[0], 0) + " - " + rs(r[2], 0) + ")^2 + (" + rs(r[0], 1) + " - " + rs(r[2], 1) + ")^2 - (" + cf.str() + ")^2");
        pl.push_back("(" + rs(r[0], 1) + " - " + rs(r[1], 1) + ") * (" + rs(r[0], 1) + " - " + rs(r[2], 1) + ") + (" + rs(r[0], 0) + " - " + rs(r[1], 0) + ") * (" + rs(r[0], 0) + " - " + rs(r[2], 0) + ")");
    }
}

void linear(vector<string> f) {
    int r[4], i, j, k, l;
    if (f[0] == "col") {
        for (i = 0; i < 3; ++i) {if (mpi.find(f[i + 1]) == mpi.end()) {cout << "Invalid" << endl; return;} r[i] = mpi[f[i + 1]];}
        pl.push_back("(" + rs(r[0], 1) + " - " + rs(r[1], 1) + ") * (" + rs(r[2], 0) + " - " + rs(r[1], 0) + ") - (" + rs(r[2], 1) + " - " + rs(r[1], 1) + ") * (" + rs(r[0], 0) + " - " + rs(r[1], 0) + ")");
    }
    if (f[0] == "coli") {
        if (mpi.find(f[1]) == mpi.end()) {cout << "Invalid" << endl; return;}
        r[0] = mpi[f[1]];
        rat cf[3];
        for (i = 0; i < 3; ++i) {
            for (j = k = l = 0; j < f[i + 2].size(); ++j) {
                if (!j && f[i + 2][j] == '-') l = 1;
                else if ('0' <= f[i + 2][j] && f[i + 2][j] <= '9') cf[i] *= 10, cf[i] += f[i + 2][j] - '0';
                else if (j && j < f[i + 2].size() - 1 && f[i + 2][j] == '/') {cf[i] = rat(1) / cf[i]; k = 1;}
                else {cout << "Invalid" << endl; return;}
            }
            if (k) cf[i] = rat(1) / cf[i];
            if (l) cf[i] = -cf[i];
        }
        pl.push_back(cf[0].str() + rs(r[0], 0) + " + " + cf[1].str() + rs(r[0], 1) + " + " + cf[2].str());
    }
    if (f[0] == "para") {
        for (i = 0; i < 4; ++i) {if (mpi.find(f[i + 1]) == mpi.end()) {cout << "Invalid" << endl; return;} r[i] = mpi[f[i + 1]];}
        pl.push_back("(" + rs(r[0], 1) + " - " + rs(r[1], 1) + ") * (" + rs(r[3], 0) + " - " + rs(r[2], 0) + ") - (" + rs(r[3], 1) + " - " + rs(r[2], 1) + ") * (" + rs(r[0], 0) + " - " + rs(r[1], 0) + ")");
    }
    if (f[0] == "per") {
        for (i = 0; i < 4; ++i) {if (mpi.find(f[i + 1]) == mpi.end()) {cout << "Invalid" << endl; return;} r[i] = mpi[f[i + 1]];}
        pl.push_back("(" + rs(r[0], 1) + " - " + rs(r[1], 1) + ") * (" + rs(r[3], 1) + " - " + rs(r[2], 1) + ") + (" + rs(r[0], 0) + " - " + rs(r[1], 0) + ") * (" + rs(r[3], 0) + " - " + rs(r[2], 0) + ")");
    }
    if (f[0] == "pbis") {
        for (i = 0; i < 3; ++i) {if (mpi.find(f[i + 1]) == mpi.end()) {cout << "Invalid" << endl; return;} r[i] = mpi[f[i + 1]];}
        pl.push_back("(2 * " + rs(r[0], 1) + " - " + rs(r[1], 1) + " - " + rs(r[2], 1) + ") * (" + rs(r[2], 1) + " - " + rs(r[1], 1) + ") + (2 * " + rs(r[0], 0) + " - " + rs(r[1], 0) + " - " + rs(r[2], 0) + ") * (" + rs(r[2], 0) + " - " + rs(r[1], 0) + ")");
    }
    if (f[0] == "abis") { //POSSIBLE EXTERNAL BISECTOR
        for (i = 0; i < 4; ++i) {if (mpi.find(f[i + 1]) == mpi.end()) {cout << "Invalid" << endl; return;} r[i] = mpi[f[i + 1]];}
        pl.push_back("");
        pl.back() += "((" + rs(r[2], 1) + " - " + rs(r[1], 1) + ") * " + rs(r[0], 0) + " + (" + rs(r[1], 1) + " - " + rs(r[2], 1) + ") * " + rs(r[0], 1) + " + " + rs(r[2], 0) + " * " + rs(r[1], 1) + " - " + rs(r[1], 0) + " * " + rs(r[2], 1) + ")^2 * ";
        pl.back() += "((" + rs(r[3], 1) + " - " + rs(r[1], 1) + ")^2 + (" + rs(r[1], 0) + " - " + rs(r[3], 0) + ")^2) - ";
        pl.back() += "((" + rs(r[3], 1) + " - " + rs(r[1], 1) + ") * " + rs(r[0], 0) + " + (" + rs(r[1], 1) + " - " + rs(r[3], 1) + ") * " + rs(r[0], 1) + " + " + rs(r[3], 0) + " * " + rs(r[1], 1) + " - " + rs(r[1], 0) + " * " + rs(r[3], 1) + ")^2 * ";
        pl.back() += "((" + rs(r[2], 1) + " - " + rs(r[1], 1) + ")^2 + (" + rs(r[1], 0) + " - " + rs(r[2], 0) + ")^2)";
    }
}

void radial(vector<string> f) {
    int r[5], i, j, k;
    if (f[0] == "rad") {
        for (i = 0; i < 3; ++i) {if (mpi.find(f[i + 1]) == mpi.end()) {cout << "Invalid" << endl; return;} r[i] = mpi[f[i + 1]];}
        pl.push_back("(" + rs(r[0], 0) + " - " + rs(r[1], 0) + ")^2 + (" + rs(r[0], 1) + " - " + rs(r[1], 1) + ")^2 - (" + rs(r[2], 0) + " - " + rs(r[1], 0) + ")^2 - (" + rs(r[2], 1) + " - " + rs(r[1], 1) + ")^2");
    }
    if (f[0] == "radi") {
        for (i = 0; i < 2; ++i) {if (mpi.find(f[i + 1]) == mpi.end()) {cout << "Invalid" << endl; return;} r[i] = mpi[f[i + 1]];}
        rat cf;
        for (i = j = k = 0; i < f[3].size(); ++i) {
            if (!i && f[3][i] == '-') k = 1;
            else if ('0' <= f[3][i] & f[3][i] <= '9') cf *= 10, cf += f[3][i] - '0';
            else if (i && i < f[3].size() - 1 && f[3][i] == '/') {cf = rat(1) / cf; j = 1;}
            else {cout << "Invalid" << endl; return;}
        }
        if (j) cf = rat(1) / cf;
        if (k) cf = -cf;
        pl.push_back("(" + rs(r[0], 0) + " - " + rs(r[1], 0) + ")^2 + (" + rs(r[0], 1) + " - " + rs(r[1], 1) + ")^2 - (" + cf.str() + ")^2");
    }
    if (f[0] == "conc") {
        for (i = 0; i < 4; ++i) {if (mpi.find(f[i + 1]) == mpi.end()) {cout << "Invalid" << endl; return;} r[i] = mpi[f[i + 1]];}
        r[4] = pcd.size();
        pcd.push_back(make_pair(nfc, nfc + 1));
        nfc += 2;
        pl.push_back("(" + rs(r[2], 0) + " - " + rs(r[4], 0) + ")^2 + (" + rs(r[2], 1) + " - " + rs(r[4], 1) + ")^2 - (" + rs(r[1], 0) + " - " + rs(r[4], 0) + ")^2 - (" + rs(r[1], 1) + " - " + rs(r[4], 1) + ")^2");
        pl.push_back("(" + rs(r[3], 0) + " - " + rs(r[4], 0) + ")^2 + (" + rs(r[3], 1) + " - " + rs(r[4], 1) + ")^2 - (" + rs(r[1], 0) + " - " + rs(r[4], 0) + ")^2 - (" + rs(r[1], 1) + " - " + rs(r[4], 1) + ")^2");
        pl.push_back("(" + rs(r[0], 0) + " - " + rs(r[4], 0) + ")^2 + (" + rs(r[0], 1) + " - " + rs(r[4], 1) + ")^2 - (" + rs(r[1], 0) + " - " + rs(r[4], 0) + ")^2 - (" + rs(r[1], 1) + " - " + rs(r[4], 1) + ")^2");
    }
}

void other(vector<string> f) {
    int r[2], i;
    if (f[0] == "eq") {
        for (i = 0; i < 2; ++i) {if (mpi.find(f[i + 1]) == mpi.end()) {cout << "Invalid" << endl; return;} r[i] = mpi[f[i + 1]];}
        pl.push_back(rs(r[1], 0) + " - " + rs(r[0], 0));
        pl.push_back(rs(r[1], 1) + " - " + rs(r[0], 1));
    }
    if (f[0] == "neq") {
        for (i = 0; i < 2; ++i) {if (mpi.find(f[i + 1]) == mpi.end()) {cout << "Invalid" << endl; return;} r[i] = mpi[f[i + 1]];}
        pl.push_back("((" + rs(r[1], 0) + " - " + rs(r[0], 0) + ")^2 + (" + rs(r[1], 1) + " - " + rs(r[0], 1) + ")^2) * w(" + to_string(nfc++) + ") - 1");
    }
    if (f[0] == "ori") {if (mpi.find(f[1]) == mpi.end()) {cout << "Invalid" << endl; return;} ori = mpi[f[1]];}
    if (f[0] == "axi") {if (mpi.find(f[1]) == mpi.end()) {cout << "Invalid" << endl; return;} axi = mpi[f[1]];}
}

/*
*/

deque<int> dq; //deque
vector<int> ty, dt, ip; //type, data, inverted powers
vector<pair<int, int> > ds; //descendants
map<char, int> om; //operator-to-integer map
vector<rat> rv; //rational value
priority_queue<pair<int, string> > uf; //used variables
deque<string> ps, rp; //polynomials, result polynomials

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
    string zp[6] = {"+", "-", "*", "/", "^", "="};
    if (ty[i] == 4 || ty[i] == 5) return;
    cmp(ds[i].first);
    cmp(ds[i].second);
    int dl = ds[i].first, dr = ds[i].second;
    string l, r;
    l = (ty[dl] < 4 || 5 < ty[dl]) ? ("z(" + to_string(dl) + ")") : (ty[dl] == 4 ? rv[dl].str() : rs(dt[dl] / 2, dt[dl] % 2));
    r = (ty[dr] < 4 || 5 < ty[dr]) ? ("z(" + to_string(dr) + ")") : (ty[dr] == 4 ? rv[dr].str() : rs(dt[dr] / 2, dt[dr] % 2));
    if (ty[i] == 0) rp.push_back(l + "-" + r);
    if (dt[i] == 0) {
        uf.push(make_pair(-i, "z(" + to_string(i) + ")"));
        ps.push_back("z(" + to_string(i) + ")-(" + l + "+" + r + ")");
    }
    if (dt[i] == 1) {
        uf.push(make_pair(-i, "z(" + to_string(i) + ")"));
        ps.push_back("z(" + to_string(i) + ")-(" + l + "-" + r + ")");
    }
    if (dt[i] == 2) {
        uf.push(make_pair(-i, "z(" + to_string(i) + ")"));
        ps.push_back("z(" + to_string(i) + ")-(" + l + "*" + r + ")");
    }
    if (dt[i] == 3) {
        uf.push(make_pair(-i, "z(" + to_string(i) + ")"));
        ps.push_back("z(" + to_string(i) + ")*" + r + "-" + l);
    }
    if (dt[i] == 4) {
        uf.push(make_pair(-i, "z(" + to_string(i) + ")"));
        if (rv[dr] > 0) {
            if (rv[dr].p == 1) ps.push_back("z(" + to_string(i) + ")^" + to_string(rv[dr].q) + "-" + l);
            else if (rv[dr].q == 1) ps.push_back("z(" + to_string(i) + ")-" + l + "^" + to_string(rv[dr].p));
            else ps.push_back("z(" + to_string(i) + ")^" + to_string(rv[dr].q) + "-" + l + "^" + to_string(rv[dr].p));
        } else {
            if (rv[dr].p == -1) ps.push_back("z(" + to_string(i) + ")^" + to_string(rv[dr].q) + "*" + l + "-1");
            else if (rv[dr].q == 1) ps.push_back("z(" + to_string(i) + ")*" + l + "^" + to_string(rv[dr].p) + "-1");
            else ps.push_back("z(" + to_string(i) + ")^" + to_string(rv[dr].q) + "*" + l + "^" + to_string(rv[dr].p) + "-1");
        }
    }
}

string crt(int i) {
    string zp[6] = {"+", "-", "*", "/", "^", "="};
    if (ty[i] == 4) return "(" + to_string(rv[i].p) + "/" + to_string(rv[i].q) + ")";
    if (ty[i] == 5) return rs(dt[i] / 2, dt[i] % 2);
    if (ty[i] == 0 || ty[i] == 3) return "(" + crt(ds[i].first) + zp[dt[i]] + crt(ds[i].second) + ")";
}

void eqi() {
    if (!cnd(0)) syr();
    ty.push_back(1);
    dt.push_back(5);
    ds.push_back(make_pair(-1, -1));
    dq.push_back(ty.size() - 1);
}

void boi() {
    inr(0);
    ty.push_back(6);
    dt.push_back(0);
    ds.push_back(make_pair(-1, -1));
    dq.push_back(ty.size() - 1);
}

void bci() {
    if (!cnd(0)) syr();
    if (dq.size() < 2 || ty[dq[dq.size() - 2]] != 6) syr();
    int x = dq.back(), i;
    for (i = 0; i < 2; ++i) dq.pop_back();
    dq.push_back(x);
    ty.push_back(7);
    dt.push_back(0);
    ds.push_back(make_pair(-1, -1));
}

void nmi(char c) {
    if (inr(1)) {dt.back() *= 10; dt.back() += c - '0';}
    else {
        ty.push_back(4);
        dt.push_back(c - '0');
        ds.push_back(make_pair(-1, -1));
        dq.push_back(ty.size() - 1);
    }
}

void opi(char c) {
    int cp[5] = {1, 1, 2, 2, 4}; //condensation priority
    if (cnd(cp[om[c]])) {
        ty.push_back(2);
        dt.push_back(om[c]);
        ds.push_back(make_pair(-1, -1));
        dq.push_back(ty.size() - 1);
    } else if (c == '-') {
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

void tri(int i) {
    inr(0);
    ty.push_back(5);
    dt.push_back(i);
    ds.push_back(make_pair(-1, -1));
    dq.push_back(ty.size() - 1);
}

void query(string s) {
    int N, f, x, i, j;
    char zp[5] = {'+', '-', '*', '/', '^'}, zq[9] = {'a', 'b', 'c', 'A', 'B', 'C', 's', 'r', 'S'}; //operator-to-integer, term-to-integer
    for (i = 0; i < 5; ++i) om[zp[i]] = i;
    ty.push_back(4);
    dt.push_back(1);
    ds.push_back(make_pair(-1, -1));
    N = s.size();
    for (i = 0; i < N; ++i) {
        if (s[i] == '=') eqi();
        if (s[i] == '(') boi();
        if (s[i] == ')') bci();
        if ('0' <= s[i] && s[i] <= '9') nmi(s[i]);
        if (om.find(s[i]) != om.end()) opi(s[i]);
        if (i + 1 < N && s[i] == 'x' && s[i + 1] == '(') {
            string a = "";
            for (i += 2; i < N && s[i] != ')'; ++i) a += s[i];
            if (i == N) syr();
            tri(mpi[a] * 2);
        }
        if (i + 1 < N && s[i] == 'y' && s[i + 1] == '(') {
            string a = "";
            for (i += 2; i < N && s[i] != ')'; ++i) a += s[i];
            if (i == N) syr();
            tri(mpi[a] * 2 + 1);
        }
        if (i + 2 < N && s[i] == 'd' && s[i + 1] == 's' && s[i + 2] == '(') {
            string a = "", b = "";
            for (i += 3; i < N && s[i] != ')'; ++i) {
                if (s[i] == ',') swap(a, b);
                else b += s[i];
            }
            if (i == N) syr();
            boi();
            boi();
            tri(mpi[a] * 2);
            opi('-');
            tri(mpi[b] * 2);
            bci();
            opi('^');
            nmi('2');
            opi('+');
            boi();
            tri(mpi[a] * 2 + 1);
            opi('-');
            tri(mpi[b] * 2 + 1);
            bci();
            opi('^');
            nmi('2');
            bci();
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
    dq.clear();
}

void gen() {
    int i;
    //if (ori < 0 && !pcd.empty()) ori = 0;
    //if (axi < 0 && pcd.size() > 1) axi = 1;
    if (axi > -1) pl.push_front(rs(axi, 1));
    if (ori > -1) {pl.push_front(rs(ori, 1)); pl.push_front(rs(ori, 0));}
    cout << "system(\"--ticks-per-sec\", 1000);" << endl;
    cout << "int tm0 = timer;" << endl;
    cout << endl;
    cout << "ring r = 0, (";
    while (!uf.empty()) {cout << uf.top().second + ", "; uf.pop();}
    for (i = 0; i < nfc; ++i) cout << "w(" + to_string(nfc - i - 1) + ")" + (i < nfc - 1 ? ", " : "");
    cout << "), dp;" << endl;
    cout << endl;
    for (i = 0; i < pl.size(); ++i) cout << "poly p" + to_string(i) + " = " + pl[i] << ";" << endl;
    for (i = 0; i < ps.size(); ++i) cout << "poly q" + to_string(i) + " = " + ps[i] << ";" << endl;
    cout << endl;
    for (i = 0; i < rp.size(); ++i) cout << "poly rs" + to_string(i) + " = " + rp[i] << ";" << endl;
    cout << endl;
    cout << "ideal I = ";
    for (i = 0; i < pl.size(); ++i) cout << "p" + to_string(i) + (i < pl.size() - 1 || !ps.empty() ? ", " : "");
    for (i = 0; i < ps.size(); ++i) cout << "q" + to_string(i) + (i < ps.size() - 1 ? ", " : "");
    cout << ";" << endl;
    cout << "I = groebner(I);" << endl;
    for (i = 0; i < rp.size(); ++i) cout << "reduce(rs" + to_string(i) + ", I);" << endl;
    cout << endl;
    cout << "int tm1 = timer;" << endl;
    cout << "tm0;" << endl;
    cout << "tm1;" << endl;
    cout << "tm1 - tm0;" << endl;
}

/*  TODO
    VERIFY
    PT NEW A
    PT NEW B
    PT NEW C
    PT NEW D
    PT NEW E
    PT NEW F
    MS NEQ A B
    MS NEQ C D
    NEG LN PARA A B C D
    FT ILL E A B C D
    FT ILL F A B C D
    QR RAW ds(E,F)=0
*/

bool interpret(vector<string> v) {
    string p;
    int i, j;
    vector<string> w(v.begin() + 1, v.end());
    if (v[0] == "pt") create(w);
    if (v[0] == "ft") finite(w);
    if (v[0] == "ln") linear(w);
    if (v[0] == "rd") radial(w);
    if (v[0] == "ms") other(w);
    if (v[0] == "neg") {
        for (i = 1; i < 2; ++i) for (j = 0; j < w[i].size(); ++j) if ('A' <= w[i][j] && w[i][j] <= 'Z') w[i][j] += 'a' - 'A';
        vector<string> u(w.begin() + 1, w.end());
        if (v[1] == "ft") {
            swap(w, u);
            u.clear();
            u.push_back("new");
            u.push_back(p = "intr_" + to_string(npm++));
            create(u);
            swap(p, w[1]);
            finite(w);
            pl.push_back("((" + rs(mpi[w[1]], 0) + " - " + rs(mpi[p], 0) + ")^2 + (" + rs(mpi[w[1]], 1) + " - " + rs(mpi[p], 1) + ")^2) * w(" + to_string(nfc++) + ") - 1");
        } else {
            if (v[1] == "ln") linear(u);
            if (v[1] == "rd") radial(u);
            if (v[1] == "raw") {
                p = "";
                for (i = 2; i < v.size(); ++i) p += v[i];
                swap(pl, ps);
                query(p);
                swap(pl, ps);
                pl.push_back(rp.back());
                rp.pop_back();
            }
            pl.back() = "(" + pl.back() + ") * w(" + to_string(nfc++) + ") - 1";
        }
    }
    if (v[0] == "raw") {
        p = "";
        for (i = 1; i < v.size(); ++i) p += v[i];
        swap(pl, ps);
        query(p);
        swap(pl, ps);
        pl.push_back(rp.back());
        rp.pop_back();
    }
    if (v[0] == "qr") {
        if (v[1] == "raw") {
            p = "";
            for (i = 2; i < v.size(); ++i) p += v[i];
            query(p);
        } else {
            for (i = 1; i < 2; ++i) for (j = 0; j < w[i].size(); ++j) if ('A' <= w[i][j] && w[i][j] <= 'Z') w[i][j] += 'a' - 'A';
            interpret(w);
            if (w[0] == "ms" && w[1] == "eq") {
                rp.push_back(pl[pl.size() - 2]);
                rp.push_back(pl.back());
                pl.pop_back();
                pl.pop_back();
            } else {
                rp.push_back(pl.back());
                pl.pop_back();
            }
        }
    }
    if (v[0] == "end") return 1;
    return 0;
}

int main() {
    freopen("in.txt", "r", stdin);
    freopen("out.txt", "w", stdout);
    ios_base::sync_with_stdio(0);
    string s;
    int N, i, j;
    for (i = 0; i < 10; ++i) apn.insert('0' + i);
    for (i = 0; i < 26; ++i) apn.insert('a' + i);
    for (i = 0; i < 26; ++i) apn.insert('A' + i);
    while (getline(cin, s)) {
        if (s.size() > 1 && s[0] == '/' && s[1] == '/') continue;
        v.clear();
        v.push_back("");
        for (i = 0; i < s.size(); ++i) {
            if (s[i] == ' ') {if (v.back() != "") v.push_back("");}
            else v.back() += s[i];
        }
        if (v.back() == "") v.pop_back();
        if (v.empty()) continue;
        for (i = 0; i < 2 && v[0] != "raw"; ++i) for (j = 0; j < v[i].size(); ++j) if ('A' <= v[i][j] && v[i][j] <= 'Z') v[i][j] += 'a' - 'A';
        if (interpret(v)) break;
    }
    gen();
    return 0;
}
