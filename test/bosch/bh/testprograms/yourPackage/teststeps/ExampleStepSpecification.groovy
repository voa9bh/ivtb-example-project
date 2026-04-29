package bosch.bh.testprograms.yourPackage.teststeps

import bosch.bh.spex.sdk.tl.MemDouble
import bosch.bh.ts3000.init.UsrAppInit
import bosch.bh.ts3000.procs.StepReturn
import bosch.bh.ts3000.procs.TLI
import bosch.bh.ts3000.procs.TLM
import bosch.bh.ts3000.pvserver.PVObject
import bosch.bh.ts3000.pvserver.PVServer
import spock.lang.Shared
import spock.lang.Specification
import bosch.bh.spex.test.TestSystem

class ExampleStepSpecification extends Specification {
  @Shared PVServer pvServer
  @Shared PVObject pvServerObject
  @Shared TLM tlm
  @Shared TLI tli
  @Shared TestSystem ts

  @Shared Binding binding

  def setupSpec() {
    ts=TestSystem.build {
      vir {
        object("MemLong","vVoltageIn")
      }

      procs {
        object("Clock","clock")
      }

      td {
        object("ResultValueDouble","Voltage1_TS1_Td",2) {
          param('value', 4.0, 0)
          param('value', 6.0, 1)
        }
        object("ResultValueDouble","SkipMe_TS1_Td",2) {
          param('value', 19.0, 0)
          param('value', 21.0, 1)
        }
      }

      rd {
        object("ResultValueDouble","Voltage1_TS1") {
          targetState=1
          setValue=handleOf("Voltage1_TS1_Td")
        }
        object("ResultValueDouble","SkipMe_TS1") {
          targetState=1
          setValue=handleOf("SkipMe_TS1_Td")
        }
      }
    }

    binding=ts.binding
  }

  def setup() {
    // reset to dummy object because it is mocked in the test
    binding.setVariable("vVoltageIn",ts.vVoltageIn)
    ts.rd.initData()
  }

  def 'test normal run with a mocked vVoltageIn'() {
    given:
    // mock the voltage in, you can also do this in the setupSpec()
    MemDouble vVoltageIn = GroovyMock(MemDouble) {
      get() >> { 5.0 } // always return 5.0 volt
    }
    // replace the original variable in the binding with the mock
    binding.setVariable("vVoltageIn",vVoltageIn)
    ExampleStep exampleStep=new ExampleStep(binding: binding)

    when:
    StepReturn stepReturn=exampleStep.call()

    then:
    stepReturn.errorCode==0
    stepReturn.testResult==0
  }

  def 'bad test with critical error (voltage too low)'() {
    given:
    // mock the voltage in, you can also do this in the setupSpec()
    MemDouble vVoltageIn = GroovyMock(MemDouble) {
      get() >> { 3.5 } // always return 3.5 volt -> too low
    }
    // replace the original variable in the binding with the mock
    binding.setVariable("vVoltageIn",vVoltageIn)

    ExampleStep exampleStep=new ExampleStep(binding: binding)

    when:
    StepReturn stepReturn=exampleStep.call()

    then:
    stepReturn.errorCode==0
    stepReturn.testResult==2
    stepReturn.criticalError==1
    stepReturn.hasErrorsOrFailedTests()
    stepReturn.hasErrors()
    stepReturn.hasFailedTests()
  }
}
