sat.d: ../../include/satMgr.h ../../include/satCmd.h 
../../include/satMgr.h: satMgr.h
	@rm -f ../../include/satMgr.h
	@ln -fs ../src/sat/satMgr.h ../../include/satMgr.h
../../include/satCmd.h: satCmd.h
	@rm -f ../../include/satCmd.h
	@ln -fs ../src/sat/satCmd.h ../../include/satCmd.h
