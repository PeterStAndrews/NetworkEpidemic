#ifndef GRAPH_HEADER
#define GRAPH_HEADER

// (C) Peter Mann (2018) [University of St Andrews].
// ======================================================================
// Strategy:
// ======================================================================
// Class creates a network object by reading data from a data file.
// (TO DO) Can probably be optimised to read input file only once. 

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iterator>
#include <map>

class GRAPH{
	// class: GRAPH
	// type: class
	// definition: creates NODE and EDGE objects to store data read from file
	// where the format is: line number = node ID, and each line contains the 
	// IDs of its neighbours. 

public:
	struct EDGE {
		// class: GRAPH
    	// type: edge struct
    	// definition: creates a edge data between two node IDs
    	// and overloads << to print.

    	int i;                              // focal node
    	int j;                              // neighbour node

    	EDGE( int i, int j) : i(i), j(j) {} // constructor

                                        // overload stream operator
    	friend std::ostream& operator<<(std::ostream& s, const EDGE & e) {
        	return s << e.i << " " << e.j;
    	}
	};          

	struct NODE{
		// class: GRAPH
    	// type: node struct
    	// definition: creates a node object.

    	unsigned int ID;                    // unique node id 
    	std::vector<EDGE> EDGES;            // list of nodes neighbours
    	int STATUS;                         // infection status {-1,0,1}

    	NODE( unsigned int ID, std::vector<EDGE> EDGES, 
        	int STATUS) : ID(ID), EDGES(EDGES), STATUS(STATUS) {} 
	};

	typedef std::vector<EDGE> EDGES;        // type: list of edges
	typedef std::vector<NODE> NODES;        // type: list of nodes
	typedef std::vector<EDGES> CONTACTS;    // type: list of list of edges

	struct ReturnValue1{
		// class: GRAPH
		// type: typedef struct 
		// definition: provides the typedef for the return of get_graph method.
		NODES nodes;
		CONTACTS contacts;
	};

	ReturnValue1 get_graph(){
		// class: GRAPH
		// type: function
		// definition: reads a data file linewise to extract the network information.
		// Will throw an error if the data file is not found.

		unsigned int ID = 0;
    	CONTACTS contacts;                  // initialise list of edge lists
        std::string filename = "data.txt";
		std::ifstream theStream(filename);
    	if (!theStream)
        	std::cerr << "FILE NOT FOUND: " << filename << std::endl;
       
    	while (true) {                      // while lines in file   
        	std::string line;
        	std::getline(theStream, line);  // grab a line as a string
        	if (line.empty())
            	break;

        	std::istringstream myStream(line); 
        	std::istream_iterator<int> begin(myStream), eof; 
        	std::vector<int> numbers(begin, eof); 

        	EDGES neighbours;               // vector of edges
        
        	for (int i =0; i< numbers.size(); i++){
            	EDGE e(ID,numbers[i]);      // create EDGE object     
            	neighbours.push_back(e);    // append edge to nodes neighbour list
        	}
        	contacts.push_back(neighbours);
        	++ID;                           // increment node ID counter
    	}

    	NODES nodes;                        // instance nodes list to susceptible
    	for (std::vector<int>::size_type n = 0; n != contacts.size(); ++n) {
        	NODE node{static_cast<unsigned int>(n), contacts[n], -1};
        	nodes.push_back(node);
    	}
    	return { nodes, contacts };
	}

    std::map<int, double> degree_distribution( NODES nodes ){
        // class: GRAPH
        // type: function 
        // definition: returns the degree distribution associated to 
        // an argument node list as a map { (int) degree, (double) value }
        std::map<int, double> Pk;

        double inv_order =  1.0 / (double) nodes.size();

        for( NODES::iterator n = nodes.begin(); n != nodes.end(); ++ n){
            unsigned int k = (int) n->EDGES.size();
            Pk[k] += inv_order;
        }
        return Pk;
    }

    double average_degree ( std::map<int,double> Pk ){
        // class: GRAPH
        // type: function 
        // definition: returns the average degree of the network given a 
        // degree distribution.
        double ave_k;
        for(std::map<int,double>::const_iterator it = Pk.begin();it != Pk.end(); ++it){
            ave_k += it->first*it->second;
        }
        return ave_k;
    }
    
};

#endif
