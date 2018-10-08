#ifndef EVENT_DRIVEN_HEADER
#define EVENT_DRIVEN_HEADER

// (C) Peter Mann (2018) [University of St Andrews].
// ======================================================================
// Strategy:
// ======================================================================
// A network SIR experiment based on a priority queue of events.
// Header for event_driven class
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

#include "rng.hpp"
#include "graph.hpp"


// ======================================================================
// Class :
// ======================================================================

class EVENT_DRIVEN{
    // type: class
    // definition: Creates an event driven SIR experiment.
    // methods:
    //      copyGraph: stores a copy of the unmodified network
    //
    //      _do: runs the experiment with setup and teardown methods
    //
    //      setup: setup method for each run of the experiment
    //
    //      teardown: reports the outcome of the experiment, parameters and metadata
    //
    //      infect: performs an infection event
    //
    //      recover: performs a recovery event
    //
    //      call_event: grabs the next event in the queue and calls it
    //
    private:
    
    using EFFECT = void(EVENT_DRIVEN::*)(unsigned int);
    
    struct EVENT {
        // type: event struct
        // definition: creates an event object
        
        double time;                            // time of event
        EFFECT effect;                     // effect of event
        unsigned int location;                  // event location (node ID)
        EVENT( double time, EFFECT effect, unsigned int location) : time(time), effect(effect), location(location) {}
    };
    
    struct COMPARISON {
        // type: comparison struct
        // definition: auxillary struct for priority queue. Compares
        // two event times and returns true if the left hand
        // side is after the right hand side. Using greater than `>`
        // gives the queue a `lowest first` ordering.
        
        bool operator() (const EVENT& l, const EVENT& r) const {
            return (l.time > r.time);
        }
    };
    
    typedef std::vector<EVENT> EVENTS;  // type: list of events
    
    public:
    double beta, gamma;                 // model parameters
    GRAPH::NODES nodes, _nodes;         // network nodes
    bool propensity;
    std::priority_queue < EVENT, EVENTS, COMPARISON > queue;
    int I,R;
    unsigned int root;                  // root of infection tree
    double r1, r2;                      // declare random numbers
    unsigned int r3;
    double tau_t, tau_r;                // declare time interval(s)
    double t;                           // start time
    
    EVENT_DRIVEN(GRAPH::NODES nodes) : nodes(nodes) {}
    
    void copyGraph(GRAPH::NODES nodes);
    
    void _do(std::map<std::string,double> params);
    
    void setup(std::map<std::string,double> params);
    
    void teardown();
    
    void infect(unsigned int ID);
    
    void recover(unsigned int ID);
    
    void call_event();
};


// ======================================================================
// Class functions:
// ======================================================================

void EVENT_DRIVEN::copyGraph(GRAPH::NODES nodes){
    // type: function
    // definition: stores a copy of the unmodified network
    _nodes = nodes;
}

void EVENT_DRIVEN::_do(std::map<std::string,double> params){
    // type: function
    // definition: runs the experiment with setup and teardown methods
    setup(params);
    while(propensity and I != 0){
        call_event();
    }
    teardown();
}

void EVENT_DRIVEN::setup(std::map<std::string,double> params){
    // type: function
    // definition: setup method for each run of the experiment
    beta = params["beta"];
    gamma = params["gamma"];
    nodes = _nodes;                // something going one with this line
    propensity = true;
    I = 0;
    R = 0;
    t = 0.0;
    // select index(s) for root of infected tree (if multiple: pick unique)
    unsigned int root = random_integer(1,nodes.size());
    nodes[root].STATUS = 0;
    infect(nodes[root].ID);
    ++I;
}

void EVENT_DRIVEN::teardown(){
    // type: function
    // definition: reports the outcome of the experiment, parameters and metadata
    double epidemic = (double) R / (double) nodes.size();
    std::cout << beta << " " << epidemic  << "\n";
}


void EVENT_DRIVEN::infect(unsigned int ID){
    // type: function
    // definition: performs an infection event at e.location. An edge is
    // selected from the neighbour list. If the neighbour is susceptible, it becomes infected and
    // its contact events and recovery events are posted to the priority queue. If
    // the neighbours are not susceptible, these contacts do not lead to further action.
    
    r3 = random_integer(1, nodes[ID].EDGES.size());
    GRAPH::EDGES::const_iterator it = nodes[ID].EDGES.begin();
    std::advance(it, r3-1);             // without -1 can sometimes break
    
    if(nodes[it->j].STATUS < 0){
        ++I;                            // infect the neighbour
        nodes[it->j].STATUS = 0;
        
        r1 = random_double(0,1);        // sample time interval until transmission/recovery
        r2 = random_double(0,1);
        tau_t = -log((double)r1/INT_MAX) / beta;
        tau_r = -log((double)r2/INT_MAX) / gamma;
        
        // while transmission time < recovery time
        while ( tau_t < tau_r ){
            // post contacts.
            queue.push( EVENT(tau_t, &EVENT_DRIVEN::infect, nodes[it->j].ID));
            tau_t += -log((double)r1/INT_MAX) / beta;
        }
        // post recovery event
        queue.push( EVENT(tau_r, &EVENT_DRIVEN::recover, nodes[it->j].ID));
    }
}

void EVENT_DRIVEN::recover(unsigned int ID){
    // type: function
    // definition: performs a recovery event at e.location and increments
    // the state counters.
    nodes[ID].STATUS = 1;
    ++R;
    --I;
}

void EVENT_DRIVEN::call_event(){
    // type: function
    // definition: grabs the next event in the queue (if there is one left)
    // and increments the time. It then decides what the event is and plays
    // it out. The event is then removed from the top of the queue.
    if (queue.empty()){
        propensity = false;
    }
    else{
        // grab the next event
        EVENT e = queue.top();
        
        // increment the time
        t += e.time;
        
        // play it out
        (this->*e.EVENT_DRIVEN::EVENT::effect)(e.location);
        
        // pop the event
        queue.pop();
    }
}


#endif

