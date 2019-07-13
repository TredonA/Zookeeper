#include "Zookeeper.h"

struct option longOpts[] = {
	{ "help", no_argument, nullptr, 'h' },
	{ "mode", required_argument, nullptr, 'm' },
	{ nullptr, 0, nullptr, '\0' }
};

int main(int argc, char **argv) {
	cout << setprecision(2);
	cout << fixed;
	std::ios_base::sync_with_stdio(false);
	try {
		int option = 0, option_index = 0;
		bool modeChosen = false;
		string modeType;
		while ((option = getopt_long(argc, argv, "hm:", longOpts,
			&option_index)) != -1) {
			switch (option) {
			case 'h':
				cout << "Welcome to Trey's zookeeper guide.\n";
				cout << "We hope that this can help you navigate you're";
				cout << " feeding responsibilities easier. Here are the";
				cout << " the commands.\n";
				cout << "-h --help Type this command for the quick guide.\n";
				cout << "-m --mode <Mode> Type this command followed by ";
				cout << "'MST', 'FASTTSP', or 'OPTTSP' to run your intended ";
				cout << "function.";
				exit(0);
			case 'm':
				modeChosen = true;
				modeType = optarg;
			}
		}
		if (modeChosen == false) {
			throw "No mode was chosen by the user.\n";
		}
		if (modeType != "MST" &&
			modeType != "FASTTSP" &&
			modeType != "OPTTSP") {
			throw "Invalid error type was given by user.\n";
		}
		switch (modeType[0]) {
		case 'M':
			MST_function();
			break;

		case 'F':
			FASTTSP_function();
			break;

		case 'O':
			OPTTSP_function();
			break;
		}
	}
	catch (const char* err) {
		std::cerr << err << "\n";
		std::cout << "invalid" << "\n";
		return 1;
	}
	return 0;
}

void MST_function() {
	int numOfCages = 0;
	cin >> numOfCages;
	vector<Vertex> cages;
	cages.reserve(numOfCages);
	int xValue = 0;
	int yValue = 0;
	int numOfWildCages = 0;
	int numOfBorderCages = 0;
	double weightOfMST = 0;
	int Index = 0;
	while (cin >> xValue >> yValue) {
		Vertex newCoord(xValue, yValue, Index);
		cages.push_back(newCoord);
		if (inThirdQuadrant(cages[Index])) {
			++numOfWildCages;
		}
		else if (onWildBorder(cages[Index])) {
			++numOfBorderCages;
		}
		++Index;
	}
	if (numOfWildCages > 0 &&
		numOfWildCages != numOfCages) {
		if (numOfBorderCages == 0) {
			throw "Cannot construct MST";
		}
	}
	int currentVertex = 0;
	int cagesInTree = 1;
	cages[0].visited = true;
	double currentMin = -1;
	int minIndex = -1;
	while (cagesInTree != numOfCages) {
		for (int i = 1; i < numOfCages; ++i) {
			if (cages[i].visited == true) {
				continue;
			}
			double distanceFromVertex = distanceBetweenTwoVertices
			(cages[currentVertex], cages[i]);
			if (distanceFromVertex != -1) {
				if (cages[i].distanceFromParent == -1) {
					cages[i].distanceFromParent = distanceFromVertex;
					cages[i].parentVertex = currentVertex;
				}
				else if (cages[i].distanceFromParent >
					distanceFromVertex) {
					cages[i].distanceFromParent = distanceFromVertex;
					cages[i].parentVertex = currentVertex;
				}
			}
			else if (cages[i].distanceFromParent == -1) {
				continue;
			}
			if (currentMin == -1) {
				currentMin = cages[i].distanceFromParent;
				minIndex = i;
			}
			else if (currentMin > cages[i].distanceFromParent) {
				currentMin = cages[i].distanceFromParent;
				minIndex = i;
			}
		}
		weightOfMST += currentMin;
		cages[minIndex].visited = true;
		currentVertex = minIndex;
		currentMin = -1;
		++cagesInTree;
	}
	cout << weightOfMST << "\n";
	for (size_t k = 1; k < cages.size(); ++k) {
		if (cages[k].parentVertex < cages[k].initialIndex) {
			cout << cages[k].parentVertex << " ";
			cout << cages[k].initialIndex << "\n";
		}
		else {
			cout << cages[k].initialIndex << " ";
			cout << cages[k].parentVertex << "\n";
		}
	}
	return;
}

