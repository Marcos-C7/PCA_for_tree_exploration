param n = read "instances/ks_4_0_parameters.txt" as "<2n>" skip 1;
param K = read "instances/ks_4_0_parameters.txt" as "<2n>" skip 2;
set I = {0 .. n - 1}

param v[I] = read "instances/ks_4_0_series_i.csv" as "<1n> 2n" skip 1;
param w[I] = read "instances/ks_4_0_series_i.csv" as "<1n> 3n" skip 1;

var x[I] binary;

do print n;
