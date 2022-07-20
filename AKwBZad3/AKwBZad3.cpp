#include <iostream>
#include <string>
#include <vector>
#include <fstream>
using namespace std;



class vector_store          
{
public:
    string name;
    vector <char> sequence;
    vector <int> value;
    vector <int> position;

    vector_store(string a)  
    {
        name = a;
    }
    void Vsequence(char s)  
    {
        sequence.push_back(s);
    }
    void Vvalue(int v)
    {
        value.push_back(v); 
    }
    void Vposition(int p)
    {
        position.push_back(p);
    }
};
vector <vector_store> vectorss;

class verticle          
{
public:
    verticle() {}
    vector <char> podciag;
    vector <int> adjlist;

    string seq_name;
    int seq_num;
    int podc_num;
    int id;

    void Vpodciag(char p)       
    {
        podciag.push_back(p);
    }
    void Vseq_num(int n)    
    {
        seq_num = n;
    }
    void Vpodc_num(int u)   
    {
        podc_num = u;
    }
    void aadjlist(int l)        
    {
        adjlist.push_back(l);
    }
    void ad_id(int d)      
    {
        id = d;
    }
    void Vseq_name(string seq)
    {
        seq_name = seq;
    }

};
vector <verticle> verticless;

class cliquee
{
public:
    cliquee() {}
    vector <char> cliq_seq; 
    vector <int> cliq_num; 
    int c_id;               
    vector <int> c_podc_num; 
    vector <string> c_seq_name;     

    void Cseq(char c)
    {
        cliq_seq.push_back(c);
    }
    void Cnum(int n)
    {
        cliq_num.push_back(n);
    }
    void C_id(int id)
    {
        c_id = id;
    }
    void Cp_num(int cp)        
    {
        c_podc_num.push_back(cp);
    }
    void Cs_name(string cs)        
    {
        c_seq_name.push_back(cs);
    }
};
vector <cliquee> Cliq;

void read(string fasta, string qual)
{
    fstream file;
    string f1 = fasta + ".txt";
    string f2 = qual + ".txt";
   
    file.open(f1, ios::in); 

    if (file.good() == false)
    {
        cout << "Brak pliku do odczytu." << endl;
    }

    string line; 
    char nt; 


    while (file.good() == true)    
    {
        int p = 1; 

        getline(file, line);    
        vector_store* temp = new vector_store(line);    

        file >> nt;                                     
        if (nt == '|') { file.close(); break; cout << "Wczytano plik fasta." << endl; }

        while (nt != '>')
        {
            temp->Vsequence(nt);                    
            temp->Vposition(p);
            p++;
            file >> nt;                               
            if (nt == '>' || nt == '|')                 
            {
                break;
            }

        }

        vectorss.push_back(*temp);                          

        if (nt == '|')
        {
            file.close();
        }
    }
   

    fstream file2;
    file2.open(f2, ios::in);

    int i = 0;  
    int val;    

    if (file2.good() == false)
    {
        cout << "Brak pliku do odczytu" << endl;
    }


    while (file2.good() == true)
    {
        getline(file2, line);       

        file2 >> val;

        while (val != 0)
        {

            vectorss[i].Vvalue(val);   
            file2 >> val;
            if (val == 0)    
            {
                i++;
                break;
            }

        }
        if (val == '|') 
        {
            file2.close();
        }
        getline(file2, line);

    }


    for (int i = 1; i < vectorss.size(); i++)    
    {
        vectorss[i].name = ">" + vectorss[i].name;
    }

    return;
}

void modyfy_sequence(int modyfy)
{


    for (int i = 0; i < vectorss.size(); i++)
    {
        for (int j = 0; j < vectorss[i].value.size(); j++)
        {
            if (vectorss[i].value[j] < modyfy)
            {
                vectorss[i].value.erase(vectorss[i].value.begin() + j);
                vectorss[i].sequence.erase(vectorss[i].sequence.begin() + j);
                vectorss[i].position.erase(vectorss[i].position.begin() + j);
                j = 0;           
            }
        }
    }

    return;
}

void creat_vert(int podc)
{
    int id = 0;
    for (int i = 0; i < vectorss.size(); i++)
    {
        for (int j = 0; j < vectorss[i].sequence.size() - podc; j++) 
        {
            verticle* temp = new verticle;


            for (int k = 0; k < podc; k++)
            {
                temp->Vpodciag(vectorss[i].sequence[j + k]);   

            }
            temp->Vseq_num(i);                           
            temp->Vpodc_num(vectorss[i].position[j]);    
            temp->ad_id(id);                            
            temp->Vseq_name(vectorss[i].name);         

            verticless.push_back(*temp);
            id++;
        }
    }

    return;
}

