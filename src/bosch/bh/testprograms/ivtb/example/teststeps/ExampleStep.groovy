package bosch.bh.testprograms.ivtb.example.teststeps

import bosch.bh.include.exceptions.TestException
import bosch.bh.include.global.Global
import bosch.bh.include.platform.teststep.TestFunction
import bosch.bh.spex.sdk.tl.MemDouble
import bosch.bh.spex.sdk.tl.ResultValueDouble
import bosch.bh.ts3000.procs.StepReturn
import bosch.bh.ts3000.pvserver.Inject

@SuppressWarnings("PropertyName")
class ExampleStep extends TestFunction {
  @Inject
  MemDouble vVoltageIn
  @Inject
  ResultValueDouble Voltage1_TS1, SkipMe_TS1

  @Override
  StepReturn apply(StepReturn stepReturn) {

    throw new TestException("Example exception in step ${name}")

    println "now is ${new Date().toString()}"

    step("Message", 5, 5000) {
      // measure the vVoltageIn and store it in TS1_Voltage1
      Voltage1_TS1.set(vVoltageIn.get())
      // now test the Result
      // alternatively you can use: stepReturn << test(Global.TEST_RESULTS_IN_LIST,Voltage1_TS1)
      stepReturn << testResults(Voltage1_TS1)

      double setVoltage = Voltage1_TS1.getTolerances().sum() / 2.0
      println "setVoltage = $setVoltage"

      // set as critical error (part may not be retested)
      // alternative to stepReturn.testResult
      // (but checks everything - testResult, errorCode, errorString, errorList, criticalError)
      if (stepReturn.hasErrorsOrFailedTests()) {
        stepReturn.criticalError = 1
      }

      // skip the TS1_Skip_Me (is marked as deliberately not tested)
      // alternatively you can use: stepReturn << test(Global.TEST_SKIP_RESULTS_IN_LIST,SkipMe_TS1)
      stepReturn << skipResults(SkipMe_TS1)

      // alternatively you can call the smart skip function to have all the assigned
      // results tested and the unassigned skipped, when you pass the test step name
      // as it is stated in the typedata file.
      stepReturn << test(Global.TEST_SMART_SKIP, "TS1")

    }


    return stepReturn
  }
}
