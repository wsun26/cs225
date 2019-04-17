/* Your code here! */

#ifndef MAZES
#define MAZES
#include "png.h"
#include "dsets.h"
#include <vector>
using namespace std; 

class SquareMaze
{
	public:
		SquareMaze(); 
		void makeMaze(int width, int height); 
		bool canTravel(int x, int y, int dir) const; 
		void setWall(int x, int y, int dir, bool exists); 
		vector<int> solveMaze(); 
		PNG* drawMaze() const; 
		PNG* drawMazeWithSolution(); 
	private:
		DisjointSets maze; 
		vector<bool> bottomWall; 
		vector<bool> rightWall; 
		vector<bool> visited; 
		int w, h; 
};
#endif
