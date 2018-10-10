NetworkEpidemic
==================

Stochastic simulation of epidemics on complex networks based on priority queue event-based algorithm in C++. 


Installation & Default Usage
-----------------------

To run an SIR experiment, clone the repo, open a python 2.7 environment and run the `network.py` code:

:: 

  python network.py
 
The default setup will create a `data.txt` file in the `examples` directory that stores the network topology of an Erdos-Renyi graph containing 1000 nodes. To run an epidemic over this system we will next run the `main.cpp` file. 

:: 

  mpic++ -std=c++11 *.cpp
  mpirun -np 2 ./a.out
  
where 2 is the number of cores we have chosen to run the experiment on. This should then create an output file in the examples directory containing the final epidemic size versus the transmissibility of the disease for a trial of 100 points in the parameter space. When plotted, the output should look something like this:

![pdftojpg me-1-3](https://user-images.githubusercontent.com/29250174/46750387-bc949f80-ccaf-11e8-8474-0a5816604a1c.jpg)

up to stochastic fluctuations and zero-sized epidemics.


Implementing your own model
------------------------------





