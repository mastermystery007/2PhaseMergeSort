#include<bits/stdc++.h>
#include <iostream> 
using namespace std;

void create_initial_runs( char* input_file, int run_size, int num_ways);
void do_final_merge(char* output_file , int run_size,int num_ways);
//void swap(MinHeapNode* x, MinHeapNode* y); 



vector<int> sizes; //get from metadata
vector<int> col_index={1}; //get from metadata
int curr_scratch_file=3;
int no_of_cols;


class Tuple 
{
    public:
        string t;
        int index;
        Tuple(string t,int in): t(t),index(in){}
};
struct CompareHeight {
    bool operator()(Tuple const& p1, Tuple const& p2)
    {
        // return "true" if "p1" is ordered 
        // before "p2", for example:
        return p1.t < p2.t;
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
    int run_size = 50; //number of rows in a file 
    int num_ways = 2;  //number of scratch files you want

    //create_initial_runs(input_file,run_size,num_ways);

    do_final_merge(output_file,run_size,num_ways);

     

  

}

void do_final_merge(char* output_file , int run_size,int num_ways)
{
    // for(int i=0;i<curr_scratch_file;i++)
    // {
    //     cout<<i <<" scratch file present"<<"\n";
    // }

    priority_queue<Tuple, vector<Tuple>, CompareHeight> Q;
   

    FILE* in[curr_scratch_file]; 
    for (int i = 0; i <curr_scratch_file-1; i++) { 
        char fileName[10]; 
  
        // convert i to string 
        snprintf(fileName, sizeof(fileName),"%d", i); 
  
        // Open output files in read mode. 
        in[i] = openFile(fileName, "r"); 
    } 
  
    
    // FINAL OUTPUT FILE 
    FILE* out = openFile(output_file, "wa"); 
  
    // Create a min heap with k heap 
    // nodes. Every heap node 
    // has first element of scratch 
    // output file 
    
    int i; 
    string s;
    char line[256];
    cout<<"first heap elements \n";
    for (i = 0;i<curr_scratch_file-1; i++)
    { 
    // break if no output file is empty and 
    // index i will be no. of input files 
    fgets(line, sizeof(line), in[i]);


    cout<<"my data is "<<line<<"\n";
    Tuple tt(line,i);
    // tt.index=i;
    // tt.t=line;
    Q.push(tt); 
    
    //cout<<line<<" ";
        
    // Index of scratch output file 

    } 

    cout<<"****";
 
  
    int count = 0; 

    int p=7;
    while (p>0) { 
        // Get the minimum element 
        // and store it in output file 
        
        Tuple temp = Q.top();
        Q.pop();
        string toput = temp.t; 
        cout<<toput<<"\n";
        fputs(toput.c_str(),out);

        
        
        // Find the next element that 
        // will replace current 
        // root of heap. The next element 
        // belongs to same 
        // input file as the current min element. 
        fgets(line, sizeof(line), in[temp.index]);

        Tuple tuppy(line,temp.index);
         Q.push(tuppy);

        // if (fgets(in[root.i], "%s",&root.element)!= 1)
        // { 
        //     fgets(line, sizeof(line), in[root.i]);
        //     root.element = INT_MAX; 
        //     count++; 
        // } 
  
        // Replace root with next 
        // element of input file 
        
        p--;
    } 
  
    // close input and output files
    for (int i = 0; i <curr_scratch_file-1; i++){fclose(in[i]);}
     
  
    fclose(out);  

}


void create_initial_runs( char* input_file, int run_size, int num_ways) 
{ 

    fstream in;
    in.open(input_file,ios::in);


    
    char fileName[10]; 
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
            for(int j=0;j<tuples[0].size();j++)
            {
                t<<tuples[i][j]<<"   ";//convert to string and remove last 2 chars
            }
            t<<"\n";
        }
        t.close();


        //((FILE)*scratch[0]).write(tuples[0][0]);
        //scratch<<tuples[0][0];
        curr_scratch_file++; 
    }
    //out.close();

} 
