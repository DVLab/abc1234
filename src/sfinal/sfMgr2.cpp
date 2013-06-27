/****************************************************************************
  FileName     [ sfMgr2.cpp ]
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
#include<math.h>
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

V3NetId getMapNetId2(V3NtkInput* fromHandler,V3NtkInput* toHandler, V3NetId netid , std::map<uint32_t,V3NetId>& IdMap ,uint32_t nid){
   map<uint32_t,V3NetId>::iterator it=IdMap.find(netid.id);
   cout<<"old_id:"<<netid.id<<endl;
	V3NetId new_nid;
   if(it!=IdMap.end()){
	   new_nid=it->second;
       new_nid.cp = netid.cp;
	   }
   else{
      string name;
	  toHandler->getNetName(netid,name);
	  if(name==""){
	  	name=fromHandler->getNetNameOrFormedWithId(netid);
	  }
	  else{
		name=fromHandler->getNetNameOrFormedWithId(netid)+"_2";
	  }
	  cout<<"create name"<<name<<endl;
       new_nid = toHandler->createNet(name+"_ntk"+v3Int2Str(nid),static_cast<V3BvNtk*>(fromHandler->getNtk())->getNetWidth(netid));
      new_nid.cp = netid.cp;
      IdMap[netid.id]=new_nid; // ???
	}
	cout<<"old_id:"<<netid.id<<"new_id:"<<new_nid.id<<endl;
	return new_nid;
}

V3NetId getMapNetId3(V3NetId netid , std::map<uint32_t,V3NetId>& IdMap ){
   map<uint32_t,V3NetId>::iterator it=IdMap.find(netid.id);
   assert(it!=IdMap.end());
   return it->second;
}

void SfMgr::solveSat2(){
	bool unsat=false;

//	for(uint32_t b1=0; b1<=1;b1++){
//	for(uint32_t b2=0; b2<=1;b2++){
//	for(uint32_t b3=0; b3<=1;b3++){
//	for(uint32_t b4=0; b4<=1;b4++){
//	for(uint32_t b5=0; b5<=1;b5++){
		//if(!unsat){continue;}
  _satSolver = new V3SvrBoolector(_mergeNtk);

  uint32_t z=0;

  for(uint32_t i=0; i<_satNets.size();i++){
	  const V3NetId outid=_satNets[i];
		static_cast<V3SvrBase*>(_satSolver)->addBoundedVerifyData(outid,z );
		_satSolver->assumeProperty(outid, false, z);
  }
	
//		_satSolver->assumeProperty(_inputVarNet[0],false , z);
//		_satSolver->assumeProperty(_inputVarNet[1],true , z);
//		_satSolver->assumeProperty(_inputVarNet[2],true , z);
//		_satSolver->assumeProperty(_inputVarNet[3],false , z);
//		_satSolver->assumeProperty(_inputVarNet[4],false , z);
	

    _satSolver->simplify();
	unsat=!_satSolver->assump_solve(); 
//	cout<<(_satSolver->assump_solve() ? "SAT" : "UNSAT")<<endl;
	if(unsat){
//		cout<<"UNSAT,sol:"<<b5<<b4<<b3<<b2<<b1<<endl;
		cout<<"UNSAT"<<endl;
		unsat=true;
//		break;
	}

//	}}}}}
	if(!unsat){
		cout<<"no solution"<<endl;

	}
	/*
  V3BitVecX dataValue;
    for (int j = _mergeNtk->getOutputSize()-1; j >= 0; --j) {
      if (_satSolver->existVerifyData(_mergeNtk->getOutput(j), z)) {
        dataValue = _satSolver->getDataValue(_mergeNtk->getOutput(j), z);
        assert (dataValue.size() == _mergeNtk->getNetWidth(_mergeNtk->getOutput(j)));
        Msg(MSG_IFO) << dataValue[0];
      } else Msg(MSG_IFO) << 'x';
    }*/
}

