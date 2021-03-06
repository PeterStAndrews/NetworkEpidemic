NetworkEpidemic
==================

Stochastic simulation of epidemics on complex networks based on priority queue event-based algorithm in C++. 


Installation & Default Usage
-----------------------

To run an SIR experiment, clone the repo, open a python 2.7 environment and run the `network.py` code:

:: 

  python network.py
 
The default setup will create a ``data.txt`` file in the ``examples`` directory that stores the network topology of an Erdos-Renyi graph containing 1000 nodes. To run an epidemic over this system we will next run the ``main.cpp`` file in ``examples``. 

:: 

  mpic++ -std=c++11 *.cpp
  mpirun -np 2 ./a.out
  
where 2 is the number of cores we have chosen to run the experiment on. This should then output the final epidemic size versus the transmissibility of the disease for a trial of 100 points in the parameter space to the console. 

Implementing your own model
------------------------------



Author & License
-----------------------

The MIT License (MIT). Please see `License File
<https://github.com/PeterStAndrews/NetworkEpidemic/blob/master/LICENSE>`_.

