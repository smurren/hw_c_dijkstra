// Dijkstra's Algorithm used to find optimal routing for a network
// Coded by Sean Murren for CMSC481
// May 2015

#include <list>
#include <iostream>
#include <string>
#include <fstream>

using namespace std;


class Node {
	public:
		int distance;
		int path;
		bool exists;
		string data[8];
	
		void setup() {
			distance = 99999;
			path = -1;
			exists = false;
			for (int i = 0; i < 8; i++)
				data[i] = "*";
		}
		
		void resetKeepLinks() {
			distance = 99999;
			path = -1;
			exists = false;
		}
};


void dijkstras(Node nodes[], int source, int destination) {
	
	int maxLinks = 8;
	int nodesToSearch[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	int nextNode = source;
	int node;
	int pathWeight;
	int distance;
	
	nodes[source-1].distance = 0;
	
	while (nextNode != 0) {
		
		for (int i = 0; i < maxLinks; i++) {
			// If link contains link data, extract node and link weight info
			if (nodes[nextNode-1].data[i][0] != '*') {
				node = nodes[nextNode-1].data[i][2] - '0';
				pathWeight = nodes[nextNode-1].data[i][4] - '0';
				
				// If end node of link has greater distance, update new lower distance
				// value and update path (current node being expanded)
				if (nodes[node-1].distance > nodes[nextNode-1].distance + pathWeight) {
					nodes[node-1].path = nextNode;
					nodes[node-1].distance = nodes[nextNode-1].distance + pathWeight;
					// Add node to array of nodes that need to be expanded
					nodesToSearch[node-1] = node;
				}
			}
		}
		
		// Explore node from this current round with lowest distance value
		node = 0;
		distance = 99999;
		for (int i = 0; i < 8; i++) {
			if (nodesToSearch[i] != 0) {
				if (nodes[nodesToSearch[i]-1].distance < distance) {
					node = nodesToSearch[i];
					distance = nodes[nodesToSearch[i]-1].distance;
				}
			}
		}
		// Remove node that is going to be explored next
		nodesToSearch[node-1] = 0;
		// set new nextNode to this node
		nextNode = node;
	}

}


int main() {
	
	int maxNodes = 9;
	Node objects[maxNodes];
	
	// initialize node data, inf distance, path -1
	for (int i = 0; i < maxNodes; i++) {
		objects[i].setup();
	}
	
	int nodeCount = 1;  // count starts at 1 for source node
	int sourceNode;
	int destNode;
	int nodeIndex;
	int linkIndex;
	// temp variables used for adding reversed link
	int tempLinkIndex;
	int tempSourceNodeInt;
	string tempSource;
	string tempDest;
	string reverseLink;
	
	// open input file
	ifstream file("graph.txt", ios::in);
	
	// open output text file
	ofstream outFile;
	outFile.open("output.txt");
	
	// read in data as a string, line by line
	if (file.is_open()) {
		
		for (string line; getline (file,line); /**/ ) {
			
			linkIndex = 0;
			
			if ((line[0] != 'S') && (line[0] != 'D')) { 
				nodeIndex = line[0] - '0';
				
				// while link element already has data
				while (objects[nodeIndex-1].data[linkIndex][1] == '$')
					linkIndex += 1;
				
				// add link data string to the source node of the link
				objects[nodeIndex-1].data[linkIndex] = line;
				
				// Add reversed link to the destination node.  Used for table routing
				// ---------------------------------------------------------------
				tempSource = line[2];
				tempDest = line[0];
				reverseLink = "";
				reverseLink = tempSource + "$" + tempDest + "$" + line[4];
				tempLinkIndex = 0;
				tempSourceNodeInt = tempSource[0] - '0';
				while (objects[tempSourceNodeInt-1].data[tempLinkIndex][1] == '$')
					tempLinkIndex += 1;
				
				objects[tempSourceNodeInt-1].data[tempLinkIndex] = reverseLink;
				// ---------------------------------------------------------------
				
				// check if the destination node in the line of data is new
				if (!objects[tempSourceNodeInt-1].exists) {
					nodeCount++;
					objects[tempSourceNodeInt-1].exists = true;
				}
				
			}
			else {
				if (line[0] == 'S') {
					sourceNode = line[7] - '0';  // Extract node# from Source$#
				}
				if (line[0] == 'D') {
					destNode = line[12] - '0';  // Extract node# from Destination$#
				}
		
			}
			
		}
		file.close();  // close input file, all lines have been read
	}
	else {
		cout << "Unable to open file.";
		return 0;
	}
	
	
	// Use dijkstra's algorithm to process node data and set
	// node values to represent shortest path information
	dijkstras(objects, sourceNode, destNode);
	
	
	// Summary of shortest path from source to destination
	// ---------------------------------------------------------------------------------
	cout << "Source? --> Destination?  " << sourceNode << " --> " << destNode << "\n";
	outFile << sourceNode << " --> " << destNode << "\n";
	
	int pathTaken[8] = {0,0,0,0,0,0,0,0};
	int nodeName;
	int pathTraceNode = destNode;
	int pathTakenIndex = 1;
	pathTaken[0] = destNode;
	
	// record node path taken to pathTaken array
	while (objects[pathTraceNode-1].path != -1) {
		pathTaken[pathTakenIndex] = objects[pathTraceNode-1].path;
		pathTraceNode = objects[pathTraceNode-1].path;
		pathTakenIndex++;
		
	}
	cout << "Traversal ";
	
	// pathTaken order goes from dest to source, so output reversed
	for (int i = 7; i >= 0; i--) {
		if (pathTaken[i] != 0) {
			nodeName = pathTaken[i];
			if (nodeName == destNode) {
				cout << nodeName << "\n";
				outFile << nodeName << "\n";
			}
			else {
				cout << nodeName << " --> ";
				outFile << nodeName << " --> ";
			}
		}
	}
	cout << "Total distance: " << objects[destNode-1].distance << "\n\n";
	outFile << "Total distance: " << objects[destNode-1].distance << "\n\n";
	// ---------------------------------------------------------------------------------

	
	// (Routing tables in order)
	// ---------------------------------------------------------------------------------
	int routeIndex;
	int forwardNode;
	
	for (int i = 0; i < nodeCount; i++) {
		cout << "Routing table of node " << i+1 << "\n";
		cout << "Destination\tNext Hop\tCost\n------------------------------------\n";
		outFile <<  "Routing table of node " << i+1 << "\n";
		outFile << "Destination\tNext Hop\tCost\n------------------------------------\n";
		
		for (int j = 0; j < nodeCount; j++) {
			if (j != i) {
				// reset objects so dijkstras can be run again, but keep link data
				for (int k = 0; k < maxNodes; k++)
					objects[k].resetKeepLinks();
				
				// call dijkstras with new source and dest nodes
				dijkstras(objects, i+1, j+1);
				
				for (int p = 0; p < 8; p++)
					pathTaken[p] = 0;
				pathTraceNode = j+1;
				pathTakenIndex = 1;
				pathTaken[0] = j+1;
				
				// record shortest path to array, used below to find forward node
				while (objects[pathTraceNode-1].path != -1) {
					pathTaken[pathTakenIndex] = objects[pathTraceNode-1].path;
					pathTraceNode = objects[pathTraceNode-1].path;
					pathTakenIndex++;	
				}
				
				routeIndex = 8;
				forwardNode = 0;
				while ((forwardNode == 0) || (forwardNode == i+1)) {
					routeIndex--;
					forwardNode = pathTaken[routeIndex];
				}
				
				cout << j+1 << "\t\t" << forwardNode << "\t\t" << objects[j].distance << "\n";
				outFile << j+1 << "\t\t" << forwardNode << "\t\t" << objects[j].distance << "\n";
			}
		}
		cout << "\n";
		outFile << "\n";
	}
	// ---------------------------------------------------------------------------------
	
	outFile.close();
	
}

