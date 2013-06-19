/****************************************************************************
  FileName     [ sfCmd.cpp ]
  PackageName  [ v3/src/sfinal ]
  Synopsis     [ SoCV Final Commands. ]
  Author       [ Mark Chang ]
  Copyright    [ Copyright(c) 2013 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/


#include "v3Msg.h"
#include "v3StrUtil.h"
#include "v3FileUtil.h"
#include "v3NtkInput.h"
#include "v3NtkParser.h"
#include "v3NtkWriter.h"
#include "v3NtkSimplify.h"
#include "sfCmd.h"
#include <iomanip>
#include <cstring>
#include "v3NtkUtil.h"
#include "sfMgr.h"
using namespace std;


/*
enum SoCVFinalType {
   // Internal Admitted Types
   SV_DESIGN=0,
   SV_LIB,
   SV_OUT
};*/

static SfMgr* sfMgr=new SfMgr();


bool initSfCmd() {
   // Set 1'b0 and [0:0] 0 to V3BVXId and V3BusId, respectively
   const V3BVXId zeroId = V3BvNtk::hashV3ConstBitVec("1'b0"); if(zeroId) assert (!zeroId); // MODIFICATION FOR SoCV
   const V3BusId boolId = V3BvNtk::hashV3BusId(0, 0); if(boolId) assert (!boolId); // MODIFICATION FOR SoCV
   // Reset Verbosity Settings
   V3NtkHandler::resetVerbositySettings();
   return (
      // SOCV FINAL Command
         v3CmdMgr->regCmd("REAd Design",           3, 1, new V3ReadDesignCmd        ) &&
         v3CmdMgr->regCmd("REAd Library",           3, 1, new V3ReadLibraryCmd        ) &&
         v3CmdMgr->regCmd("GO",           2, new V3GoCmd        )&&
         v3CmdMgr->regCmd("DEBUG",           5, new V3DebugCmd       ) 
   );
}

//----------------------------------------------------------------------
// Debug <-design | -lib> 
//----------------------------------------------------------------------
V3CmdExecStatus
V3DebugCmd::exec(const string& option) {
   vector<string> options;
   V3CmdExec::lexOptions(option, options);

  if(options.size()<1){
	   return V3CmdExec::errorOption(CMD_OPT_MISSING,  "<-design | -lib>"  );
  }
  const string token = options[0];
  if (v3StrNCmp("-design", token, 3) == 0) {
	 v3Handler.setCurHandlerFromId(sfMgr->getDesignHandler());
  }
  else if (v3StrNCmp("-lib", token, 3) == 0) {
	 v3Handler.setCurHandlerFromId(sfMgr->getLibraryHandler());
  }
 else return V3CmdExec::errorOption(CMD_OPT_ILLEGAL, token);

   return CMD_EXEC_DONE;
}

void
V3DebugCmd::usage() const {
   Msg(MSG_IFO) << " Debug <-design | -lib>  " << endl;
}

void
V3DebugCmd::help() const {
   Msg(MSG_IFO)  << "Debug:" << "Debug" << endl;
}

//----------------------------------------------------------------------
// GO 
//----------------------------------------------------------------------
V3CmdExecStatus
V3GoCmd::exec(const string& option) {
	
	sfMgr->traverseFanin();





	return CMD_EXEC_DONE;
}

void
V3GoCmd::usage() const {
   Msg(MSG_IFO) << " GO " << endl;
}

void
V3GoCmd::help() const {
   Msg(MSG_IFO)  << "GO: " << "GO" << endl;
}
//----------------------------------------------------------------------
// REAd Library <(string fileName)> 
//----------------------------------------------------------------------
V3CmdExecStatus
V3ReadLibraryCmd::exec(const string& option) {
   vector<string> options;
   V3CmdExec::lexOptions(option, options);

   string fileName = ""; 
//   bool golden=false,revised=false;
/*
  if(options.size()<2){
   return V3CmdExec::errorOption(CMD_OPT_MISSING, "<(string fileName)>");
 }*/
  if(options.size()<1){
   return V3CmdExec::errorOption(CMD_OPT_MISSING, "<string filenName>");
  }

  const string token2 = options[0];
  fileName = token2;
   if (fileName == "") return V3CmdExec::errorOption(CMD_OPT_MISSING, "<(string fileName)>");

   V3NtkInput* inputHandler = V3NtkFromQuteRTL(fileName.c_str(), false);

   if (!inputHandler) Msg(MSG_ERR) << "Parse Failed !!" << endl;
   else {
   	v3Handler.pushAndSetCurHandler(inputHandler);
	V3Ntk* ntk= new V3Ntk();
	*ntk = *(inputHandler->getNtk());
	sfMgr->addLibrary(v3Handler.getCurHandlerId(),ntk);
	}
   return CMD_EXEC_DONE;
}

void
V3ReadLibraryCmd::usage() const {
   Msg(MSG_IFO) << " REAd Library <(string fileName)> " << endl;
}

void
V3ReadLibraryCmd::help() const {
   Msg(MSG_IFO)  << "REAd Library: " << "Read RTL (Verilog) Library." << endl;
}

//----------------------------------------------------------------------
// REAd Design <(string fileName)> 
//----------------------------------------------------------------------
V3CmdExecStatus
V3ReadDesignCmd::exec(const string& option) {
   vector<string> options;
   V3CmdExec::lexOptions(option, options);

   string fileName = ""; 
//   bool golden=false,revised=false;

  if(options.size()<1){
   return V3CmdExec::errorOption(CMD_OPT_MISSING, "<(string fileName)>");
  }
  const string& token2 = options[0];
  fileName = token2;
   if (fileName == "") return V3CmdExec::errorOption(CMD_OPT_MISSING, "<(string fileName)>");

   V3NtkInput* inputHandler = V3NtkFromQuteRTL(fileName.c_str(), false);
   if (!inputHandler) Msg(MSG_ERR) << "Parse Failed !!" << endl;

   else{
		v3Handler.pushAndSetCurHandler(inputHandler);
		V3Ntk* ntk= new V3Ntk();
		*ntk = *(inputHandler->getNtk());
		sfMgr->setDesign(v3Handler.getCurHandlerId(),ntk);
	}
   return CMD_EXEC_DONE;
}

void
V3ReadDesignCmd::usage() const {
   Msg(MSG_IFO) << " REAd Design <(string fileName)> " << endl;
}

void
V3ReadDesignCmd::help() const {
   Msg(MSG_IFO)   << "REAd Design: " << "Read RTL (Verilog) Design." << endl;
}