void SfMgr::testAddNtk3(){

  V3NtkInput* inputHandler = new V3NtkInput(false,"test_ntk");
  V3BvNtk* test_ntk=(V3BvNtk*)( inputHandler->getNtk());
  v3Handler.pushAndSetCurHandler(inputHandler);
  uint32_t _testHandler = v3Handler.getCurHandlerId();

  map<uint32_t,V3NetId> design_1_Map;
  map<uint32_t,V3NetId> design_2_Map;
  map<uint32_t,bool> block_list;

  addNtk2(_designHandler,_testHandler,design_1_Map,0,block_list,true,true);
  addNtk2(_designHandler,_testHandler,design_2_Map,2,block_list,true,true);

  setMerge(_testHandler,test_ntk);
}

   
void SfMgr::testAddNtk2(){
  V3NtkInput* inputHandler = new V3NtkInput(false,"test_ntk");
  V3BvNtk* test_ntk=(V3BvNtk*)( inputHandler->getNtk());
/*  V3NetId new_nid0 = inputHandler->createNet("test0",1);
  V3NetId new_nid1 = inputHandler->createNet("test1",1);
  V3NetId new_nid = inputHandler->createNet("test2",1);
  createInput(test_ntk, new_nid0);
  createInput(test_ntk, new_nid1);
  createBvPairGate(test_ntk, BV_AND, new_nid, new_nid0, new_nid1);*/
  v3Handler.pushAndSetCurHandler(inputHandler);
  uint32_t _testHandler = v3Handler.getCurHandlerId();

  map<uint32_t,V3NetId> design_1_Map;
  map<uint32_t,V3NetId> library_2_Map;
  map<uint32_t,V3NetId> design_2_Map;
  map<uint32_t,bool> block_list;

	//**********Create ntk************************//
  addNtk2(_designHandler,_testHandler,design_1_Map,0,block_list,true,false);
  addNtk2(_libraryHandler[0],_testHandler,library_2_Map,2,block_list,false,false);


	//27,24,18,17
  uint32_t block_list_array[] ={1,10,11,21,12,15,20, 9,13,19,17, 22,14,18,25, 24};
  for(int i=0;i<16;i++){
	block_list[block_list_array[i]]=false;
  }
  uint32_t boundary_list_array[] ={27,24,18,17};
 for(int i=0;i<3;i++){
	block_list[boundary_list_array[i]]=false;
  }
  //block_list.assign(block_list_array,block_list_array+14);
  addNtk2(_designHandler,_testHandler,design_2_Map,1,block_list,false,false);


	//***********Merge ntk************************//
  inputHandler = (V3NtkInput*)v3Handler.getHandler(_testHandler);
  test_ntk=(V3BvNtk*)( inputHandler->getNtk());
  
  V3NetId input_1=inputHandler->createNet("TEMP_MERGE_1",4);
  V3NetId input_2=inputHandler->createNet("TEMP_MERGE_2",4);
  V3NetId input_const_0=inputHandler->createNet("TEMP_CONST_0",2);
  V3NetId input_const_1=inputHandler->createNet("TEMP_CONST_1",2);

	V3NetId m_input_0=inputHandler->createNet("TEMP_MINPUT_0",1);
	V3NetId m_input_1=inputHandler->createNet("TEMP_MINPUT_1",1);
	V3NetId m_input_2=inputHandler->createNet("TEMP_MINPUT_2",1);
	V3NetId m_input_3=inputHandler->createNet("TEMP_MINPUT_3",1);
	V3NetId m_input_4=inputHandler->createNet("TEMP_MINPUT_4",1);
	_inputVarNet.push_back(m_input_0);
	_inputVarNet.push_back(m_input_1);
	_inputVarNet.push_back(m_input_2);
	_inputVarNet.push_back(m_input_3);
	_inputVarNet.push_back(m_input_4);

	cout<<"minput_id:"<<m_input_1.id<<" "<<m_input_2.id<<" "<<m_input_3.id<<" "<<m_input_4.id<<" "<<endl;

//  assert(createBvPairGate( test_ntk  ,  BV_MERGE, input_1 ,  V3NetId::makeNetId(0), test_ntk->getInput(0)));
  assert(createBvPairGate( test_ntk  ,  BV_MERGE, input_2   ,   input_const_0,input_const_1 ));
//  assert(createBvPairGate( test_ntk  , BV_MERGE , input_const_0 ,  V3NetId::makeNetId(0), V3NetId::makeNetId(0) ));
//  assert(createBvPairGate( test_ntk  ,  BV_MERGE, input_const_1 ,  V3NetId::makeNetId(0,1), V3NetId::makeNetId(0,1) ));

  assert(createBvPairGate( test_ntk  ,  BV_MERGE, input_1 ,  m_input_0, test_ntk->getInput(0)));
  assert(createBvPairGate( test_ntk  , BV_MERGE , input_const_0 ,  m_input_4, m_input_3 ));
  assert(createBvPairGate( test_ntk  , BV_MERGE , input_const_1 ,  m_input_2, m_input_1 ));

 assert(createBvConstGate( test_ntk , m_input_0,"1'b0" ));   
 assert(createBvConstGate( test_ntk , m_input_1,"1'b1"));   
 assert(createBvConstGate( test_ntk , m_input_2,"1'b1"));   
 assert(createBvConstGate( test_ntk , m_input_3,"1'b0"));   
 assert(createBvConstGate( test_ntk , m_input_4,"1'b0"));   

  assert(createV3BufGate( test_ntk , getMapNetId3(V3NetId::makeNetId(2),library_2_Map),input_1)); 
  assert(createV3BufGate( test_ntk , getMapNetId3(V3NetId::makeNetId(1),library_2_Map),input_2));

		assert(createBvSliceGate(test_ntk,   getMapNetId3(V3NetId::makeNetId(27),design_2_Map) ,
											getMapNetId3(V3NetId::makeNetId(3),library_2_Map) , 0,0));
		assert(createBvSliceGate(test_ntk,   getMapNetId3(V3NetId::makeNetId(24),design_2_Map) ,
											getMapNetId3(V3NetId::makeNetId(3),library_2_Map) , 1,1));
		assert(createBvSliceGate(test_ntk,  getMapNetId3(V3NetId::makeNetId(18),design_2_Map) ,
											~getMapNetId3(V3NetId::makeNetId(3),library_2_Map) , 2,2));
		assert(createBvSliceGate(test_ntk,  getMapNetId3(V3NetId::makeNetId(17),design_2_Map) ,
											~getMapNetId3(V3NetId::makeNetId(3),library_2_Map) , 3,3));


	cout<<"****insert_input****"<<endl;
  V3NetId inputid0=test_ntk->getInput(1);
  V3NetId inputid1=getMapNetId3(_designNtk->getInput(1),design_2_Map);
	cout<<"old:"<<inputid0.id  << " type:"<<
		(inputid0.cp ? "~" : "")<< V3GateTypeStr[test_ntk->getGateType(inputid0)]<<endl;
	cout<<"new:"<<inputid1.id  << " type:"<<
		(inputid1.cp ? "~" : "")<< V3GateTypeStr[test_ntk->getGateType(inputid1)]<<endl;


  assert(createV3BufGate( test_ntk , getMapNetId3(_designNtk->getInput(1),design_2_Map), getMapNetId3(_designNtk->getInput(1),design_1_Map) )); 

  for(uint32_t i=0,j=_designNtk->getOutputSize();i<j;++i){
      V3NetId outid=_designNtk->getOutput(i);
      V3NetId new_outid=getMapNetId3(outid,design_1_Map);
      V3NetId new_outidd=getMapNetId3(outid,design_2_Map);

      string name="miter_"+i;
	  V3NetId new_nid=inputHandler->createNet(name,static_cast<V3BvNtk*>(_designNtk)->getNetWidth(outid));
      createBvPairGate(test_ntk,BV_XOR,new_nid,new_outid,new_outidd);
      assert(createOutput(test_ntk,new_outid));
      assert(createOutput(test_ntk,new_outidd));
	  for(uint32_t k=0; k< static_cast<V3BvNtk*>(_designNtk)->getNetWidth(outid); k++){
		  V3NetId new_outid2=inputHandler->createNet(name+"_new_out1_"+v3Int2Str(new_nid.id)+"_"+v3Int2Str(k),1);
			assert(createBvSliceGate(test_ntk,new_outid2, new_nid, k ,k));
			assert(createOutput(test_ntk,new_outid2));
			_satNets.push_back(new_outid2);	
	  }
  }


	V3StrVec outputName(test_ntk->getOutputSize(), "");
	for (uint32_t i = 0; i < test_ntk->getOutputSize(); ++i){ 
		  if (outputName[i].size()){
		  	inputHandler->recordOutName(i, outputName[i]);
		  }
		  else{
		    inputHandler->recordOutName(i, "output_" + v3Int2Str(i));
		  }
  	}

	//CREATE INPUT FOR RTL
	/*
			assert(createInput(test_ntk,m_input_0));
			assert(createInput(test_ntk,m_input_1));
			assert(createInput(test_ntk,m_input_2));
			assert(createInput(test_ntk,m_input_3));
			assert(createInput(test_ntk,m_input_4));
*/
	//END CREATE INPUT

  setMerge(_testHandler,test_ntk);
}

