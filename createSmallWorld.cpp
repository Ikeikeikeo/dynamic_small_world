#include "createSmallWorld.h"

std::map<std::pair<int, bool>, SmallWorld> dict;

std::vector<std::vector<int>> creatingLattice(int N, int k,bool isLoop) {
	int length = int(sqrt(N));
	if (length * length != N) {
		std::cout << "ERROR createSmallWorld" << std::endl;
		std::cout << "N is not a squared integer" << std::endl;
		exit(EXIT_SUCCESS);
	}
	else if (N < k) {
		std::cout << "k is larger than N, so You need to increase Number of N or decrease Number of k" << std::endl;
		exit(EXIT_SUCCESS);
	}
	//N,kに応じて生成Latticeを生成
	return _connectEachNeighbor(N, length, k, isLoop);
}

std::vector<std::vector<int>> _connectEachNeighbor(int N, int length, int k, bool isLoop) {
	std::vector<std::vector<int>> graph(N);
	std::vector<std::vector<nodeInfo>> neighborList;
	//各nodeに於いて仮想面を張るのがここ,高速化するならここ
	neighborList = _createGraph(N, length, k, isLoop);
	SmallWorld sw(N);
	for (int n = 0; n < N; ++n) {
		// 距離、角度、IDの順番でsort、自作structなので比較用にlambda式を使用
		std::sort(neighborList[n].begin(), neighborList[n].end(),[](const nodeInfo& fr, const nodeInfo& se) {
			if (fr.distance != se.distance) return fr.distance < se.distance;
			else if (fr.arg != se.arg) return fr.arg < se.arg;
			else return fr.to < se.to;
		});
		int limit = k;
		if (!isLoop) limit = std::min(k, (int)neighborList[n].size()); //loopなしの場合はkか隣接ノードの総数がlimitになる
		for (int node = 0; node < limit; ++node) {
			int connectedNode = neighborList[n][node].to;
			graph[n].emplace_back(connectedNode);
			auto pr = std::make_pair(std::min(n, connectedNode), std::max(n, connectedNode));
			sw.neighborList[n].emplace(connectedNode);
			sw.newEdges.emplace(pr);
		}
		std::sort(graph[n].begin(), graph[n].end());
	}
	std::vector<std::pair<int, int>> edge;
	for (auto& e : sw.newEdges) edge.emplace_back(e);
	sw.Edges = edge;
	dict[std::make_pair(k, isLoop)] = sw;
	return graph;
}

std::vector<std::vector<nodeInfo>> _createGraph(int N, int length, int k, bool isLoop) {
	std::vector<std::vector<nodeInfo>> graph(N);
	coordinate position;
	int dist = 1;
	int _n = 3;
	while (_n * _n < k) {
		_n += 2;
		dist++;
	}
	for (int n = 0; n < N; ++n) {
		int cnt = 0;
		position.x = n % length;
		position.y = n / length;
		coordinate next;
		nodeInfo Info;
		for (int dx = -dist; dx <= dist; ++dx) for (int dy = -dist; dy <= dist; ++dy) {
			if (dx == 0 && dy == 0) continue;
			next.x = position.x + dx;
			next.y = position.y + dy;
			if (!isLoop and (next.x < 0 || next.x >= length || next.y < 0 || next.y >= length)) continue;
			if (next.x < 0) next.x += length;
			else if (next.x >= length) next.x -= length;
			if (next.y < 0) next.y += length;
			else if (next.y >= length) next.y -= length;
			int to = next.x + next.y * length;
			double pi = atan(1) * 4;
			double arg = std::atan2(dy, dx) * 180 / pi;
			if (arg > 90) arg -= 90;
			else if (-90 <= arg && arg < 0) arg += 90;
			else if (arg < -90) arg += 180;
			int d = dx * dx + dy * dy;
			Info.setValue(d, arg, to);
			graph[n].emplace_back(Info);
		}
	}
	return graph;
}

bool Evaluator(int pickedNode, int notChosen, int newNode, std::vector<std::vector<int>>& reference,SmallWorld& sw) {
	if (newNode == pickedNode) return false;
	else if (newNode == notChosen) return false;
	else if (std::binary_search(reference[pickedNode].begin(), reference[pickedNode].end(), newNode)) return false;
	else if (sw.newEdges.find(std::make_pair(std::min(pickedNode, newNode), std::max(pickedNode, newNode))) != sw.newEdges.end()) return false;
	else return true;
}

std::vector<std::vector<int>> creatingSW(int k,double p,bool isLoop, std::vector<std::vector<int>>& reference) {
	int reconnectedEdge = 0;
	int N = reference.size();
	auto pr = std::make_pair(k, isLoop);
	SmallWorld sw;
	UnionFind uf(N);
	bool isConnected = false;
	std::vector<std::vector<int>> retVec(N);
	while (!isConnected) {
		retVec = std::vector<std::vector<int>>(N);
		if (dict.find(pr) == dict.end()) {
			std::cerr << "Cash error. Basic information of average edge = " << k << " is Not found. " << std::endl;
			std::cerr << "You must Initialize each Lattice network by using creating Lattice function." << std::endl;
			exit(true);
		}
		else sw = dict[pr];
		int NumofEdges = sw.Edges.size();
		Random* random;
		random = new Random();
		//std::cout << "H" << std::endl;
		while ((double)reconnectedEdge / NumofEdges < p) {
			int len = sw.Edges.size() - 1;
			int picker = random->random(len);
			auto picked = sw.Edges[picker];
			auto [pickedNode, notChosen] = picked;
			//選んだ辺が双方
			if (sw.neighborList[pickedNode].size() == 1 && sw.neighborList[notChosen].size() == 1) continue;
			if (random->random(1.0) < 0.5) std::swap(pickedNode, notChosen);
			if (sw.neighborList[notChosen].size() == 1)  std::swap(pickedNode, notChosen);
			std::iter_swap(sw.Edges.begin() + picker, sw.Edges.end() - 1);
			sw.Edges.pop_back();
			int&& newNode = -1;
			//std::cout << "I" << std::endl;
			do {
				newNode = random->random(N - 1);
			} while (!Evaluator(pickedNode, notChosen, newNode, reference, sw));
			sw.newEdges.erase(picked);
			sw.newEdges.emplace(std::make_pair(std::min(pickedNode, newNode), std::max(pickedNode, newNode)));
			sw.neighborList[pickedNode].emplace(newNode); sw.neighborList[newNode].emplace(pickedNode);
			sw.neighborList[notChosen].erase(pickedNode);
			++reconnectedEdge;
			//std::cout << "J" << std::endl;
		}
		for (auto&& [from, to] : sw.newEdges) {
			retVec[from].emplace_back(to);
			retVec[to].emplace_back(from);
			uf.unite(from, to);
		}
		int cnt = 0;
		if (uf.size(0) == N) isConnected = true;
	}
	//std::cout << "K" << std::endl;
	return retVec;
}