void creat_graph()
{
    int V = verticless.size();

    for (int i = 0; i < verticless.size(); i++)  
    {

        for (int j = i +1; j < verticless.size(); j++)  
        {
            int N = 0;                              

            if (i != j)                              
            {
                
                if (verticless[i].podc_num - verticless[j].podc_num < 10 * verticless[i].podciag.size()|| verticless[j].podc_num - verticless[i].podc_num < 10* verticless[i].podciag.size())
                {
                    if (verticless[i].seq_num != verticless[j].seq_num)
                    {
                        for (int k = 0; k < verticless[i].podciag.size(); k++)
                        {
                            if (verticless[i].podciag[k] == verticless[j].podciag[k])    
                            {
                                N++;

                            }
                        }
                    }
                }    
               
                if (N == verticless[i].podciag.size())      
                {
                    verticless[i].aadjlist(j);

                }
            }
        }
    }
    return;
}

void clique()
{
    bool sequences[5];
    bool fine = true;
    int id = 1;

    for (int i = 0; i < verticless.size(); i++)
    {
        cliquee* temp = new cliquee;

        for (int b = 0; b < 5; b++)
        {
            sequences[b] = false;
        }

        if (verticless[i].adjlist.size() >= 4)   
        {
            for (int j = 0; j < verticless[i].adjlist.size(); j++)   
            {
              
                if (verticless[i].seq_num == 0)
                {
                    sequences[0] = true;
                }
                for (int d = 1; d < 5; d++)
                {
                    if (verticless[verticless[i].adjlist[j]].seq_num == d)
                    {
                        sequences[d] = true;
                    }
                }
                
            }

            for (int k = 0; k < 5; k++)          
            {
                if (sequences[k] == false)
                {
                    fine = false;
                }

            }
            if (fine == true)
            {
                temp->cliq_seq = verticless[i].podciag;     
                temp->Cnum(verticless[i].id);               

                temp->C_id(id);                             
                id++;
                for (int g = 0; g < verticless[i].adjlist.size(); g++)   
                {
                    temp->Cnum(verticless[i].adjlist[g]);
                }

                Cliq.push_back(*temp);              
            }


        }

    }
    
    for (int i = 0; i < Cliq.size(); i++)            
    {
        for (int j = 0; j < Cliq[i].cliq_num.size(); j++)
        {
            for (int k = 0; k < verticless.size(); k++)
            {
                if (Cliq[i].cliq_num[j] == verticless[k].id) 
                {
                    Cliq[i].Cs_name(verticless[k].seq_name); 
                    Cliq[i].Cp_num(verticless[k].podc_num); 
                }
            }
        }
    }


    return;
}

int main()
{
    int modyfy; 
    int podc;   
    int C_lenght_Max = 0; 
    int C_lenght_Min = 0; 
    string file1;
    string file2;


    cout << "Podaj nazwe pliku fastq do odczytu: ";
    cin >> file1;
    cout << "Podaj nazwe pliku qual do odczytu: ";
    cin >> file2;

    read(file1, file2);
    cout << "Podaj wartosc wiarygodnosci: " << endl;
    cin >> modyfy;
    modyfy_sequence(modyfy);

    cout << "Podaj dlugosc podciagow w zakresie od 4 do 9: " << endl;
    cin >> podc;
    while (podc < 4 || podc > 9)
    {
        cout << "Podana wartosc jest nieprawidlowa. Podaj wartosc podciagow w zakresie od 4 do 9." << endl;
        cin >> podc;
    }
    creat_vert(podc);
    creat_graph();
    clique();

    
    for (int i = 0; i < Cliq.size(); i++)
    {
        if (Cliq[i].cliq_num.size() > C_lenght_Max)
        {
            C_lenght_Max = Cliq[i].cliq_num.size();
        }
    }
    cout << "Ilosc znalezionych klik: " << Cliq.size() << endl;
     //for (int i = 0; i < Cliq.size(); i++)
    //{
        cout << "Klika nr: " << Cliq[0].c_id << endl;
        cout << "--------------" << endl;
        cout << "Sekwencja kliki" << endl;
        cout << "--------------" << endl;
        for (int j = 0; j < Cliq[0].cliq_seq.size(); j++)
        {
            cout << Cliq[0].cliq_seq[j] << " ";
        }
        cout << "" << endl;
        cout << "--------------" << endl;
        cout << "Elementy kliki:" << endl;
        for (int k = 0; k < Cliq[0].cliq_num.size(); k++)
        {
            cout << "--------------" << endl;
            cout << "Nazwa sekwencji: " << Cliq[0].c_seq_name[k] << " |Numer podciagu z sekwencji oryginalnej: " << Cliq[0].c_podc_num[k] << endl;

        }
        cout << "--------------" << endl;
        cout << "" << endl;

    //}



    return 0;
}
