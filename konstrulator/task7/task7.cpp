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


    std::vector< set<string> > DF(names.size());
    std::vector< string > iDom(names.size());

    for (int i = 0; i < iDom.size(); i++)
    {
        int flag = 0;
        if (names[i] == first)
            continue;

        set<string> tmp = dom[i];

        tmp.erase(names[i]);
        for (set<string>::iterator it1 = tmp.begin(); it1 != tmp.end(); it1++)
        {
            for (int j = 0; j < names.size(); j++)
                if (*it1 == names[j])
                {
                    set<string> tmpInter;
                    for (set<string>::iterator it2 = tmp.begin(); it2 != tmp.end(); it2++)
                        for (set<string>::iterator it3 = dom[j].begin(); it3 != dom[j].end(); it3++)
                            if (*it2 == *it3)
                                tmpInter.insert(*it2);

                    if (tmpInter == tmp)
                    {
                        flag = 1;
                        iDom[i] = names[j];
                    }
                    break;
                }

            if (flag == 1)
                break;
        }
    }
    
    for (Blk *blk = fn->start; blk; blk = blk->link)
    {
        if (blk->npred > 1)
        {
            int num = 0;
            string curName = "";
            for (int j = 0; j < names.size(); j++)
                if ("@" + string(blk->name) == names[j])
                {
                    curName = names[j];
                    num = j;
                    break;
                }

            for (int i = 0; i < blk->npred; i++)
            {
                string blkTmp = "@" + string((blk->pred)[i]->name);

                for (int j = 0; j < names.size(); j++)
                    if (blkTmp == names[j])
                    {
                        string point = blkTmp;
                        int dot = 0;

                        while (point != iDom[num])
                        {
                            for (int k = 0; k < names.size(); k++)
                                if (point == names[k])
                                {
                                    dot = k;
                                    break;
                                }

                            DF[dot].insert(curName);
                            point = iDom[dot];
                        }
                        break;
                    }
            }
        }
    }

    for (int i = 0; i < names.size(); i++)
    {
        cout << names[i] << ":\t";

        for (set<string>::iterator it = DF[i].begin(); it != DF[i].end(); it++)
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