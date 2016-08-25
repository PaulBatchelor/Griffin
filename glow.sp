(
_grif "./griffin.so" fl
_trig _gate _grif fe
_sig 0 varset
_lvl 0 varset
_dry var

_seq "0 2 11 9 0 7 2 9" gen_vals

_trig get 0.1 tgate
#0.1 port
0.3 0.001 1 0.3 adsr
_sig set

_sig get 0.1 port 0.01 0.001 scale dmetro
0.005 tgate 3.9 0.001 1 0.01 adsr 0.4 port

# _trig get 0.01 tgate 1.1 0.01 1 0.1 adsr
# 0.1 port 0.09 0.0005 scale dmetro 0.01 tgate 0.8 port
# 0.3 port 440 3000 scale

6 0.4 sine
_trig get 0.01 tgate 1.1 0.01 1 0.1 adsr
0.1 port 0.09 0.0005 scale dmetro 0.01 tgate 0.8 port
0.3 port dup 0.15 gt 0.3 port * *
_gate get 0.5 1 thresh 0 _seq tseq 69 + 0.2 port  + mtof

0.8 saw 1000 butlp * _sig get 0.5 * *
dup
_dry set
#_dry get dup
_dry get
0.8 1 delay -6 ampdb * +
dup 0.97 10000 revsc
0.1 * _dry get + swap
0.1 * _dry get +
_grif fc
)
