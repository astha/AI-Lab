one(1).
zero(0).
myconnect(X,Y) :- one(X),one(Y).
myconnect(X,Y) :- zero(X),zero(Y).

mynand(X1,X2,Y) :- one(X1),one(X2),zero(Y).
mynand(X1,X2,Y) :- one(X1),zero(X2),one(Y).
mynand(X1,X2,Y) :- zero(X1),one(X2),one(Y).
mynand(X1,X2,Y) :- zero(X1),zero(X2),one(Y).

mynot(X,Y) :- mynand(X,X,Y).
myand(X1,X2,Y) :- mynand(X1,X2,Y1),myconnect(Y1,Y2),mynot(Y2,Y).
myor(X1,X2,Y) :- mynot(X1,X11),mynot(X2,X21),myconnect(X11,X12),myconnect(X21,X22),mynand(X12,X22,Y).

mynor(X1,X2,Y) :- myor(X1,X2,X11),myconnect(X11,X12),mynot(X12,Y).


myxor(I1,I2,P) :- mynot(I1,T1), mynot(I2,T2), myconnect(T1,T11), myconnect(T2,T21), myand(I1,T21,T3), myand(T11,I2,T4),
        myconnect(T3,T31), myconnect(T4,T41), myor(T31,T41,P).


element([H|_],1,H).
element([_|T],N,X) :- N1 is N-1, element(T,N1,X),!.

half-adder(I1,I2,O2,O1) :- myxor(I1,I2,O1), myand(I1,I2,O2).
full-adder(I1,I2,I3,O2,O1) :- myxor(I1,I2,T1), myconnect(T1,T11), myxor(T11,I3, O1), 
              myconnect(T1,T12), myconnect(I3,T3), myand(T12,T3,T4), 
              myconnect(I1,T13), myconnect(I2,T21), myand(T13,T21,T5), myconnect(T4,T41), myconnect(T5,T51),
              myor(T51,T41,O2).


majority(I1,I2,I3,O) :- myand(I1,I2,T1), myand(I2,I3,T2) , myand(I1,I3,T3),
            myconnect(T1,T11), myconnect(T2,T21), myconnect(T3,T31),
            myor(T11,T21,T4),
            myconnect(T4,T41),
            myor(T41,T31,O).

parity(I1,I2,I3,O) :- myxor(I1,I2,T1), myconnect(T1,T11), myxor(T11,I3,O).

palindrome(I1,I3,I2,O) :- myxor(I1,I2,T1), myconnect(T1,T11), mynot(T11,O).

verify(xor,I,O) :- element(I,1,I1), element(I,2,I2), element(O,1,O1), myxor(I1,I2,O11), !, myconnect(O11,O1).
verify(half-adder,I,O) :- element(I,1,I1), element(I,2,I2), element(O,1,O1), element(O,2,O2), half-adder(I1,I2,O11,O21), !,
             myconnect(O1,O11), myconnect(O2,O21).
verify(full-adder,I,O) :- element(I,1,I1), element(I,2,I2), element(I,3,I3), element(O,1,O1), element(O,2,O2),
            full-adder(I1,I2,I3,O11,O21), !, myconnect(O1,O11), myconnect(O2,O21).
            
verify(majority,I,O) :- element(I,1,I1), element(I,2,I2), element(I,3,I3), element(O,1,O1), !,
            majority(I1,I2,I3,O11),
            myconnect(O11,O1).

verify(parity,I,O) :- element(I,1,I1), element(I,2,I2), element(I,3,I3), element(O,1,O1), !,
          parity(I1,I2,I3,O11),
          myconnect(O11,O1).
verify(palindrome,I,O) :- element(I,1,I1), element(I,2,I2),element(I,3,I3),element(O,1,O1), !,
          palindrome(I1,I2,I3,O11),
          myconnect(O11,O1).
