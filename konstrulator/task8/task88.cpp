#define export exports
extern "C" {
    #include "../qbe/include/qbe/all.h"
    // #include <qbe/all.h>
}
#undef export

// #include "../qbe/include/qbe/all.h"
// #include <qbe/all.h>

#include <stdio.h>
#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <string>
#include <algorithm>

using namespace std;


// static void readfn (Fn *fn)
// {
//     std::vector<string> names;
//     map<string, set<string> > pred;

//     string first = "";
//     string last = "";
//     for (Blk *blk = fn->start; blk; blk = blk->link)
//     {
//         if (blk->npred == 0)
//             first = "@" + string(blk->name);
//         names.push_back("@" + string(blk->name));

//         if ((blk->s1 == NULL) && (blk->s2 == NULL))
//             last = "@" + string(blk->name);

//         set<string> emp;
//         pred[string("@" + string(blk->name))] = emp;
//     }

//     for (Blk *blk = fn->start; blk; blk = blk->link)
//     {
//         if (blk->s1 != NULL)
//             pred[string("@" + string(blk->name))].insert(string("@" + string(blk->s1->name)));
//         if (blk->s2 != NULL)
//             pred[string("@" + string(blk->name))].insert(string("@" + string(blk->s2->name)));
//     }

//     std::vector< set<string> > dom;
//     std::vector<string> namesDom;

//     for (int i = 1; i < names.size(); i++)
//         namesDom.push_back(names[i]);

//     set<string> firstSet;

//     firstSet.clear();
//     for (int i = 0; i < names.size(); i++)
//         firstSet.insert(names[i]);

//     for (int i = 0; i < names.size(); i++)
//     {
//         if (names[i] == last)
//         {
//             set<string> tmp;
//             tmp.insert(last);
//             dom.push_back(tmp);
//             continue;
//         }
//         dom.push_back(firstSet);
//     }

//     int changed = 1;
//     while (changed == 1)
//     {
//         changed = 0;
//         int cnt = -1;
//         // for (Blk *blk = fn->start; blk; blk = blk->link)
//         for (int k = 0; k < names.size(); k++)
//         {
//             cnt++;
//             if (names[k] == last)
//                 continue;

//             set<string> intersectDoms;
//             set<string> tmp = firstSet;

//             set<string> curPred = pred[names[k]];

//             // for (int i = 0; i < blk->npred; i++)
//             for (set<string>::iterator itK = curPred.begin(); itK != curPred.end(); itK++)
//             {
//                 string blkTmp = "@" + string(*itK);

//                 for (int j = 0; j < names.size(); j++)
//                     if (blkTmp == names[j])
//                     {
//                         for (set<string>::iterator it = tmp.begin(); it != tmp.end(); it++)
//                             for (set<string>::iterator it1 = dom[j].begin(); it1 != dom[j].end(); it1++)
//                                 if (*it == *it1)
//                                     intersectDoms.insert(*it);
//                         break;
//                     }

//                 tmp = intersectDoms;
//                 intersectDoms.clear();
//             }
//             intersectDoms = tmp;
//             intersectDoms.insert(names[cnt]);

//             if (intersectDoms != dom[cnt])
//             {
//                 dom[cnt] = intersectDoms;
//                 changed = 1;
//             }
//         }
//     }

//     std::vector< set<string> > reverseDom(names.size());
//     for (int i = 0; i < names.size(); i++)
//     {
//         for (set<string>::iterator it = dom[i].begin(); it != dom[i].end(); it++)
//             for (int j = 0; j < names.size(); j++)
//                 if (*it == names[j])
//                     reverseDom[j].insert(names[i]);
//     }

//     std::vector< set<string> > DF(names.size());
//     std::vector< string > iDom(names.size());

//     for (int i = 0; i < iDom.size(); i++)
//     {
//         int flag = 0;
//         if (names[i] == last)
//             continue;

//         set<string> tmp = dom[i];

