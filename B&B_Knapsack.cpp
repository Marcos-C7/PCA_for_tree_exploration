#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <utility>
#include <algorithm>

using namespace std;


bool compare(pair<float, int> a, pair<float, int> b)
{
	return a.first >= b.first;
}

struct input
{
	input(int nn = 0, int KK = 0)
	{
		n = 0;
		K = KK;
		alloc(nn);
	}
	
	void alloc(int nn)
	{
		if(n > 0)
		{
			delete[] v;
			delete[] w;
		}
		v = 0;
		w = 0;
		if(nn > 0)
		{
			v = new int[nn];
			w = new int[nn];
			order.reserve(nn);
		}
		n = nn;
	}
	
	~input()
	{
		if(v) delete v;
		if(w) delete w;
	}
	
	void set_order()
	{
		for(int i = 0; i < n; ++i) order.push_back(make_pair(float(v[i]) / float(w[i]), i));
		sort(order.begin(), order.end(), compare);
	}
	
	int n;
	int K;
	int *v;
	int *w;
	vector< pair<float, int> > order;
};

struct output
{
	output(int nn = 0)
	{
		n = 0;
		alloc(nn);
	}
	
	void alloc(int nn)
	{
		obj = 0;
		opt = 0;
		if(n > 0) delete[] x;
		x = 0;
		if(nn > 0)
		{
			x = new int[nn];
			for(int i = 0; i < nn; ++i) x[i] = 0;
		}
		n = nn;
	}
	
	~output()
	{
		if(x) delete[] x;
	}
	
	int obj;
	int opt;
	int n;
	int *x;
};


void solve(input &in, output &out, output &best_out, int j, int current_val, int current_weight, int &best)
{
	int best_possible(0);
	int aux_weight(current_weight);
	int next_weight(0);
	
	//cout<<"Estamos en: x_"<<(j < in.n ? in.order[j].second : -1)<<endl;
	
	if(j == in.n)
	{
		if(current_val > best)
		{
			best = current_val;
			best_out.obj = current_val;
			best_out.opt = 1;
			for(int i = 0; i < out.n; ++i) best_out.x[i] = out.x[i];
			//cout<<"Esta hoja define un nuevo minimo: "<<best<<endl;
		}
		return;
	}
	else
	{
		best_possible = current_val;
		for(int i = j; i < in.n; ++i)
		{
			if(aux_weight >= in.K) break;
			next_weight = min(in.K - aux_weight, in.w[in.order[i].second]);
			aux_weight += next_weight;
			best_possible += next_weight * in.order[i].first;
		}
		
		if(best_possible < best) return;
		
		out.x[in.order[j].second] = 0;
		solve(in, out, best_out, j + 1, current_val, current_weight, best);
		
		if(in.w[in.order[j].second] + current_weight <= in.K)
		{
			out.x[in.order[j].second] = 1;
			solve(in, out, best_out, j + 1, current_val + in.v[in.order[j].second], current_weight + in.w[in.order[j].second], best);
			out.x[in.order[j].second] = 0;
		}
	}
}

/*
output solve2(input &in)
{
	int		**table = new int*[2];
	int		*aux_row(0);
	int		*backtrace(0);
	int		max[2] = {0,0};
	output	out(in.n);
	
	backtrace = new int[in.n];
	table[0] = new int[in.K + 1];
	table[1] = new int[in.K + 1];
	
	for(int i = 0; i <= in.K; ++i) table[1][i] = 0;
	
	// Over every element [0, ..., n -1].
	for(int i = 0; i < in.n; ++i)
	{
		aux_row = table[0];
		table[0] = table[1];
		table[1] = aux_row;
		
		max[0] = -9999999;
		// Over every capacity [0, ..., K].
		for(int k = 0; k <= in.K; ++k)
		{
			// If the weight does not fit in the capacity, then just copy the value of the previous object.
			// Else the maximum between using this object and not using it.
			if(k < in.w[i]) table[1][k] = table[0][k];
			else table[1][k] = (table[0][k] > table[0][k - in.w[i]] + in.v[i] ? table[0][k] : table[0][k - in.w[i]] + in.v[i]);
			if(table[1][k] > max[0])
			{
				max[0] = table[1][k];
				max[1] = k;
			}
		}
		
		for(int k = 0; k <= in.K; ++k) cout<<table[1][k]<<"\t";
		cout<<endl;
		
		// Adjust the backtrace.
		backtrace[i] = max[1];
		if(i > 0 && table[0][max[1]] < table[1][max[1]])
		{
			int d = backtrace[i - 1] + in.w[i] - backtrace[i];
			for(int h = 0; h < i; ++h) backtrace[h] -= d;
		}
	}
	
	out.obj = table[1][in.K];
	for(int i = 0; i < in.n; ++i) {out.x[i] = backtrace[i]; cout<<backtrace[i]<<"  ";}
	
	delete table[0];
	delete table[1];
	delete table;
	delete backtrace;
	
	return out;
}
*/

int main(int argc, char **argv)
{
	string			file_name = argc > 1 ? argv[1] : "knapsack_instances/ks_400_0";
	ifstream		file;
	int				n(0), K(0);
	input			in;
	output			out, best_out;
	int				best(-1);
	
	
	file.open(file_name.c_str(), ios::in);
	
	file>>n;
	file>>K;
	in.K = 14000;
	in.alloc(n);
	
	for(int i = 0; i < n; ++i)
	{
		file>>in.v[i];
		file>>in.w[i];
	}
	
	file.close();
	
	in.set_order();
	
	out.alloc(in.n);
	best_out.alloc(in.n);
	
	cout<<"n="<<in.n<<"   K="<<in.K<<endl;
	solve(in, out, best_out, 0, 0, 0, best);
	
	cout<<best_out.obj<<endl;
	//for(int i = 0; i < out.n; ++i) cout<<best_out.x[i]<<(i < out.n - 1 ? "  " : "");
	
	return 0;
}







