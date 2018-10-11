//
//  test_main.cpp
//
//  Created by Pete on 04/10/2018.
//  Copyright © 2018 Peter Mann. All rights reserved.
//
// (C) Peter Mann (2018) [University of St Andrews].
// ======================================================================
// Strategy:
// ======================================================================
// A test suite for the network SIR experiment based on a priority queue 
// of events. To implement the tests I will be using catch available at
// 
// https://github.com/catchorg/Catch2
//
// This test focuses on the `EVENT_DRIVEN` simulation class.
//
//	g++ -std=c++11 test_EVENT_DRIVEN.cpp -o t
// 	./t --reporter compact --success

#define CATCH_CONFIG_MAIN  
#include "catch.h"
#include <iostream>
#include <map>

#include "../src/event_driven.hpp"

SCENARIO( "Lets create an epidemic over a network", "[epidemic]") {

	GIVEN( "A data.txt file containing network data ..." ){

		GRAPH g; 			     	     // create graph class
    		GRAPH::ReturnValue1 g_data = g.get_graph();  // call method `get_graph`
    		GRAPH::NODES nodes = g_data.nodes;           // extract node list

    		EVENT_DRIVEN sim(nodes);                     // create simulation object
    		sim.copyGraph(nodes);                        // create network copy.
	
    		WHEN("A disease with beta = 0.5 and gamma = 0.3 ... "){

    			std::map<std::string,double> params;      // create params dict
    			params["beta"] = (double) 0.5; 		  // initialise the infection probability
        		params["gamma"] = 0.3;			  // initialise the recovery probability
        		sim._do(params); 			  // simulate the epidemic

        		std::map<std::string, double> res;	  // create results map
        		res = sim.get_results();		  // extract simulation results

        		THEN("expect at least one root node was infected or an epidemic occured ..."){
        		
        			REQUIRE(res["S"] > 0);		  // non-zero susceptibles
        			REQUIRE(res["I"] == 0);		  // no infected nodes 
        			REQUIRE(res["R"] > 0);	          // at least one removed node
								  // population is conserved
        			REQUIRE( res["S"] + res["I"] + res["R"] == g_data.nodes.size());
        		}
        	}								 
    	}
}





















