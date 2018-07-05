#define export exports
extern "C" {
    #include "../qbe/include/qbe/all.h"
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

    std::vector< set<string> > vecGen;
    for (Blk *blk = fn->start; blk; blk = blk->link)
    {
        set<string> tmp;
        names.push_back("@" + string(blk->name));
        
        for (int i = 0; i < blk->nins; i++)
        {
            if (blk->nins && Tmp0 <= blk->ins[i].to.val)
            {
                string s = "%" +  string(fn->tmp[blk->ins[i].to.val].name);
                tmp.insert(s);
            }
        }
        vecGen.push_back(tmp);

    }

    std::vector< set<string> > vecKill;
    for (int i = 0; i < vecGen.size(); i++)
    {
        set<string> tmp;
        for (int j = 0; j < vecGen.size(); j++)
        {
            if (i == j)
                continue;


            for (set<string>::iterator it = vecGen[i].begin(); it != vecGen[i].end(); it++)
                for (set<string>::iterator it1 = vecGen[j].begin(); it1 != vecGen[j].end(); it1++)
                    if (*it == *it1)
                    {
                        string s = names[j] + *it1;
                        tmp.insert(s);
                    }
        }
        vecKill.push_back(tmp);
    }

    for (int i = 0; i < names.size(); i++)
    {
        cout << names[i] << endl;

        cout << "\tgen = ";
        for (set<string>::iterator it = vecGen[i].begin(); it != vecGen[i].end(); it++)
            cout << names[i] << *it << " ";
        
        cout << endl;

        cout << "\tkill = ";
        for (set<string>::iterator it = vecKill[i].begin(); it != vecKill[i].end(); it++)
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