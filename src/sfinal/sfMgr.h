/****************************************************************************
  FileName     [ sfMgr.h ]
  PackageName  [ sf ]
  Synopsis     [ Define socv final package interface ]
  Author       [ Mark Chang ]
  Copyright    [ Copyleft(c) 2013 LaDs(III), GIEE, NTU, Taiwan ]
 ****************************************************************************/

#ifndef SF_MGR_H_
#define SF_MGR_H_

#include<vector>
#include "v3Ntk.h"
using namespace std;
class SfMgr{
public:
	void run(){};
	void traverseFanin();
    unsigned splitModule(const string&,vector<string>&); 
	uint32_t getDesignHandler(){
		return _designHandler;
	}
	uint32_t getLibraryHandler(uint32_t i=0){
		return _libraryHandler[i];
	}
	void setDesign(uint32_t id,V3Ntk* ntk){
		_designHandler=id;
		_designNtk=ntk;
	}
	void addLibrary(uint32_t id,V3Ntk* ntk){
		_libraryHandler.push_back(id);
		_libraryNtk.push_back(ntk);
	}
private:
	uint32_t	_designHandler;
	vector<uint32_t>	_libraryHandler;
    V3Ntk* _designNtk;
	vector<V3Ntk*> _libraryNtk;
//		uint32_t	_outHandler;
};




#endif
