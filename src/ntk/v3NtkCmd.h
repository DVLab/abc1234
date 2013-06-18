/****************************************************************************
  FileName     [ v3NtkCmd.h ]
  PackageName  [ v3/src/ntk ]
  Synopsis     [ Ntk Commands. ]
  Author       [ Cheng-Yin Wu ]
  Copyright    [ Copyright(c) 2012 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef V3_NTK_CMD_H
#define V3_NTK_CMD_H

#include "v3CmdMgr.h"


// Network Input Commands
V3_COMMAND(V3ReadRTLCmd,         CMD_TYPE_IO);
V3_COMMAND(V3ReadAIGCmd,         CMD_TYPE_IO);
V3_COMMAND(V3ReadBTORCmd,        CMD_TYPE_IO);
// Network Output Commands
V3_COMMAND(V3WriteRTLCmd,        CMD_TYPE_IO);
V3_COMMAND(V3WriteAIGCmd,        CMD_TYPE_IO);
V3_COMMAND(V3WriteBTORCmd,       CMD_TYPE_IO);
// Network Report Commands
V3_COMMAND(V3PrintNtkCmd,        CMD_TYPE_PRINT);
V3_COMMAND(V3PlotNtkCmd,         CMD_TYPE_PRINT);
V3_COMMAND(V3NetInfoCmd ,        CMD_TYPE_PRINT); // MODIFICATION FOR SoCV BDD
// Network Hierarchy Manipulation Commands
V3_COMMAND(V3SetNtkCmd,          CMD_TYPE_MANIP);
V3_COMMAND(V3ListNtkCmd,         CMD_TYPE_MANIP);
V3_COMMAND(V3WorkNtkCmd,         CMD_TYPE_MANIP);
// Network Simplification Commands
V3_COMMAND(V3StrashNtkCmd,       CMD_TYPE_SYNTHESIS);
V3_COMMAND(V3ReduceNtkCmd,       CMD_TYPE_SYNTHESIS);
V3_COMMAND(V3RewriteNtkCmd,      CMD_TYPE_SYNTHESIS);
// SOCV FINAL Command
V3_COMMAND(V3ReadDesignCmd,      CMD_TYPE_FINAL);
V3_COMMAND(V3ReadLibraryCmd,     CMD_TYPE_FINAL);
V3_COMMAND(V3GoCmd,              CMD_TYPE_FINAL);
V3_COMMAND(V3DebugCmd,           CMD_TYPE_FINAL);


#endif
