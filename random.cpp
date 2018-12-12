#include <bits/stdc++.h>
using namespace std;

/*
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

/*
0   QUERY
1   INEQUALITY OPERATOR
2   EXPRESSION - ARITHMETIC
3   EXPRESSION - GEOMETRIC
4   EXPRESSION - EXPONENTIAL
5   RATIONAL POWER - ARITHMETIC
6   RATIONAL POWER - GEOMETRIC
7   RATIONAL POWER - EXPONENTIAL
8   RATIONAL - ARITHMETIC
9   RATIONAL - GEOMETRIC
10  RATIONAL - EXPONENTIAL
11  INTEGER - ARITHMETIC
12  INTEGER - GEOMETRIC
13  INTEGER - EXPONENTIAL
14  BASE TRIANGLE TERM
15  OPERATOR
16  LEFT PARENTHESIS
17  RIGHT PARENTHESIS
*/

/*
0   a
1   b
2   c
3   sin A
4   sin B
5   sin C
6   s - semiperimeter
7   r - radius of circumcircle
*/

deque<int> dq;
vector<int> type, data;
vector<pair<int, pair<int, int> > > desc;

inline string rf(string s, string a, string b) {for (int i = 0; (i = s.find(a, i)) < s.size(); i += b.size()) s.replace(i, a.size(), b); return s;}

int main() {
    ios_base::sync_with_stdio(0);
    int N, i;
    string input;
    getline(cin, input);
    input = rf(input, " ", "");
    input = rf(input, "(-", "(0-");
    if (input[0] == '-') input = "0" + input;
    N = input.size();
    for (i = 0; i < N; ++i) {

    }
}
