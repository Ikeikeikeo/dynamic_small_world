#include "Infection.h"
#include <chrono>

Infection::Infection(int _N, int _p,int _k) {
	N = _N;
	p = _p;
	k = _k;
	agent.random = new Random();
	isLoop = false;
}

Infection::~Infection() {
	delete agent.random;
}

void Infection::initial_probability() {
    S_to_I_probability = I_to_R_probability = 0;

    for (int i = 0; i < N; ++i) {
        switch (agent.state[i]) {
        case S:
            S_to_I_probability += agent.NI[i] * Beta;
            break;
        case I:
            I_to_R_probability += Gamma;
            break;
        default:
            break;
        }
    }
}

void Infection::time_change() {
	double dt, Lambda;
	Lambda = S_to_I_probability + I_to_R_probability;

	double u = agent.random->random(1.0);
	dt = -log(1.0 - u) / Lambda;
	time += dt;
}

void Infection::state_change() {
	double x = agent.random->random(1.0);
	double p = S_to_I_probability / (S_to_I_probability + I_to_R_probability);

	if (x < p) {
		double x_prime = x / p;
		S_to_I_change(x_prime);
	}
	else {
		double x_prime = (x - p) / (1.0 - p);
		I_to_R_change(x_prime);
	}
}

void Infection::S_to_I_change(double prime_x) {
	double probability = 0;

	for (int i = 0; i < (int)agent.S_list.size(); ++i) {
		int num = agent.S_list[i];
		probability += Beta * agent.NI[num] / S_to_I_probability;

		if (probability > prime_x) {
			agent.state[num] = I;
			agent.I_list.emplace_back(num);
			iter_swap(agent.S_list.begin() + i, agent.S_list.end() - 1);
			agent.S_list.pop_back();
			--S_number;
			++I_number;
			S_to_I_probability -= Beta * agent.NI[num];
			I_to_R_probability += Gamma;

			for (int j = 0; j < (int)agent.neighbors[num].size(); ++j) {
				int nb = agent.neighbors[num][j];
				if (agent.state[nb] == S) {
					++agent.NI[nb];
					S_to_I_probability += Beta;
				}
			}
			break;
		}
	}
}

void Infection::I_to_R_change(double prime_x) {
	double probability = 0;

	for (int i = 0; i < (int)agent.I_list.size(); ++i) {
		int num = agent.I_list[i];
		probability += Gamma / I_to_R_probability;

		if (probability > prime_x) {
			agent.state[num] = R;
			iter_swap(agent.I_list.begin() + i, agent.I_list.end() - 1);
			agent.I_list.pop_back();
			--I_number;
			++R_number;
			I_to_R_probability -= Gamma;

			for (int j = 0; j < (int)agent.neighbors[num].size(); ++j) {
				int nb = agent.neighbors[num][j];
				if (agent.state[nb] == S) {
					--agent.NI[nb];
					S_to_I_probability -= Beta;
				}
			}
			break;
		}
	}
}

void Infection::infection_part(map<int,vector<vector<int>>>& LatticeList) {
	Gamma = 1.0 / 3.0;
	Beta = 2.5 * Gamma / k;
	Peak_Infected = 0;
	FES = 0;
	switch_count_ave = 0;
	int episode = 1;

	//for (int i = 8; i <= 64; i += 8) agent.LatticeList[i] = creatingLattice(N, i, isLoop);
	agent.switch_network(N, p, k, isLoop,LatticeList);

	for (int i = 0; i < episode; ++i) {
		S_number = N;
		I_number = 0;
		R_number = 0;
		time = 0;
		double time_span = 3.0;
		double day = time_span;
		int I_max = 0;
		int switch_count = 0;
		agent.initialize_state(N, S_number, I_number, R_number);
		initial_probability();  //‚P‰ñ–Ú‚¾‚¯‚Í”‚¦ã‚°

		while (I_number > 0) {
			time_change();
			state_change();

			if (I_max < I_number) I_max = I_number;

			if (time > day) {
				agent.switch_network(N, p, k, isLoop,LatticeList);
				int NI_plus = agent.update_NI(N);
				S_to_I_probability = Beta * NI_plus;
				day = int(time / time_span) * time_span + time_span;

				switch_count += 1;
			}
		}
		switch_count_ave += switch_count;
		Peak_Infected += double(I_max) / double(N);
		FES += double(R_number) / double(N);
		//cout << "Now is ( " << i+1 << " / 5 ) times..." << "noaverage FES is = " << double(R_number) / double(N) << endl;
	}
	switch_count_ave /= episode;
	FES /= episode;
	Peak_Infected /= episode;
}