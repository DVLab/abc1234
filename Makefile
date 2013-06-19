# source codes to be compiled
SRCPKGS	= minisat cmd ntk alg ext io trans svr adt vrf util sat sfinal

# library to be linked (should include SRCPKGS)
LIBPKGS	= vrf trans alg ext bdd ntk io sat svr cmd adt bdd ntk prove util minisat sfinal 

# engine header files and libraries to be soft linked
# ENGPKGS	+= minisat_2.2
ENGPKGS	+= boolector_1.5

# front-end Verilog parser
ENGPKGS	+= quteRTL

# specific directories
ENGSSRC	= eng
MAIN	= main

# libraries
EXTLIBS	= -lz -ldl -lm -lreadline -ltermcap -lgmp -lstdc++
LIBS	= $(addprefix -l, $(LIBPKGS)) $(addprefix -l, $(ENGPKGS))
SRCLIBS	= $(addsuffix .a, $(addprefix lib, $(LIBPKGS))) $(addsuffix .a, $(addprefix lib, $(ENGPKGS)))
LIBFILES = $(addsuffix .a, $(addprefix lib, $(SRCPKGS)))

# compile outputs
EXEC	= socv
LIB	= libv3.a

all:	srcLib
	@echo "Checking $(MAIN)..."
	@cd src/$(MAIN); make --no-print-directory INCLIB="$(LIBS) $(EXTLIBS)" EXEC=$(EXEC); cd ../.. ;
	@ln -fs bin/$(EXEC) .

srcLib:	engLib
	@for pkg in $(SRCPKGS); \
	do \
		echo "Checking $$pkg..."; \
		cd src/$$pkg; make --no-print-directory PKGNAME=$$pkg; \
		cd ../.. ; \
	done

engLib:	
	@for pkg in $(ENGPKGS);	\
	do \
		cd include ; ln -fs ../src/$(ENGSSRC)/$$pkg/*.h .; cd .. ; \
		cd lib ; ln -fs ../src/$(ENGSSRC)/$$pkg/*.a .; cd .. ; \
	done

clean:	
	@for pkg in $(SRCPKGS); \
	do \
		echo "Cleaning $$pkg..."; \
		cd src/$$pkg; make --no-print-directory PKGNAME=$$pkg clean; \
		cd ../.. ; \
	done
	@echo "Cleaning $(MAIN)..."
	@cd src/$(MAIN); make --no-print-directory clean
	@echo "Removing $(LIBFILES)..."
	@cd lib; rm -f $(LIBFILES)
	@echo "Removing $(EXEC)..."
	@rm -f bin/$(EXEC) 
	@echo "Removing obj/"
	@cd obj/; rm -rf *.o

ctags:	
	@rm -f src/tags
	@for pkg in $(SRCPKGS); \
	do \
		echo "Tagging $$pkg..."; \
		cd src; ctags -a $$pkg/*.cpp $$pkg/*.h; cd ..; \
	done
	@echo "Tagging $(MAIN)..."
	cd src; ctags -a $(MAIN)/*.cpp

rmdep:	
	@for pkg in $(SRCPKGS); \
	do \
		rm src/$$pkg/.*.mak; \
	done
	@rm src/main/.*.mak;
