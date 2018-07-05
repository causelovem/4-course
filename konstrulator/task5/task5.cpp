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
    std::vector< set<string> > vecDef;
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
        vecDef.push_back(tmp);
    }

    std::vector< set<string> > vecUse;
    int cnt = -1;
    for (Blk *blk = fn->start; blk; blk = blk->link)
    {
        cnt++;
        set<string> tmp;

        if ((blk->s1 == NULL) && (blk->s2 == NULL))
        {
            string retVar ="%" + string(fn->tmp[blk->jmp.arg.val].name);

            if (retVar.length() != 1)
            {
                int retVarFlag = 0;
                for (set<string>::iterator it = vecDef[cnt].begin(); it != vecDef[cnt].end(); it++)
                    if (*it == retVar)
                    {
                        retVarFlag = 1;
                        break;
                    }
                if (retVarFlag == 0)
                    tmp.insert(retVar);

            }
        }
        
        for (int i = 0; i < blk->nins; i++)
        {
            int flag = 0;
            string s1 = "%" + string(fn->tmp[blk->ins[i].to.val].name);
            if (blk->nins && Tmp0 <= blk->ins[i].arg[0].val)
            {
                string s = "%" +  string(fn->tmp[blk->ins[i].arg[0].val].name);

                if (s1 == s)
                    tmp.insert(s);
                else
                {
                    for (set<string>::iterator it = vecDef[cnt].begin(); it != vecDef[cnt].end(); it++)
                        if (*it == s)
                        {
                            flag = 1;
                            break;
                        }
                    if (flag == 0)
                        tmp.insert(s);
                }

                flag = 0;
            }
            if (blk->nins && Tmp0 <= blk->ins[i].arg[1].val)
            {
                string s = "%" +  string(fn->tmp[blk->ins[i].arg[1].val].name);

                if (s1 == s)
                    tmp.insert(s);
                else
                {
                    for (set<string>::iterator it = vecDef[cnt].begin(); it != vecDef[cnt].end(); it++)
                        if (*it == s)
                        {
                            flag = 1;
                            break;
                        }
                    if (flag == 0)
                        tmp.insert(s);
                }

                flag = 0;
            }
        }
        vecUse.push_back(tmp);
    }

    std::vector< set<string> > vecIn(vecDef.size());
    std::vector< set<string> > vecOut(vecDef.size());
    int changed = 1;
    while (changed == 1)
    {
        changed = 0;
        int cnt = -1;
        for (Blk *blk = fn->start; blk; blk = blk->link)
        {
            cnt++;

            string blkTmp;
            vecOut[cnt].clear();
            if (blk->s1 != NULL)
            {
                blkTmp = "@" + string(blk->s1->name);

                if (blkTmp.length() != 1)
                    for (int j = 0; j < names.size(); j++)
                        if (blkTmp == names[j])
                        {
                            for (set<string>::iterator it = vecIn[j].begin(); it != vecIn[j].end(); it++)
                                vecOut[cnt].insert(*it);

                            break;
                        }
            }

            if (blk->s2 != NULL)
            {
                blkTmp = "@" + string(blk->s2->name);

                if (blkTmp.length() != 1)
                    for (int j = 0; j < names.size(); j++)
                        if (blkTmp == names[j])
                        {
                            for (set<string>::iterator it = vecIn[j].begin(); it != vecIn[j].end(); it++)
                                vecOut[cnt].insert(*it);

                            break;
                        }
            }

            set<string> tmp = vecOut[cnt];
            for (set<string>::iterator it = vecDef[cnt].begin(); it != vecDef[cnt].end(); it++)
                for (set<string>::iterator it1 = vecOut[cnt].begin(); it1 != vecOut[cnt].end(); it1++)
                    if (*it == *it1)
                        tmp.erase(*it);

            set<string> newIn = vecUse[cnt];
            for (set<string>::iterator it = tmp.begin(); it != tmp.end(); it++)
                newIn.insert(*it);


            if (newIn.size() != vecIn[cnt].size())
            {
                vecIn[cnt] = newIn;
                changed = 1;
            }
        }
    }


    for (int i = 0; i < names.size(); i++)
    {
        cout << names[i] << endl;

        cout << "\tlv_out = ";
        for (set<string>::iterator it = vecOut[i].begin(); it != vecOut[i].end(); it++)
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
