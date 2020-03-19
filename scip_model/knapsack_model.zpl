# f = open("instances/ks_400_0_series_i.csv", "r")
# lines = [l for l in f]
# f.close()
# nlines = nlines = [str(i-1) + "," + lines[i] for i in range(len(lines))]
# f = open("instances/ks_400_0_series_i.csv", "w")
# f.write("".join(nlines))
# f.close()
# comenzo: 00:37:00 hrs - 12:40:41: 00:03:41
param n := read "instances/ks_400_0_parameters.txt" as "2n" skip 1 use 1;
param K := read "instances/ks_400_0_parameters.txt" as "2n" skip 2 use 1;
set I := {0 .. n - 1};

param v[I] := read "instances/ks_400_0_series_i.csv" as "<1n> 2n" skip 1;
param w[I] := read "instances/ks_400_0_series_i.csv" as "<1n> 3n" skip 1;

var x[I] binary;

#######################################################################
maximize value:
    sum <i> in I: v[i] * x[i];
#
#######################################################################

########################
subto weight_capacity:
    sum <i> in I: w[i] * x[i] <= K;
#
########################
