package bosch.bh.testprograms.ivtb.global

import bosch.bh.ts3000.procs.StepReturn

class PutAdaptStatReturn extends StepReturn {
  PutAdaptStatReturn(String name) {
    super(name)
  }

  int countCyc
  int pressureSet
  int pressureAct
}
