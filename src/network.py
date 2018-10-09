# (c) Peter Mann (2018) [University of St Andrews].
#
# Generates a networkx graph and outputs the formatted data to a txt file
# that can be read by the c++ program. This should be run in python 2.7.
# The resulting file has the following format:
# line number = node ID
# numbers (space separated) = neighbours
#
# To-Do: use JSON rather than txt file. 


import networkx 

# create the network
N = 1000                                # number of nodes in the network
kmean = 5                               # mean degree of a node
phi = (kmean + 0.0) / N                 # probability of two nodes being connected
g = networkx.erdos_renyi_graph(N, phi)

# clean the network
g = g.subgraph(max(networkx.connected_components(g), key = len)).copy()
g.remove_edges_from(list(g.selfloop_edges()))

# output data to file
output = open("data.txt", "w")
for n in g.nodes():                     # for node in graph
    e = g.neighbors(n)                  # list of neigbhours of node n
    for k,v in enumerate(e):            # iterate over ns neighbours
        v = str(v)                      # convert neighbour ID to string
        output.write(v)                 # print string ID to file
        if k != len(list(e))-1:         # if the element is not the final neighbour:
            output.write(" ")           # separate data by single space
    output.write("\n")                  # print newline for each node
output.close()                          # close the output file
