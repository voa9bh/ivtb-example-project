package bosch.bh.testprograms.ivtb.global

import bosch.bh.include.platform.teststep.TestFunction
import bosch.bh.ts3000.procs.StepReturn
import bosch.bh.ts3000.procs.testprogrambuilder.WithTestStepSpec

class PutAdaptStatFunction extends TestFunction  implements WithTestStepSpec {
  @Override
  StepReturn apply(StepReturn stepReturn) {
    PutAdaptStatReturn putAdaptStatReturn = new PutAdaptStatReturn("PutAdaptStat")

    // dummy values for testing
    putAdaptStatReturn.countCyc=10
    putAdaptStatReturn.pressureSet=1000
    putAdaptStatReturn.pressureAct=1010

    return putAdaptStatReturn
  }


}
