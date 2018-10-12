//
//  test_main.cpp
//
//  Created by Pete on 11/10/2018.
//  Copyright © 2018 Peter Mann. All rights reserved.
//
// (C) Peter Mann (2018) [University of St Andrews].
// ======================================================================
// Strategy:
// ======================================================================
// A test suite for the network generation class. To implement the tests 
// I will be using catch available at
// 
// https://github.com/catchorg/Catch2
//
// This test focuses on the `GRAPH` class.
//
//	g++ -std=c++11 test_GRAPH.cpp -o t
// 	./t --reporter compact --success

#define CATCH_CONFIG_MAIN  
#include "catch.h"
#include <iostream>
#include <map>

#include "../src/graph.hpp"

SCENARIO( "Let us create a network", "[graph]") {

	GIVEN( "A data.txt file containing network data ..." ){

        THEN("create a GRAPH instance and extract it ..."){

            GRAPH g; 			     	                 // create graph class
    	    GRAPH::ReturnValue1 g_data = g.get_graph();  // call method `get_graph`
    	    GRAPH::NODES nodes = g_data.nodes;           // extract node list

            // compute degree distribution & average degree
            std::map<int,double> Pk = g.degree_distribution(nodes);
            double ave_k = g.average_degree(Pk);

            REQUIRE( ave_k > 0);                         // test average degree matches expected 		  
        									 
    	}
    }
}
