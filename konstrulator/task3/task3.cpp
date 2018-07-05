
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
    // for (Blk *blk = fn->start; blk != NULL; blk = blk->link)
    // {   
    //     std::cout << "@" << blk->name << std::endl;
    //     for (int j = 0; j < blk->nins; j++) {
    //         Ins &ins = blk->ins[j];
            
    //         std::string arg_0(fn->tmp[ins.arg[0].val].name);
    //         std::string arg_1(fn->tmp[ins.arg[1].val].name);
    //         std::string to(fn->tmp[ins.to.val].name);
            
    //         std::cout << to << " = ";
    //         std::cout << optab[ins.op].name; 
    //         std::cout << "(" << arg_0 << "," << arg_1 << ")" << std::endl;
        
    //     }
    //     std::cout << std::endl << std::endl;                                                                                                                   
    // }

    cout << endl << endl << endl;


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


    for (int i = 0; i < names.size(); i++)
    {
        cout << names[i] << endl;

        cout << "\tdef = ";
        for (set<string>::iterator it = vecDef[i].begin(); it != vecDef[i].end(); it++)
            cout << *it << " ";
        
        cout << endl;

        cout << "\tuse = ";
        for (set<string>::iterator it = vecUse[i].begin(); it != vecUse[i].end(); it++)
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