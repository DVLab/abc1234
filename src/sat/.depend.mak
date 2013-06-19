reader.o: reader.cpp reader.h
satCmd.o: satCmd.cpp ../../include/v3NtkUtil.h \
  ../../include/v3NtkHandler.h ../../include/v3Ntk.h \
  ../../include/v3Misc.h ../../include/v3Map.h ../../include/v3Set.h \
  ../../include/v3Vec.h ../../include/v3Hash.h ../../include/v3HashUtil.h \
  ../../include/v3Type.h ../../include/v3IntType.h \
  ../../include/v3Queue.h ../../include/v3Stack.h \
  ../../include/v3BitVec.h ../../include/v3StrUtil.h \
  ../../include/v3Msg.h satCmd.h ../../include/v3CmdMgr.h satMgr.h \
  ../../include/v3SvrMiniSat.h ../../include/v3SvrBase.h \
  ../../include/v3SvrType.h ../../include/Solver.h \
  ../../include/SolverTypes.h ../../include/Global.h \
  ../../include/VarOrder.h ../../include/Heap.h ../../include/Proof.h \
  ../../include/File.h ../../include/boolector.h reader.h
satMgr.o: satMgr.cpp ../../include/v3Msg.h ../../include/v3NtkUtil.h \
  ../../include/v3NtkHandler.h ../../include/v3Ntk.h \
  ../../include/v3Misc.h ../../include/v3Map.h ../../include/v3Set.h \
  ../../include/v3Vec.h ../../include/v3Hash.h ../../include/v3HashUtil.h \
  ../../include/v3Type.h ../../include/v3IntType.h \
  ../../include/v3Queue.h ../../include/v3Stack.h \
  ../../include/v3BitVec.h ../../include/v3SvrMiniSat.h \
  ../../include/v3SvrBase.h ../../include/v3SvrType.h \
  ../../include/Solver.h ../../include/SolverTypes.h \
  ../../include/Global.h ../../include/VarOrder.h ../../include/Heap.h \
  ../../include/Proof.h ../../include/File.h ../../include/boolector.h \
  reader.h satMgr.h
