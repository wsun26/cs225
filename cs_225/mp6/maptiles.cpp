/**
 * @file maptiles.cpp
 * Code for the maptiles function.
 */

#include <iostream>
#include <map>
#include "maptiles.h"

using namespace std;

MosaicCanvas* mapTiles(SourceImage const& theSource,
                       vector<TileImage> const& theTiles)
{
    /**
     * @todo Implement this function!
     */
	MosaicCanvas* mosaic = new MosaicCanvas(theSource.getRows(), theSource.getColumns()); 
	vector<Point<3>> tree; 
	map<Point<3>, TileImage> tiles; 
	for(size_t i = 0; i < theTiles.size(); i++){
		RGBAPixel curr_pixel = theTiles[i].getAverageColor(); 
		Point<3> temp; 
		temp[0] = curr_pixel.red; 
		temp[1] = curr_pixel.green; 
		temp[2] = curr_pixel.blue; 
		tiles[temp] = theTiles[i]; 
		tree.push_back(temp); 
	}
	KDTree<3> mosaicTree = KDTree<3>(tree); 
	for(int i = 0; i < theSource.getRows(); i++){
		for(int j = 0; j < theSource.getColumns(); j++){
			RGBAPixel curr_pixel = theSource.getRegionColor(i, j); 
			Point<3> temp; 
			temp[0] = curr_pixel.red; 
			temp[1] = curr_pixel.green; 
			temp[2] = curr_pixel.blue; 
			Point<3> sourceTemp = mosaicTree.findNearestNeighbor(temp); 
			mosaic->setTile(i, j, tiles[sourceTemp]); 
		}
	}
    return mosaic;
}

