/* Your code here! */
#include "dsets.h"
#include <iostream>
void DisjointSets::addelements(int num)
{
	for(int i = 0; i < num; i++){
		nodes.push_back(-1); 
	}
}

int DisjointSets::find(int elem)
{
	if(nodes[elem] < 0) return elem; 
	else{
		nodes[elem] = find(nodes[elem]); 
		return nodes[elem]; 
	}
}

void DisjointSets::setunion(int a, int b)
{
	int first = find(a); 
	int second = find(b); 
	int size = nodes[first] + nodes[second]; 
	if(nodes[first] > nodes[second]){
		nodes[first] = second; 
		nodes[second] = size; 
	} else {
		nodes[second] = first; 
		nodes[first] = size; 
	}		
}

int DisjointSets::operator[](int elem){
	return nodes[elem]; 
}

