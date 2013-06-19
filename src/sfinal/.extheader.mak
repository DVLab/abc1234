sfinal.d: ../../include/sfCmd.h ../../include/sfMgr.h 
../../include/sfCmd.h: sfCmd.h
	@rm -f ../../include/sfCmd.h
	@ln -fs ../src/sfinal/sfCmd.h ../../include/sfCmd.h
../../include/sfMgr.h: sfMgr.h
	@rm -f ../../include/sfMgr.h
	@ln -fs ../src/sfinal/sfMgr.h ../../include/sfMgr.h
