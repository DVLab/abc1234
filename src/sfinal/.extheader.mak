sfinal.d: ../../include/sfCmd.h 
../../include/sfCmd.h: sfCmd.h
	@rm -f ../../include/sfCmd.h
	@ln -fs ../src/sfinal/sfCmd.h ../../include/sfCmd.h
