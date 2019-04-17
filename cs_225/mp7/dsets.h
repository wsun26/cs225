/* Your code here! */
#ifndef DSETS
#define DSETS

#include <vector>

using namespace std; 
class DisjointSets
{
	public:
		void addelements(int num); 
		int find(int elem); 
		void setunion(int a, int b); 
		int operator[](int elem); 
	private:
		vector<int> nodes; 
}; 
#endif

