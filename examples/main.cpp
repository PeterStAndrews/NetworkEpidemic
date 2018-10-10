//
//  main.cpp
//
//  Created by Pete on 05/10/2018.
//  Copyright © 2018 Peter Mann. All rights reserved.
//
// (C) Peter Mann (2018) [University of St Andrews].
// ======================================================================
// Strategy:
// ======================================================================
// A network SIR experiment based on a priority queue of events.
//
// mpic++ -std=c++11 *.cpp
// mpirun -np 2 ./a.out
//

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iterator>
#include <random>
#include <algorithm>
#include <chrono>
#include <map>
#include <queue>
#include <set>

#include <mpi.h>

#include "event_driven.hpp"

// ======================================================================
// Main:
// ======================================================================

int main ( int argc, char *argv[] ) {
    
    GRAPH g; 								   	 // create graph class
    GRAPH::ReturnValue1 g_data = g.get_graph();  // call method `get_graph`
    GRAPH::NODES nodes = g_data.nodes;           // extract node list
    
    // compute degree distribution & average degree
    std::map<int,double> Pk = g.degree_distribution(nodes);
    double ave_k = g.average_degree(Pk);
    std::map<std::string,double> params;         // create params dict
    
    EVENT_DRIVEN sim(nodes);                     // create simulation object
    sim.copyGraph(nodes);                        // create network copy.
    
    unsigned int N = 100;                		 // number of points to take
    
    // Record start time
    auto start = std::chrono::high_resolution_clock::now();

         // initialise the MPI 
     int ierr = MPI_Init(&argc, &argv);
     int procid, numprocs;
     ierr = MPI_Comm_rank(MPI_COMM_WORLD, &procid);
     ierr = MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
     // partition = job over processors. 
     unsigned int partition = N / numprocs;
     // if N not divisible by numprocs:
     if (N % numprocs > 0) {
         partition++;
         N = partition * numprocs;
     }
    
    // iterate over partitions on processors
    for (unsigned int i = procid*partition; i<= partition * (procid + 1); i++){
        params["beta"] = (double) i/N;
        params["gamma"] = 0.1;
        sim._do(params);   
    }
    ierr = MPI_Finalize();
    
    // // Record end time
    auto finish = std::chrono::high_resolution_clock::now();
    
    // // compute elapsed time
    std::chrono::duration<double> elapsed = finish - start;
    
    // // print to console
    std::cout << "Elapsed time: " << elapsed.count() << " s\n";
    std::cout << "Average time per run: " << (double) elapsed.count() /N << " s\n";
    
    return 0;
}
