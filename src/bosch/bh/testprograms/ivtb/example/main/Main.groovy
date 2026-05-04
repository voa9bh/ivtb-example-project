package bosch.bh.testprograms.ivtb.example.main

import baselib.buildinfo.MetaBuildInfoParser
import bosch.bh.include.global.Global
import bosch.bh.include.platform.rtmodules.soft.AverageFunctionSoft
import bosch.bh.include.platform.teststep.TestFunction
import bosch.bh.spex.sdk.tl.MemLong
import bosch.bh.spex.sdk.tl.ResultValueText
import bosch.bh.testprograms.ivtb.example.teststeps.AV_B10_Switch_Time_TAN_T2K
import bosch.bh.ts3000.procs.StepReturn
import bosch.bh.ts3000.procs.StepReturnError
import bosch.bh.ts3000.procs.TaskMessageHandler
import bosch.bh.ts3000.procs.TestProgramMain
import bosch.bh.ts3000.procs.TestProgramBuilder
import bosch.bh.ts3000.procs.testprogrambuilder.TestProgramSpec
import bosch.bh.ts3000.pvserver.Inject

class Main extends TestFunction implements TestProgramMain, TaskMessageHandler {

  @Inject
  public Global global
  @Inject
  public AV_B10_Switch_Time_TAN_T2K av_b10_switch_time_tan_t2K
  @Inject
  public Auto auto
  @Inject
  public Manual manual

  @SuppressWarnings("PropertyName")
  //needed for each Inject with special property name
  @Inject
  ResultValueText GlobalErrorString_TSX

  /* this part is used to dispatch asynchronous messages */
  protected final List<TaskMessageHandler> messageHandlers = [].asSynchronized()

  void addTaskMessageHandler(TaskMessageHandler tmh) {
    messageHandlers << tmh
  }

  void removeTaskMessageHandler(TaskMessageHandler tmh) {
    messageHandlers.remove(tmh)
  }

  void messageHandler(int sender, int index, int actionNumber, int dataSize, String message) {
    messageHandlers*.messageHandler(sender, index, actionNumber, dataSize, message)
  }

  @Override
  String getFieldValue(String fieldName) {
    return null
  }

  @Override
  Object programInit() {
    StepReturn stepReturn = StepReturn.newInstance("programInit")

    TestProgramSpec testProgram = new TestProgramBuilder().build {
      libObject("global", Global)

      testStep("av_b10_switch_time_tan_t2K", AV_B10_Switch_Time_TAN_T2K) {
        failOnError(true)
      }

      libObject("auto", Auto)
      libObject("manual", Manual)

      libObject("averageFunction", AverageFunctionSoft)
    }

    registerLibObject("testProgram", testProgram)

    return stepReturn
  }

  @Override
  Object programClose() {
    // this method is called when another program is loaded. Used for cleanup.
  }

  @Override
  Object programBreak() {
    println "programBreak()"
    StepReturn stepReturn = StepReturn.newInstance("programBreak")
    stepReturn << new StepReturnError(200, "Program-Break reached!")
    GlobalErrorString_TSX.set(stepReturn.errorString)
    stepReturn << test(Global.TEST_SMART_SKIP_ALL)
    global.processTestResult(stepReturn)

    return stepReturn
  }

  @Override
  Object main() {
    call()
  }

  @Inject
  MemLong vModeFlag

  @Override
  StepReturn apply(StepReturn stepReturn) {
    MetaBuildInfoParser metaBuildInfoParser = new MetaBuildInfoParser()
    println metaBuildInfoParser.getDependencies()
    println metaBuildInfoParser.getVersion("ivtb-example-project")

    try {
      if (vModeFlag.get()) {
        stepReturn << manual.call()
      } else {
        stepReturn << auto.call()
      }

    } catch (Exception e) {
      stepReturn << e
      throw e
    } finally {
      global.printStepReturn(stepReturn)
    }

    global.processTestResult(stepReturn)

    return stepReturn
  }
}