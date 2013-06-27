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
#include "v3SvrBoolector.h"
using namespace std;

using namespace std;
class SfMgr{
public:
	void run(){};
	void createMergeNtk();
	void traverseFanin();
	void solveSat();
	unsigned splitModule(const string&,vector<string>&);
	void addNtk(uint32_t from_handler,uint32_t to_handler,V3BvNtk* ntk,map<uint32_t,V3NetId> IdMap);
	uint32_t getMergeHandler(){
		return _mergeHandler;
	}
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
	void setMerge(uint32_t id,V3BvNtk* ntk){
		_mergeHandler=id;
		_mergeNtk=ntk;
	}
	void addLibrary(uint32_t id,V3BvNtk* ntk){
		_libraryHandler.push_back(id);
		_libraryNtk.push_back(ntk);
	}
private:
	uint32_t	_designHandler;
	vector<uint32_t>	_libraryHandler;
	uint32_t	_mergeHandler;
	V3BvNtk* _designNtk;
	vector<V3BvNtk*> _libraryNtk;
	V3BvNtk* _mergeNtk;
	V3BvNtk* _consNtk;
	V3NetVec orderedNets;
	V3SvrBoolector* _satSolver;
//		uint32_t	_outHandler;


	V3NetVec _satNets;

};




#endif
