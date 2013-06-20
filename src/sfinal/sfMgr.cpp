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
using namespace std;

extern SATMgr* satMgr;
extern const uint32_t dfsNtkForGeneralOrder(V3Ntk* const, V3NetVec&, const bool& );
extern const uint32_t dfsNtkForReductionOrder(V3Ntk* const, V3NetVec&, const V3NetVec&, const bool& );

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
  for(unsigned i=0;i<orderedNets.size();i++){
		V3NetId& netid=	orderedNets[i];
		cout<<"i:"<<i<<" name:"<< v3Handler.getCurHandler()->getNetNameOrFormedWithId(netid)<<endl;
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
