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

  GroovyPVObject hTriggerOut
  GroovyPVObject hStlk
  GroovyPVObject hMeskA
  GroovyPVObject hMeskB

  int relativ
  int inver
  int fac_mes
  int div_mes
  int stlw
  int stlz
  int narrow
  int kompen
  int tvor
  int tnac
  int tgrad
  int anz_vf
  int filter
  int anz_nf
  int stpmin
  int stpmax
  int stl_limit

  @Override
  StepReturn checkConfigData(StepReturn stepReturn) {


    return stepReturn
  }
}