void FASTTSP_function() {
	int numOfCages = 0;
	cin >> numOfCages;
	vector<Vertex> cages;
	cages.reserve(numOfCages);
	int xValue = 0;
	int yValue = 0;
	double weightOfMST = 0;
	int Index = 0;
	while (cin >> xValue >> yValue) {
		Vertex newCoord(xValue, yValue, Index);
		cages.push_back(newCoord);
		++Index;
	}
	int currentVertex = 0;
	vector<int> orderOfVisits;
	orderOfVisits.reserve(numOfCages);
	orderOfVisits.push_back(0);
	cages[0].visited = true;
	double currentMin = -1;
	int minIndex = -1;
	while (int(orderOfVisits.size()) != numOfCages + 1) {
		if (orderOfVisits.size() < 2) {
			for (int i = 1; i < numOfCages; ++i) {
				if (cages[i].visited == true) {
					continue;
				}
				double distanceFromVertex = distanceBetweenTwoVertices2
				(cages[currentVertex], cages[i]);
				cages[i].distanceFromParent = distanceFromVertex;
				cages[i].parentVertex = currentVertex;
				if (currentMin == -1) {
					currentMin = cages[i].distanceFromParent;
					minIndex = i;
				}
				else if (currentMin > cages[i].distanceFromParent) {
					currentMin = cages[i].distanceFromParent;
					minIndex = i;
				}
			}
			cages[minIndex].visited = true;
			orderOfVisits.push_back(minIndex);
		}
		else {
			orderOfVisits.push_back(cages[0].initialIndex);
			arbitraryInsertFunctor(cages, orderOfVisits, numOfCages);
		}
	}
	for (int t = 0; t < int(orderOfVisits.size()) - 1; ++t) {
		weightOfMST += distanceBetweenTwoVertices2(cages[orderOfVisits[t]],
			cages[orderOfVisits[t + 1]]);
	}
	cout << weightOfMST << "\n";
	for (size_t k = 0; k < orderOfVisits.size() - 1; ++k) {
		cout << orderOfVisits[k] << " ";
	}
	return;
}

void OPTTSP_function() {
	int numOfCages = 0;
	cin >> numOfCages;
	vector<Vertex> cages;
	cages.reserve(numOfCages);
	int xValue = 0;
	int yValue = 0;
	double weightOfMST = 0;
	int Index = 0;
	while (cin >> xValue >> yValue) {
		Vertex newCoord(xValue, yValue, Index);
		cages.push_back(newCoord);
		++Index;
	}
	int currentVertex = 0;
	vector<int> orderOfVisits;
	orderOfVisits.reserve(numOfCages);
	orderOfVisits.push_back(0);
	cages[0].visited = true;
	double currentMin = -1;
	int minIndex = -1;
	while (int(orderOfVisits.size()) != numOfCages + 1) {
		if (orderOfVisits.size() < 2) {
			for (int i = 1; i < numOfCages; ++i) {
				if (cages[i].visited == true) {
					continue;
				}
				double distanceFromVertex = distanceBetweenTwoVertices2
				(cages[currentVertex], cages[i]);
				cages[i].distanceFromParent = distanceFromVertex;
				cages[i].parentVertex = currentVertex;
				if (currentMin == -1) {
					currentMin = cages[i].distanceFromParent;
					minIndex = i;
				}
				else if (currentMin > cages[i].distanceFromParent) {
					currentMin = cages[i].distanceFromParent;
					minIndex = i;
				}
			}
			cages[minIndex].visited = true;
			orderOfVisits.push_back(minIndex);
		}
		else {
			orderOfVisits.push_back(cages[0].initialIndex);
			arbitraryInsertFunctor(cages, orderOfVisits, numOfCages);
		}
	}
	for (int t = 0; t < int(orderOfVisits.size()) - 1; ++t) {
		weightOfMST += distanceBetweenTwoVertices2(cages[orderOfVisits[t]],
			cages[orderOfVisits[t + 1]]);
		cages[orderOfVisits[t]].visited = false;
		cages[orderOfVisits[t]].distanceFromParent = -1;
	}
	double distance = 0;
	orderOfVisits.pop_back();
	mostOptimalSolution currentOptimalSolution(weightOfMST, orderOfVisits);
	genPerms(cages, 1, distance, currentOptimalSolution);
	cout << currentOptimalSolution.weight << "\n";
	for (int k = 0; k < int(cages.size()); ++k) {
		cout << currentOptimalSolution.path[k] << " ";
	}
}

