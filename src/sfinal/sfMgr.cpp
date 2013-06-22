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

V3NetId getNewNetId(V3NetId netid){
	map<uint32_t,V3NetId>::iterator it=netidMap.find(netid.id);
	assert(it!=netidMap.end());
	return it->second;
}


void
SfMgr::traverseFanin(){
  v3Handler.setCurHandlerFromId(_designHandler);
 // V3NetId id = V3NetId::makeNetId(); //_designNtk->getOutput(0);
  V3NetVec orderedNets;
//  V3NetVec targetNets;
  orderedNets.clear();
  orderedNets.reserve(_designNtk->getNetSize());
//  targetNets.push_back(id);
  dfsNtkForGeneralOrder(_designNtk,orderedNets);
  assert (orderedNets.size() <= _designNtk->getNetSize());

  V3NtkInput* inputHandler = new V3NtkInput(false,"new_ntk");

//  V3Ntk* new_ntk=new V3Ntk();
  

   V3BvNtk* new_ntk=(V3BvNtk*)( inputHandler->getNtk());
//   V3BvNtk* new_ntk=new V3BvNtk();
   
//   (V3BvNtk*)inputHandler->getNtk();
 //  assert(createInput(new_ntk,new_nid));
   V3NtkHandler* curHandler= v3Handler.getCurHandler();


  

  for(unsigned i=0;i<orderedNets.size();i++){
	   V3NetId  netid=	orderedNets[i];
	   const V3GateType& type = _designNtk->getGateType(netid);
	   string name =curHandler-> getNetNameOrFormedWithId(netid);
		cout<<"i:"<<netid.id<<" name:"<< name <<" type:"<<V3GateTypeStr[type]<<endl;
		
	   V3NetId new_nid=inputHandler->createNet(name+"_"+ v3Int2Str(netid.id) ,static_cast<V3BvNtk*>(_designNtk)->getNetWidth(netid));
	  netidMap[netid.id]=new_nid;

		cout<<"getGateType:"<<V3GateTypeStr[new_ntk->getGateType(new_nid)]<<endl;
		   if (V3_MODULE == type) {
			  Msg(MSG_WAR) << "Currently Expression Over Module Instances has NOT Been Implemented !!" << endl;
		   }
			 if (isV3PairType(type)) {
				const string name1 = curHandler->getNetExpression(_designNtk->getInputNetId(netid, 0)); assert (name1.size());
				const string name2 = curHandler->getNetExpression(_designNtk->getInputNetId(netid, 1)); assert (name2.size());

			   V3NetId new_nid0=getNewNetId(_designNtk->getInputNetId(netid, 0));
			   //=new_ntk->createNet(static_cast<V3BvNtk*>(_designNtk)->getNetWidth(_designNtk->getInputNetId(netid, 0) ));
			   V3NetId new_nid1=getNewNetId(_designNtk->getInputNetId(netid, 1));
			   //=new_ntk->createNet(static_cast<V3BvNtk*>(_designNtk)->getNetWidth(_designNtk->getInputNetId(netid, 1) ));
			
				assert(createBvPairGate(new_ntk,type, new_nid, new_nid0,new_nid1));
				if (BV_MERGE == type) name = "{" + name1 + ", " + name2 + "}";
				else {

				   switch (type) {
					  case BV_AND      : name = "(" + name1 + " & "  + name2 + ")"; break;
					  case BV_XOR      : name = "(" + name1 + " ^ "  + name2 + ")"; break;
					  case BV_ADD      : name = "(" + name1 + " + "  + name2 + ")"; break;
					  case BV_SUB      : name = "(" + name1 + " - "  + name2 + ")"; break;
					  case BV_MULT     : name = "(" + name1 + " * "  + name2 + ")"; break;
					  case BV_SHL      : name = "(" + name1 + " << " + name2 + ")"; break;
					  case BV_SHR      : name = "(" + name1 + " >> " + name2 + ")"; break;
					  case BV_DIV      : name = "(" + name1 + " / "  + name2 + ")"; break;
					  case BV_MODULO   : name = "(" + name1 + " % "  + name2 + ")"; break;
					  case BV_EQUALITY : name = "(" + name1 + (netid.cp ? " != " : " == ") + name2 + ")";break;
					  case BV_GEQ      : name = "(" + name1 + (netid.cp ? " < "  : " >= ") + name2 + ")";break;
					  default          : assert (0);
				   }
				}
			 }
			 else if (isV3ReducedType(type)) {//OK
				const string name1 = curHandler->getNetExpression(_designNtk->getInputNetId(netid, 0)); assert (name1.size());
				
			   V3NetId new_nid0=getNewNetId(_designNtk->getInputNetId(netid, 0));
			   //new_ntk->createNet(static_cast<V3BvNtk*>(_designNtk)->getNetWidth(_designNtk->getInputNetId(netid, 0) ));
				assert(createBvReducedGate(new_ntk,type, new_nid,new_nid0));
				switch (type) {
				   case BV_RED_AND :
				   name = "(&" + name1 + ")"; 
				   break;
				   case BV_RED_OR  : 
				   name = "(|" + name1 + ")"; 
				   break;
				   case BV_RED_XOR :
				   name = "(^" + name1 + ")"; 
				   break;
				   default         : assert (0);
				}

			 }
			 else if (BV_MUX == type) {//OK
				const string fName = curHandler->getNetExpression(_designNtk->getInputNetId(netid, 0)); assert (fName.size());
				const string tName = curHandler->getNetExpression(_designNtk->getInputNetId(netid, 1)); assert (tName.size());
				const string sName = curHandler->getNetExpression(_designNtk->getInputNetId(netid, 2)); assert (sName.size());
			 	
			   V3NetId new_nid0=getNewNetId(_designNtk->getInputNetId(netid, 0));
			   V3NetId new_nid1=getNewNetId(_designNtk->getInputNetId(netid, 1));
			   V3NetId new_nid2=getNewNetId(_designNtk->getInputNetId(netid, 2));
			   // V3NetId new_nid0=new_ntk->createNet(static_cast<V3BvNtk*>(_designNtk)->getNetWidth(_designNtk->getInputNetId(netid, 0) ));
			   // V3NetId new_nid1=new_ntk->createNet(static_cast<V3BvNtk*>(_designNtk)->getNetWidth(_designNtk->getInputNetId(netid, 1) ));
			   // V3NetId new_nid2=new_ntk->createNet(static_cast<V3BvNtk*>(_designNtk)->getNetWidth(_designNtk->getInputNetId(netid, 2) ));
				assert(createBvMuxGate(new_ntk,new_nid, new_nid0,new_nid1,new_nid2));
				name = "(" + sName + " ? " + tName + " : " + fName + ")";
			 }
			 else if (BV_SLICE == type) {//OK
				const string name1 = curHandler->getNetExpression(_designNtk->getInputNetId(netid, 0)); assert (name1.size());

			   V3NetId new_nid0=getNewNetId(_designNtk->getInputNetId(netid, 0));
			   // V3NetId new_nid0=new_ntk->createNet(static_cast<V3BvNtk*>(_designNtk)->getNetWidth(_designNtk->getInputNetId(netid, 0) ));
				cout<<"width 1:"<<new_ntk->getNetWidth(new_nid)<<endl;
	   			cout<<"width 2:"<<new_ntk->getNetWidth(new_nid0)<<endl;
				cout<<"width 3:"<<static_cast<V3BvNtk*>(_designNtk)->getNetWidth(_designNtk->getInputNetId(netid, 0)) <<endl;
				cout<<"width 4:"<<static_cast<V3BvNtk*>(_designNtk)->getNetWidth(netid) <<endl;
				const uint32_t msb = static_cast<V3BvNtk*>(_designNtk)->getInputSliceBit(netid, true);
				const uint32_t lsb = static_cast<V3BvNtk*>(_designNtk)->getInputSliceBit(netid, false);
				const uint32_t width = _designNtk->getNetWidth(_designNtk->getInputNetId(netid, 0)); assert (width);
				cout<<"msb:"<<msb<<" lsb:"<<lsb<<endl;
			 	assert(createBvSliceGate(new_ntk,new_nid, new_nid0, msb,lsb));
				name = ((msb >= lsb) && (width == (1 + msb - lsb))) ? name1 : 
					   (msb == lsb) ? (name1 + "[" + v3Int2Str(msb) + "]") : 
					   (name1 + "[" + v3Int2Str(msb) + " : " + v3Int2Str(lsb) + "]");
			 }
			 else if(BV_CONST == type){//OK
				const V3BitVecX* const value = static_cast<V3BvNtk*>(_designNtk)->getInputConstValue(netid); assert (value);
			//	cout<<"to:"<<value->toExp()<<endl;
			//	cout<<"reg:"<<value->regEx()<<endl;
			 	assert(createBvConstGate(new_ntk,new_nid, v3Int2Str(value->size())+"'b"+value->regEx()));
				name= v3Int2Str(value->size()) + "'b" + (netid.cp ? (~(*value)).regEx() : value->regEx());
			 }
			 else if (AIG_NODE == type) {
				const string name1 = curHandler->getNetExpression(_designNtk->getInputNetId(netid, 0)); assert (name1.size());
				const string name2 = curHandler->getNetExpression(_designNtk->getInputNetId(netid, 1)); assert (name2.size());
				name = name1 + " && " + name2;
			 }
			 else if(AIG_FALSE == type){
				name= (netid.cp ? "AIGER_TRUE" : "AIGER_FALSE");
			 }
			 else if(V3_PI ==type){//OK
			    assert(createInput(new_ntk,new_nid));
				cout<<"IO type:"<<V3GateTypeStr[type]<<endl;
			 }
			 else if(V3_PIO==type){//OK
				assert(createInout(new_ntk,new_nid));
				cout<<"IO type:"<<V3GateTypeStr[type]<<endl;
			 }
			 else{
				cout<<"UNKNOWN"<<V3GateTypeStr[type]<<endl;
			 }
   	cout<<netid.id<<" "<< (netid.cp ? "~" : "") + name<<endl;
  }
  for(uint32_t i=0,j=_designNtk->getOutputSize();i<j;++i){
      V3NetId outid=_designNtk->getOutput(i);
      V3NetId new_outid=getNewNetId(outid);
      new_ntk->createOutput(new_outid);
  }
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
      cout<<i<<": "<<strVec[i]<<endl;
  strVec.clear();
  orderedNets.clear();
  orderedNets.reserve(new_ntk->getNetSize());
//  targetNets.push_back(id);
  dfsNtkForGeneralOrder(new_ntk,orderedNets);
  assert (orderedNets.size() <= new_ntk->getNetSize());
  cout<<"*****NEW CIRCUIT*****"<<endl;
  for(unsigned i=0;i<orderedNets.size();i++){
		V3NetId netid=	orderedNets[i];
		const V3GateType& type = new_ntk->getGateType(netid);
	    string name ="";
		inputHandler->getNetName(netid,name);
		//cout<<"i:"<<netid.id <<" name:"<< name << " type:"<<(netid.cp ? "~" : "")<< V3GateTypeStr[type]<<endl;
		//cout<<"i:"<<netid.id <<" type:"<<(netid.cp ? "~" : "")<< V3GateTypeStr[type]<<endl;
        string str=V3GateTypeStr[type]+(netid.cp?"~":"")+" name:"+name;
        strVec.push_back(str);
	}
  sort(strVec.begin(),strVec.end());
  for(unsigned i=0;i<strVec.size();++i)
      cout<<i<<": "<<strVec[i]<<endl;
  strVec.clear();
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
