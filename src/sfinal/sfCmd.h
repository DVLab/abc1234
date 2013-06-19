/****************************************************************************
  FileName     [ sfCmd.h ] 
  PackageName  [ v3/src/sfinal ]
  Synopsis     [ SoCV Final Commands. ]
  Author       [ Mark Chang ]
  Copyright    [ Copyright(c) 2013 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef SF_CMD_H
#define SF_CMD_H

#include "v3CmdMgr.h"


// SOCV FINAL Command
V3_COMMAND(V3ReadDesignCmd,      CMD_TYPE_FINAL);
V3_COMMAND(V3ReadLibraryCmd,     CMD_TYPE_FINAL);
V3_COMMAND(V3GoCmd,              CMD_TYPE_FINAL);
V3_COMMAND(V3DebugCmd,           CMD_TYPE_FINAL);

#endif
