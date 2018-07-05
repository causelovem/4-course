#define export exports
extern "C" {
    // #include "../qbe/include/qbe/all.h"
    #include <qbe/all.h>
}
#undef export

// #include "../qbe/include/qbe/all.h"
// #include <qbe/all.h>

#include <stdio.h>
#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <algorithm>

using namespace std;


static void readfn (Fn *fn)
{
    std::vector<string> names;

    string first = "";
    for (Blk *blk = fn->start; blk; blk = blk->link)
    {
        if (blk->npred == 0)
            first = "@" + string(blk->name);
        names.push_back("@" + string(blk->name));
    }

    std::vector< set<string> > dom;
    std::vector<string> namesDom;

    for (int i = 1; i < names.size(); i++)
        namesDom.push_back(names[i]);

    set<string> firstSet;

    firstSet.clear();
    for (int i = 0; i < names.size(); i++)
        firstSet.insert(names[i]);

    for (int i = 0; i < names.size(); i++)
    {
        if (names[i] == first)
        {
            set<string> tmp;
            tmp.insert(first);
            dom.push_back(tmp);
            continue;
        }
        dom.push_back(firstSet);
    }

    int changed = 1;
    while (changed == 1)
    {
        changed = 0;
        int cnt = -1;
        for (Blk *blk = fn->start; blk; blk = blk->link)
        {
            cnt++;
            if ("@" + string(blk->name) == first)
                continue;

            set<string> intersectDoms;
            set<string> tmp = firstSet;

            for (int i = 0; i < blk->npred; i++)
            {
                string blkTmp = "@" + string((blk->pred)[i]->name);

                for (int j = 0; j < names.size(); j++)
                    if (blkTmp == names[j])
                    {
                        for (set<string>::iterator it = tmp.begin(); it != tmp.end(); it++)
                            for (set<string>::iterator it1 = dom[j].begin(); it1 != dom[j].end(); it1++)
                                if (*it == *it1)
                                    intersectDoms.insert(*it);
                        break;
                    }

                tmp = intersectDoms;
                intersectDoms.clear();
            }
            intersectDoms = tmp;
            intersectDoms.insert(names[cnt]);

            if (intersectDoms != dom[cnt])
            {
                dom[cnt] = intersectDoms;
                changed = 1;
            }
        }
    }


    std::vector< set<string> > reverseDom(names.size());
    for (int i = 0; i < names.size(); i++)
    {
        for (set<string>::iterator it = dom[i].begin(); it != dom[i].end(); it++)
            for (int j = 0; j < names.size(); j++)
                if (*it == names[j])
                    reverseDom[j].insert(names[i]);
    }

    for (int i = 0; i < reverseDom.size(); i++)
    {
        cout << names[i] << "\t";

        for (set<string>::iterator it = reverseDom[i].begin(); it != reverseDom[i].end(); it++)
            cout << *it << " ";
        cout << endl;
    }
}

static void readdat (Dat *dat)
{
    (void) dat;
}

int main ()
{
    parse(stdin, (char *)"<stdin>", readdat, readfn);
    freeall();
}