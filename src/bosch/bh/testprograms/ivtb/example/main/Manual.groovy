package bosch.bh.testprograms.ivtb.example.main

import bosch.bh.include.platform.teststep.TestFunction
import bosch.bh.spex.sdk.tl.MemLong
import bosch.bh.testprograms.ivtb.example.teststeps.ExampleStep
import bosch.bh.ts3000.procs.StepReturn
import bosch.bh.ts3000.pvserver.Inject

class Manual extends TestFunction {

  @Inject
  ExampleStep exampleStep

  @Inject
  MemLong vModeFlag

  @Override
  StepReturn apply(StepReturn stepReturn) {
    // by definition vModeFlag==1 means a console menu that lets the user select the step
    // vModeFlag>1 executes a single test step

    int manualFunction = vModeFlag.get()

    if (manualFunction == 1) {
      println "Choose manual function:"
      println "2: ExampleStep"
      Object input = read(manualFunction)
      if (input != null) {
        manualFunction = input
      }
    }

    switch (manualFunction) {
      case 2:
        exampleStep.call()
        break
      default:
        println "unknown function. Ending the program"
    }

    return stepReturn
  }
}
