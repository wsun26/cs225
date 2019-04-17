/**
 * @file quadtree.cpp
 * Quadtree class implementation.
 * @date Spring 2008
 */
 #include "quadtree.h"
 #include <iostream>
 #include <math.h>
Quadtree::Quadtree()
{
	root = NULL; 
}

Quadtree::Quadtree(PNG const& source, int resolution)
{
	root = NULL; 
	buildTree(source, resolution); 
}

Quadtree::QuadtreeNode::QuadtreeNode()
{
	nwChild = NULL; 
	neChild = NULL; 
	swChild = NULL; 
	seChild = NULL; 	
	resolution = 0; 
}

/*
Quadtree::Quadtree::QuadtreeNode(RGBAPixel& source)
{
	nwChild = NULL; 	
	neChild = NULL; 
	swChild = NULL; 
	seChild = NULL; 
	element = source; 
}
*/

Quadtree::Quadtree(Quadtree const& other)
{
	root = copy(other.root); 
}

Quadtree::QuadtreeNode* Quadtree::copy(QuadtreeNode* subnode)
{
	if(subnode == NULL) return NULL; 
	QuadtreeNode* node = new QuadtreeNode(); 
	node->resolution = subnode->resolution; 
	node->element = subnode->element;  
	node->nwChild = copy(subnode->nwChild); 	
	node->neChild = copy(subnode->neChild); 
	node->swChild = copy(subnode->swChild); 
	node->seChild = copy(subnode->seChild); 
	return node; 
}

Quadtree::~Quadtree()
{
	clear(root); 
}

void Quadtree::clear(QuadtreeNode*& subnode)
{
	if(subnode == NULL) return; 
	clear(subnode->nwChild); 
	clear(subnode->neChild); 
	clear(subnode->swChild); 
	clear(subnode->seChild); 
	delete subnode; 	
	subnode = NULL; 
}

Quadtree const& Quadtree::operator=(Quadtree const& other)
{
	if(this != &other){
		clear(this->root); 
		root = copy(other.root); 
	}
	return *this; 
}

void Quadtree::buildTree(PNG const& source, int resolution)
{
	clear(root); 
	buildTreeHelper(source, resolution, 0, 0, root); 
}

void Quadtree::buildTreeHelper(PNG const& source, int resolution, int x, int y, QuadtreeNode*& subnode)
{
	subnode = new QuadtreeNode(); 
	if(resolution == 0) return; 
	subnode->resolution = resolution; 
	if(resolution == 1){
		subnode->element = *source(x, y); 
		return; 
	}
	buildTreeHelper(source, resolution/2, x, y, subnode->nwChild); 
	buildTreeHelper(source, resolution/2, x+resolution/2, y, subnode->neChild); 
	buildTreeHelper(source, resolution/2, x, y+resolution/2, subnode->swChild); 
	buildTreeHelper(source, resolution/2, x+resolution/2, y+resolution/2, subnode->seChild); 
	(subnode->element).red = ((subnode->nwChild->element).red + (subnode->neChild->element).red + (subnode->swChild->element).red + (subnode->seChild->element).red)/4; 
	(subnode->element).green = ((subnode->nwChild->element).green + (subnode->neChild->element).green + (subnode->swChild->element).green + (subnode->seChild->element).green)/4; 
	(subnode->element).blue = ((subnode->nwChild->element).blue + (subnode->neChild->element).blue + (subnode->swChild->element).blue + (subnode->seChild->element).blue)/4; 
}

RGBAPixel Quadtree::getPixel(int x, int y) const
{
	return getPixel(root, x, y); 
}


RGBAPixel Quadtree::getPixel(QuadtreeNode const* subnode, int x, int y) const
{
	RGBAPixel ret; 
	if(subnode->resolution == 1) return subnode->element; 
	if(x > subnode->resolution) x -= subnode->resolution; 
	if(y > subnode->resolution) y -= subnode->resolution; 
	if(x <= subnode->resolution/2 && y <= subnode->resolution/2){
		if(subnode->nwChild == NULL) return subnode->element; 	
		ret = getPixel(subnode->nwChild, x, y); 
	}
	if(x > subnode->resolution/2 && y <= subnode->resolution/2){
		if(subnode->neChild == NULL) return subnode->element; 	
		ret = getPixel(subnode->neChild, x, y); 
	}
	if(x <= subnode->resolution/2 && y > subnode->resolution/2){
		if(subnode->swChild == NULL) return subnode->element; 
		ret = getPixel(subnode->swChild, x, y); 
	}
	if(x > subnode->resolution/2 && y > subnode->resolution/2){
		if(subnode->seChild == NULL) return subnode->element; 
		ret = getPixel(subnode->seChild, x, y); 
	}
	return ret; 
}

PNG Quadtree::decompress() const
{
	if (root == NULL) return PNG();
/*
	PNG out(root->resolution, root->resolution);
	
	for (int i = 0; i < root->resolution; i++) {
	
		for (int j = 0; j < root->resolution; j++) {
		
			*out(i, j) = getPixel(i, j);
		}
	}
	

	return out;
*/
	PNG ret(root->resolution, root->resolution); 
	decompress(root, 0, 0, ret); 
	return ret; 

}

