/****************************************************************************
  FileName     [ sfMgr.cpp ]
  PackageName  [ sf ]
  Synopsis     [ Define socv final package interface ]
  Author       [ Mark Chang ]
  Copyright    [ Copyleft(c) 2013 LaDs(III), GIEE, NTU, Taiwan ]
 ****************************************************************************/
#include "sfMgr.h"
#include "v3NtkInput.h"
#include "v3NtkUtil.h"
#include "satMgr.h"
#include "v3Msg.h"
#include "v3StrUtil.h"
#include<algorithm>
using namespace std;

extern const bool v3Str2Int(const string&, int&);
extern const string v3Int2Str(const int&);

extern SATMgr* satMgr;
extern const uint32_t dfsNtkForGeneralOrder(V3Ntk* const, V3NetVec&, const bool& );
extern const uint32_t dfsNtkForReductionOrder(V3Ntk* const, V3NetVec&, const V3NetVec&, const bool& );
extern const V3NetId createNet(V3Ntk* const, uint32_t );
extern const bool createInput(V3Ntk* const, const V3NetId&);
extern const bool createOutput(V3Ntk* const, const V3NetId&);
extern const bool createInout(V3Ntk* const, const V3NetId&);
// General Gate Type Construction Functions for V3 Ntk
extern const bool createV3BufGate(V3Ntk* const, const V3NetId&, const V3NetId&);
extern const bool createV3FFGate(V3Ntk* const, const V3NetId&, const V3NetId&, const V3NetId&);
extern const bool createV3AndGate(V3Ntk* const, const V3NetId&, const V3NetId&, const V3NetId&);
// General Gate Type Construction Functions for AIG Ntk
extern const bool createAigAndGate(V3AigNtk* const, const V3NetId&, const V3NetId&, const V3NetId&);
extern const bool createAigFalseGate(V3AigNtk* const, const V3NetId&);
// General Gate Type Construction Functions for BV Ntk
extern const bool createBvReducedGate(V3BvNtk* const, const V3GateType&, const V3NetId&, const V3NetId&);
extern const bool createBvPairGate(V3BvNtk* const, const V3GateType&, const V3NetId&, const V3NetId&, const V3NetId&);
extern const bool createBvMuxGate(V3BvNtk* const, const V3NetId&, const V3NetId&, const V3NetId&, const V3NetId&);
extern const bool createBvSliceGate(V3BvNtk* const, const V3NetId&, const V3NetId&, uint32_t, uint32_t);
extern const bool createBvConstGate(V3BvNtk* const, const V3NetId&, const string&);
// General Hierarchical Ntk Construction Functions for V3 Ntk


map<uint32_t,V3NetId> netidMap;
map<uint32_t,V3NetId> o2rMap;

V3NetId getNewNetId(V3NetId netid){
	map<uint32_t,V3NetId>::iterator it=netidMap.find(netid.id);
	assert(it!=netidMap.end());
	return it->second;
}
V3NetId getRevisedNetId(V3NetId netid){
	map<uint32_t,V3NetId>::iterator it=o2rMap.find(netid.id);
	assert(it!=o2rMap.end());
	return it->second;
}
V3NetId getMapNetId(V3NetId netid , std::map<uint32_t,V3NetId> IdMap ){
   map<uint32_t,V3NetId>::iterator it=IdMap.find(netid.id);
   assert(it!=IdMap.end());
   return it->second;
}

