#include<bits/stdc++.h>
#include <iostream> 
using namespace std;

void create_initial_runs( char* input_file, int run_size, int num_ways);
void do_final_merge(char* output_file , int run_size,int num_ways);
//void swap(MinHeapNode* x, MinHeapNode* y); 



vector<int> sizes; //get from metadata
vector<int> col_index={1,2}; //get from metadata
int curr_scratch_file=0;
int no_of_cols;


class Tuple 
{
    public:
        string t;
        int index;
        Tuple(string t,int in): t(t),index(in){}
};
struct CompareTuples {
    bool operator()(Tuple const& p1, Tuple const& p2)
    {
        vector<string> elem1;
        vector<string> elem2;
        string elem1t = p1.t;
        string elem2t = p2.t;
        for(int i=0;i<sizes.size();i++)
        {
            string token = elem1t.substr(0,sizes[i]);
            elem1.push_back(token);
            elem1t.erase(0,  sizes[i]+2);
        }

        for(int i=0;i<sizes.size();i++)
        {
            string token = elem2t.substr(0,sizes[i]);
            elem2.push_back(token);
            elem2t.erase(0,  sizes[i]+2);
        }

        for(int i=0;i<col_index.size();i++)
        {
            if(elem1[col_index[i]] == elem2[col_index[i]]){continue;}
            return elem1[col_index[i]] > elem2[col_index[i]];
        }
        return true;
    }
};





FILE* openFile(char* fileName, char* mode) 
{ 
    FILE* fp = fopen(fileName, mode); 
    if (fp == NULL)
    { 
        perror("Error while opening the file.\n"); 
        exit(EXIT_FAILURE); 
    } 
    return fp; 
} 
bool sortcol( const vector<string>& v1,const vector<string>& v2 )
{ 
    for(int i=0;i<col_index.size();i++)
    {
        if(v1[col_index[i]] == v2[col_index[i]]){continue;}
        return v1[col_index[i]] < v2[col_index[i]];
    }
    //cout<<"v1 "<<v1[1]<<" v2 "<<v2[1]<<"\n";
     return true;
} 
  

int main()
{
    
    ifstream MyReadFile("input.txt");
    ofstream MyFile("output.txt");

    //get column data from metadata.txt
    string md;
    string delimiter = " ";
    string token;
    ifstream Metadata("metadata.txt");
    getline (Metadata,md);
    while(true)
    {
        if(md.find(delimiter)==std::string::npos)break;
        token = md.substr(0, md.find(delimiter));
        sizes.push_back(stoi(token));
        md.erase(0, md.find(delimiter) + delimiter.length());

    }
    sizes.push_back(stoi(md));
    no_of_cols = sizes.size();

    //get input file details
    char input_file[] = "input.txt"; 
    //FILE* in = openFile(input_file, "r");

    char output_file[] = "output.txt";

    

    //
    //TODO      num_scratch_files = totalsize/(runsize*size of one row)
    int run_size = 10; //number of rows in a file 
    int num_ways = 10;  //number of scratch files you want

    create_initial_runs(input_file,run_size,num_ways);

    do_final_merge(output_file,run_size,num_ways);

     

  

}

void do_final_merge(char* output_file , int run_size,int num_ways)
{

    priority_queue<Tuple, vector<Tuple>, CompareTuples> Q;
   

    FILE* in[curr_scratch_file]; 
    for (int i = 0; i <curr_scratch_file; i++)
    { 
        char fileName[10]; 
        snprintf(fileName, sizeof(fileName),"%d", i); 
        // Open output files in read mode. 
        in[i] = openFile(fileName, "r"); 
    } 
  
    
    // FINAL OUTPUT FILE 
    FILE* out = openFile(output_file, "wa"); 
    
    int i; 
    string s;
    char line[256];
    cout<<"first heap elements \n";
    for (i = 0;i<curr_scratch_file; i++)
    { 
        memset(line, 0, 256);
        if(fgets(line, sizeof(line), in[i])!=NULL)
        {
            cout<<"my data is "<<line<<"\n";
            Tuple* tt = new Tuple(line,i);
            Q.push(*tt); 
        }
        else{curr_scratch_file--;}

    } 

    cout<<"****";
 
    cout<<"current size is "<<Q.size();
    int count = 0; 

    int p=0;
    while (p<curr_scratch_file) { 
        memset(line, 0, 256);
        
        Tuple temp = Q.top();
        Q.pop();
        string toput = temp.t; 
        cout<<"removing "<<toput<<" from file "<<temp.index <<"\n";
        fputs(toput.c_str(),out);

        
        //Find the next element that will replace current root of heap. The next element belongs to same  input file as the current min element. 
        if(fgets(line, sizeof(line), in[temp.index])==NULL)
        {
            cout<<"trying for "<<temp.index<<" ";
            p++;
            continue;
        }
        else
        {
            
            
            cout<<"getting "<<line<<" from file "<<temp.index<<" \n";
            Tuple* tuppy = new Tuple(line,temp.index);
            Q.push(*tuppy);
        }
        
        
    } 
    

    // close input and output files
    for (int i = 0; i <curr_scratch_file-1; i++){fclose(in[i]);}
    fclose(out);  

}


void create_initial_runs( char* input_file, int run_size, int num_ways) 
{ 

    fstream in;
    in.open(input_file,ios::in);

    //char fileName[10]; 
    //int next_output_file=0; 
    
    bool moreinput = true;
    string line;
    int i;
    
    while(moreinput)
    {
        vector<vector<string>> tuples;

        for(int i=0;i<run_size;i++)
        {
        
        getline(in, line);
        
        if(line.size()==0){moreinput=false;break;}

        //get all strings in a line
        vector<string> temp;
        for(int i=0;i<sizes.size();i++)
        {
            string token = line.substr(0,sizes[i]);
            temp.push_back(token);
            line.erase(0,  sizes[i]+2);
        }
        tuples.push_back(temp);

        }
        //sort tuples vector
        sort(tuples.begin(), tuples.end(),sortcol);


        //write output to scratch file
        fstream t;
        t.open(to_string(curr_scratch_file),ios::out);

        for(int i=0;i<tuples.size();i++)
        {
            string  addend= "";
            for(int j=0;j<tuples[0].size();j++)
            {
                addend  =addend + tuples[i][j]+"  ";//convert to string and remove last 2 chars
            }
            
            string n_a= addend.substr(0,addend.size()-2)+"\n";
            t<<n_a;
        }
        t.close();


        //((FILE)*scratch[0]).write(tuples[0][0]);
        //scratch<<tuples[0][0];
        curr_scratch_file++; 
    }
    //out.close();

} 
