#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <getopt.h>
#include <queue>
#include <iomanip>
#include <sstream>
#include <getopt.h>
#include <cmath>
using namespace std;

struct Vertex {
	bool visited;
	int xVal;
	int yVal;
	int initialIndex;
	int parentVertex;
	double distanceFromParent;
	Vertex(int x, int y, int indexVal) :
		visited(false), xVal(x), yVal(y), 
		initialIndex(indexVal), parentVertex(-1), distanceFromParent(-1) {};
};

struct mostOptimalSolution{
	double weight;
	vector<int> path;
	mostOptimalSolution(double totalWeight, vector<int> &cages) :
		weight(totalWeight), path(cages) {};
};
// MST driver for figuring out the best path for the zookeeper.
void MST_function();
// FASTTSP driver for figuring out the best path for the zookeeper.
void FASTTSP_function();
// OPTTSP driver for figuring out the best path for the zookeeper.
void OPTTSP_function();
// Function to see if vertex is in third quadrant
bool inThirdQuadrant(Vertex &vertex);
// Function to see if vertex is on wild cage border
bool onWildBorder(Vertex &vertex);
// Calculates distance between vertices. Accounts for cages
// within wild cage section as well as cages on the border of the
// wild area.
double distanceBetweenTwoVertices(Vertex &vertex1, Vertex &vertex2);
// Same as above function but is intended for parts B and C. Thus,
// the wild cage/border checks have been removed.
double distanceBetweenTwoVertices2(Vertex &vertex1, Vertex &vertex2);
// Quick function to help with arbitrary insertion tour calculation
void arbitraryInsertFunctor(vector<Vertex> &vertices, vector<int> &
	currentOrder, int numOfCages);
// Gen perm from Project 4 Tutorial
void genPerms(vector<Vertex> &path, size_t permLength, double distance, 
	mostOptimalSolution &currentBest);
// Accompanying 'promising' function for genPerms
bool promising(vector<Vertex> &path, size_t permLength, 
double distance, mostOptimalSolution &currentBest);
