#include <bits/stdc++.h>
using namespace std;

typedef struct {

} opr;

typedef struct {

} tri;

typedef struct {

} intr;

typedef struct {

} ratl;

typedef struct {

} radi;

typedef struct expr {
    expr *l;
    opr op;
    expr *r;
} expr;

typedef struct ineq {
    int type;
} ineq;

typedef struct query {
    expr lhs;
    ineq op;
    expr rhs;
} query;

int main() {

}
