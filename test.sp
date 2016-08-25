(
_grif "./griffin.so" fl
_trig _gate _grif fe
_sig var
_trig get
0.001 0 1 0.1 incr _sig set
_sig get 0.001 4 scale dmetro
0.005 0.005 0.005 tenvx
_sig get 3000 400 scale 1.0 sine *
_gate get 0.5 0 thresh tog 0.001 port *
dup
_grif fc
)