void Quadtree::decompress(QuadtreeNode const* subnode, int x, int y, PNG& pic) const
{
	if(subnode == NULL) return; 
	if(subnode->nwChild == NULL && subnode->neChild == NULL && subnode->swChild == NULL && subnode->seChild == NULL){
		for(int i = x; i < x + subnode->resolution; i++){
			for(int j = y; j < y + subnode->resolution; j++){
				*(pic(i, j)) = subnode->element; 
			}
		}
	}
	decompress(subnode->nwChild, x, y, pic); 
	decompress(subnode->neChild, (subnode->resolution)/2 + x, y, pic); 
	decompress(subnode->swChild, x, (subnode->resolution)/2 + y, pic); 
	decompress(subnode->seChild, (subnode->resolution)/2 + x, (subnode->resolution)/2 + y, pic);  
}

void Quadtree::clockwiseRotate()
{
	clockwiseRotate(root); 
}

void Quadtree::clockwiseRotate(QuadtreeNode*& subnode)
{
	if(subnode == NULL) return;
	clockwiseRotate(subnode->nwChild); 
	clockwiseRotate(subnode->neChild); 
	clockwiseRotate(subnode->swChild); 
	clockwiseRotate(subnode->seChild); 
	QuadtreeNode* temp = subnode->swChild; 
	subnode->swChild = subnode->seChild; 
	subnode->seChild = subnode->neChild; 
	subnode->neChild = subnode->nwChild; 
	subnode->nwChild = temp; 
}

void Quadtree::prune(int tolerance)
{
	if(root == NULL) return; 
	prune(tolerance, root); 
}

void Quadtree::prune(int tolerance, QuadtreeNode*& subnode)
{
	if(subnode == NULL) return; 
	if(prunehelper(tolerance, subnode, subnode) == false){
		prune(tolerance, subnode->nwChild);	 
		prune(tolerance, subnode->neChild);
		prune(tolerance, subnode->swChild);
		prune(tolerance, subnode->seChild);
	} else {
		clear(subnode->nwChild); 
		clear(subnode->neChild); 
		clear(subnode->swChild); 
		clear(subnode->seChild);
	} 
}

bool Quadtree::prunehelper(int tolerance, QuadtreeNode*& subnode, QuadtreeNode*& child)
{
	if(child->nwChild == NULL && child->neChild == NULL && child->swChild == NULL && child->seChild == NULL){
		return tolerancecheck(tolerance, subnode, child); 
	}
	return prunehelper(tolerance, subnode, child->nwChild) && prunehelper(tolerance, subnode, child->neChild) && prunehelper(tolerance, subnode, child->swChild) && prunehelper(tolerance, subnode, child->seChild); 
}

bool Quadtree::tolerancecheck(int tolerance, QuadtreeNode*& subnode, QuadtreeNode *& leaf)
{
	return (pow(((subnode->element).red - (leaf->element).red), 2) + pow(((subnode->element).green - (leaf->element).green), 2) + pow(((subnode->element).blue - (leaf->element).blue), 2) <= tolerance); 
}

int Quadtree::pruneSize(int tolerance) const 
{
	if(root == NULL) return 0;  
	int ret = 0; 
	ret = pruneSize(tolerance, root, ret); 
	return ret; 
}

int Quadtree::pruneSize(int tolerance, QuadtreeNode const * subnode, int& ret) const
{
	if(subnode == NULL) return ret; 
	if(sizehelper(tolerance, subnode, subnode) == false){
		pruneSize(tolerance, subnode->nwChild, ret);	 
		pruneSize(tolerance, subnode->neChild, ret);
		pruneSize(tolerance, subnode->swChild, ret);
		pruneSize(tolerance, subnode->seChild, ret);
	} else {
		return ret += 1; 
	}
	return ret;
}

bool Quadtree::sizehelper(int tolerance, QuadtreeNode const* subnode, QuadtreeNode const* child) const
{
	if(child->nwChild == NULL && child->neChild == NULL && child->swChild == NULL && child->seChild == NULL){
		return sizetolerancecheck(tolerance, subnode, child); 
	}
	return sizehelper(tolerance, subnode, child->nwChild) && sizehelper(tolerance, subnode, child->neChild) && sizehelper(tolerance, subnode, child->swChild) && sizehelper(tolerance, subnode, child->seChild); 	
}

bool Quadtree::sizetolerancecheck(int tolerance, QuadtreeNode const* subnode, QuadtreeNode const* leaf) const
{
	return (pow(((subnode->element).red - (leaf->element).red), 2) + pow(((subnode->element).green - (leaf->element).green), 2) + pow(((subnode->element).blue - (leaf->element).blue), 2) <= tolerance); 
}

int Quadtree::idealPrune(int numLeaves) const
{
	if(root == NULL) return 0; 
	if(numLeaves > (root->resolution)*(root->resolution)) return 0; 
	return idealPrune(numLeaves, 0, 3*255*255); 
}

int Quadtree::idealPrune(int numLeaves, int lowTol, int upTol) const
{
	int midTol = (upTol + lowTol)/2; 
	int prune = pruneSize(midTol); 
	if(numLeaves < prune){
		return idealPrune(numLeaves, midTol+1, upTol); 
	}
	else if(numLeaves > prune){
		return idealPrune(numLeaves, lowTol, midTol-1); 
	} else {
		if(pruneSize(midTol-1) == prune){
			return idealPrune(numLeaves, lowTol, midTol-1); 
		} else {
			return midTol; 
		}
	}
}