void SfMgr::solveSat(){

  _satSolver = new V3SvrBoolector(_mergeNtk);

  uint32_t z=0;

  for(uint32_t i=0; i<_satNets.size();i++){
	  const V3NetId outid=_satNets[i];
		static_cast<V3SvrBase*>(_satSolver)->addBoundedVerifyData(outid,z );
		_satSolver->assumeProperty(outid, false, z);
  }

    _satSolver->simplify();
	cout<<(_satSolver->assump_solve() ? "SAT" : "UNSAT")<<endl;

  V3BitVecX dataValue;
    for (int j = _mergeNtk->getOutputSize()-1; j >= 0; --j) {
      if (_satSolver->existVerifyData(_mergeNtk->getOutput(j), z)) {
        dataValue = _satSolver->getDataValue(_mergeNtk->getOutput(j), z);
        assert (dataValue.size() == _mergeNtk->getNetWidth(_mergeNtk->getOutput(j)));
        Msg(MSG_IFO) << dataValue[0];
      } else Msg(MSG_IFO) << 'x';
    }
}
void
SfMgr::createMergeNtk(){
/*
	netidMap.clear();
  v3Handler.setCurHandlerFromId(_designHandler);
 // V3NetId id = V3NetId::makeNetId(); //_designNtk->getOutput(0);
  V3NetVec orderedNets;
//  V3NetVec targetNets;
  orderedNets.clear();
  orderedNets.reserve(_designNtk->getNetSize());
//  targetNets.push_back(id);
  dfsNtkForGeneralOrder(_designNtk,orderedNets);
  assert (orderedNets.size() <= _designNtk->getNetSize());

   V3NtkInput* curHandler= (V3NtkInput*) v3Handler.getCurHandler();

	V3BvNtk* _designNtk=(V3BvNtk*)(curHandler->getNtk());
  for(unsigned i=0;i<orderedNets.size();i++){
	   V3NetId  netid=	orderedNets[i];
	   const V3GateType& type = _designNtk->getGateType(netid);
	   string name =curHandler-> getNetNameOrFormedWithId(netid);
	//	cout<<"i:"<<netid.id<<" name:"<< name <<" type:"<<V3GateTypeStr[type]<<endl;
			
	   if(type!=BV_CONST){
		V3NetId new_nid=curHandler->createNet(name+"_new_"+v3Int2Str(netid.id) ,static_cast<V3BvNtk*>(_designNtk)->getNetWidth(netid));
	   
	   new_nid.cp = netid.cp;
	   netidMap[netid.id]=new_nid;

	//	cout<<"getGateType:"<<V3GateTypeStr[_designNtk->getGateType(new_nid)]<<endl;
		   if (V3_MODULE == type) {
			  Msg(MSG_WAR) << "Currently Expression Over Module Instances has NOT Been Implemented !!" << endl;
		   }
			 if (isV3PairType(type)) {
				const string name1 = curHandler->getNetExpression(_designNtk->getInputNetId(netid, 0)); assert (name1.size());
				const string name2 = curHandler->getNetExpression(_designNtk->getInputNetId(netid, 1)); assert (name2.size());
			   V3NetId new_nid0=getNewNetId(_designNtk->getInputNetId(netid, 0));
			   V3NetId new_nid1=getNewNetId(_designNtk->getInputNetId(netid, 1));
				assert(createBvPairGate(_designNtk,type, new_nid, new_nid0,new_nid1));
			 }
			 else if (isV3ReducedType(type)) {//OK
				const string name1 = curHandler->getNetExpression(_designNtk->getInputNetId(netid, 0)); assert (name1.size());
				
			   V3NetId new_nid0=getNewNetId(_designNtk->getInputNetId(netid, 0));
				assert(createBvReducedGate(_designNtk,type, new_nid,new_nid0));
			 }
			 else if (BV_MUX == type) {//OK
			   V3NetId new_nid0=getNewNetId(_designNtk->getInputNetId(netid, 0));
			   V3NetId new_nid1=getNewNetId(_designNtk->getInputNetId(netid, 1));
			   V3NetId new_nid2=getNewNetId(_designNtk->getInputNetId(netid, 2));
				assert(createBvMuxGate(_designNtk,new_nid, new_nid0,new_nid1,new_nid2));
			 }
			 else if (BV_SLICE == type) {//OK
			   V3NetId new_nid0=getNewNetId(_designNtk->getInputNetId(netid, 0));
				const uint32_t msb = static_cast<V3BvNtk*>(_designNtk)->getInputSliceBit(netid, true);
				const uint32_t lsb = static_cast<V3BvNtk*>(_designNtk)->getInputSliceBit(netid, false);

			 	assert(createBvSliceGate(_designNtk,new_nid, new_nid0, msb,lsb));
			 }
			 else if(BV_CONST == type){//OK
			 	assert(0);
			 	//const V3BitVecX* const value = static_cast<V3BvNtk*>(_designNtk)->getInputConstValue(netid);
				//assert(createBvConstGate(_designNtk,new_nid, v3Int2Str(value->size())+"'b"+value->regEx())); 
			 }
			 else if (AIG_NODE == type) {
			 	assert(0);
			 }
			 else if(AIG_FALSE == type){
			 	assert(0);
			 }
			 else if(V3_PI ==type){//OK
			 	assert(createBvSliceGate(_designNtk,new_nid, netid, static_cast<V3BvNtk*>(_designNtk)->getNetWidth(new_nid)-1 ,0));
			    //assert(createInput(_designNtk,new_nid));
			 }
			 else if(V3_PIO==type){//OK
			 	assert(createBvSliceGate(_designNtk,new_nid, netid, static_cast<V3BvNtk*>(_designNtk)->getNetWidth(new_nid)-1 ,0));
				//assert(createInout(_designNtk,new_nid));
			 }
			 else{
				assert(0);
			 }
		}
  }

   for(uint32_t i=0,k=_designNtk->getOutputSize();i<k;++i){
      V3NetId outid=_designNtk->getOutput(i);
      V3NetId new_nid=getNewNetId(outid);
	   string name =curHandler-> getNetNameOrFormedWithId(outid);
	  V3NetId new_outid=curHandler->createNet(name+"_new_out0_"+v3Int2Str(outid.id) ,static_cast<V3BvNtk*>(_designNtk)->getNetWidth(outid));
	  assert(createBvPairGate(_designNtk,BV_XOR, new_outid, new_nid,outid));
      assert(createOutput(_designNtk,new_outid));
	  for(uint32_t j=0; j< static_cast<V3BvNtk*>(_designNtk)->getNetWidth(new_outid); j++){
		  V3NetId new_outid2=curHandler->createNet(name+"_new_out1_"+v3Int2Str(new_outid.id)+"_"+v3Int2Str(j),1);
			assert(createBvSliceGate(_designNtk,new_outid2, new_outid, j ,j));
			assert(createOutput(_designNtk,new_outid2));
			_satNets.push_back(new_outid2);	
	  }
  }
  _mergeNtk=_designNtk;
  orderedNets.clear();
  orderedNets.reserve(_designNtk->getNetSize());
//  targetNets.push_back(id);
  dfsNtkForGeneralOrder(_designNtk,orderedNets);
  assert (orderedNets.size() <= _designNtk->getNetSize());
  vector<string> strVec;
  strVec.clear();
	cout<<"*****ORIGINAL CIRCUIT*****"<<endl;
	for(unsigned i=0;i<orderedNets.size();i++){
		V3NetId netid=	orderedNets[i];
		const V3GateType& type = _designNtk->getGateType(netid);
	   string name =curHandler->getNetNameOrFormedWithId(netid);
		//cout<<"i:"<<netid.id <<" name:"<< name << " type:"<<(netid.cp ? "~" : "")<< V3GateTypeStr[type]<<endl;
		//cout<<"i:"<<netid.id <<" type:"<<(netid.cp ? "~" : "")<< V3GateTypeStr[type]<<endl;
        string str=V3GateTypeStr[type]+(netid.cp?"~":"")+" name:"+name;
        strVec.push_back(str);
	}
  sort(strVec.begin(),strVec.end());
  for(unsigned i=0;i<strVec.size();++i)
      cout<<i<<": "<<strVec[i]<<endl;*/
/*  strVec.clear();
  orderedNets.clear();
  orderedNets.reserve(_designNtk->getNetSize());
//  targetNets.push_back(id);
  dfsNtkForGeneralOrder(_designNtk,orderedNets);
  assert (orderedNets.size() <= _designNtk->getNetSize());
  cout<<"*****NEW CIRCUIT*****"<<endl;
  for(unsigned i=0;i<orderedNets.size();i++){
		V3NetId netid=	orderedNets[i];
		const V3GateType& type = _designNtk->getGateType(netid);
	    string name ="";
		static_cast<V3NtkHandler*>(inputHandler)->getNetName(netid);
		//cout<<"i:"<<netid.id <<" name:"<< name << " type:"<<(netid.cp ? "~" : "")<< V3GateTypeStr[type]<<endl;
		//cout<<"i:"<<netid.id <<" type:"<<(netid.cp ? "~" : "")<< V3GateTypeStr[type]<<endl;
        string str=V3GateTypeStr[type]+(netid.cp?"~":"")+" name:"+name;
        strVec.push_back(str);
	}
  sort(strVec.begin(),strVec.end());
  for(unsigned i=0;i<strVec.size();++i)
      cout<<i<<": "<<strVec[i]<<endl;
  strVec.clear();

        v3Handler.pushAndSetCurHandler(inputHandler);
        V3BvNtk* ntk= new V3BvNtk();
        *ntk = *((V3BvNtk*)inputHandler->getNtk());
        setMerge(v3Handler.getCurHandlerId(),ntk);
*/
//	_mergeNtk=_designNtk;
}

