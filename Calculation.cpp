#include "Calculation.h"
using namespace std;


bool WhatNum_d(double VVV_in, string x_st) {
	//小数の場合
    string VVV_strn = to_string(VVV_in);
	VVV_strn.substr(0, 4); //4は文字数
    if (VVV_strn == x_st) return true;
	else return false;
}

bool WhatNum_i(int VVV_in, string x_st) {
	//整数の場合
	string VVV_strn = to_string(VVV_in);
	if (VVV_strn == x_st) return true;
	else return false;
}

double WhatVal(string SSS) {
	int m = 1;
	double v = 0;
	bool fg = false;
	vector<int> number;
	for (int i = 0; i < SSS.size(); i++) {
		int k;
		switch (SSS[i]) {
		case '0':
			k = 0;
			if (!fg) number.push_back(k);
			break;
		case '1':
			k = 1;
			if (!fg) number.push_back(k);
			break;
		case '2':
			k = 2;
			if (!fg) number.push_back(k);
			break;
		case '3':
			k = 3;
			if (!fg) number.push_back(k);
			break;
		case '4':
			k = 4;
			if (!fg) number.push_back(k);
			break;
		case '5':
			k = 5;
			if (!fg) number.push_back(k);
			break;
		case '6':
			k = 6;
			if (!fg) number.push_back(k);
			break;
		case '7':
			k = 7;
			if (!fg) number.push_back(k);
			break;
		case '8':
			k = 8;
			if (!fg) number.push_back(k);
			break;
		case '9':
			k = 9;
			if (!fg) number.push_back(k);
			break;
		case '.':
			k = 0;
			fg = true;
			break;
		default:
			cout << "ERROR TYPE VAL :: " << SSS[i] << endl;
			exit(EXIT_SUCCESS);
			break;
		}
			
		v += k * 1.0 / m;
        if (fg) m *= 10;
	}
    if (SSS.size() == number.size()){
		int v_int=0;
		for(int j=0; j<number.size(); j++) v_int += number[j] * pow(10,( (number.size() - 1) - j));
		return v_int;
	} 
	else return v;
}


Calculation::Calculation(int _D_hor, int _D_ver, int _filenumber){
    
    D_hor = _D_hor;
    D_ver = _D_ver;
    filenumber = _filenumber;
    //NNN = filenumber;
}

void Calculation::calculation() {

	bool flag = get_calculation_n();

	string FF = "./Result/" + to_string(filenumber) + ".dat";
	ofstream ofs(FF, ios::app);
	//ofs << "k" << "," << "p" << "," << "Transmission_rate" << "," << "FES" << "," << "Peak_Infected" << "," << "switch_count_ave" << endl;
	
	int N = 10000;

	map<int, vector<vector<int>>> LatticeList;
	for (int k = 8; k <= 64; k += 8) LatticeList[k] = creatingLattice(N, k, false);

	if (flag) {
#ifdef _OPENMP
#pragma omp parallel for 
#endif //_OPENMP
		for (int t = 0; t < n.size(); ++t) {

			int p = 0;
			int k = 0;

			p = n[t] / D_ver;
			k = 8 + 8 * ((n[t]) % D_ver);
			//cout << "p = " << p << endl;

			Infection* infection = new Infection(N, p, k);

			//if(p<10) cout <<"p = 0.0" <<  p << ", <k> =" <<k << endl;
			//else cout <<"p = 0." <<  p << ", <k> =" <<k << endl;

			infection->infection_part(LatticeList);

			//if (t==0) cout << "num0=" << infection->agent.num0 << "  ,num1=" << infection->agent.num1 << "   ,num2=" << infection->agent.num2 << "   ,num3=" << infection->agent.num3 << "   ,num4=" << infection->agent.num4 << endl;
#ifdef _OPENMP
#pragma omp critical //競合を防ぐ
#endif //_OPENMP
			{
				ofs << p << "," << k << "," << infection->Beta << "," << infection->FES << "," << infection->Peak_Infected << "," << infection->switch_count_ave << endl;
				cout << "p=" << p << ", k=" << k << ", Transmission_rate=" << infection->Beta << ", FES=" << infection->FES << ", Peak_Infected=" << infection->Peak_Infected << ", switch_count_ave=" << infection->switch_count_ave << endl;
				//if (t == 0) ofs << "num0=" << infection->agent.num0 << ",num1=" << infection->agent.num1 << ",num2=" << infection->agent.num2 << ",num3=" << infection->agent.num3 << ",num4=" << infection->agent.num4 << endl;
			}

			delete infection;
		}
	}
	ofs.close();
}


