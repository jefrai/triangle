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
1   NEWC    CREATE POINT (IDENTIFIER, COORDINATE IDENTIFIERS)
2   NEWF    CREATE POINT (IDENTIFIER, ORIGIN COORDINATES)
*/

/*  1 - FINITE LOCUS CONSTRAINT
0   ILL     INTERSECTION LINE-LINE (2, 2)
1   ILC     INTERSECTION LINE-CIRCLE (2, 2)
2   ILI     INTERSECTION LINE-CIRCLE IMMEDIATE (2, 1, 1I)
3   ICC     INTERSECTION CIRCLE-CIRCLE (2, 2)
4   ICI     INTERSECTION CIRCLE-CIRCLE IMMEDIATE (2, 1, 1I)
5   IC2     INTERSECTION CIRCLE-CIRCLE DIIMMEIDATE (1, 1I, 1, 1I)
5   MID     MIDPOINT (2)
6   MIR     MIRROR OVER LINE (1, 2)
*/

/*  2 - LINE LOCUS CONSTRAINT
0   COL     COLLINEAR (2)
1   COLI    COLLINEAR IMMEDIATE (1, 3I)
2   PARA    PARALLEL (1, 2)
3   PER     PERPENDICULAR (1, 2)
4   PBIS    PERPENDICULAR BISECTOR (2)
5   ABIS    ANGLE BISECTOR (1, 2)
6   TAN     TANGENT (3)
7   TANI    TANGENT IMMEDIATE (2, 1I)
*/

/*  3 - CIRCLE/RADIUS LOCUS CONSTRAINT
0   RAD     RADIUS (1, 2)
1   RADI    RADIUS IMMEDIATE (1, 1I)
2   CONC    CONCYCLIC (3)
*/

/*  4 - OTHER
0   NEQ     NONEQUAL POINTS (2)
*/

int nc; //number of coordinates
set<char> apn; //alphanumeric characters
vector<string> v; //vector
vector<int> pv; //priority vector
map<string, int> mi; //map from point identifier to index
vector<pair<int, int> > ci; //coordinate indices of point

void create(vector<string> f) {
    if (f[0] == "new") {
        mi[f[1]] = ci.size();
        ci.push_back(make_pair(nc, nc + 1));
        nc += 2;
    }
    if (f[0] == "newc") {

    }
    if (f[0] == "newf") {

    }
}

void finite(vector<string> f) {

}

void linear(vector<string> f) {

}

void radial(vector<string> f) {

}

void other(vector<string> f) {

}

/*  TODO
    queries
*/

int main() {
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
    }
}