//         tmp.erase(names[i]);
//         for (set<string>::iterator it1 = tmp.begin(); it1 != tmp.end(); it1++)
//         {
//             for (int j = 0; j < names.size(); j++)
//                 if (*it1 == names[j])
//                 {
//                     set<string> tmpInter;
//                     for (set<string>::iterator it2 = tmp.begin(); it2 != tmp.end(); it2++)
//                         for (set<string>::iterator it3 = dom[j].begin(); it3 != dom[j].end(); it3++)
//                             if (*it2 == *it3)
//                                 tmpInter.insert(*it2);

//                     if (tmpInter == tmp)
//                     {
//                         flag = 1;
//                         iDom[i] = names[j];
//                     }
//                     break;
//                 }

//             if (flag == 1)
//                 break;
//         }
//     }
    
//     // for (Blk *blk = fn->start; blk; blk = blk->link)
//     for (int k = 0; k < names.size(); k++)
//     {
//         set<string> curPred = pred[names[k]];
//         // if (blk->npred > 1)
//         if (curPred.size() > 1)
//         {
//             int num = 0;
//             string curName = "";
//             for (int j = 0; j < names.size(); j++)
//                 if ("@" + string(names[k]) == names[j])
//                 {
//                     curName = names[j];
//                     num = j;
//                     break;
//                 }

//             // for (int i = 0; i < blk->npred; i++)
//             for (set<string>::iterator itK = curPred.begin(); itK != curPred.end(); itK++)
//             {
//                 string blkTmp = "@" + string(*itK);

//                 for (int j = 0; j < names.size(); j++)
//                     if (blkTmp == names[j])
//                     {
//                         string point = blkTmp;
//                         int dot = 0;

//                         while (point != iDom[num])
//                         {
//                             for (int k = 0; k < names.size(); k++)
//                                 if (point == names[k])
//                                 {
//                                     dot = k;
//                                     break;
//                                 }

//                             DF[dot].insert(curName);
//                             point = iDom[dot];
//                         }
//                         break;
//                     }
//             }
//         }
//     }

//     for (int i = 0; i < names.size(); i++)
//     {
//         cout << names[i] << ":\t";

//         for (set<string>::iterator it = DF[i].begin(); it != DF[i].end(); it++)
//             cout << *it << " ";
//         cout << endl;
//     }
// }