bool inThirdQuadrant(Vertex &vertex) {
	return (vertex.xVal < 0 && vertex.yVal < 0);
}
bool onWildBorder(Vertex &vertex) {
	return ((vertex.xVal == 0 && vertex.yVal <= 0)
		|| (vertex.yVal == 0 && vertex.xVal <= 0) ||
		(vertex.yVal == 0 && vertex.xVal == 0));
}

double distanceBetweenTwoVertices(Vertex &vertex1, Vertex &vertex2) {
	if (inThirdQuadrant(vertex1)) {
		if (inThirdQuadrant(vertex2) || onWildBorder(vertex2)) {
			return sqrt((double(vertex1.xVal - vertex2.xVal) *
				double(vertex1.xVal - vertex2.xVal)) +
				(double(vertex1.yVal - vertex2.yVal) *
					double(vertex1.yVal - vertex2.yVal)));
		}
		else {
			return -1;
		}
	}
	else if (inThirdQuadrant(vertex2)) {
		if (onWildBorder(vertex1)) {
			return sqrt((double(vertex1.xVal - vertex2.xVal) *
				double(vertex1.xVal - vertex2.xVal)) +
				(double(vertex1.yVal - vertex2.yVal)
					* double(vertex1.yVal - vertex2.yVal)));
		}
		else {
			return -1;
		}
	}
	return sqrt((double(vertex1.xVal - vertex2.xVal) *
		double(vertex1.xVal - vertex2.xVal)) +
		(double(vertex1.yVal - vertex2.yVal) *
			double(vertex1.yVal - vertex2.yVal)));
}

double distanceBetweenTwoVertices2(Vertex &vertex1, Vertex &vertex2) {
	double total = sqrt(((double(vertex1.xVal - vertex2.xVal) *
		double(vertex1.xVal - vertex2.xVal))) +
		((double(vertex1.yVal - vertex2.yVal) *
			double(vertex1.yVal - vertex2.yVal))));
	return total;
}

void arbitraryInsertFunctor(vector<Vertex> &vertices, vector<int> &
	currentOrder, int numOfCages) {
	int verticesIndex = 1;
	while (verticesIndex != numOfCages) {
		if (vertices[verticesIndex].visited) {
			++verticesIndex;
			continue;
		}
		double possibleMin = -1;
		int possibleIndex = -1;
		for (int i = 0; i < int(currentOrder.size()) - 1; ++i) {
			double newMin =
				distanceBetweenTwoVertices2(vertices[currentOrder[i]],
					vertices[verticesIndex]) +
				distanceBetweenTwoVertices2(vertices[currentOrder[i + 1]],
					vertices[verticesIndex]) -
				distanceBetweenTwoVertices2(vertices[currentOrder[i]],
					vertices[currentOrder[i + 1]]);
			if (possibleMin == -1) {
				possibleMin = newMin;
				possibleIndex = i;
			}
			else if (possibleMin > newMin) {
				possibleMin = newMin;
				possibleIndex = i;
			}
		}
		vector<int>::iterator it = currentOrder.begin();
		currentOrder.insert(it + possibleIndex + 1, verticesIndex);
		vertices[verticesIndex].visited = true;
		++verticesIndex;
	}
}

