package bosch.bh.testprograms.ivtb.global

import bosch.bh.ts3000.procs.StepReturn
import bosch.bh.ts3000.procs.testprogrambuilder.ConfigData
import bosch.bh.ts3000.pvserver.GroovyPVObject
import groovy.transform.AutoClone
import groovy.transform.EqualsAndHashCode
import groovy.transform.ToString

@ToString
@EqualsAndHashCode
@AutoClone
class PutAdaptStatConfig implements ConfigData {

  GroovyPVObject triggerChannel
  GroovyPVObject settingChannel
  GroovyPVObject measuringChannel
  GroovyPVObject deltaMeasuringChannel

  int sensorRel
  int diffSetInv
  int measMult
  int measDiv
  int setBefore
  int setWithTol
  int tolPercentReduction
  int compensation
  int restTime
  int constTime
  int cyclicTime
  int gradTime
  int meanValues
  int filterFlag
  int amontMeanFilt
  int minSteps
  int maxSteps
  int maxSetValue

  @Override
  StepReturn checkConfigData(StepReturn stepReturn) {


    return stepReturn
  }
}
