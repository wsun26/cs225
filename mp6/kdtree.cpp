/**
 * @file kdtree.cpp
 * Implementation of KDTree class.
 */
#include <iostream>
#include <math.h>
using namespace std; 
template <int Dim>
bool KDTree<Dim>::smallerDimVal(const Point<Dim>& first,
                                const Point<Dim>& second, int curDim) const
{
    /**
     * @todo Implement this function!
     */
	if(first[curDim] < second[curDim]) return true; 
	if(first[curDim] == second[curDim]){
		if(first < second) return true; 
		else return false; 
	}
    return false;
}

template <int Dim>
bool KDTree<Dim>::shouldReplace(const Point<Dim>& target,
                                const Point<Dim>& currentBest,
                                const Point<Dim>& potential) const
{
    /**
     * @todo Implement this function!
     */
	int best_sum = 0; 
	int target_sum = 0; 
	for(int i = 0; i < Dim; i++){
		target_sum += pow((potential[i] - target[i]), 2); 
		best_sum += pow((currentBest[i] - target[i]), 2); 		
	}
	if(target_sum < best_sum) return true; 
	if(best_sum == target_sum){
		if(potential < currentBest) return true; 
		else return false; 
	}
    return false;
}

template <int Dim>
KDTree<Dim>::KDTree(const vector<Point<Dim>>& newPoints)
{
    /**
     * @todo Implement this function!
     */
	if(newPoints.empty()) return; 
	points.resize(newPoints.size()); 
	vector<Point<Dim>> PointsCopy = newPoints; 
	BuildTree(PointsCopy, points, 0, 0, points.size()-1); 
}

template <int Dim>
void KDTree<Dim>::BuildTree(vector<Point<Dim>>& original, vector<Point<Dim>>& retpoints, int dimension, int left, int right)
{
	if(left > right) return; 
	if(left == right){
		retpoints[left] = original[left]; 
		return; 
	}
	int middle = (left + right)/2;
	retpoints[middle] = kthSmallest(original, dimension%Dim, left, right, middle); 
	BuildTree(original, retpoints, dimension+1, left, middle-1); 
	BuildTree(original, retpoints, dimension+1, middle+1, right); 
}

template <int Dim>
Point<Dim> KDTree<Dim>::kthSmallest(vector<Point<Dim>>& original, int dimension, int left, int right, int middle)
{
	if(left == right) return original[right]; 
	int position = partition(original, left, right, dimension); 
	if(position == middle) return original[position]; 
	if(position > middle) return kthSmallest(original, dimension, left, position-1, middle); 
	else return kthSmallest(original, dimension, position+1, right, middle); 
}

template <int Dim>
int KDTree<Dim>::partition(vector<Point<Dim>>& original, int left, int right, int dimension)
{
	Point<Dim> last = original[right]; 
	int l_side = left; 
	for(int i = left; i < right; i++){
		if(smallerDimVal(original[i], last, dimension)){
			swap(original[l_side], original[i]); 
			l_side++; 
		}
	}
	swap(original[l_side], original[right]); 
	return l_side; 
}

template <int Dim>
void KDTree<Dim>::swap(Point<Dim>& first, Point<Dim>& second)
{
	Point<Dim> temp = first; 
	first = second; 
	second = temp; 
}

template <int Dim>
Point<Dim> KDTree<Dim>::findNearestNeighbor(const Point<Dim>& query) const
{
    /**
     * @todo Implement this function!
     */
	Point<Dim> ret; 
	ret = findNearestNeighbor(query, 0, 0, points.size()-1); 
    return ret;
}

template <int Dim>
Point<Dim> KDTree<Dim>::findNearestNeighbor(const Point<Dim>& query, size_t dimension, int left, int right) const
{
	Point<Dim> ret;  
	Point<Dim> possible_ret; 
	dimension = dimension%Dim; 
	//std::cout<<(left+right)/2<<std::endl; 
	//std::cout << left << std::endl; 
	//std::cout << right << std::endl; 
	bool trav_left = false; 
	if(left > right) return points[(points.size()-1)/2]; 
	if(left == right) return points[left]; 
	if(smallerDimVal(query, points[(left+right)/2], dimension)){
		ret = findNearestNeighbor(query, (dimension+1), left, (left+right)/2-1); 
		trav_left = true; 
	} else {
		ret = findNearestNeighbor(query, (dimension+1), (left+right)/2+1, right); 
	}
	int root_radius = findRadius(query, points[(left+right)/2]); 
	int ret_radius = findRadius(query, ret); 
	if(ret_radius > root_radius){
		ret = points[(left+right)/2]; 
		ret_radius = root_radius; 
	} else if(ret_radius == root_radius){
		if(points[(left+right)/2] < ret){
			ret = points[(left+right)/2]; 
			ret_radius = root_radius; 
		}
	}
	if(pow(points[(left+right)/2][dimension]-query[dimension], 2) <= ret_radius){
		if(trav_left) possible_ret = findNearestNeighbor(query, (dimension+1), (left+right)/2+1, right); 
		else possible_ret = findNearestNeighbor(query, (dimension+1), left, (left+right)/2-1); 
		int possible_radius = findRadius(query, possible_ret); 
		if(possible_radius < ret_radius || (possible_radius == ret_radius && possible_ret < ret)){
			ret = possible_ret; 
		}
	}
	return ret; 
}


template <int Dim>
int KDTree<Dim>::findRadius(const Point<Dim>& query, const Point<Dim>& subnode) const
{
	int sum = 0; 
	for(int i = 0; i < Dim; i++){
		sum += pow(query[i]-subnode[i], 2); 
	}
	return sum; 
}
