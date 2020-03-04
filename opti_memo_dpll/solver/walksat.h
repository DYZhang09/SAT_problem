#pragma once

#include"../parser/parser.h"
#include"../data_struct/data_struct.h"
#include"../../config/config.h"

typedef int v_iterator;

struct BinVector formula;
struct Vector vars = vecInit();
struct Vector falses = vecInit();
struct Vector true_clause = vecInit();
int clause_num;

unsigned int i_rand(unsigned int range)
{
    unsigned int x = (RAND_MAX + 1u) / range;
    unsigned int y = x * range;
    unsigned int r;

    do
    {
        r = rand();
    } while (r >= y);

    return (r / x);
}


float f_rand()
{
    return ((float)rand()) / ((float)RAND_MAX + 1.0);
}


float pos_vs_neg(unsigned int var)
{
    unsigned int pos = 0, neg = 0, cl, li;

    for (cl = 0; cl < clause_num; cl++)
        for (li = 0; li < vecSize(binVecNth(formula, cl)); li++)
        {
            if (binVecGrid(formula, cl, li)> 0 && abs(binVecGrid(formula, cl, li)) == var) pos++;
            if ((binVecGrid(formula, cl, li) < 0) && abs(binVecGrid(formula, cl, li)) == var) neg++;
        }

    if (pos == neg) return 0;
    else if (pos == 0) return -1;
    else if (neg == 0) return 1;
    else if (pos > neg) return neg / pos;
    else if (neg > pos) return -(pos / neg);
}


bool not_in_falses(struct Vector falses, int cl)
{
    if (vecSearch(falses, cl) == -1) return true;
    return false;
}


int test_model()
{
    bool not_sat = false;
    int var, cl;
    v_iterator vars_it, li_it;

    for (cl = 0; cl < clause_num; cl++)
        vecNth_re(&true_clause, cl, false);

    for (var = 1, vars_it = 1; vars_it != vecSize(vars); vars_it++, var++) {
        for (cl = 0; cl < clause_num; cl++) {
            if (vecNth(true_clause, cl)) continue;

            for (li_it = 0; li_it < vecSize(binVecNth(formula, cl)); li_it++) {
                if (var == abs(binVecGrid(formula, cl, li_it))) {
                    if ((vecNth(vars, vars_it) < 0 && binVecGrid(formula, cl, li_it) < 0) ||
                        (vecNth(vars, vars_it) > 0 && binVecGrid(formula, cl, li_it) > 0)) {
                        vecNth_re(&true_clause, cl, true);
                        break;
                    }
                }
            }
        }
    }

    for (cl = 0; cl < clause_num; cl++) {
        if (!vecNth(true_clause, cl)) {
            if(not_in_falses(falses, cl))
                vec_push_back(&falses, cl);
            not_sat = true;
        }
    }

    if (!not_sat)   return clause_num;
    else return clause_num - vecSize(falses);
}


void climb(unsigned int cl, unsigned int true_clauses)
{
    unsigned int result;
    int best_lit = -1;
    v_iterator li_it;

    for (li_it = 0; li_it < vecSize(binVecNth(formula, cl)); li_it++) {
        vecNth_re(&vars, abs(binVecGrid(formula, cl, li_it)),
                            -vecNth(vars, abs(binVecGrid(formula, cl, li_it))));
        result = test_model();
        vecNth_re(&vars, abs(binVecGrid(formula, cl, li_it)),
                             -vecNth(vars, abs(binVecGrid(formula, cl, li_it))));

        if (result > true_clauses) {
            best_lit = binVecGrid(formula, cl, li_it);
            true_clauses = result;
        }
    }
    if (best_lit != -1) {
        vecNth_re(&vars, best_lit, -(vecNth(vars, best_lit) + 1));
    }
}



void flip_random_lit(unsigned int cl)
{
    int lit = abs(binVecGrid(formula, cl, i_rand(vecSize(binVecNth(formula, cl)))));
    vecNth_re(&vars, lit, -(vecNth(vars, lit)));
}


bool walksat(int mf, int pr)
{
    unsigned int true_clauses;
    unsigned int clause;

    for (int i = 0; i < mf; i++) {
        true_clauses = test_model();

        if (true_clauses == clause_num) return true;
        else {
            clause = vecNth(falses, i_rand(vecSize(falses)));

            if (f_rand() < (pr / 100.0)) {
                flip_random_lit(clause);
            }
            else climb(clause, true_clauses);
        }
    }
    return false;
}


bool WALKSAT(int mf=1, int pr=50)
{
    bool result = false;
    unsigned int var;
    float ratio;
    clause_num = info.num_clause;

    srand((unsigned int)time(NULL));

    for (int i = 0; i < clause_num; i++)
        vec_push_back(&true_clause, false);

    v_iterator vars_it;
    for (var = 1, vars_it = 1; vars_it < vecSize(vars); var++, vars_it++) {
        /*ratio = pos_vs_neg(var);

        if (ratio == 0) vecNth_re(&vars, vars_it,( i_rand(2)?-var:var));
        else if (ratio == 1)vecNth_re(&vars, vars_it, var);
        else if (ratio == -1)vecNth_re(&vars, vars_it, -var);
        else if (ratio > 0) vecNth_re(&vars, vars_it, (f_rand() > ratio ? var : -var));
        else vecNth_re(&vars, vars_it, (f_rand() > -ratio ? -var : var));*/
        vecNth_re(&vars, vars_it, (i_rand(2) ? -var : var));
    }

    result = walksat(mf, pr);
    return result;
}