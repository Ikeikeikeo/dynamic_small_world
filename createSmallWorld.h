#pragma once
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <string>
#include <set>
#include <map>
#include <unordered_set>
#include <numeric>
#include <fstream>
#include <chrono>
#include <queue>
#include "random.h"

struct nodeInfo {
	int distance;
	double arg;
	int to;
	nodeInfo setValue(int distance, double arg, int to) {
		this->distance = distance;
		this->arg = arg;
		this->to = to;
		return *this;
	}
};

class SmallWorld {
public:
	std::vector<std::pair<int, int>> Edges; //Lattice�ł��Ƃ��Ƒ��݂���ӂ��Ǘ��B��������ӂ𗐑�����smallworld�𐶐�����B
	std::set<std::pair<int, int>> newEdges; //����smallworld�ɂȂ�ӂ��Ǘ��Bp=0�̎���Edges�ƈꏏ�B
	std::vector<std::unordered_set<int>> neighborList; //����node�̋ߖT���Ǘ��B���d�ӂ⎩�ȃ��[�v�̊Ǘ��ɕ֗��B
	int filename{};
	SmallWorld(int N) { neighborList.resize(N); }
	SmallWorld() {};
};
struct coordinate {
	int x;
	int y;
};

struct UnionFind{
	std::vector<int> parents;
	UnionFind(int size) {
		parents.assign(size, -1);
	}
	int findRoot(int x) {
		if (parents[x] < 0) return x;
		return parents[x] = findRoot(parents[x]);
	}
	bool unite(int x, int y) {
		x = findRoot(x);
		y = findRoot(y);
		if (x == y) return false;
		if (parents[x] > parents[y]) std::swap(x, y);
		parents[x] += parents[y];
		parents[y] = x;
		return true;
	}
	int size(int x) { return -parents[findRoot(x)]; }
	bool isSameGroup(int x, int y) { return findRoot(x) == findRoot(y); }
};

struct position {
	int nodeName;
	int distance;
};

std::vector<std::vector<int>> creatingLattice(int N, int k, bool isLoop);
std::vector<std::vector<nodeInfo>> _createGraph(int N, int length, int k, bool isLoop);
std::vector<std::vector<int>> _connectEachNeighbor(int N, int length, int k, bool isLoop);
std::vector<std::vector<int>> creatingSW(int k, double p, bool isLoop, std::vector<std::vector<int>>& reference);
bool Evaluator(int pickedNode, int notChosen, int newNode, std::vector<std::vector<int>>& reference, SmallWorld& sw);