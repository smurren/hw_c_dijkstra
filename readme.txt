// Dijkstra's Algorithm used to find optimal routing for a network
// Coded by Sean Murren for CMSC481
// May 2015

To run on GL:
linux1[1]% g++ dijkstra.c -o dijkstra
linux1[2]% ./dijkstra

Notes:
1.) Input file name = graph.txt
    Output file name = output.txt
    Similar output is printed to console as well.

2.) Program works as intended, for up to 9 routing nodes with up to 8 links each.
    Those are the max amounts the program is set up for, but will work with less.

3.) The input data is read from a graph.txt file and is output to console and to
 an output.txt file.  Output includes: Source? --> Destination?, Traversal, and
 the routing tables for each node in the graph.

4.) C++ was used to take advantage of OOP.  Each node on the graph is made into a 
 Node object that contains information on distance (to reach node from source), a
 list of links that connect it to other nodes, and the nodes predecessor (path).  

5.) The link information is left formatted the same as it was read from graph.txt 
 (as a string).  A link is one string per line of the input file.  The important 
 information is extracted from the string when the dijkstras method is accessing 
 the information.

6.) Nodes 1-9 are indexed in sequential order in the list of nodes on indexes 0-8.
 The list is set to size 9 for this program, as graphs with 6-9 nodes is the
 expected input.

7.) The Source and Destination node lines are identified by the first character of
 the line being 'S' or 'D', otherwise the line is assumed to contain a link.