static void readfn (Fn *fn) {
    fillrpo(fn);
    fillpreds(fn);
    filluse(fn);
    ssa(fn);

    map<string, set<string> > pred;
    map<string, set<string> > dom;
    string start;
    string end;

    // for (Blk *blk = fn->start; blk; blk = blk->link) {
    //     if(blk->npred == 0)
    //         start = blk->name;

    //     set<string> tmp;
    //     for (int i = 0; i < blk->npred; i++)
    //         tmp.insert(string((blk->pred)[i]->name));
    //     pred[string(blk->name)] = tmp;
    // }

    // map<string, set<string> > def;
    map<string, pair<string, int> > def;
    for (Blk *blk = fn->start; blk; blk = blk->link) {
        if (blk->npred == 0)    
            start = string(blk->name);
        set<string> emp;
        pred[string(blk->name)] = emp;
    }

    for (Blk *blk = fn->start; blk; blk = blk->link)
    {
        pair<string, int> pa;

        for (int i = 0; i < blk->nins; i++)
        {
            if (blk->nins && Tmp0 <= blk->ins[i].to.val)
            {
                string s = string(fn->tmp[blk->ins[i].to.val].name);
                pa = make_pair(string(blk->name), i);
                def[s] = pa;
            }
        }
    }

    set<string> endSet;
    for (Blk *blk = fn->start; blk; blk = blk->link) {
        if (blk->s1 != NULL)
            pred[string(blk->name)].insert(string(blk->s1->name));
        if (blk->s2 != NULL)
            pred[string(blk->name)].insert(string(blk->s2->name));

        if ((blk->s1 == NULL) && (blk->s2 == NULL))
            // end = string(blk->name);
            endSet.insert(string(blk->name));
    }
    pred["commonExit"] = endSet;
    start = string("commonExit");

    dom[start].insert(start);
    set<string> V;
    for(map<string, set<string> >::iterator it = pred.begin(); it != pred.end(); it++)
        V.insert(it->first);
    for(map<string, set<string> >::iterator it = pred.begin(); it != pred.end(); it++)
        if (it->first != start)
            dom[it->first] = V;

    bool changes = true;
    while(changes) {
        changes = false;
        for(map<string, set<string> >::iterator it = dom.begin(); it != dom.end(); it++) {
            if (it->first == start)
                continue;

            set<string> tmp;
            if (pred[it->first].size() != 0){
                set<string>::iterator it2 = pred[it->first].begin();
                tmp = dom[*it2];
                it2++;
                for(; it2 != pred[it->first].end(); it2++) {
                    set<string> inter;
                    set_intersection(tmp.begin(), tmp.end(), dom[*it2].begin(), dom[*it2].end(), inserter(inter, inter.begin()));
                    tmp = inter;
                }
            }
            tmp.insert(it->first);

            if (tmp != it->second) {
                changes = true;
                it->second = tmp;
            }
        }
    }

    map<string, set<string> > result;
    for(map<string, set<string> >::iterator it = dom.begin(); it != dom.end(); it++)
        for(set<string>::iterator it2 = (it->second).begin(); it2 != (it->second).end(); it2++)
            result[*it2].insert(it->first);

    //Вычисление idom 
    map<string, string> idom;
    for(map<string, set<string> >::iterator it = result.begin(); it != result.end(); it++)
        idom[it->first] = "";

    for(map<string, string>::iterator it = idom.begin(); it != idom.end(); it++) {
        if(it->first == start)
            continue;

        set<string> tmp = dom[it->first];
        tmp.erase(it->first);
        for (set<string>::iterator it1 = tmp.begin(); it1 != tmp.end(); it1++) {
            set<string> intersect;
            set_intersection(tmp.begin(), tmp.end(), dom[*it1].begin(), dom[*it1].end(), inserter(intersect, intersect.begin()));
            if (intersect == tmp) {
                idom[it->first] = *it1;
                break;
            }
        }
    }

    //Вычисление границ доминирования
    map<string, set<string> > DF;
    for(map<string, set<string> >::iterator it = result.begin(); it != result.end(); it++)
        DF[it->first] = set<string>();
    
    for(map<string, set<string> >::iterator it = DF.begin(); it != DF.end(); it++) {
        if (pred[it->first].size() <= 1)
            continue;

        // if(it->first == start)
        //     continue;

        for (set<string>::iterator it2 = pred[it->first].begin(); it2 != pred[it->first].end(); it2++) {
            string r = *it2;
            while(r != idom[it->first]) {
                DF[r].insert(it->first);
                r = idom[r];
            }
        }
    }

    // DF[start] = set<string>();
    // cout << DF.size() << endl;
    // for(map<string, set<string> >::iterator it = DF.begin(); it != DF.end(); it++) {
    //     cout << "@" << it->first << ": ";
    //     // cout << (it->second).size();
    //     for(set<string>::iterator it2 = (it->second).begin(); it2 != (it->second).end(); it2++)
    //         cout << "@" << *it2 << " ";
    //     cout << endl;
    // }


    //Функция Mark
    set< pair<string, int> > workList;
    set< pair<string, int> > mark;
    for (Blk *blk = fn->start; blk; blk = blk->link)
    {
        pair<string, int> pa;
        for (int i = 0; i < blk->nins; i++)
        {
            pa = make_pair(string(blk->name), i);
            mark.erase(pa);
            // cout << blk->ins[i].to.val << " " << blk->ins[i].arg[0].val << " " << blk->ins[i].arg[1].val << endl;

            // cout << blk->nins << endl;
            // cout << pa.first << " " << pa.second << endl;
            // function call
            if ((blk->ins[i].op == Ocall) || (blk->ins[i].op == Ovacall))
            {
                mark.insert(pa);
                workList.insert(pa);
                
            // add args
                for (int i0 = i - 1; (i0 >= 0) && (isarg(blk->ins[i0].op)); i0--)
                {
                    mark.insert(pa);
                    workList.insert(pa);
                }
            // memory
            }
            else if (isstore(blk->ins[i].op))
            {
                mark.insert(pa);
                workList.insert(pa);
            }
        }

        // isret
        if (isret(blk->jmp.type) || (blk->jmp.type > 6))
        {
            // cout << "Test " << blk->name << endl;
            mark.insert(make_pair(string(blk->name), -1));
            workList.insert(make_pair(string(blk->name), -1));
        }

        for (Phi *phi = blk->phi; phi; phi = phi->link)
        {
            pa = make_pair(string(blk->name), -2);
            mark.insert(pa);
            workList.insert(pa);
        }
    }

    cout << "!!" << endl;
    cout << mark.size();
    for(set< pair<string, int> >::iterator it = mark.begin(); it!=mark.end(); it++) {
        cout << it->first << " " << it->second << endl;
    }
    cout << "!!" << endl;

    while (workList.size() != 0)
    {
        pair<string, int> pa;
        pa = *(workList.begin());

        workList.erase(pa);

        if ((pa.second != -1) && (pa.second != -2))
            for (Blk *blk = fn->start; blk; blk = blk->link)
            {
                cout << "???" << endl;
                if (string(blk->name) == pa.first)
                {   
                    string s = string(fn->tmp[blk->ins[pa.second].arg[0].val].name);
                    cout << s << endl;
                    if (s != "")
                    {
                        pair<string, int> pa1;
                        pa1 = def[s];
                        if (mark.find(pa1) == mark.end())
                        {
                            mark.insert(pa1);
                            workList.insert(pa1);
                        }
                        // mark.insert(pa1);
                    }

                    s = string(fn->tmp[blk->ins[pa.second].arg[1].val].name);
                    cout << s << endl;
                    if (s != "")
                    {
                        pair<string, int> pa1;
                        pa1 = def[s];
                        if (mark.find(pa1) == mark.end())
                        {
                            mark.insert(pa1);
                            workList.insert(pa1);
                        }
                        // mark.insert(pa1);
                    }
                }
            }
        else
        if (pa.second != -2)
            for(set<string>::iterator it = (DF[pa.first]).begin(); it != (DF[pa.first]).end(); it++)
            {
                for (Blk *blk = fn->start; blk; blk = blk->link)
                {
                    if (string(blk->name) == *it)
                    {
                        pair<string, int> pa;

                        pa = make_pair(string(blk->name), -1);
                        if (mark.find(pa) == mark.end())
                        {
                            mark.insert(pa);
                            workList.insert(pa);
                        }
                    }       
                }
            }
        else
            for(set<string>::iterator it = (DF[pa.first]).begin(); it != (DF[pa.first]).end(); it++)
            {
                for (Blk *blk = fn->start; blk; blk = blk->link)
                {
                    if (string(blk->name) == *it)
                    {
                        pair<string, int> pa;

                        pa = make_pair(string(blk->name), -2);
                        if (mark.find(pa) == mark.end())
                        {
                            mark.insert(pa);
                            workList.insert(pa);
                        }
                    }       
                }
            }
    }

    // set< pair<string, int> > mark;
    cout << "!!" << endl;
    cout << mark.size();
    for(set< pair<string, int> >::iterator it = mark.begin(); it!=mark.end(); it++) {
        cout << it->first << " " << it->second << endl;
    }
    cout << "!!" << endl;

    for (Blk *blk = fn->start; blk; blk = blk->link)
    {
        pair<string, int> pa;
        pa = make_pair(string(blk->name), -1);
        if (mark.find(pa) == mark.end())
        {
            blk->jmp.type = Onop;
            blk->jmp.arg = R;
        }

        for (int i = 0; i < blk->nins; i++)
        {
            pair<string, int> pa;
            pa = make_pair(string(blk->name), i);
            if (mark.find(pa) == mark.end())
            {
                blk->ins[i].op = Onop;
                blk->ins[i].to = R;
                blk->ins[i].arg[0] = R;
                blk->ins[i].arg[1] = R;
            }
        }
    }


    fillpreds(fn);
    fillrpo(fn);
    printfn(fn, stdout);
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