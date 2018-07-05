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
        set<string> tmp;
        for (set<string>::iterator it = vecGen[i].begin(); it != vecGen[i].end(); it++)
        {
            string s = names[i] + *it;
            tmp.insert(s);
        }
        vecGen[i] = tmp;
    }

    std::vector< set<string> > vecIn(vecGen.size());
    std::vector< set<string> > vecOut(vecGen.size());
    int changed = 1;
    while (changed == 1)
    {
        changed = 0;
        int cnt = -1;
        for (Blk *blk = fn->start; blk; blk = blk->link)
        {
            cnt++;
            set<string> tmp = vecIn[cnt];
            for (set<string>::iterator it = vecKill[cnt].begin(); it != vecKill[cnt].end(); it++)
                for (set<string>::iterator it1 = vecIn[cnt].begin(); it1 != vecIn[cnt].end(); it1++)
                    if (*it == *it1)
                        tmp.erase(*it);

            set<string> newOut = vecGen[cnt];
            for (set<string>::iterator it = tmp.begin(); it != tmp.end(); it++)
                newOut.insert(*it);

            vecIn[cnt].clear();
            for (int i = 0; i < blk->npred; i++)
            {
                string blkTmp = "@" + string((blk->pred)[i]->name);

                for (int j = 0; j < names.size(); j++)
                    if (blkTmp == names[j])
                        for (set<string>::iterator it = vecOut[j].begin(); it != vecOut[j].end(); it++)
                            vecIn[cnt].insert(*it);
            }

            if (newOut.size() != vecOut[cnt].size())
            {
                vecOut[cnt] = newOut;
                changed = 1;
            }
        }
    }


    for (int i = 0; i < names.size(); i++)
    {
        cout << names[i] << endl;

        cout << "\trd_in = ";
        for (set<string>::iterator it = vecIn[i].begin(); it != vecIn[i].end(); it++)
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