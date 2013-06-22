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
#include "sfMgr.h"
#include "v3NtkInput.h"
#include "v3NtkUtil.h"
#include "v3Type.h"
#include "satMgr.h"
#include "v3Msg.h"
using namespace std;

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
	void setDesign(uint32_t id,V3BvNtk* ntk){
		_designHandler=id;
		_designNtk=ntk;
	}
	void addLibrary(uint32_t id,V3BvNtk* ntk){
		_libraryHandler.push_back(id);
		_libraryNtk.push_back(ntk);
	}
private:
	uint32_t	_designHandler;
	vector<uint32_t>	_libraryHandler;
    V3BvNtk* _designNtk;
	vector<V3BvNtk*> _libraryNtk;
//		uint32_t	_outHandler;
};




#endif