void
SfMgr::traverseFanin(){
/*
  v3Handler.setCurHandlerFromId(_designHandler);
  V3NetVec orderedNets;
  orderedNets.clear();
  orderedNets.reserve(_designNtk->getNetSize());
  dfsNtkForGeneralOrder(_designNtk,orderedNets);
  assert (orderedNets.size() <= _designNtk->getNetSize());

  V3NtkInput* inputHandler = new V3NtkInput(false,"new_ntk");
  V3BvNtk* new_ntk=(V3BvNtk*)( inputHandler->getNtk());
  V3NtkInput* curHandler=(V3NtkInput*) v3Handler.getCurHandler();

  for(unsigned i=0;i<orderedNets.size();i++){
	   V3NetId  netid=	orderedNets[i];
	   const V3GateType& type = _designNtk->getGateType(netid);
	   string name =curHandler-> getNetNameOrFormedWithId(netid);
//		cout<<"i:"<<netid.id<<" name:"<< name <<" type:"<<V3GateTypeStr[type]<<endl;
		
	   V3NetId new_nid=inputHandler->createNet(name ,static_cast<V3BvNtk*>(_designNtk)->getNetWidth(netid));
	   new_nid.cp = netid.cp;
       V3NetId new_nidd;
	   netidMap[netid.id]=new_nid;
       if(BV_CONST==type) 
           continue;
       if(V3_PI!=type){
    	   new_nidd=inputHandler->createNet(name+"Dupli" ,static_cast<V3BvNtk*>(_designNtk)->getNetWidth(netid));
           new_nidd.cp = netid.cp;
	       o2rMap[new_nid.id]=new_nidd;
       }
       else
           o2rMap[new_nid.id]=new_nid;

	//	cout<<"getGateType:"<<V3GateTypeStr[new_ntk->getGateType(new_nid)]<<endl;
		   if (V3_MODULE == type) {
			  Msg(MSG_WAR) << "Currently Expression Over Module Instances has NOT Been Implemented !!" << endl;
		   }
			 if (isV3PairType(type)) {
				//const string name1 = curHandler->getNetExpression(_designNtk->getInputNetId(netid, 0)); assert (name1.size());
				//const string name2 = curHandler->getNetExpression(_designNtk->getInputNetId(netid, 1)); assert (name2.size());

				V3NetId old_nid0=_designNtk->getInputNetId(netid, 0);
				V3NetId old_nid1=_designNtk->getInputNetId(netid, 1);


			   V3NetId new_nid0=getNewNetId(old_nid0);
			   V3NetId new_nid1=getNewNetId(old_nid1);
				
		
		//		cout<<"old_id:"<<netid.id<<" cp:"<< netid.cp <<" type:"<<V3GateTypeStr[type]<<endl;
		//		cout<<"id0:"<<old_nid0.id<<" cp:"<< old_nid0.cp <<endl;//" type:"<<V3GateTypeStr[type]<<endl;
		//		cout<<"id1:"<<old_nid1.id<<" cp:"<< old_nid1.cp <<endl;//" type:"<<V3GateTypeStr[type]<<endl;

		//		cout<<"new_id:"<<new_nid.id<<" cp:"<< new_nid.cp <<" type:"<<V3GateTypeStr[type]<<endl;
		//		cout<<"id0:"<<new_nid0.id<<" cp:"<< new_nid0.cp  <<endl;//" type:"<<V3GateTypeStr[type]<<endl;
		//		cout<<"id1:"<<new_nid1.id<<" cp:"<< new_nid1.cp <<endl;//" type:"<<V3GateTypeStr[type]<<endl;
	
			   V3NetId new_nid2=getRevisedNetId(new_nid0);
			   V3NetId new_nid3=getRevisedNetId(new_nid1);

				new_nid0.cp=old_nid0.cp;
				new_nid1.cp=old_nid1.cp;
				new_nid2.cp=old_nid0.cp;
				new_nid3.cp=old_nid1.cp;

				assert(createBvPairGate(new_ntk,type, new_nid, new_nid0,new_nid1));
				assert(createBvPairGate(new_ntk,type, new_nidd, new_nid2,new_nid3));
			 }
			 else if (isV3ReducedType(type)) {//OK
				//const string name1 = curHandler->getNetExpression(_designNtk->getInputNetId(netid, 0)); assert (name1.size());
				V3NetId old_nid0=_designNtk->getInputNetId(netid, 0);

			   V3NetId new_nid0=getNewNetId(old_nid0);
			   V3NetId new_nid1=getRevisedNetId(new_nid0);

				new_nid0.cp=old_nid0.cp;
				new_nid1.cp=old_nid0.cp;

				assert(createBvReducedGate(new_ntk,type, new_nid,new_nid0));
				assert(createBvReducedGate(new_ntk,type, new_nidd,new_nid1));
			 }
			 else if (BV_MUX == type) {//OK
				V3NetId old_nid0=_designNtk->getInputNetId(netid, 0);
				V3NetId old_nid1=_designNtk->getInputNetId(netid, 1);
				V3NetId old_nid2=_designNtk->getInputNetId(netid, 2);
			   
			   V3NetId new_nid0=getNewNetId(old_nid0);
			   V3NetId new_nid1=getNewNetId(old_nid1);
			   V3NetId new_nid2=getNewNetId(old_nid2);

			   V3NetId new_nid3=getRevisedNetId(new_nid0);
			   V3NetId new_nid4=getRevisedNetId(new_nid1);
			   V3NetId new_nid5=getRevisedNetId(new_nid2);

				new_nid0.cp=old_nid0.cp;
				new_nid1.cp=old_nid1.cp;
				new_nid2.cp=old_nid2.cp;

				new_nid3.cp=old_nid0.cp;
				new_nid4.cp=old_nid1.cp;
				new_nid5.cp=old_nid2.cp;

				assert(createBvMuxGate(new_ntk,new_nid, new_nid0,new_nid1,new_nid2));
				assert(createBvMuxGate(new_ntk,new_nidd, new_nid3,new_nid4,new_nid5));
			 }
			 else if (BV_SLICE == type) {//OK
			  // V3NetId new_nid0=getNewNetId(_designNtk->getInputNetId(netid, 0));
				V3NetId old_nid0=_designNtk->getInputNetId(netid, 0);
			   V3NetId new_nid0=getNewNetId(old_nid0);
			   V3NetId new_nid1=getRevisedNetId(new_nid0);
				new_nid0.cp=old_nid0.cp;
				new_nid1.cp=old_nid0.cp;

				const uint32_t msb = static_cast<V3BvNtk*>(_designNtk)->getInputSliceBit(netid, true);
				const uint32_t lsb = static_cast<V3BvNtk*>(_designNtk)->getInputSliceBit(netid, false);

			 	assert(createBvSliceGate(new_ntk,new_nid, new_nid0, msb,lsb));
			 	assert(createBvSliceGate(new_ntk,new_nidd, new_nid1, msb,lsb));
			 }
			 else if(BV_CONST == type){//OK

			 }
			 else if (AIG_NODE == type) {
			 	assert(0);
			 }
			 else if(AIG_FALSE == type){
			 	assert(0);
			 }
			 else if(V3_PI ==type){//OK
			    assert(createInput(new_ntk,new_nid));
			 }
			 else if(V3_PIO==type){//OK
				assert(createInout(new_ntk,new_nid));
				assert(createInout(new_ntk,new_nidd));
			 }
			 else{
				assert(0);
			 }
  }
  for(uint32_t i=0,j=_designNtk->getOutputSize();i<j;++i){
      V3NetId outid=_designNtk->getOutput(i);
      V3NetId new_outid=getNewNetId(outid);
      V3NetId new_outidd=getRevisedNetId(new_outid);
	
		new_outid.cp=outid.cp;
		new_outidd.cp=outid.cp;

      string name="miter"+i;
	  V3NetId new_nid=inputHandler->createNet(name,static_cast<V3BvNtk*>(_designNtk)->getNetWidth(outid));
      createBvPairGate(new_ntk,BV_XOR,new_nid,new_outid,new_outidd);
      assert(createOutput(new_ntk,new_outid));
      assert(createOutput(new_ntk,new_outidd));
	  for(uint32_t k=0; k< static_cast<V3BvNtk*>(_designNtk)->getNetWidth(outid); k++){
		  V3NetId new_outid2=inputHandler->createNet(name+"_new_out1_"+v3Int2Str(new_nid.id)+"_"+v3Int2Str(k),1);
			assert(createBvSliceGate(new_ntk,new_outid2, new_nid, k ,k));
			assert(createOutput(new_ntk,new_outid2));
			_satNets.push_back(new_outid2);	
	  }
  }


	V3StrVec outputName(new_ntk->getOutputSize(), "");
	for (uint32_t i = 0; i < new_ntk->getOutputSize(); ++i){ 
		  if (outputName[i].size()){
		  	inputHandler->recordOutName(i, outputName[i]);
		  }
		  else{
		    inputHandler->recordOutName(i, "output_" + v3Int2Str(i));
		  }
  	}
  v3Handler.pushAndSetCurHandler(inputHandler); 
  setMerge(v3Handler.getCurHandlerId(),new_ntk);*/
}

