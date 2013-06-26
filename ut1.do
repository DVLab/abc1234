read design ./testcases/ut1/design.v 
read lib ./testcases/ut1/lib.v
debug -design
debug -lib
go
write rtl merge.v