void SfMgr::addNtk2(uint32_t from_handler,uint32_t to_handler,map<uint32_t,V3NetId>& IdMap,uint32_t nid,map<uint32_t,bool>& block_list,bool add_in,bool add_out){

   V3NtkInput* fromHandler = (V3NtkInput*)v3Handler.getHandler(from_handler);
   V3NtkInput* toHandler   = (V3NtkInput*)v3Handler.getHandler(to_handler);
//   cout<<"*****create_net_start:"<<fromHandler->getNtkName()<<"*****"<<endl;
   V3NetVec orderedNets;
   orderedNets.clear();

   V3BvNtk* ntk = (V3BvNtk*)(fromHandler->getNtk());
   orderedNets.reserve(ntk->getNetSize());
   dfsNtkForGeneralOrder(ntk,orderedNets);
   assert (orderedNets.size() <= ntk->getNetSize());

   V3BvNtk* new_ntk = (V3BvNtk*)(toHandler->getNtk());

   for(unsigned i = 0 ; i < orderedNets.size() ; ++i){

      V3NetId old_nid = orderedNets[i];

		map<uint32_t,bool>::iterator it = block_list.find( old_nid.id);
		if(it!=block_list.end()){
			if(!it->second){
				cout<<"block:"<<it->first<<endl;
				continue;
				}
			else if(it->second){
				//const V3GateType& type = ntk->getGateType(old_nid);
				string name = fromHandler->getNetNameOrFormedWithId(old_nid);
				V3NetId new_nid= getMapNetId2(fromHandler,toHandler,old_nid,IdMap,nid);
				cout<<"boundary:"<<it->first<<endl;
				continue;
			}
		}

      const V3GateType& type = ntk->getGateType(old_nid);
        string name = fromHandler->getNetNameOrFormedWithId(old_nid);
 //     V3NetId new_nid = toHandler->createNet(name ,static_cast<V3BvNtk*>(ntk)->getNetWidth(old_nid));
 //     new_nid.cp = old_nid.cp;
  //    IdMap[old_nid.id]=new_nid; // ???
		V3NetId new_nid= getMapNetId2(fromHandler,toHandler,old_nid,IdMap,nid);

		cout<<"i:"<<old_nid.id <<" name:"<< name << " type:"<<(old_nid.cp ? "~" : "")<< V3GateTypeStr[type]<<endl;

      if(BV_CONST==type) 
         continue;
      if (V3_MODULE == type) {
         Msg(MSG_WAR) << "Currently Expression Over Module Instances has NOT Been Implemented !!" << endl;
      }
      if (isV3PairType(type)) {
         V3NetId old_nid0 = ntk->getInputNetId(old_nid, 0);
         V3NetId old_nid1 = ntk->getInputNetId(old_nid, 1);
         V3NetId new_nid0 = getMapNetId2(fromHandler,toHandler,old_nid0,IdMap,nid);
         V3NetId new_nid1 = getMapNetId2(fromHandler,toHandler,old_nid1,IdMap,nid);
        // new_nid0.cp = old_nid0.cp;
        // new_nid1.cp = old_nid1.cp;

		cout<<"new:"<<new_nid.id  << " type:"<<
		(new_nid.cp ? "~" : "")<< V3GateTypeStr[new_ntk->getGateType(new_nid)]<<endl;
		cout<<"new0:"<<new_nid0.id << " type:"<<
		(new_nid0.cp ? "~" : "")<< V3GateTypeStr[new_ntk->getGateType(new_nid0)]<<endl;
		cout<<"new1:"<<new_nid1.id  << " type:"<<
		(new_nid1.cp ? "~" : "")<< V3GateTypeStr[new_ntk->getGateType(new_nid1)]<<endl;

         assert(createBvPairGate(new_ntk, type, new_nid, new_nid0, new_nid1));
      }
      else if (isV3ReducedType(type)) {
         V3NetId old_nid0 = ntk->getInputNetId(old_nid, 0);
         V3NetId new_nid0 = getMapNetId2(fromHandler,toHandler,old_nid0,IdMap,nid);
       //  new_nid0.cp = old_nid0.cp;
         assert(createBvReducedGate(new_ntk, type, new_nid, new_nid0));
      }
      else if (BV_MUX == type) {
         V3NetId old_nid0 = ntk->getInputNetId(old_nid, 0);
         V3NetId old_nid1 = ntk->getInputNetId(old_nid, 1);
         V3NetId old_nid2 = ntk->getInputNetId(old_nid, 2);
         V3NetId new_nid0 = getMapNetId2(fromHandler,toHandler,old_nid0,IdMap,nid);
         V3NetId new_nid1 = getMapNetId2(fromHandler,toHandler,old_nid1,IdMap,nid);
         V3NetId new_nid2 = getMapNetId2(fromHandler,toHandler,old_nid2,IdMap,nid);
       //  new_nid0.cp = old_nid0.cp;
       //  new_nid1.cp = old_nid1.cp;
       //  new_nid2.cp = old_nid2.cp;
         assert(createBvMuxGate(new_ntk, new_nid, new_nid0, new_nid1, new_nid2));
      }
      else if (BV_SLICE == type) {
         V3NetId old_nid0 = ntk->getInputNetId(old_nid, 0);
         V3NetId new_nid0 = getMapNetId2(fromHandler,toHandler,old_nid0,IdMap,nid);
       //  new_nid0.cp = old_nid0.cp;
         const uint32_t msb = static_cast<V3BvNtk*>(ntk)->getInputSliceBit(old_nid, true);
         const uint32_t lsb = static_cast<V3BvNtk*>(ntk)->getInputSliceBit(old_nid, false);
         assert(createBvSliceGate(new_ntk, new_nid, new_nid0, msb,lsb));
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
	  	if(add_in){
         assert(createInput(new_ntk, new_nid));
		 }
      }
      else if(V3_PIO==type){
	  	if((add_in)&&(add_out)){
         assert(createInout(new_ntk, new_nid));
		 }
      }
      else{
         assert(0);
      }
   }
   if(add_out){
	   for(uint32_t i = 0 , j = ntk->getOutputSize() ; i < j ; ++i){
		  V3NetId outid = ntk->getOutput(i);
		  V3NetId new_outid = getMapNetId2(fromHandler,toHandler,outid,IdMap,nid);
		//  new_outid.cp = outid.cp;
		  assert(createOutput(new_ntk, new_outid));
	   }
	}
   V3StrVec outputName(new_ntk->getOutputSize(), "");
   for (uint32_t i = 0; i < new_ntk->getOutputSize(); ++i){ 
      if (outputName[i].size()){
         toHandler->recordOutName(i, outputName[i]);
      }
      else{
         toHandler->recordOutName(i, "output_" + v3Int2Str(i));
      }
   }

//   cout<<"*****create_net_end:"<<fromHandler->getNtkName()<<"*****"<<endl;
}

