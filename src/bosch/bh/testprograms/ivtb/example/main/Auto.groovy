package bosch.bh.testprograms.ivtb.example.main

import bosch.bh.ts3000.procs.StepReturn
import bosch.bh.ts3000.procs.testprogrambuilder.TestProgramSpec
import bosch.bh.ts3000.procs.TestStep
import bosch.bh.ts3000.pvserver.Inject

class Auto extends TestStep {

  @Inject
  TestProgramSpec testProgram

  @Override
  StepReturn call() {
    final StepReturn stepReturn = StepReturn.newInstance("Auto")

    stepReturn << testProgram.call()

    return stepReturn
  }
}