bool Calculation::get_calculation_n() {
	bool flag = true;
	string Data_file = "./Result/" + to_string(filenumber) + ".dat";
	ifstream ifs(Data_file);

	if (!ifs) {
		n = vector<int>(D_hor * D_ver);
#ifdef _OPENMP
#pragma omp parallel for
#endif //_OPENMP
		for (int i=0; i < D_hor * D_ver; ++i) n[i] = i;
	}
	else {
		vector<bool> fg(D_hor * D_ver, true);
		string SSS;
		int nnn = 0;
		while (getline(ifs, SSS)) {
			++nnn;
		}
		if (nnn > 0) {
			ifs.clear();
			ifs.seekg(0, ios::beg);
			while (getline(ifs, SSS)) {
				string p_st, k_st;
				p_st = k_st = "";
				int w = 0;
				for (; w < SSS.size(); w++) {
					if (SSS[w] == ',') break;
					else p_st += SSS[w];
				}
				for (++w; w < SSS.size(); w++) {
					if (SSS[w] == ',') break;
					else k_st += SSS[w];
				}

				int p_int = getIntNum_k(p_st, D_hor);  //p,kはまだstringだからintに変換
				int k_int = getIntNum_k(k_st, D_ver);

                //cout<< "p_int = " << p_int << "," << "k_int = " << k_int << endl; 
				
                //cout << "p_int(0~100) = " << p_int << endl;
				//cout << "k_int(4~24) = " << k_int << endl;
				int Num = (p_int * D_ver) + (k_int / 8) - 1;
				//cout << "Line = " << Line << endl; 
				fg[ Num ] = false;
				if (ifs.eof()) break; //data_fileの最後の行に来たら終わり
			}
			flag = false;
			for (int i=0; i < D_hor * D_ver; ++i){
				if (fg[i]) { //fg[i] = true だったら  ---> まだ数値がもとまっていなかったら
					n.push_back(i);   //その参照番号 i をcalculationで使う p,k を得るための配列 n[t] に追加していく
					flag = true;      //calculation内で計算に移るため
				}
			}

            //for (int i=0; i < n.size(); i++) cout << "n[" << i << "] = " << n[i] << endl;

		}
		else {
			n = vector<int>(D_hor * D_ver);
#ifdef _OPENMP
#pragma omp parallel for
#endif //_OPENMP
			for (int i=0; i<D_hor * D_ver; ++i) n[i] = i;
		}
	}
	ifs.close();
	return flag;
}

int Calculation::getIntNum_p(string p_st, int D_hor) {
    //cout << "p_st" << p_st << endl;
	double val = WhatVal(p_st);      //(string p_st=0.33) --> double val=0.33
    //cout << "val = " << val << endl;
	int III = val * (D_hor - 1);     // double val=0.33  -->  int III = 33
    //cout << "III = " << III << endl; 
    double VVV = III * 0.01;         //たまにおかしいヤツが出てくるから
    //cout << "VVV = " << VVV << endl;
	if ( !WhatNum_d(VVV, p_st)) {
		int IIIp = III + 1;
		int IIIm = III - 1;
		while (true) {
			if (WhatNum_d(IIIp * 0.01, p_st)) {
				III = IIIp;
				break;
			}
			else IIIp++;
			if (WhatNum_d(IIIm * 0.01, p_st)) {
				III = IIIm;
				break;
			}
			else IIIm--;
		}
	}
	return III;
}

int Calculation::getIntNum_k(string k_st, int D_ver) {
	double val = WhatVal(k_st);    //(string k_st = 4) --> double val = 4.000
	int III = val * 1.0;           // double val=4.000 --->  (int III = 4)
    /*cout << "III = " << III << endl;
    double VVV = III * 1.0;               
	if (!WhatNum_d(VVV, k_st)) {
		int IIIp = III + 4;
		int IIIm = III - 4;
		while (true) {
			if (WhatNum_d(IIIp * 1.0, k_st)) {
				III = IIIp;
				break;
			}
			else IIIp++;
			if (WhatNum_d(IIIm * 1.0, k_st)) {
				III = IIIm;
				break;
			}
			else IIIm--;
		}
	}*/
	return III;
}