void SfMgr::testAddNtk(){
/*  V3NtkInput* inputHandler = new V3NtkInput(false,"test_ntk");
  V3BvNtk* test_ntk=(V3BvNtk*)( inputHandler->getNtk());
  V3NetId new_nid0 = inputHandler->createNet("test0",1);
  V3NetId new_nid1 = inputHandler->createNet("test1",1);
  V3NetId new_nid = inputHandler->createNet("test2",1);
  createInput(test_ntk, new_nid0);
  createInput(test_ntk, new_nid1);
  createBvPairGate(test_ntk, BV_AND, new_nid, new_nid0, new_nid1);
  v3Handler.pushAndSetCurHandler(inputHandler);
  uint32_t _testHandler = v3Handler.getCurHandlerId();
  setMerge(_testHandler,test_ntk);
  map<uint32_t,V3NetId> design_1_Map;
  addNtk2(_designHandler,_testHandler,design_1_Map); */

}

void replaceLib2(uint32_t lib_handler,uint32_t origin_handler,map<uint32_t,V3NetId> rid,map<uint32_t,V3NetId> replaceMap,map<uint32_t,V3NetId> IdMap){
// lib_handler.....Macro's Handler
// origin_handler..Origin's Handler
// rid.............will be replaced netid
// replaceMap......will be replaced netid's Map to which Macro' net
// IdMap...........Map for Origin netid & New netid
   V3NtkInput* libHandler = (V3NtkInput*) v3Handler.getHandler(lib_handler);
   V3NtkInput* originHandler = (V3NtkInput*) v3Handler.getHandler(origin_handler);
   V3NtkInput* replaceHandler = new V3NtkInput(false,"replaced_ntk");

   V3BvNtk* originNtk  = (V3BvNtk*)(originHandler->getNtk());
   V3BvNtk* libNtk     = (V3BvNtk*)(libHandler->getNtk());
   V3BvNtk* replaceNtk = (V3BvNtk*)(replaceHandler->getNtk());

   V3NetVec orderedNets;
   orderedNets.clear();
   orderedNets.reserve(originNtk->getNetSize());
   dfsNtkForGeneralOrder(originNtk,orderedNets);
   assert(orderedNets.size() <= originNtk->getNetSize());

   for(unsigned i = 0 ; i < orderedNets.size() ; ++i){
      V3NetId originId = orderedNets[i];
      bool to_construct = true;
      map<uint32_t,V3NetId>::iterator it = rid.find(originId.id);
      if(it != rid.end()){
         V3NetId r = it->second;
         const V3GateType& r_type = libNtk->getGateType(r);
         if(r_type != V3_PI)
            to_construct = false;
      }

      const V3GateType& type = originNtk->getGateType(originId);
      if(to_construct){
         string name = originHandler->getNetNameOrFormedWithId(originId);
         V3NetId new_nid = replaceHandler->createNet(name ,static_cast<V3BvNtk*>(originNtk)->getNetWidth(originId));
         new_nid.cp = originId.cp;
         IdMap[originId.id] = new_nid;

         if(BV_CONST == type)
            continue;
         if (V3_MODULE == type) {
            Msg(MSG_WAR) << "Currently Expression Over Module Instances has NOT Been Implemented !!" << endl;
         }
         if(isV3PairType(type)){
            V3NetId old_nid0 = originNtk->getInputNetId(originId, 0);
            V3NetId old_nid1 = originNtk->getInputNetId(originId, 1);
            V3NetId new_nid0 = getMapNetId(old_nid0,IdMap);
            V3NetId new_nid1 = getMapNetId(old_nid1,IdMap);
            new_nid0.cp = old_nid0.cp;
            new_nid1.cp = old_nid1.cp;
            assert(createBvPairGate(replaceNtk, type, new_nid, new_nid0, new_nid1));
         }
         else if (isV3ReducedType(type)) {
            V3NetId old_nid0 = originNtk->getInputNetId(originId, 0);
            V3NetId new_nid0 = getMapNetId(old_nid0,IdMap);
            new_nid0.cp = old_nid0.cp;
            assert(createBvReducedGate(replaceNtk, type, new_nid, new_nid0));
         }
         else if (BV_MUX == type) {
            V3NetId old_nid0 = originNtk->getInputNetId(originId, 0);
            V3NetId old_nid1 = originNtk->getInputNetId(originId, 1);
            V3NetId old_nid2 = originNtk->getInputNetId(originId, 2);
            V3NetId new_nid0 = getMapNetId(old_nid0,IdMap);
            V3NetId new_nid1 = getMapNetId(old_nid1,IdMap);
            V3NetId new_nid2 = getMapNetId(old_nid2,IdMap);
            new_nid0.cp = old_nid0.cp;
            new_nid1.cp = old_nid1.cp;
            new_nid2.cp = old_nid2.cp;
            assert(createBvMuxGate(replaceNtk, new_nid, new_nid0, new_nid1, new_nid2));
         }
         else if (BV_SLICE == type) {
            V3NetId old_nid0 = originNtk->getInputNetId(originId, 0);
            V3NetId new_nid0 = getMapNetId(old_nid0,IdMap);
            new_nid0.cp = old_nid0.cp;
            const uint32_t msb = static_cast<V3BvNtk*>(originNtk)->getInputSliceBit(originId, true);
            const uint32_t lsb = static_cast<V3BvNtk*>(originNtk)->getInputSliceBit(originId, false);
            assert(createBvSliceGate(replaceNtk, new_nid, new_nid0, msb,lsb));
         }
         else if(BV_CONST == type){
         }
         else if (AIG_NODE == type) {
            assert(0);
         }
         else if(AIG_FALSE == type){
            assert(0);
         }
         else if(V3_PI ==type){
            assert(createInput(replaceNtk, new_nid));
         }
         else if(V3_PIO==type){
            assert(createInout(replaceNtk, new_nid));
         }
         else{
            assert(0);
         }
      }
      else{}
   }
}


unsigned SfMgr::splitModule(const string& fileName,vector<string>& moduleNames){
    cerr<<"split module\n";
    ifstream ifile(fileName.c_str());
    if(!ifile){
        cout<<"Unable to open file"<<endl;
        return 0;
    }
    string line;
    unsigned ret=0;
    while(ifile.good()){
        ifile>>line;
        if(line=="module"){
            ++ret;
            getline(ifile,line,'(');
            line=line.substr(line.find_first_not_of(" \n\r\t"));
            moduleNames.push_back(line);
            ofstream ofile;
            ofile.open(line.c_str(),ios::out|ios::trunc);
            ofile<<"module "<<line<<'(';
            while(ifile.good()){
                getline(ifile,line);
                ofile<<line<<endl;
                size_t found=line.find("endmodule");
                if(found==string::npos)
                    continue;
                ofile.close();
                break;
            }
        }
    }
    return ret;
}
