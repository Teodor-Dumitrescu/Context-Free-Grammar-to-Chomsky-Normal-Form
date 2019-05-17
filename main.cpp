#include <iostream>
#include <queue>
#include <fstream>
#include <string.h>

using namespace std;

ifstream f("FNC.in");
ofstream g("FNC.out");

char terminale[30],prod[300][100][100],treplace[30];
unsigned char treplace2[30];
unsigned char neterminale[30];
int cprod[300],nt,t,nr_prod;
char S,net;
queue <char> coada;
///prod: 300 pt neterminale, 100 pt cate productii, 100 pt lungimea unei productii
int neterminale_folosite[1000];
unsigned char NET;

unsigned char get_NET()
{
    unsigned char i;
    i='A';
    while(neterminale_folosite[i]==1)
        i++;
    neterminale_folosite[i]=1;
    return i;
}
void afisare()
{
    int i,j;
    g<<"Nr. neterminale: "<<nt<<endl;
    for(i=0;i<nt;i++)
        g<<neterminale[i]<<" ";
    g<<endl;
    g<<"Nr. terminale: "<<t<<endl;
    for(i=0;i<t;i++)
        g<<terminale[i]<<" ";
    g<<endl;
    g<<"Simbol de start: "<<S<<endl;
    g<<"Nr. productii: "<<nr_prod<<endl;
    for(i=0;i<nt;i++)
    {
        g<<neterminale[i]<<" -> ";
        for(j=0;j<cprod[neterminale[i]];j++)
        {
            g<<prod[neterminale[i]][j]<<" ";
        }
        g<<endl;
    }
}
void add$()
{
    int i,j,p=0;
    for(i=0;i<nt;i++)
    {
        for(j=0;j<cprod[neterminale[i]];j++)
        {
            if(strchr(prod[neterminale[i]][j],S))
                p=1;
        }
    }
    if(p==1)
    {
        nt++;
        neterminale[nt-1]='$';
        prod['$'][0][0]=S;
        cprod['$']++;
        nr_prod++;
        S='$';
        neterminale_folosite[S]=1;
    }
}
void divide(char target, char net , int contor, int stop, char sir[100],int l)
{
    char aux[100],i=0,j=0;
    //cout<<sir<<endl;
    if(contor==stop)
    {
        if(strlen(sir)==0)
            coada.push(net);
        if(strcmp(prod[net][l],sir)!=0 && strlen(sir)>0)
        {
            cprod[net]++;
            nr_prod++;
            strcpy(prod[net][cprod[net]-1],sir);
        }
        return;
    }
    while(i<contor+1-(strlen(prod[net][l])-strlen(sir)))
    {
        if(sir[j]==target)
            i++;
        j++;
    }
    j--;
    strcpy(aux,sir);
    strcpy(aux+j,aux+j+1);
    divide(target,net,contor+1,stop,sir,l);
    divide(target,net,contor+1,stop,aux,l);
}
void null_production_removal()
{
    int i,j,k,p,lim,stop;
    char net,target;
    for(i=0;i<nt;i++)
    {
        net=neterminale[i];
        lim=cprod[net];
        for(j=0;j<lim;j++)
        {
            if(prod[net][j][0]=='#')
                coada.push(net);
        }
    }
    while(coada.empty()==0)
    {
        target=coada.front();
        coada.pop();
        p=0;
        while(p<cprod[target] && prod[target][p][0]!='#')
            p++;
        if(p<cprod[target]);
        {
            for(i=p;i<cprod[target]-1;i++)
                strcpy(prod[target][i],prod[target][i+1]);
            cprod[target]--;
            nr_prod--;
        }
        for(i=0;i<nt;i++)
        {
            net=neterminale[i];
            lim=cprod[net];
            for(j=0;j<lim;j++)
            {
                if(strchr(prod[net][j],target))
                {
                    if(strlen(prod[net][j])==1)
                    {
                        cprod[net]++;
                        nr_prod++;
                        strcpy(prod[net][cprod[net]-1],"#");
                        coada.push(net);
                    }
                    else
                    {
                        stop=0;
                        for(k=0;k<strlen(prod[net][j]);k++)
                            if(prod[net][j][k]==target)
                                stop++;
                        divide(target,net,0,stop,prod[net][j],j);
                    }
                }
            }
        }
    }
}
void remove_duplicates()
{
    int i,j,k;
    for(i=0;i<nt;i++)
    {
        net=neterminale[i];
        for(j=0;j<cprod[net]-1;j++)
        {
            for(k=j+1;k<cprod[net];k++)
            {
                if(strcmp(prod[net][j],prod[net][k])==0)
                {
                    strcpy(prod[net][k],"out");
                }
            }
        }
        for(j=0;j<cprod[net];j++)
        {
            if(strcmp(prod[net][j],"out")==0)
            {
                for(k=j;k<cprod[net]-1;k++)
                    strcpy(prod[net][k],prod[net][k+1]);
                cprod[net]--;
                nr_prod--;
                j--;
            }
        }
    }
}
void remove_unit_productions()
{
    int i,j,k,l,net,p;
    for(i=0;i<nt;i++)                       ///pentru fiecare neterminal
    {
        //cout<<neterminale[i]<<endl;
        net=neterminale[i];
        for(j=0;j<cprod[net];j++)          ///pentru fiecare productie
        {
            p=0;
            if(strlen(prod[net][j])==1)     ///daca productia are lungime 1
            {
                for(k=0;k<nt;k++)
                {
                    if(prod[net][j][0]==neterminale[k])
                        p=1;
                }
            }
            if(p==1)                        ///daca productia este un neterminal
            {
                if(net==prod[net][j][0])
                {
                    for(k=j;k<cprod[net]-1;k++)
                        strcpy(prod[net][k],prod[net][k+1]);
                    cprod[net]--;
                    nr_prod--;
                    j--;
                }
                else
                {
                    for(k=0;k<cprod[prod[net][j][0]];k++)
                    {
                        cprod[net]++;
                        nr_prod++;
                        strcpy(prod[net][cprod[net]-1],prod[prod[net][j][0]][k]);
                    }
                    for(k=j;k<cprod[net]-1;k++)
                        strcpy(prod[net][k],prod[net][k+1]);
                    cprod[net]--;
                    nr_prod--;
                    j--;
                }
            }
        }
    }
}
void replace_terminals()
{
    int i,j,k,l;
    for(i=0;i<t;i++)                                                     ///pentru fiecare terminal
    {
        for(j=0;j<nt;j++)                                                ///pentru fiecare neterminal
        {
            for(k=0;k<cprod[neterminale[j]];k++)                         ///pentru fiecare productie
            {
                if(strlen(prod[neterminale[j]][k])>1)                    ///daca productia are mai mult de 1 litera
                {
                    for(l=0;l<strlen(prod[neterminale[j]][k]);l++)       ///pentru fiecare litera din productie
                        if(prod[neterminale[j]][k][l]==terminale[i])
                            {
                                if(treplace[i]==0)
                                {
                                    treplace[i]=1;
                                    NET=get_NET();
                                    treplace2[i]=NET;
                                    prod[neterminale[j]][k][l]=NET;
                                }
                                else
                                {
                                    prod[neterminale[j]][k][l]=treplace2[i];
                                }

                            }
                }
            }
        }
    }
    for(i=0;i<t;i++)
    {
        if(treplace[i]==1)                          ///adauga terminale 'Xa' pentru fiecare 'a' din productie >1
        {
            nt++;
            neterminale[nt-1]=treplace2[i];
            strcpy(prod[neterminale[nt-1]][0]," ");
            prod[neterminale[nt-1]][0][0]=terminale[i];
            cprod[neterminale[nt-1]]++;
            nr_prod++;
        }
    }
}
void replace_nonterminals()
{
    int i,j,net,lim;
    for(i=0;i<nt;i++)
    {
        net=neterminale[i];
        lim=cprod[net];
        for(j=0;j<lim;j++)
        {
            if(strlen(prod[net][j])>2)
            {
                nt++;
                NET=get_NET();
                neterminale[nt-1]=NET;
                cprod[neterminale[nt-1]]++;
                nr_prod++;
                strcpy(prod[neterminale[nt-1]][0],prod[net][j]+1);
                strcpy(prod[net][j]+1,prod[net][j]+strlen(prod[net][j]));
                strcat(prod[net][j]," ");
                prod[net][j][1]=neterminale[nt-1];
            }
        }
    }
}
void remove_useless_nonterminals()
{
    int i,j,k,l,m,n;
    cout<<nt<<endl;
    for(i=0;i<nt-1;i++)
    {
        for(j=i+1;j<nt;j++)
        {
            //cout<<"i: "<<i<<" "<<neterminale[i]<<endl;
            //cout<<"j: "<<j<<" "<<neterminale[j]<<endl;
            if(strcmp(prod[neterminale[i]][0],prod[neterminale[j]][0])==0)
            {
                for(k=0;k<nt;k++)
                    for(l=0;l<cprod[neterminale[k]];l++)
                        for(n=0;n<strlen(prod[neterminale[k]][l]);n++)
                            if((unsigned char)prod[neterminale[k]][l][n]==neterminale[j])
                                prod[neterminale[k]][l][n]=neterminale[i];
                cprod[neterminale[j]]=0;
                nr_prod--;
                for(k=j;k<nt-1;k++)
                    neterminale[k]=neterminale[k+1];
                nt--;
                j--;
            }
        }
    }
}
int main()
{

    int i;
    char pro[100];
    f>>nt;
    for(i=0;i<nt;i++)
    {
        f>>neterminale[i];
        neterminale_folosite[neterminale[i]]=1;
    }
    f>>t;
    for(i=0;i<t;i++)
    {
        f>>terminale[i];
    }
    f>>S;
    f>>nr_prod;
    for(i=1;i<=nr_prod;i++)
    {
        f>>net;
        f.get();
        f.get(pro,100);
        f.get();
        strcpy(prod[net][cprod[net]],pro);
        //cout<<prod[net][cprod[net]]<<endl;
        cprod[net]++;
    }
    add$();
    null_production_removal();
    remove_unit_productions();
    //cout<<neterminale[nt-1];
    replace_terminals();
    remove_duplicates();
    //afisare();
    replace_nonterminals();
    //afisare();
    remove_useless_nonterminals();
    afisare();
    return 0;

}
