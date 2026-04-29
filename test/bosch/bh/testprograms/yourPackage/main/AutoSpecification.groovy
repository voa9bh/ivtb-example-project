package bosch.bh.testprograms.yourPackage.main

import bosch.bh.include.global.Global
import bosch.bh.spex.test.TestSystem
import bosch.bh.ts3000.procs.StepReturn
import bosch.bh.ts3000.procs.StepReturnError
import bosch.bh.ts3000.procs.TestProgramBuilder
import bosch.bh.ts3000.procs.testprogrambuilder.TestProgramSpec
import spock.lang.Specification
import bosch.bh.testprograms.yourPackage.teststeps.ExampleStep
import bosch.bh.testprograms.yourPackage.teststeps.LoggingStep

class AutoSpecification extends Specification {
  def 'test the error collection'() {
    given:
    TestSystem ts = TestSystem.build {

    }

    Global global = GroovyMock(Global)

    ExampleStep exampleStep
    exampleStep = GroovyMock(ExampleStep) {
      call() >> {
        StepReturn stepReturn = new StepReturn()
        // add another error for checking the error list capabilities
        stepReturn.addErrorToList(new StepReturnError(-1, "undefined is not a function"))
        stepReturn.errorCode = 42
        stepReturn.errorString = "The answer to the universe and everything"
        return stepReturn
      }

      asType(_) >> {
        return exampleStep
      }
    }

    LoggingStep loggingStep
    loggingStep = GroovyMock(LoggingStep) {
      call() >> {
        StepReturn stepReturn = new StepReturn()
        return stepReturn
      }

      asType(_) >> {
        return loggingStep
      }
    }

    ts.activateInjectionForThread()

    TestProgramSpec testProgram = TestProgramBuilder.build {
      libObject("global", global)

      testStep("exampleStep", exampleStep) {
        failOnError(false)
      }
      testStep("loggingStep", loggingStep) {
        dependsOn("exampleStep")
      }
    }

    ts.binding.setVariable("testProgram", testProgram)

    Auto auto = new Auto()

    when:
    StepReturn stepReturn = auto.call()

    then:
    stepReturn.errorCode == 42
    stepReturn.errorString == "The answer to the universe and everything"
    stepReturn.errorList.contains(new StepReturnError(-1, "undefined is not a function"))
  }
}
