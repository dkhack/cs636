#include <cassert>
#include <iostream>
#include "csr.h"
#include <queue>
#include "omp.h"


using std::cout;
using std::endl;


void parallelBfs(graph_t& g, vid_t root)
{

	auto start = std::chrono::high_resolution_clock::now();	
	
	cout << "parallelized version bfs" << endl;
	 csr_t* csr = &g.csr;
    csr_t* csc = &g.csc;

    //TODO

    cout << "root = " << root << endl;
    
    
    vid_t* offset = csr->get_offset();
    vid_t* nbrs = csr->get_nbrs();
    
    int totalV = (int)csr->get_vcount();
    
    int visited[totalV];
    
    for (int i = 0; i < totalV; i++){
      visited[i] = 1000;
    }
    
    visited[root] = 0;
    queue<vid_t> q;
	
    q.push(root);
    
    omp_set_num_threads(totalV);
    while(!q.empty()){
    
      vid_t currNode = q.front();
      
      
      vid_t start = offset[currNode];
      vid_t end = offset[currNode + 1];
	
      #pragma omp parallel for	
      for (int i = start; i < end; i++){
      	
      	vid_t currNeb = nbrs[i];	
        if (visited[currNeb] > visited[currNode] + 1 ){
    
          visited[currNeb] = visited[currNode] + 1;
        
          q.push(currNeb);
          
       
        }
        
      }
      q.pop();
    }
 
    //cout << "root = " << root << endl;
    
   
    int levelCount = 0;
    for(int i = 0; i < totalV; i++){
      if (visited[i] > levelCount){
        levelCount = visited[i];
        
      }
    }
    
    
    //level count
    int levels[levelCount+1] = {0};
    for(int i = 0; i < totalV; i++){
      levels[visited[i]] = levels[visited[i]]+ 1;
    }
    
    //printing each level
    for(int i = 0; i < levelCount + 1; i++){
      cout<<"Level"<< i << ": " << levels[i] <<" nodes"<<endl;
    }
    
    cout<<"end of parallel"<<endl;
} 

void run_bfs(graph_t& g, vid_t root)
{
    cout <<"Inside single thread bfs"<<endl;	
    csr_t* csr = &g.csr;
    csr_t* csc = &g.csc;

    //TODO

    cout << "root = " << root << endl;
    
    
    vid_t* offset = csr->get_offset();
    vid_t* nbrs = csr->get_nbrs();
    
    int totalV = (int)csr->get_vcount();
    
    int visited[totalV];
    
    for (int i = 0; i < totalV; i++){
      visited[i] = 1000;
    }
    
    visited[root] = 0;
    queue<vid_t> q;
	
    q.push(root);
    while(!q.empty()){
    
      vid_t currNode = q.front();
      
      
      vid_t start = offset[currNode];
      vid_t end = offset[currNode + 1];

      for (int i = start; i < end; i++){
      	
      	vid_t currNeb = nbrs[i];	
        if (visited[currNeb] > visited[currNode] + 1 ){
    
          visited[currNeb] = visited[currNode] + 1;
        
          q.push(currNeb);
          
       
        }
        
      }
      q.pop();
    }

    //cout << "root = " << root << endl;
    int levelCount = 0;
    for(int i = 0; i < totalV; i++){
      if (visited[i] > levelCount){
        levelCount = visited[i];
        
      }
    }
    
    
    //level count
    int levels[levelCount+1] = {0};
    for(int i = 0; i < totalV; i++){
      levels[visited[i]] = levels[visited[i]]+ 1;
    }
    
    //printing each level
    for(int i = 0; i < levelCount + 1; i++){
      cout<<"Level"<< i << ": " << levels[i] <<" nodes"<<endl;
    }
}