void genPerms(vector<Vertex> &path,
	size_t permLength, double distance,
	mostOptimalSolution &currentBest) {
	double newDistance = distance;
	if (path.size() == permLength) {
		newDistance += distanceBetweenTwoVertices2(path[permLength - 1],
			path[0]);
		if (newDistance < currentBest.weight) {
			currentBest.weight = newDistance;
			for (size_t l = 0; l < permLength; ++l) {
				currentBest.path[l] = path[l].initialIndex;
			}
		}
		return;
	} // if
	if (!promising(path, permLength, distance, currentBest))
		return;
	for (size_t i = permLength; i < path.size(); ++i) {
		swap(path[permLength], path[i]);
		double minorDistanceAdd =
			distanceBetweenTwoVertices2(path[permLength - 1],
				path[permLength]);
		newDistance += minorDistanceAdd;
		genPerms(path, permLength + 1,
			newDistance, currentBest);
		newDistance -= minorDistanceAdd;
		swap(path[permLength], path[i]);
	} // for
} // genPerms()

bool promising(vector<Vertex> &path, size_t permLength,
	double distance, mostOptimalSolution &currentBest) {
	if (path.size() - permLength <= 4) {
		return true;
	}
		vector<Vertex> cages = path;
		int currentVertex = int(permLength);
		int cagesInTree = int(permLength);
		int numOfCages = int(path.size());
		double mstTreeWeight = 0;
		cages[permLength].visited = true;
		double currentMinX = -1;
		double currentMin1 = -1;
		double currentMin2 = -1;
		int minIndex = -1;
		while (cagesInTree != numOfCages) {
			for (int i = int(permLength); i < numOfCages; ++i) {
				if (cages[i].visited == true) {
					continue;
				}
				double distanceFromVertex = distanceBetweenTwoVertices2
				(cages[currentVertex], cages[i]);
				if (distanceFromVertex != -1) {
					if (cages[i].distanceFromParent == -1) {
						cages[i].distanceFromParent = distanceFromVertex;
						cages[i].parentVertex = currentVertex;
					}
					else if (cages[i].distanceFromParent >
						distanceFromVertex) {
						cages[i].distanceFromParent = distanceFromVertex;
						cages[i].parentVertex = currentVertex;
					}
				}
				if (currentMinX == -1) {
					currentMinX = cages[i].distanceFromParent;
					minIndex = i;
				}
				else if (currentMinX > cages[i].distanceFromParent) {
					currentMinX = cages[i].distanceFromParent;
					minIndex = i;
				}
				if (permLength == 1) {
					double possibleMin = distanceBetweenTwoVertices2(cages[0],
						cages[i]);
					if (currentMin1 == -1) {
						currentMin1 = possibleMin;
						currentMin2 = possibleMin;
					}
					else {
						if (currentMin1 > possibleMin) {
							currentMin2 = possibleMin;
							currentMin1 = possibleMin;
						}
					}
				}
				else {
					double possibleMin1st = distanceBetweenTwoVertices2(cages[0],
						cages[i]);
					double possibleMinLast =
						distanceBetweenTwoVertices2(cages[permLength - 1],
							cages[i]);
					if (currentMin1 == -1) {
						currentMin1 = possibleMin1st;
					}
					else if (currentMin1 > possibleMin1st) {
						currentMin1 = possibleMin1st;
					}
					if (currentMin2 == -1) {
						currentMin2 = possibleMinLast;
					}
					else if (currentMin2 > possibleMinLast) {
						currentMin2 = possibleMinLast;
					}
				}
			}
			mstTreeWeight += currentMinX;
			cages[minIndex].visited = true;
			currentVertex = minIndex;
			currentMinX = -1;
			++cagesInTree;
		}

		mstTreeWeight += currentMin1;
		mstTreeWeight += currentMin2;
		mstTreeWeight += distance;
		if (mstTreeWeight >= currentBest.weight) {
			return false;
		}
		else {
			return true;
		}
	
}
