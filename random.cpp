#include <bits/stdc++.h>
using namespace std;

/*  INIT
0   PT      NEW POINT
1   FT      FINITE LOCUS CONSTRAINT
2   LN      LINE LOCUS CONSTRAINT
3   RD      CIRCLE/RADIUS LOCUS CONSTRAINT
4   MS      MISC.
*/

/*  0 - NEW POINT
0   NEW     CREATE POINT (IDENTIFIER)
1   NEC     CREATE POINT (IDENTIFIER, COORDINATE IDENTIFIERS)
*/

/*  1 - FINITE LOCUS CONSTRAINT
0   ILL     INTERSECTION LINE-LINE (2, 2)
1   ILC     INTERSECTION LINE-CIRCLE (2, 2)
//2   ILI     INTERSECTION LINE-CIRCLE IMMEDIATE (2, 1, 1I)
3   ICC     INTERSECTION CIRCLE-CIRCLE (2, 2)
//4   ICI     INTERSECTION CIRCLE-CIRCLE IMMEDIATE (2, 1, 1I)
//5   IC2     INTERSECTION CIRCLE-CIRCLE DIIMMEIDATE (1, 1I, 1, 1I)
5   MID     MIDPOINT (2)
6   MIR     MIRROR OVER LINE (1, 2)
7   CEN     CENTROID (3)
8   INC     INCENTRE (3)
9   CIR     CIRCUMCENTRE (3)
*/

/*  2 - LINE LOCUS CONSTRAINT
0   COL     COLLINEAR (2)
//1   COLI    COLLINEAR IMMEDIATE (1, 3I)
2   PARA    PARALLEL (1, 2)
3   PER     PERPENDICULAR (1, 2)
4   PBIS    PERPENDICULAR BISECTOR (2)
5   ABIS    ANGLE BISECTOR (1, 2)
6   TAN     TANGENT (3) SOURCE-CENTRE-RADIUS
//7   TANI    TANGENT IMMEDIATE (2, 1I)
*/

/*  3 - CIRCLE/RADIUS LOCUS CONSTRAINT
0   RAD     RADIUS (1, 2) CENTRE-RADIUS
//1   RADI    RADIUS IMMEDIATE (1, 1I)
2   CONC    CONCYCLIC (3)
*/

/*  4 - OTHER
0   NEQ     NONEQUAL POINTS (1)
1   ORI     POINT AS ORIGIN (0)
2   AXI     POINT ON X-AXIS (0)
*/

int nfc, ori, axi; //number of coordinates, (0, 0) point, (x, 0) point
set<char> apn; //alphanumeric characters
vector<string> v; //vector
vector<int> pv; //priority vector
map<string, int> mpi, mci; //map from point identifier to index
vector<pair<int, int> > pcd; //coordinate indices of point
deque<string> pl; //polynomials

inline string rs(int i, bool d) {return "z(" + to_string(d ? pcd[i].second : pcd[i].first) + ")";}

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
    int r[4], i;
    if (f[0] == "ill") {

    }
    if (f[0] == "ilc") {

    }
}

void linear(vector<string> f) {
    int r[4], i;
    if (f[0] == "col") {
        for (i = 0; i < 3; ++i) {if (mpi.find(f[i + 1]) == mpi.end()) {cout << "Invalid" << endl; return;} r[i] = mpi[f[i + 1]];}
        pl.push_back("(" + rs(r[0], 1) + " - " + rs(r[1], 1) + ") * (" + rs(r[2], 0) + " - " + rs(r[1], 0) + ") - (" + rs(r[2], 1) + " - " + rs(r[1], 1) + ") * (" + rs(r[0], 0) + " - " + rs(r[1], 0) + ")");
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
        pl.push_back("(2 * " + rs(r[0], 1) + " - " + rs(r[1], 1) + " - " + rs(r[2], 1) + ") * (" + rs(r[2], 1) + " - " + rs(r[1], 1) + ") + (2 * " + rs(r[0], 0) + " - " + rs(r[1], 0) + " - " + rs(r[2], 0) + ")(" + rs(r[2], 0) + " - " + rs(r[1], 0) + ")");
    }
    if (f[0] == "abis") { //POSSIBLE EXTERNAL BISECTOR
        for (i = 0; i < 4; ++i) {if (mpi.find(f[i + 1]) == mpi.end()) {cout << "Invalid" << endl; return;} r[i] = mpi[f[i + 1]];}
        pl.push_back("");
        pl.back() += "((" + rs(r[2], 1) + " - " + rs(r[1], 1) + ") * " + rs(r[0], 0) + " + (" + rs(r[1], 1) + " - " + rs(r[2], 1) + ") * " + rs(r[0], 1) + " + " + rs(r[2], 0) + " * " + rs(r[1], 1) + " - " + rs(r[1], 0) + " * " + rs(r[2], 1) + ")^2 * ";
        pl.back() += "((" + rs(r[3], 1) + " - " + rs(r[1], 1) + ")^2 + (" + rs(r[1], 0) + " - " + rs(r[3], 0) + ")^2) - ";
        pl.back() += "((" + rs(r[3], 1) + " - " + rs(r[1], 1) + ") * " + rs(r[0], 0) + " + (" + rs(r[1], 1) + " - " + rs(r[3], 1) + ") * " + rs(r[0], 1) + " + " + rs(r[3], 0) + " * " + rs(r[1], 1) + " - " + rs(r[1], 0) + " * " + rs(r[3], 1) + ")^2 * ";
        pl.back() += "((" + rs(r[2], 1) + " - " + rs(r[1], 1) + ")^2 + (" + rs(r[1], 0) + " - " + rs(r[2], 0) + ")^2)";
    }
    if (f[0] == "tan") {

    }
}

