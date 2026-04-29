package bosch.bh.testprograms.yourPackage.teststeps

import bosch.bh.ts3000.procs.TLM
import org.apache.logging.log4j.Level
import spock.lang.Shared
import spock.lang.Specification
import bosch.bh.spex.test.TestSystem

class SimpleTestSpecification extends Specification {
  @Shared TestSystem ts
  @Shared TLM tlm

  def setupSpec() {
    ts=TestSystem.build(Level.INFO) {
      procs {
        object("PVSystem", "system")
      }
    }

    tlm=ts._TLM
  }

  def "endStep"() {
    when:
    tlm.step("Step1", 1, 0)
    tlm.step("Step2", 1, 0)
    tlm.step("Step3", 1, 0)
    tlm.step("Step3.1", 2, 0)
    tlm.step("Step3.1.1", 3, 0)
    tlm.step("Step3.2", 2, 0)

    tlm.endStep()
    tlm.endStep()
    tlm.endStep()
    tlm.endStep()
    tlm.endStep()

    then:
    noExceptionThrown()

  }
}
