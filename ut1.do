read design ./testcases/ut1/design.v 
read lib ./testcases/ut1/lib.v
debug -design
print ntk -n
debug -lib
print ntk -n