void radial(vector<string> f) {
    int r[5], i;
    if (f[0] == "rad") {
        for (i = 0; i < 3; ++i) {if (mpi.find(f[i + 1]) == mpi.end()) {cout << "Invalid" << endl; return;} r[i] = mpi[f[i + 1]];}
        pl.push_back("(" + rs(r[0], 0) + " - " + rs(r[1], 0) + ")^2 + (" + rs(r[0], 1) + " - " + rs(r[1], 1) + ")^2 - (" + rs(r[2], 0) + " - " + rs(r[1], 0) + ")^2 + (" + rs(r[2], 1) + " - " + rs(r[1], 1) + ")^2");
    }
    if (f[0] == "conc") {
        for (i = 0; i < 5; ++i) {if (mpi.find(f[i + 1]) == mpi.end()) {cout << "Invalid" << endl; return;} r[i] = mpi[f[i + 1]];}
        r[5] = pcd.size();
        pcd.push_back(make_pair(nfc, nfc + 1));
        nfc += 2;
        pl.push_back("(" + rs(r[2], 0) + " - " + rs(r[5], 0) + ")^2 + (" + rs(r[2], 1) + " - " + rs(r[5], 1) + ")^2 - (" + rs(r[1], 0) + " - " + rs(r[5], 0) + ")^2 + (" + rs(r[1], 1) + " - " + rs(r[5], 1) + ")^2");
        pl.push_back("(" + rs(r[3], 0) + " - " + rs(r[5], 0) + ")^2 + (" + rs(r[3], 1) + " - " + rs(r[5], 1) + ")^2 - (" + rs(r[1], 0) + " - " + rs(r[5], 0) + ")^2 + (" + rs(r[1], 1) + " - " + rs(r[5], 1) + ")^2");
        pl.push_back("(" + rs(r[0], 0) + " - " + rs(r[5], 0) + ")^2 + (" + rs(r[0], 1) + " - " + rs(r[5], 1) + ")^2 - (" + rs(r[1], 0) + " - " + rs(r[5], 0) + ")^2 + (" + rs(r[1], 1) + " - " + rs(r[5], 1) + ")^2");
    }
}

void other(vector<string> f) {
    int r[2], i;
    if (f[0] == "neq") {
        for (i = 0; i < 2; ++i) {if (mpi.find(f[i + 1]) == mpi.end()) {cout << "Invalid" << endl; return;} r[i] = mpi[f[i + 1]];}
        pl.push_back("((" + rs(r[1], 0) + " - " + rs(r[0], 0) + ")^2 + (" + rs(r[1], 1) + " - " + rs(r[0], 1) + ")^2) * z(" + to_string(nfc++) + ") - 1");
    }
}

/*  TODO
    queries
*/

int main() {
    freopen("in.txt", "r", stdin);
    ios_base::sync_with_stdio(0);
    string s;
    int N, i, j;
    for (i = 0; i < 10; ++i) apn.insert('0' + i);
    for (i = 0; i < 26; ++i) apn.insert('a' + i);
    for (i = 0; i < 26; ++i) apn.insert('A' + i);
    while (getline(cin, s)) {
        for (i = 0; i < s.size(); ++i) if ('A' <= s[i] && s[i] <= 'Z') s[i] += 'a' - 'A';
        v.clear();
        v.push_back("");
        for (i = 0; i < s.size(); ++i) {
            if (apn.find(s[i]) == apn.end()) {if (v.back() != "") v.push_back("");}
            else v.back() += s[i];
        }
        if (v.back() == "") v.pop_back();
        if (v.empty()) continue;
        vector<string> w(v.begin() + 1, v.end());
        if (v[0] == "pt") create(w);
        if (v[0] == "ft") finite(w);
        if (v[0] == "ln") linear(w);
        if (v[0] == "rd") radial(w);
        if (v[0] == "ms") other(w);

        for (i = 0; i < pl.size(); ++i) cout << pl[i] << endl;
    }
}
